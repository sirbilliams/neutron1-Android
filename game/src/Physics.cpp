#include "PortLoadTiming.h" // port: load timing
/*  CPhysics - see Physics.h. Reconstructed from Neutron.exe 0x478fa0..0x47ced0 and the geometry
    helpers it uses (0x469500..0x469b70, 0x477dd0). Static initialisers (0x478d20, 0x478d50)
    set the w components of g_phys_ray and g_gravity.

    Comparisons follow the x87 code: `!(a > b)` where the original branched on "not above", so
    unordered (NaN) operands take the same branch; FEq() is the "C3 set" test (true for NaN too). */

#include "Physics.h"
#include "CGameObject.h"
#include "C3DObject.h"
#include "lab/C3DSphere.h"
#include "level1/C3DCube.h"
#include "CGameType.h"
#include "Game.h"
#include "Todo.h"
#include "OMedia3DShape.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include "OMedia3DPoint.h"
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

using namespace neg;

// --------------------------------------------------------------------------- state

namespace {

// A terrain polygon in the collision grid (0x1c bytes, owned by s_terrain_polys).
struct TerrainPoly
{
	OMedia3DPolygon	*poly;			// +0x00
	OMedia3DShape	*shape;			// +0x04
	OMedia3DVector	 center;		// +0x08  bounding sphere (w = 1)
	float			 radius;		// +0x18
};

typedef std::list<TerrainPoly *> PolyList;
typedef std::list<CGameObject *> ObjectList;

enum { GRID = 0x104 };					// 260 x 260 cells of 500 units, (0, 0) in cell (130, 130)
const double kCellScale  = 0.0020000000949949026;	// 0x4d6a04 (float 0.002)
const double kCellOffset = 130.0;					// 0x49b1fc
const float  kSteep      = 0.699999988079071f;		// 0x48d974: |normal.y| below this: wall
const float  kEpsilon    = 1.1920928955078125e-07f;	// 0x4d5e40 (FLT_EPSILON)
const float  kNoFloor    = -25000.0f;				// 0x4d6a08
const float  kFarAway    = 130000.0f;				// 0x47fde800: "no face hit yet" at a sweep start

PolyList	s_terrain_polys;				// 0x5cfc30  every record (owner)
PolyList	s_cell_polys[GRID][GRID];		// 0x5cfc78  every polygon touching the cell
PolyList	s_cell_walls[GRID][GRID];		// 0x509b20  the steep ones (normal.y < 0.7)
int			s_terrain_poly_count = 0;		// 0x695d80  polygons offered by PhysicsAddTerrain

ObjectList				s_debug_cubes;		// 0x509b00  marker pools ('3CUB' / '3SPH' objects)
ObjectList::iterator	s_next_cube;		// 0x5cfc70
ObjectList				s_debug_spheres;	// 0x509b10
ObjectList::iterator	s_next_sphere;		// 0x5cfbf8
OMedia3DVector			s_cube_colour(1.0f, 1.0f, 1.0f, 1.0f);		// 0x5cfc20
OMedia3DVector			s_sphere_colour(1.0f, 1.0f, 1.0f, 1.0f);	// 0x5cfc10
CGameObject				*s_markers[6];		// 0x50981c  deleted by Reset; nothing in the exe sets them

float			 s_frame_dt = 0;			// 0x695d60
bool			 s_was_on_floor = false;	// 0x695d49  v133() when the object's move started
float			 s_closest_edge = 0;		// 0x5cfbe0  nearest edge contact of the sweep (-1 = none)
OMedia3DVector	 s_contact(0, 0, 0, 1.0f);	// 0x5cfbe8  last contact point
OMedia3DPolygon	*s_last_wall = NULL;		// 0x695d4c  last polygon a sweep hit
OMedia3DPolygon	*s_hit_poly = NULL;			// 0x695d54  last ray hit
OMedia3DShape	*s_hit_shape = NULL;		// 0x695d58
int				 s_allow_low_edges = 0;		// 0x695d74  edges below the centre count (objects with f_42c)
int				 s_ray_tests = 0;			// 0x695d68  statistics, never shown
int				 s_sweep_tests = 0;			// 0x695d70
int				 s_sweep_calls = 0;			// 0x695d78
int				 s_moved_count = 0;			// 0x695d7c
int				 s_debug_line = 0;			// 0x695d84  PhysicsDebugText line
// Debug switches: nothing in the shipped exe sets them.
bool			 s_debug_contacts = false;	// 0x695d44  markers + text for every contact
bool			 s_debug_box_corners = false;	// 0x695d45  markers on the corners of boxes
bool			 s_show_touch_stats = false;	// 0x695d47  "TOUCH=..." line

// 0x478d20 / 0x478d50: w components of the ray, default gravity
struct InitPhysicsGlobals
{
	InitPhysicsGlobals()
	{
		g_phys_ray.origin.w = 1.0f;
		g_phys_ray.dir.w = 0;
		g_phys_ray.end.w = 1.0f;
		g_gravity = OMedia3DVector(0, -5000.0f, 0, 0);
		s_next_cube = s_debug_cubes.begin();
		s_next_sphere = s_debug_spheres.begin();
	}
} s_init_physics_globals;

// --------------------------------------------------------------------------- small helpers

inline bool FEq(float a, float b)		{ return a == b || a != a || b != b; }	// x87 "equal or unordered"

inline short Cell(double v)				{ return (short)(int)(v * kCellScale + kCellOffset); }	// _ftol, low word
inline bool InGrid(short x, short z)	{ return x >= 0 && x < GRID && z >= 0 && z < GRID; }

typedef void (CGameObject::*VectorGetter)(float *);
inline OMedia3DVector Get(CGameObject *o, VectorGetter g)	{ OMedia3DVector v; (o->*g)(&v.x); return v; }
inline OMedia3DVector Position(CGameObject *o)		{ return Get(o, &CGameObject::v195); }	// +0x30c
inline OMedia3DVector PrevPosition(CGameObject *o)	{ return Get(o, &CGameObject::v189); }	// +0x2f4
inline OMedia3DVector Velocity(CGameObject *o)		{ return Get(o, &CGameObject::v174); }	// +0x2b8
inline OMedia3DVector Impulse(CGameObject *o)		{ return Get(o, &CGameObject::v160); }	// +0x280
inline OMedia3DVector Acceleration(CGameObject *o)	{ return Get(o, &CGameObject::v155); }	// +0x26c
inline OMedia3DVector Spin(CGameObject *o)			{ return Get(o, &CGameObject::v170); }	// +0x2a8

inline OMedia3DVector Sub(const OMedia3DVector &a, const OMedia3DVector &b)	// a - b, w = 0
{
	return OMedia3DVector(a.x - b.x, a.y - b.y, a.z - b.z, 0);
}

inline const OMedia3DPoint &Vertex(OMedia3DShape *shape, OMedia3DPolygon *poly, short i)
{
	return (*shape->get_vertices())[poly->get_point(i)];		// 0x477f10
}

inline OMedia3DVector ToVector(const OMedia3DPoint &p)	{ return OMedia3DVector(p.x, p.y, p.z, p.w); }

// 0x469490 is std::list::erase; 0x47cea0 / 0x47ced0 are list node allocation and construction.

}	// namespace

// --------------------------------------------------------------------------- geometry (0x469530..)

// 0x469530: the segment a -> b
static CPhysicsRay MakeRay(const OMedia3DVector &a, const OMedia3DVector &b)
{
	CPhysicsRay r;
	r.origin = a;
	r.dir = OMedia3DVector(b.x - a.x, b.y - a.y, b.z - a.z, 0);
	r.end = b;
	return r;
}

// 0x469880: the point of segment a-b nearest to p (projection with OMT's quick magnitude)
static OMedia3DVector ClosestPointOnSegment(OMedia3DVector a, OMedia3DVector b, OMedia3DVector p)
{
	OMedia3DVector ap(p.x - a.x, p.y - a.y, p.z - a.z, 0);
	OMedia3DVector ab(b.x - a.x, b.y - a.y, b.z - a.z, 0);
	float len = ab.quick_magnitude();
	if (FEq(len, 0)) return a;
	float inv = 1.0f / ab.quick_magnitude();
	ab.x *= inv; ab.y *= inv; ab.z *= inv;
	float d = ap.z * ab.z + ap.y * ab.y + ap.x * ab.x;
	if (!(d >= 0.0f)) return a;
	if (d > len) return b;
	return OMedia3DVector(a.x + ab.x * d, a.y + ab.y * d, a.z + ab.z * d, a.w);
}

// 0x4695b0: the point of the triangle's edges nearest to p (the face itself is the ray test's job)
static OMedia3DVector ClosestPointOnTriangle(OMedia3DVector p, OMedia3DVector a, OMedia3DVector b, OMedia3DVector c)
{
	OMedia3DVector r1 = ClosestPointOnSegment(a, b, p);
	OMedia3DVector r2 = ClosestPointOnSegment(b, c, p);
	OMedia3DVector r3 = ClosestPointOnSegment(c, a, p);
	float m1 = Sub(p, r1).quick_magnitude();
	float m2 = Sub(p, r2).quick_magnitude();
	float m3 = Sub(p, r3).quick_magnitude();
	if (m2 > m1)
		return (m3 > m1) ? r1 : r3;
	return (m3 > m2) ? r2 : r3;
}

// 0x469b70: Moller-Trumbore with the direction normalised (t is a distance). Faces with
// det <= -eps always count; the others only when two_sided and det >= eps. A hit must lie within
// the segment; it moves ray->end to the hit point.
static bool RayHitsTriangle(CPhysicsRay *ray, OMedia3DVector p0, OMedia3DVector p1, OMedia3DVector p2,
							float *u, float *v, float *t, bool two_sided)
{
	float e1x = p1.x - p0.x, e1y = p1.y - p0.y, e1z = p1.z - p0.z;
	OMedia3DVector d = ray->dir;
	float e2x = p2.x - p0.x, e2y = p2.y - p0.y, e2z = p2.z - p0.z;
	float inv = 1.0f / d.quick_magnitude();
	d.x *= inv; d.y *= inv; d.z *= inv;
	float px = e2z * d.y - e2y * d.z;
	float py = -(e2z * d.x - e2x * d.z);
	float pz = e2y * d.x - e2x * d.y;
	float det = e1z * pz + e1y * py + px * e1x;
	if (det > -kEpsilon)								// (unordered: computed like det <= -eps)
	{
		if (!two_sided) return false;
		if (!(det >= kEpsilon)) return false;
	}
	float inv_det = 1.0f / det;
	float tx = ray->origin.x - p0.x, ty = ray->origin.y - p0.y, tz = ray->origin.z - p0.z;
	*u = (tz * pz + ty * py + tx * px) * inv_det;
	if (!(*u >= 0.0f) || *u > 1.0f) return false;
	float qx = ty * e1z - tz * e1y;
	float qy = -(e1z * tx - tz * e1x);
	float qz = e1y * tx - ty * e1x;
	*v = (qz * d.z + qy * d.y + qx * d.x) * inv_det;
	if (!(*v >= 0.0f) || *v + *u > 1.0f) return false;
	*t = (qz * e2z + qy * e2y + qx * e2x) * inv_det;
	if (!(*t >= 0.0f)) return false;
	if (!(ray->dir.quick_magnitude() > *t)) return false;
	ray->end.x = d.x * *t + ray->origin.x;
	ray->end.y = d.y * *t + ray->origin.y;
	ray->end.z = d.z * *t + ray->origin.z;
	return true;
}

// 0x469a10: the first three vertices of a terrain polygon (+ offset) against the ray
static bool RayHitsPolygon(CPhysicsRay *ray, OMedia3DShape *shape, OMedia3DPolygon *poly,
						   OMedia3DVector offset, float *t, bool two_sided)
{
	OMedia3DVector p0 = ToVector(Vertex(shape, poly, 0));
	p0.x += offset.x; p0.y += offset.y; p0.z += offset.z;
	OMedia3DVector p1 = ToVector(Vertex(shape, poly, 1));
	p1.x += offset.x; p1.y += offset.y; p1.z += offset.z;
	OMedia3DVector p2 = ToVector(Vertex(shape, poly, 2));
	p2.x += offset.x; p2.y += offset.y; p2.z += offset.z;
	float u, v;
	return RayHitsTriangle(ray, p0, p1, p2, &u, &v, t, two_sided);
}

// 0x469500: delete the six objects of 0x50981c (always empty in the shipped game)
static void DeleteMarkerObjects(void)
{
	for (int i = 0; i < 6; i++)
		if (s_markers[i])
		{
			delete s_markers[i];							// (+0x30)->vtable +8 (1): deleting destructor
			s_markers[i] = NULL;
		}
}

// 0x477dd0: the average of the polygon's vertices. As compiled, the y and z sums are rounded to
// float before the division, the x sum is not. No vertices: division by zero, as in the original.
static OMedia3DVector PolygonCenter(OMedia3DShape *shape, OMedia3DPolygon *poly)
{
	double sx = 0, sy = 0, sz = 0;
	for (short i = 0; i < poly->get_num_points(); i++)
	{
		const OMedia3DPoint &p = Vertex(shape, poly, i);
		sx += p.x; sy += p.y; sz += p.z;
	}
	double n = poly->get_num_points();
	float fy = (float)sy, fz = (float)sz;
	return OMedia3DVector((float)(sx / n), (float)(fy / n), (float)(fz / n), 1.0f);
}

// --------------------------------------------------------------------------- objects

void neg::PhysicsRemoveObject(CGameObject *o)				// 0x478fa0 "CPhysics::RemoveObject()"
{
	g_physics_objects.remove(o);
}

std::list<CGameObject *> neg::g_physics_objects;				// 0x695d38

static void DeletePool(ObjectList &pool, ObjectList::iterator &next)
{
	next = pool.begin();	// (as the original: set before the pool is emptied; Process rewinds it)
	while (!pool.empty())
	{
		CGameObject *o = pool.front();
		pool.remove(o);
		if (o) delete o;									// (+0x30)->vtable +8 (1)
	}
	next = pool.end();		// (the original's cursor is left on a freed node here)
}

void neg::PhysicsReset(void)								// 0x479000 "CPhysics::Reset()"
{
	DeletePool(s_debug_cubes, s_next_cube);
	DeletePool(s_debug_spheres, s_next_sphere);
	DeleteMarkerObjects();									// 0x469500
	g_physics_objects.clear();								// (the objects are not deleted)
	PhysicsClearTerrain();
	s_terrain_poly_count = 0;
}

// --------------------------------------------------------------------------- terrain

// 0x4794c0: polygons with the material "cstart" / "ccheck1".."ccheck9" are the level's checkpoints:
// hidden, reported to the game type and left out of the collision grid.
static bool TakeCheckpointPolygon(OMedia3DPolygon *poly, OMedia3DShape *shape)
{
	static const char *const kNames[10] = {
		"cstart", "ccheck1", "ccheck2", "ccheck3", "ccheck4", "ccheck5", "ccheck6", "ccheck7", "ccheck8", "ccheck9" };
	if (!poly || !poly->get_material()) return false;
	std::string name;
	poly->get_material()->get_chunk_name(name);
	for (int i = 0; i < 10; i++)
		if (name == kNames[i])
		{
			poly->set_flags(om3pf_Hide);					// flags = 4 (not or-ed)
			g_gametype->ev77(i, poly, shape);				// vtable +0x12c
			return true;
		}
	return false;
}

// 0x479800: put the record in cell (x, z) unless its polygon is there already; steep polygons go
// in the wall grid too.
static bool AddPolyToCell(short x, short z, TerrainPoly *t)
{
	if (!InGrid(x, z)) return false;
	PolyList &cell = s_cell_polys[x][z];
	for (PolyList::iterator i = cell.begin(); i != cell.end(); i++)
		if ((*i)->poly == t->poly) return false;
	float ny = t->poly->get_normal().y;
	cell.push_back(t);
	if (!(ny >= kSteep))
		s_cell_walls[x][z].push_back(t);
	return true;
}

// 0x479ac0: centre and radius (largest centre-vertex distance) of a polygon
static void PolygonBoundingSphere(OMedia3DShape *shape, OMedia3DPolygon *poly, OMedia3DVector *center, float *radius)
{
	OMedia3DVector c = PolygonCenter(shape, poly);
	float r = 0;
	for (int i = 0; i < poly->get_num_points(); i++)
	{
		const OMedia3DPoint &p = Vertex(shape, poly, (short)i);
		double dx = (double)p.x - c.x, dy = (double)p.y - c.y, dz = (double)p.z - c.z;
		double d = sqrt(dx * dx + dy * dy + dz * dz);
		if (d > r) r = (float)d;
	}
	*center = OMedia3DVector(c.x, c.y, c.z, 1.0f);
	*radius = r;
}

void neg::PhysicsAddTerrain(OMedia3DShape *shape)			// 0x479140 "Add3DTerrain()"
{
	PortLoadTiming::Phase load_phase("terrain-build"); // port: load timing
	if (!shape) return;
	std::vector<OMedia3DPolygon> &polys = *shape->get_polygons();
	s_terrain_poly_count += (int)polys.size();
	for (size_t k = 0; k < polys.size(); k++)
	{
		OMedia3DPolygon *poly = &polys[k];
		if (TakeCheckpointPolygon(poly, shape)) continue;
		if (g_gametype->ev79(shape, poly, PolygonCenter(shape, poly)))		// vtable +0x134
		{
			g_terrain_poly_claimed = true;
			continue;
		}
		// x/z extent of the polygon (the original also copies the vertices into a 4-entry stack
		// array it never reads - it would overflow on polygons with more than four vertices)
		float min_x = 99999.0f, max_x = -99999.0f, min_z = 99999.0f, max_z = -99999.0f;
		for (short i = 0; i < poly->get_num_points(); i++)
		{
			const OMedia3DPoint &p = Vertex(shape, poly, i);
			if (min_x >= p.x) min_x = p.x;
			if (!(max_x > p.x)) max_x = p.x;
			if (min_z >= p.z) min_z = p.z;
			if (!(max_z > p.z)) max_z = p.z;
		}
		TerrainPoly *t = new TerrainPoly;
		t->poly = poly;
		t->shape = shape;
		PolygonBoundingSphere(shape, poly, &t->center, &t->radius);
		short x1 = Cell(max_x), z0 = Cell(min_z), z1 = Cell(max_z), x0 = Cell(min_x);
		bool added = false;
		for (short x = x0; x <= x1; x++)
			for (short z = z0; z <= z1; z++)
				if (InGrid(x, z))
				{
					AddPolyToCell(x, z, t);
					added = true;							// (also when the cell had the polygon)
				}
		if (added)
		{
			s_terrain_polys.push_back(t);
			g_terrain_memory += (int)0x1c;
		}
		else delete t;
	}
}

void neg::PhysicsClearTerrain(void)							// 0x479920 "ClearPolyTerrains()"
{
	for (int x = 0; x < GRID; x++)
		for (int z = 0; z < GRID; z++)
		{
			s_cell_polys[x][z].clear();
			s_cell_walls[x][z].clear();
		}
	while (!s_terrain_polys.empty())
	{
		delete s_terrain_polys.front();
		s_terrain_polys.pop_front();
	}
}

void neg::PhysicsAddObject(CGameObject *o)					// 0x479a40 "Add3DObject(%s)"
{
	o->v83(0, 0, 0);										// vtable +0x14c: floor position (initial and current)
	o->ResetObject();										// vtable +0x28
	g_physics_objects.push_back(o);
}

// --------------------------------------------------------------------------- one frame

// 0x47cc10 / 0x47cdb0: every marker still shown from the last frame is hidden (the element's own
// hide, slot 22) and the pool's cursor goes back to its first object.
static void HideMarkers(ObjectList &pool, ObjectList::iterator &next)
{
	for (ObjectList::iterator i = pool.begin(); i != pool.end(); i++)
	{
		OMediaElement *e = *i ? dynamic_cast<OMediaElement *>(*i) : NULL;
		if (e && e->getvisible()) e->hide(true);			// element +0x70 hide count == 0
	}
	next = pool.begin();
}

void neg::PhysicsProcess(float dt)							// 0x479bc0
{
	if (FEq(dt, 0)) return;
	s_frame_dt = dt;
	HideMarkers(s_debug_cubes, s_next_cube);
	HideMarkers(s_debug_spheres, s_next_sphere);
	if (g_phys_on)
	{
		s_moved_count = 0;
		for (ObjectList::iterator i = g_physics_objects.begin(); i != g_physics_objects.end(); i++)
			if (*i) PhysicsMoveObject(*i, dt);
		PhysicsCollideObjects(dt);
	}
	s_sweep_calls = 0;
}

void neg::PhysicsMoveObject(CGameObject *o, float dt)		// 0x479d20
{
	s_was_on_floor = o->v133() != 0;
	if (o->v113()) o->v134(0);								// floor-snapping objects leave the floor for now
	if (!o->v131()) return;									// physics off
	OMedia3DVector pos = Position(o);
	// (the original also takes the quick magnitude of previous - current position, unused)
	if (g_phys_terrain_on)
		o->v188(0, kNoFloor, 0);
	else
	{
		o->v134(1);											// no terrain: stay at this height
		o->v188(0, pos.y, 0);
		o->v157(0, -g_gravity.y, 0);						// (Negatron only with its gravity flag f_142)
		OMedia3DVector v = Velocity(o);
		o->v176(v.x, 0, v.z);
	}

	OMedia3DVector spin = Spin(o);
	o->AddRotation(spin.x * dt, spin.y * dt, spin.z * dt);	// vtable +0x334 (Negatron scaled by its time scale)
	OMedia3DVector imp = Impulse(o);
	o->v178(imp.x, imp.y, imp.z);							// velocity += impulse, once
	o->v162(0, 0, 0);
	OMedia3DVector acc = Acceleration(o);
	o->v178(acc.x * dt, acc.y * dt, acc.z * dt);
	o->v178(g_gravity.x * dt, g_gravity.y * dt, g_gravity.z * dt);		// always (Negatron: with f_142)
	OMedia3DVector vel = Velocity(o);
	OMedia3DVector step(vel.x * dt, vel.y * dt, vel.z * dt, 0);

	if (o->f_140)
	{
		if (o->v115())
		{
			// swept against the walls; moving down, only the horizontal part is swept
			OMedia3DVector p = Position(o);
			s_last_wall = NULL;
			g_closest_face_t = kFarAway;
			s_closest_edge = o->v151();
			o->v190(p.x, p.y, p.z, p.w);					// start of the move
			s_debug_line = 0;
			if (step.y > 0)
				PhysicsMoveAgainstTerrain(o, &p, OMedia3DVector(step.x, step.y, step.z, 0), true);
			else
			{
				PhysicsMoveAgainstTerrain(o, &p, OMedia3DVector(step.x, 0, step.z, 0), true);
				p.y = p.y + step.y;
			}
			// keep the result unless a contact callback moved the object meanwhile
			OMedia3DVector a = PrevPosition(o), b = Position(o);
			if (FEq(a.x, b.x) && FEq(a.y, b.y) && FEq(a.z, b.z) && FEq(a.w, b.w))
			{
				if (!(p.y >= kNoFloor)) p.y = kNoFloor;
				o->v196(p.x, p.y, p.z, p.w);
			}
		}
		else
		{
			OMedia3DVector p = Position(o);
			o->v190(p.x, p.y, p.z, p.w);
			o->v198(step.x, step.y, step.z, 0);				// position += step
		}
		if (o->v113()) PhysicsSnapToGround(o);
		if (FEq(o->v153(), 0)) PhysicsApplyFriction(o, dt);
	}
	else if (!o->v137())
		o->v198(step.x, step.y, step.z, 0);

	// come to rest
	float rest = o->v139();
	if (!(Spin(o).quick_magnitude() * rest > 0.1f)) o->v172(0, 0, 0);
	if (!(Velocity(o).quick_magnitude() * rest > 0.1f) && FEq(o->v153(), 0)) o->v176(0, 0, 0);
}

void neg::PhysicsApplyFriction(CGameObject *o, float dt)	// 0x47a330
{
	if (!o || !o->v133()) return;							// on the floor only
	float k = o->v145();
	OMedia3DVector v = Velocity(o);
	if (!(v.quick_magnitude() > 0)) return;
	o->v178(-(v.x * k * dt), 0, -(v.z * k * dt));
	OMedia3DVector s = Spin(o);
	o->v173(-(s.x * k * dt), -(s.y * k * dt), -(s.z * k * dt));
}

void neg::PhysicsCollideObjects(float dt)					// 0x47a430
{
	(void)dt;
	int touch = 0, touching = 0, untouch = 0;
	if (g_phys_objects_on)
	{
		for (ObjectList::iterator i = g_physics_objects.begin(); i != g_physics_objects.end(); i++)
		{
			CGameObject *a = *i;
			if (!a->f_140) continue;
			for (ObjectList::iterator j = i; j != g_physics_objects.end(); j++)
			{
				CGameObject *b = *j;
				if (!b->f_140 || a == b || !a->v231(b) || !b->v231(a)) continue;	// vtable +0x39c
				bool hit;
				if (a->v93() == 2) hit = PhysicsCollideWithBox(a, b);
				else if (b->v93() == 2) hit = PhysicsCollideWithBox(b, a);
				else
				{
					OMedia3DVector pb = Position(b);
					OMedia3DVector pa = Position(a);
					OMedia3DVector d = Sub(pb, pa);
					float rb = b->v151();
					float reach = a->v151() + rb;
					float dist = d.quick_magnitude();
					hit = false;
					if (!(dist >= reach))
					{
						bool linked = false;					// linked objects never collide
						for (ObjectList::iterator k = a->m_linked.begin(); k != a->m_linked.end(); k++)
							if (*k == b) linked = true;
						if (!linked)
						{
							a->Collide(b, dist);				// vtable +0x4c
							b->Collide(a, dist);
							hit = true;
						}
					}
				}
				if (hit)
				{
					if (a->IsTouching(b)) { a->v18(b); touching++; }
					else { a->AddTouching(b); touch++; }
					if (b->IsTouching(a)) { b->v18(a); touching++; }
					else { b->AddTouching(a); touch++; }
				}
				else
				{
					if (a->IsTouching(b)) { a->RemoveTouching(b); untouch++; }
					if (b->IsTouching(a)) { b->RemoveTouching(a); untouch++; }
				}
			}
		}
	}
	if (s_show_touch_stats)
		DrawText(0, 60, 'TEXT', "TOUCH=%d TOUCHING=%d UNTOUCH=%d", touch, touching, untouch);
}

// 0x47bd80: the sphere (radius r) moving from start to *end against triangle a-b-c with normal n.
// Face: a ray from start to the sphere's leading point (end - n*r, but not below end.y); the
// nearest face hit of the sweep wins and puts *end at hit + n*r (y only for ceilings). Edges
// (test_edges, walls only): the nearest edge point within s_closest_edge pushes *end out
// horizontally. Both record the contact point.
static bool SphereHitsTriangle(OMedia3DVector a, OMedia3DVector b, OMedia3DVector c, OMedia3DVector n,
							   OMedia3DVector start, OMedia3DVector *end, float r, bool test_edges)
{
	float ox = n.x * r;
	float lead_x = end->x - ox;
	float end_y = end->y;
	float oy = n.y * r;
	float lead_y = end->y - oy;
	float oz = n.z * r;
	float lead_z = end->z - oz;
	if (!(lead_y >= end_y)) lead_y = end_y;
	CPhysicsRay ray = MakeRay(start, OMedia3DVector(lead_x, lead_y, lead_z, 1.0f));
	float t, u, v;
	if (RayHitsTriangle(&ray, a, b, c, &u, &v, &t, false))
	{
		if (!FEq(g_closest_face_t, -1.0f) && t >= g_closest_face_t) return false;
		if (s_debug_contacts)
		{
			PhysicsDebugSphere(start, 10.0f);
			PhysicsDebugCube(ray.end, 10.0f);
		}
		g_closest_face_t = t;
		end->x = ox + ray.end.x;
		if (!(n.y >= -kSteep)) end->y = oy + ray.end.y;	// ceiling: pushed down
		else end->y = end_y;
		end->z = oz + ray.end.z;
		s_contact = ray.end;
		return true;
	}
	if (!test_edges) return false;
	if (!(n.y > -kSteep)) return false;
	if (n.y >= kSteep) return false;
	OMedia3DVector q = ClosestPointOnTriangle(*end, a, b, c);
	if (s_allow_low_edges == 0 && !(q.y - end_y >= 0.0f)) return false;	// edges below the centre: no
	OMedia3DVector d = Sub(q, *end);
	float dist = d.quick_magnitude();
	float inv = 1.0f / d.quick_magnitude();
	d.x *= inv; d.y *= inv; d.z *= inv;
	float dot = d.y * n.y + d.x * n.x + d.z * n.z;
	if (dot >= 0.0f) return false;						// edge in front of the polygon only
	if (!FEq(s_closest_edge, -1.0f) && dist >= s_closest_edge) return false;
	if (s_debug_contacts)
	{
		PhysicsDebugText("%c%f", 'N', (double)dot);
		PhysicsDebugCube(q, 10.0f);
	}
	s_closest_edge = dist;
	end->x = q.x - d.x * r;									// (y is kept)
	end->z = q.z - d.z * r;
	s_contact = q;
	return true;
}

// The 12 triangles of a box (corners: X/x = max/min x, ...), as listed in the switch at 0x47ab7b.
namespace {
enum { XYZ, XYz, XyZ, Xyz, xYZ, xYz, xyZ, xyz };
const unsigned char kBoxTriangles[12][3] = {
	{ XYZ, XYz, XyZ }, { XYz, Xyz, XyZ },		// +x
	{ XYZ, XyZ, xYZ }, { XyZ, xyZ, xYZ },		// +z
	{ xYZ, xyZ, xYz }, { xyZ, xyz, xYz },		// -x
	{ xYz, xyz, Xyz }, { xYz, Xyz, XYz },		// -z
	{ XYZ, xYZ, XYz }, { xYZ, xYz, XYz },		// +y
	{ XyZ, Xyz, xyz }, { xyz, xyZ, XyZ },		// -y
};
}

bool neg::PhysicsCollideWithBox(CGameObject *box, CGameObject *o)	// 0x47a5e0
{
	if (!o->v115()) return false;
	float box_r = box->v151();
	OMedia3DVector box_pos = Position(box);
	float r = o->v151();
	OMedia3DVector pos = Position(o);
	float dist = Sub(pos, box_pos).quick_magnitude();
	if (o->v93() == 2) return r + box_r >= dist;			// box against box: spheres only
	if (!(box_r + box_r + r >= dist)) return false;

	OMedia3DVector mx = Get(box, &CGameObject::v184), mn = Get(box, &CGameObject::v182);	// +0x2e0 / +0x2d8
	OMedia3DVector corner[8];
	corner[XYZ] = box->v224(mx.x, mx.y, mx.z);				// vtable +0x380: to world
	corner[XYz] = box->v224(mx.x, mx.y, mn.z);
	corner[XyZ] = box->v224(mx.x, mn.y, mx.z);
	corner[Xyz] = box->v224(mx.x, mn.y, mn.z);
	corner[xYZ] = box->v224(mn.x, mx.y, mx.z);
	corner[xYz] = box->v224(mn.x, mx.y, mn.z);
	corner[xyZ] = box->v224(mn.x, mn.y, mx.z);
	corner[xyz] = box->v224(mn.x, mn.y, mn.z);
	if (s_debug_box_corners)
		for (int i = 0; i < 8; i++) PhysicsDebugCube(corner[i], 10.0f);

	OMedia3DVector prev = PrevPosition(o);
	pos = Position(o);
	s_closest_edge = r;
	g_closest_face_t = -1.0f;
	for (int k = 0; k < 12; k++)
	{
		const OMedia3DVector &t0 = corner[kBoxTriangles[k][0]];
		const OMedia3DVector &t1 = corner[kBoxTriangles[k][1]];
		const OMedia3DVector &t2 = corner[kBoxTriangles[k][2]];
		float e2x = t2.x - t0.x, e2y = t2.y - t0.y, e2z = t2.z - t0.z;
		float e1x = t1.x - t0.x, e1y = t1.y - t0.y, e1z = t1.z - t0.z;
		OMedia3DVector n(e1y * e2z - e1z * e2y, -(e1x * e2z - e1z * e2x), e1x * e2y - e1y * e2x, 0);
		float inv = 1.0f / n.quick_magnitude();
		n.x *= inv; n.y *= inv; n.z *= inv;
		SphereHitsTriangle(t0, t2, t1, n, prev, &pos, r, true);
	}
	if (FEq(g_closest_face_t, -1.0f) && s_closest_edge >= r) return false;
	if (!box->v119())										// the box pushes: move o again from where it was
	{
		OMedia3DVector p = prev;
		PhysicsMoveAgainstTerrain(o, &p, Sub(pos, prev), false);
		o->v196(p.x, p.y, p.z, p.w);
	}
	return true;
}

// 0x47bbb0: the sphere against one terrain polygon (its first three vertices)
static void SphereHitsPolygon(OMedia3DPolygon *poly, OMedia3DShape *shape, OMedia3DVector n, OMedia3DVector start,
							  OMedia3DVector *end, bool *hit, float r, bool test_edges)
{
	OMedia3DVector v2 = ToVector(Vertex(shape, poly, 2));
	OMedia3DVector v1 = ToVector(Vertex(shape, poly, 1));
	OMedia3DVector v0 = ToVector(Vertex(shape, poly, 0));
	if (SphereHitsTriangle(v0, v1, v2, n, start, end, r, test_edges))
	{
		s_last_wall = poly;
		*hit = true;
	}
}

void neg::PhysicsMoveAgainstTerrain(CGameObject *o, OMedia3DVector *pos, OMedia3DVector move, bool test_edges)	// 0x47b4b0
{
	float len = move.quick_magnitude();
	if (!o->f_42c && !(len >= kEpsilon)) return;
	s_sweep_calls++;
	OMedia3DVector dest(move.x + pos->x, move.y + pos->y, move.z + pos->z, 1.0f);
	float r = o->v151();
	double reach = (double)r + len;							// (kept on the x87 stack)
	bool hit = false;
	short x0 = Cell(dest.x - reach), a = Cell(pos->x - reach);
	if (a < x0) x0 = a;
	short x1 = Cell(dest.x + reach); a = Cell(reach + pos->x);
	if (a > x1) x1 = a;
	short z0 = Cell(pos->z - reach); a = Cell(dest.z - reach);
	if (!(z0 < a)) z0 = a;
	short z1 = Cell(reach + pos->z); a = Cell(dest.z + reach);
	if (z1 < a) z1 = a;
	for (short x = x0; x <= x1; x++)
		for (short z = z0; z <= z1; z++)
		{
			if (!InGrid(x, z)) continue;
			PolyList &cell = s_cell_walls[x][z];
			for (PolyList::iterator i = cell.begin(); i != cell.end(); i++)
			{
				s_sweep_tests++;
				if (!g_phys_terrain_on) continue;
				TerrainPoly *t = *i;
				OMedia3DVector n = t->poly->get_normal();
				if (o->f_42c) s_allow_low_edges = 1;
				SphereHitsPolygon(t->poly, t->shape, n, *pos, &dest, &hit, r, test_edges);
				s_allow_low_edges = 0;
			}
		}
	if (hit)
	{
		// sweep again towards the corrected end (no depth limit, as in the original)
		PhysicsMoveAgainstTerrain(o, pos, OMedia3DVector(dest.x - pos->x, dest.y - pos->y, dest.z - pos->z, move.w), test_edges);
		return;
	}
	*pos = dest;
	OMedia3DPolygon *wall = s_last_wall;
	if (wall)
	{
		OMedia3DVector n = wall->get_normal();
		if (n.y > -kSteep) o->v220(n.x, n.y, n.z, n.w, wall);	// vtable +0x370: wall or floor
		else o->v219(n.x, n.y, n.z, n.w, wall);				// vtable +0x36c: ceiling
	}
}

void neg::PhysicsSnapToGround(CGameObject *o)				// 0x47b890
{
	if (!g_phys_terrain_on) return;
	float r = o->v151();
	float step = o->v147();									// step height, in radii
	float step_h = step * r;
	OMedia3DVector acc = Acceleration(o);
	OMedia3DVector vel = Velocity(o);
	float rot[4];
	o->v201(rot);											// (fetched and not used)
	OMedia3DVector prev = PrevPosition(o);
	OMedia3DVector pos = Position(o);
	float top = (pos.y > prev.y) ? pos.y : prev.y;
	float h;
	if (PhysicsClosestHit(prev, pos, false))				// something between: from the hit
		h = (Sub(pos, prev).quick_magnitude() - Sub(g_phys_ray.end, prev).quick_magnitude()) + top;
	else
		h = top;
	float s = (FEq(step, 1.0f) || !(step >= 0.1f)) ? r : step_h;
	OMedia3DVector from(pos.x, s + h, pos.z, 1.0f);
	OMedia3DVector to(pos.x, pos.y - (r + r), pos.z, 1.0f);
	if (!PhysicsClosestHit(from, to, false)) return;
	float floor_y = g_phys_ray.end.y + step_h;
	if (o->v129() && pos.y > floor_y) return;				// only pulled down
	o->v188(0, floor_y, 0);
	o->v197(pos.x, floor_y, pos.z);							// position on the floor
	o->v157(acc.x, 0, acc.z);
	if (!s_was_on_floor) o->v222();							// landed (vtable +0x378)
	o->v176(vel.x, 0, vel.z);
	o->v134(1);
	o->v210(s_hit_shape);									// vtable +0x348
	o->v206(s_hit_poly);									// vtable +0x338
}

// --------------------------------------------------------------------------- ray tests

bool neg::PhysicsLineBlocked(OMedia3DVector a, OMedia3DVector b, bool two_sided)	// 0x47c210
{
	if (!g_phys_terrain_on) return false;
	if (PhysicsLineHitsWall(a, b, two_sided)) return true;
	return PhysicsLineHitsTerrain(a, b, two_sided);
}

// 0x47c280 / 0x47c4b0: the first polygon of the cells under the segment that it crosses
static bool LineHitsCells(PolyList (*cells)[GRID], OMedia3DVector a, OMedia3DVector b, bool two_sided)
{
	if (!g_phys_terrain_on) return false;
	s_hit_poly = NULL;
	s_hit_shape = NULL;
	g_phys_ray = MakeRay(a, b);
	short cb = Cell(b.x), ca = Cell(a.x);
	short x0 = (ca < cb) ? ca : cb, x1 = (ca > cb) ? ca : cb;
	cb = Cell(b.z); ca = Cell(a.z);
	short z0 = (ca < cb) ? ca : cb, z1 = (ca > cb) ? ca : cb;
	for (short x = x0; x <= x1; x++)
		for (short z = z0; z <= z1; z++)
		{
			if (!InGrid(x, z)) continue;
			PolyList &cell = cells[x][z];
			for (PolyList::iterator i = cell.begin(); i != cell.end(); i++)
			{
				TerrainPoly *t = *i;
				s_ray_tests++;
				float tt;
				if (RayHitsPolygon(&g_phys_ray, t->shape, t->poly, OMedia3DVector(0, 0, 0, 1.0f), &tt, two_sided))
				{
					s_hit_shape = t->shape;
					s_hit_poly = t->poly;
					return true;
				}
			}
		}
	return false;
}

bool neg::PhysicsLineHitsWall(OMedia3DVector a, OMedia3DVector b, bool two_sided)	// 0x47c280
{
	return LineHitsCells(s_cell_walls, a, b, two_sided);
}

bool neg::PhysicsLineHitsTerrain(OMedia3DVector a, OMedia3DVector b, bool two_sided)	// 0x47c4b0
{
	return LineHitsCells(s_cell_polys, a, b, two_sided);
}

bool neg::PhysicsClosestHit(OMedia3DVector a, OMedia3DVector b, bool two_sided)		// 0x47c6e0
{
	if (!g_phys_terrain_on) return false;
	CPhysicsRay best_ray;
	best_ray.origin.w = 1.0f; best_ray.end.w = 1.0f;
	s_hit_poly = NULL;
	s_hit_shape = NULL;
	g_phys_ray = MakeRay(a, b);
	float best = g_phys_ray.dir.quick_magnitude();
	short cb = Cell(b.x), ca = Cell(a.x);
	short x0 = (ca < cb) ? ca : cb, x1 = (ca > cb) ? ca : cb;
	cb = Cell(b.z); ca = Cell(a.z);
	short z0 = (ca < cb) ? ca : cb, z1 = (ca > cb) ? ca : cb;
	for (short x = x0; x <= x1; x++)
		for (short z = z0; z <= z1; z++)
		{
			if (!InGrid(x, z)) continue;
			PolyList &cell = s_cell_polys[x][z];
			for (PolyList::iterator i = cell.begin(); i != cell.end(); i++)
			{
				TerrainPoly *t = *i;
				CPhysicsRay ray = g_phys_ray;
				s_ray_tests++;
				float tt;
				if (RayHitsPolygon(&ray, t->shape, t->poly, OMedia3DVector(0, 0, 0, 1.0f), &tt, two_sided))
				{
					float d = Sub(ray.end, ray.origin).quick_magnitude();
					if (!(d >= best))
					{
						best = d;
						best_ray = ray;
						s_hit_shape = t->shape;
						s_hit_poly = t->poly;
					}
				}
			}
		}
	if (!s_hit_poly) return false;
	g_phys_ray = best_ray;
	return true;
}

OMedia3DPolygon *neg::PhysicsHitPolygon(void)	{ return s_hit_poly; }
OMedia3DShape *neg::PhysicsHitShape(void)		{ return s_hit_shape; }

// --------------------------------------------------------------------------- debug markers

static void SetupCube(CGameObject *o, OMedia3DVector colour, float size)
{
	C3DCube *c = dynamic_cast<C3DCube *>(o);				// (the original: o - 0xc0)
	if (c) c->InitCube(colour, size);						// element +0xdc
}

static void SetupSphere(CGameObject *o, OMedia3DVector colour, float size)
{
	C3DSphere *s = dynamic_cast<C3DSphere *>(o);			// (the original: o - 0xc0)
	if (s) s->InitSphere(colour, size);						// element +0xe0
}

// 0x47cac0 / 0x47cc60: the next object of the pool (a new one when it is used up) at pos, shown.
// Quirk kept: after a new object is added the cursor moves on from the list head, i.e. back to
// the first object, so the next call of the frame reuses that one.
static CGameObject *PlaceMarker(ObjectList &pool, ObjectList::iterator &next, int id, bool sphere,
								OMedia3DVector pos, float size)
{
	CGameObject *o;
	if (next == pool.end())
	{
		o = CreateObject(id);								// 0x459cb0 (the element is at result - 0xc0)
		if (o)
		{
			if (sphere) SetupSphere(o, s_sphere_colour, size);
			else SetupCube(o, s_cube_colour, size);
		}
		pool.push_back(o);
		next = pool.begin();								// head->next
	}
	else
	{
		o = *next;
		next++;
	}
	if (!o) return NULL;
	o->v196(pos.x, pos.y, pos.z, pos.w);					// CGameObject vtable +0x310
	OMediaElement *e = dynamic_cast<OMediaElement *>(o);
	if (e && !e->getvisible()) e->hide(false);				// element +0x58 when the hide count is not 0
	return o;
}

CGameObject *neg::PhysicsDebugCube(OMedia3DVector pos, float size)		// 0x47cac0
{
	return PlaceMarker(s_debug_cubes, s_next_cube, '3CUB', false, pos, size);
}

CGameObject *neg::PhysicsDebugSphere(OMedia3DVector pos, float size)	// 0x47cc60
{
	return PlaceMarker(s_debug_spheres, s_next_sphere, '3SPH', true, pos, size);
}

void neg::PhysicsDebugText(const char *fmt, ...)			// 0x47ce00
{
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof buf, fmt, ap);					// (vsprintf in the original)
	va_end(ap);
	DrawText(0, (short)(s_debug_line * 15 + 0x41), 'SMAL', "%s", buf);	// 0x468660 = DrawText
	s_debug_line++;
}

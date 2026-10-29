#include "PortLoadTiming.h" // port: load timing
/*  C3DObject - see C3DObject.h. Reconstructed from Neutron.exe. */
#include "C3DObject.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "Physics.h"
#include "OMediaWorld.h"
#include "OMediaLayer.h"
#include "OMediaViewPort.h"
#include "OMedia3DShape.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include "OMediaCanvas.h"
#include "OMediaTrigo.h"
#include "CGameType.h"
#include <math.h>
#include <stdlib.h>

using namespace neg;


C3DObject::C3DObject()										// 0x462040
{
	SetName((char *)"C3DOBJECT");
	Log("C3DObject()");
	SetObjectID('3OBJ');									// 0x462220 (direct call): element +0x78, CGameObject +0x404
	f_574 = 0;
}

C3DObject::~C3DObject()										// 0x462170
{
	Log("~C3DObject()");
}

void C3DObject::set_shape(OMedia3DShape *shape)				// 0x462230
{
	OMedia3DShapeElement::set_shape(shape);
	if (shape) FitToShape();								// element vtable +200
}

void C3DObject::SetObjectID(int id)							// 0x462220
{
	OMediaElement::set_id(id);
	m_object_id = id;
}

void C3DObject::FitToShape(void)							// 0x462270: radius and box from the shape
{
	OMedia3DShape *sh = shape;								// element +0x8c (read directly, not get_shape)
	f_414 = sh;
	v67(sh->get_radius());									// CGameObject +0x10c: radius (visual sphere +0x4c)
	float minx, miny, minz, maxx, maxy, maxz;
	sh->find_cube(minx, miny, minz, maxx, maxy, maxz);		// shape vtable +0x44
	v185(minx, miny, minz, 1.0f);							// +0x2e4: box corner f_1d4
	v183(maxx, maxy, maxz, 1.0f);							// +0x2dc: box corner f_1c4
}

// 0x4626f0 / 0x462750: texture filtering of every textured polygon of the shape on (linear) or
// off (nearest). Nothing in windowed mode (g_windowed 0x509a13). No NULL test of the shape, as
// in the original.
static void SetShapeFiltering(OMedia3DShape *sh, omt_CanvasFiltering f)
{
	if (g_windowed) return;
	omt_PolygonList *polys = sh->get_polygons();			// shape +0x88 / +0x8c, 0x30 per polygon
	for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); ++p)
	{
		OMedia3DMaterial *m = (*p).get_material();			// polygon +4
		if (m && m->get_texture())							// material +0x80
			m->get_texture()->set_filtering(f, f);			// canvas +0x60, +0x64; +0x24 vtable +0x10
	}
}

void C3DObject::ev51(void) { SetShapeFiltering(get_shape(), omtfc_Linear); }	// 0x4626f0
void C3DObject::ev52(void) { SetShapeFiltering(get_shape(), omtfc_Nearest); }	// 0x462750

void C3DObject::ev53(void)									// 0x4627b0: scale from the shape's height
{
	if (get_shape())
	{
		float minx, miny, minz, maxx, maxy, maxz;
		get_shape()->find_cube(minx, miny, minz, maxx, maxy, maxz);
		v64(fabsf(miny) / get_shape()->get_radius());		// CGameObject +0x100
	}
}

void C3DObject::InitObject(void)							// 0x462340
{
	Log("InitObject()");
	CLocalGameObject::InitObject();
	link(g_world);											// element +0x2c
	link_layer(g_layer_world);								// element +0x34 (0x509a30)
}

void C3DObject::UnInitObject(void)							// 0x462390
{
	Log("UnInitObject()");
	CLocalGameObject::UnInitObject();
	set_shape(NULL);										// element +0xac
	f_414 = NULL;
}

void C3DObject::InitPhysics(void)							// 0x4623d0
{
	PortLoadTiming::Phase load_physics("object-physics"); // port: load timing
	Log("InitPhysics()");
	CLocalGameObject::InitPhysics();
	v90(x, y, z);											// position from the element (+0x44..)
	const float k = 360.0f / 16384.0f;						// 0x48e5b4: OMT angle units -> degrees
	v92((float)anglex * k, (float)angley * k, (float)anglez * k);
	PhysicsAddObject(this);									// 0x479a40 Add3DObject
}

void C3DObject::UnInitPhysics(void)							// 0x462480
{
	Log("UnInitPhysics()");
	CGameObject::UnInitPhysics();
}

// 0x476ff0: angles (degrees, 0..360) that tilt an object onto a floor whose normal is n, keeping
// its heading (*ay, in degrees: read, and overwritten with the computed value). The three tilt
// angles come from the cos table's acos part, as OMediaCosSinTable::atan2 does (|n| components,
// sign fixed afterwards). Quirk: when n.z (resp. n.y) is 0 the original uses the low 16 bits of
// its last argument (a pointer) as the angle; port: 0. Negative results are folded to
// 360 - |(int)a| (the fraction is lost), results above 360 lose 360.
static omt_Angle FloorTiltAngle(float adj, float opp)		// |adj|, |opp|
{
	if (fabsf(opp) == 0.0 && fabsf(adj) == 0.0) return 0;
	const double a2 = (double)adj * adj;
	const int t = (int)(a2 * -16384.0 / ((double)opp * opp + a2));	// x87: no float rounding
	return OMediaCosSinTable::global.get_acostab()[-t];
}

static float FoldDegrees(double a)
{
	if (a < 0.0) return (float)(360 - abs((int)a));
	if (a > 360.0) return (float)(a - 360.0);
	return (float)a;
}

void neg::FloorNormalToAngles(const OMedia3DVector &n, float *ax, float *ay, float *az)	// 0x476ff0
{
	short a = 0, b = 0, c = 0;
	if (n.z != 0.0f)
	{
		a = FloorTiltAngle(fabsf(n.z), fabsf(n.x));
		if ((n.x < 0.0f) != (n.z < 0.0f)) a = -a;
	}
	if (n.y != 0.0f)
	{
		b = FloorTiltAngle(fabsf(n.y), fabsf(n.z));
		if ((n.z < 0.0f) != (n.y < 0.0f)) b = -b;
		c = FloorTiltAngle(fabsf(n.y), fabsf(n.x));
		if ((n.x < 0.0f) != (n.y < 0.0f)) c = -c;
	}
	// rotate (a, b, c) by the heading; the x87 code keeps everything in extended precision
	const int yaw = (int)((double)*ay * (double)(16384.0f / 360.0f)) & 0x3fff;	// 0x48e5e8
	const double sy = omd_Sin(yaw), cy = omd_Cos(yaw);
	const double s0 = omd_Sin(0), c0 = omd_Cos(0);
	const double x1 = c0 * a - s0 * b;
	const double y1 = c0 * b + s0 * a;
	const double z2 = c0 * (float)c - x1 * s0;
	const double p = y1 * cy - z2 * sy;
	const double q = y1 * sy + z2 * cy;
	const double r = x1 * c0 + (float)c * s0;
	const double k = 360.0f / 16384.0f;									// 0x48e5b4
	*ax = FoldDegrees(-(p * k));
	*ay = FoldDegrees(r * k);
	*az = FoldDegrees(q * k);
}

void C3DObject::Process(float dt)							// 0x462520
{
	if (GameIsRunning() && v131() && v205())				// physics on, standing on a polygon
	{
		OMedia3DPolygon *poly = v205();
		OMedia3DMaterial *mat = poly->get_material();		// polygon +4
		OMediaCanvas *tex = mat ? mat->get_texture() : NULL;	// material +0x80
		g_gametype->TouchSurface(this, mat, tex, dt);		// vtable +0x138
		if (v121())											// align to the floor
		{
			poly = v205();
			float rot[4];
			v201(rot);										// rotation (degrees)
			OMedia3DVector n = poly->get_normal();			// polygon +8
			float ax = rot[0], ay = rot[1], az = rot[2];
			FloorNormalToAngles(n, &ax, &ay, &az);
			v203(ax, rot[1], az);							// the heading is kept, the computed one dropped
		}
	}
	CLocalGameObject::Process(dt);
}

// 0x462650: put the viewport behind the object (camera slot, called with dt).
void C3DObject::v243(float dt)
{
	(void)dt;
	const float r = v151();
	OMedia3DVector p = v224(0.0f, r, r * -2.0f);			// 0x4d36d4 = -2: above and behind
	g_viewport->place(p.x, p.y, p.z);						// viewport +0x44..
	float rot[4];
	v201(rot);
	const double kAngle = 16384.0f / 360.0f;				// 0x48e5e8 (x87: product not rounded)
	g_viewport->set_angle((omt_WAngle)(int)(rot[0] * kAngle), (omt_WAngle)(int)(rot[1] * kAngle), 0);	// +0x50
}

void C3DObject::v247(void)					// 0x4624b0
{
	float p[4];
	v195(p);												// CGameObject +0x30c: current position
	x = p[0]; y = p[1]; z = p[2];
}

void C3DObject::v248(void)									// 0x4624f0: jmp to CGameObject's (0x473540)
{
	CGameObject::v248();
}

void C3DObject::v249(short ax, short ay, short az)	// 0x462500
{
	anglex = ax; angley = ay; anglez = az;
}

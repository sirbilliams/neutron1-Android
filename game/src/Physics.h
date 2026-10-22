/*  CPhysics - movement and collision of the game objects (Neutron.exe 0x478fa0..N2:0x4b7d60, the
    original's log strings call it "CPhysics::"; it is a set of free functions over globals).

    * Objects: every C3DObject puts itself in the physics list when its physics starts
      (PhysicsAddObject, from InitPhysics); the CGameObject destructor takes it out again.
      PhysicsProcess(dt) runs once a frame (CViewPort): each object is moved by its velocity,
      against the terrain (PhysicsMoveObject), then all pairs are tested against each other and
      told about touches (PhysicsCollideObjects -> IsTouching/AddTouching/v18/RemoveTouching and
      Collide(other, distance)).
    * Terrain: the polygons of the terrain shapes (PhysicsAddTerrain, from C3DTerrain) are kept in
      a 260 x 260 grid of 500-unit cells centred on the origin, one list with every polygon and one
      with the steep ones only (normal.y < 0.7: walls and ceilings). Objects are spheres (radius
      v151) swept against the steep polygons; the floor is found by a ray cast down
      (PhysicsSnapToGround).
    * Ray tests for the game objects: PhysicsLineBlocked (line of sight), PhysicsClosestHit
      (nearest polygon along a segment). Their result is g_phys_ray: the segment, and its end
      moved to the hit point (g_phys_ray.end = 0x5cfc60, read directly by many objects).

    CGameObject members the physics works on (named f_<offset> in gen/CGameObject_fields.inc):
      f_214 v195/v196 position          f_1f4 v189/v190 position at the start of the move
      f_1a4 v174/176/178 velocity       f_174 v160/162 impulse (added to the velocity once)
      f_164 v155/157 constant acceleration (x dt)   f_194 v170/172/173 angular velocity (deg/s)
      f_224 v201 rotation (degrees)      f_15c v151 radius     f_430 time scale
      f_1e4 v188 floor position (y: floor height, -25000 = none)
      f_150 v145 friction   f_144 v139 rest threshold   f_154 v147 step height (x radius)
      f_160 v153 0 = friction and resting apply   f_124 v93 collision type (2 = box)
      f_1c4 v182 / f_1d4 v184 box corners (local)  f_234 v206 floor polygon  f_23c v210 floor shape
      flags: f_13e v131 physics on, f_140 collides, f_13c v115 swept against walls,
             f_13b v113 snaps to the floor, f_13f v133/v134 on the floor, f_141 v137 frozen,
             f_142 gravity, f_13a v129 snap down only, f_42c sweep even when not moving.

    Floating point: the original keeps intermediate values on the x87 stack (extended precision);
    the grid-cell computations use double here for the same reason, the rest stays in float. */
#pragma once
#include <list>
#include "OMedia3DVector.h"

class CGameObject;
class OMedia3DShape;
class OMedia3DPolygon;

// A segment for the ray tests (0x30 bytes): its start, the direction (end - start, w = 0) and its
// end, which a hit moves to the hit point.
class CPhysicsRay
{
	public:
	OMedia3DVector	origin;		// +0x00
	OMedia3DVector	dir;		// +0x10
	OMedia3DVector	end;		// +0x20  hit point after a successful test
};

#include "gen/globals.h"		// g_gravity, g_phys_ray, ... (tools/decomp/global_types/physics.txt)

namespace neg {

extern std::list<CGameObject *> g_physics_objects;			// 0x695d38  objects PhysicsProcess moves

// --- objects
void PhysicsAddObject(CGameObject *o);						// 0x479a40  "Add3DObject(%s)"
// void PhysicsRemoveObject(CGameObject *o);				// 0x478fa0  declared in CGameObject.h
void PhysicsReset(void);									// 0x479000  "CPhysics::Reset()": empty everything
void PhysicsProcess(float dt);								// N2:0x4b4c60  one frame

// --- terrain
void PhysicsAddTerrain(OMedia3DShape *shape);				// 0x479140  "Add3DTerrain()"
void PhysicsClearTerrain(void);								// 0x479920  "ClearPolyTerrains()"

// --- parts of a frame, also called by some objects directly
void PhysicsMoveObject(CGameObject *o, float dt);			// 0x479d20
void PhysicsApplyFriction(CGameObject *o, float dt);		// 0x47a330
void PhysicsCollideObjects(float dt);						// N2:0x4b54d0  "TOUCH=%d TOUCHING=%d UNTOUCH=%d"
bool PhysicsCollideWithBox(CGameObject *box, CGameObject *o);	// 0x47a5e0  o against box's 12 triangles
// Move the sphere of o from *pos by move, sliding along the steep terrain polygons; *pos gets
// the end position. test_edges: also push away from polygon edges (walls only).
// Callers that start a new sweep set g_closest_face_t = 130000 first (PhysicsMoveObject, cars).
void PhysicsMoveAgainstTerrain(CGameObject *o, OMedia3DVector *pos, OMedia3DVector move, bool test_edges);	// 0x47b4b0
void PhysicsSnapToGround(CGameObject *o);					// 0x47b890

// --- ray tests (results in g_phys_ray / PhysicsHitPolygon / PhysicsHitShape)
bool PhysicsLineBlocked(OMedia3DVector a, OMedia3DVector b, bool two_sided);		// 0x47c210  walls, then all
bool PhysicsLineHitsWall(OMedia3DVector a, OMedia3DVector b, bool two_sided);	// 0x47c280  first steep polygon hit
bool PhysicsLineHitsTerrain(OMedia3DVector a, OMedia3DVector b, bool two_sided);	// 0x47c4b0  first polygon hit
bool PhysicsClosestHit(OMedia3DVector a, OMedia3DVector b, bool two_sided);		// 0x47c6e0  nearest polygon hit
OMedia3DPolygon *PhysicsHitPolygon(void);					// 0x695d54  polygon of the last ray hit
OMedia3DShape *PhysicsHitShape(void);						// 0x695d58  its shape

// --- debug markers (only drawn when the debug switches 0x695d44/0x695d45 are set - never in
//     the shipped game): pooled '3CUB' / '3SPH' objects placed at a point, hidden each frame
CGameObject *PhysicsDebugCube(OMedia3DVector pos, float size);	// 0x47cac0
CGameObject *PhysicsDebugSphere(OMedia3DVector pos, float size);	// 0x47cc60
void PhysicsDebugText(const char *fmt, ...);				// 0x47ce00  one line of the debug list

}

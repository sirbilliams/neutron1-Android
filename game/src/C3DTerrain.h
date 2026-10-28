/*  C3DTerrain ('3TER') - one shape of a level's terrain database. The game type's CreateTerrain
    makes one per shape (CJimmyGame 0x44d4f0 / 0x44d810 / 0x44dbf0, CGameType 0x475550 /
    0x4757a0), gives it the shape, and InitPhysics puts the shape's polygons into the physics
    terrain grid (PhysicsAddTerrain). The element is drawn in the world layer.

    Reconstructed from Neutron.exe: constructor 0x4648f0, destructor 0x464a20 (thunks 0x464b70 /
    0x464b60 / 0x4649f0 / 0x464b80). Same layout as C3DObject (0x590 bytes + the streamer,
    factory size 0x594), no members of its own. The element vtable (0x4d516c) is C3DObject's
    unchanged; the CGameObject vtable (0x4d4ccc) overrides slots 0, 7, 11 and 240.            */
#pragma once
#include "C3DObject.h"

class C3DTerrain : public C3DObject
{
	public:
	C3DTerrain();															// 0x4648f0
	virtual ~C3DTerrain();													// SLOT 0  0x464a20

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x464ac0
	virtual void InitPhysics(void);											// SLOT 11 0x464b00
	virtual void Process(float dt);											// SLOT 241 0x4682e0 (thunk to C3DObject::Process)
};

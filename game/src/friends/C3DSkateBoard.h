/*  C3DSkateBoard - Nick's skateboard: an OMT morph object ("board.ase", texture "skate.png",
    animation BOARD) without collisions or physics, created by C3DNick's constructor and moved
    under him while he patrols. No factory ID ('3BOA' is set as its ObjectID).
    Reconstructed from Neutron.exe: constructor 0x4402a0 (0x6c0 bytes), destructor 0x4403f0
    (C3DAnimated's, inlined; deleting thunks 0x4403c0 / 0x440500 / 0x440510 / 0x440520).

    Original layout: as C3DAnimated (element vtable, 54 slots; CLocalGameObject at +0xc0, only slot 7 overridden), no members
    of its own: vtordisp 0x6bc, streamer 0x6c0.                                   */
#pragma once
#include "../C3DAnimated.h"

class C3DSkateBoard : public C3DAnimated
{
	public:
	C3DSkateBoard();														// 0x4402a0
	virtual ~C3DSkateBoard();												// 0x4403f0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x440440 the animation and the texture
};

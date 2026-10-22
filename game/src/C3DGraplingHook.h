/*  C3DGraplingHook - the rope of Jimmy's grappling hook: an .ase strip ("rope01.ase",
    "jimycarl.png"), stretched along z (C3DJimmy scales its
    shape) from Jimmy's hand towards the anchor. C3DJimmy's constructor makes the one rope
    (f_b0c). No factory ID (ObjectID '3GRA'; the name keeps the original's spelling).
    Reconstructed from Neutron.exe: constructor 0x41eeb0 (0x6c4 bytes), destructor 0x41f010
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x41f180 / 0x41efe0 / 0x41f190
    / 0x41f170). InitObject (slot 7, 0x41f060) is its only own slot: Neutron.exe's rope does not
    scroll its texture or turn translucent (Negatron's did, in a Process and a v260 here).

    Original layout: as C3DAnimated (72 element slots, listener at +0x30, CLocalGameObject
    at +0xc0), no members of its own (vtordisp 0x6bc, streamer 0x6c0).                         */
#pragma once
#include "C3DAnimated.h"

class C3DGraplingHook : public C3DAnimated
{
	public:
	C3DGraplingHook();														// 0x41eeb0
	virtual ~C3DGraplingHook();												// 0x41f010

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x41f060 the rope animation and texture, turned 180
};

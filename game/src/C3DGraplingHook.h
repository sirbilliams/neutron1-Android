/*  C3DGraplingHook - the rope of Jimmy's grappling hook: a translucent .ase strip ("rope01.ase",
    "laser.png", its texture scrolling while shown), stretched along z (C3DJimmy scales its
    shape) from Jimmy's hand towards the anchor. C3DJimmy's constructor makes the one rope
    (f_d20). No factory ID (ObjectID '3GRA'; the name keeps the original's spelling).
    Reconstructed from Neutron.exe: constructor 0x415760 (0x92c bytes), destructor 0x41f010
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x41f180 / 0x41efe0 / 0x41f190
    / 0x41f170).

    Original layout: as C3DAnimated (element vtable N2:0x4e02b4, 94 slots; OMediaAnim vtable
    N2:0x4e0264 at +0x90; listener N2:0x4e02a0 at +0x30; CLocalGameObject vtable N2:0x4dfde8 at +0xc0),
    no members of its own (vtordisp 0x924, streamer 0x928).                                    */
#pragma once
#include "C3DAnimated.h"

class C3DGraplingHook : public C3DAnimated
{
	public:
	C3DGraplingHook();														// 0x415760
	virtual ~C3DGraplingHook();												// 0x41f010

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x41f060 the rope animation and texture, turned 180
	virtual void Process(float dt);											// SLOT 240 N2:0x43d030 scroll the texture while shown
	virtual void v260(void);												// SLOT 260 N2:0x43d070 translucent (0.4)
};

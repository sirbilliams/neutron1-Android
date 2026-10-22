/*  C3DFireStrato - the exhaust flame of the rocket ship (the "Strato" XL): a morph-animated
    C3DAnimated with the animations DEFAULT ("firestrato.ase") and AFTERBURN ("firestrato2.ase")
    and the texture "fire.png", additive, no collision, ObjectID '3FST'. C3DRocketShip creates one
    and moves it with itself; no factory ID.
    Reconstructed from Neutron.exe: constructor 0x4192e0 (0x92c bytes), destructor 0x419480
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x419450 / 0x419520 / 0x419530 /
    0x419540).

    Original layout: as C3DAnimated (element vtable 0x49c274, 94 slots; CLocalGameObject vtable
    0x49bdd4 at +0xc0), no members of its own: vtordisp 0x924, streamer 0x928.             */
#pragma once
#include "C3DAnimated.h"

class C3DFireStrato : public C3DAnimated
{
	public:
	C3DFireStrato();														// 0x4192e0
	virtual ~C3DFireStrato();												// 0x419480

	// ---- CGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10 0x4194d0 + hidden
};

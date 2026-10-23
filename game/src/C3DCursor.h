/*  C3DCursor - a 3D arrow ("3darrow.ase" scaled x5, "3darrow.png", additive, facing the
    viewport) that points the way; C3DGoddard keeps two (f_1b80, f_1b84: towards the target
    the AI trigger "ShowTarget" gives Jimmy, and his marker in the rocket ship). ObjectID '3CUR',
    no factory ID.
    Reconstructed from Neutron.exe: constructor 0x41eeb0 (0x92c bytes), destructor 0x4158b0
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4159e0 / 0x415880 / 0x4159f0
    / 0x4159d0).

    Original layout: as C3DAnimated (element vtable 0x497ac0, 72 slots; OMediaAnim vtable
    0x497a70 at +0x90; listener 0x497aac at +0x30; CLocalGameObject vtable 0x497620 at +0xc0),
    no members of its own (vtordisp 0x924, streamer 0x928).                                    */
#pragma once
#include "C3DAnimated.h"

class C3DCursor : public C3DAnimated
{
	public:
	C3DCursor();															// 0x41eeb0
	virtual ~C3DCursor();													// 0x4158b0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x415900 the arrow animation and texture
};

/*  C3DTeleportFX ('3TEL') - the teleporter effect in Jimmy's lab: the morph animation HITEL
    ("sfxteleport.ase", "sfxteleport.png", played as TEL), half transparent,
    no collision, shown or hidden by the cut scenes' ToggleObject / AIHideObj.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x445600 (factory '3TEL',
    0x6c8 bytes), destructor 0x4457c0 (only resets the vtables, then ~C3DAnimated; deleting
    thunks 0x445790 / 0x445840 / 0x445850 / 0x445860). Game object vtable (+0xc0): only the
    destructor and 241 (C3DPhoneBooth's shared 0x448bc0: C3DAnimated's Process) differ from
    C3DAnimated's; streamer: C3DLabScreen's. Own member 0x6bc (0), vtordisp 0x6c0, streamer
    0x6c4.                                                                                     */
#pragma once
#include "../C3DAnimated.h"

class C3DTeleportFX : public C3DAnimated
{
	public:
	C3DTeleportFX();														// 0x445600
	virtual ~C3DTeleportFX();												// 0x4457c0

	virtual void Process(float dt);											// SLOT 241 0x448bc0 (shared) C3DAnimated's

	int					f_6bc;			// 0x6bc  (0; not used)
};

/*  C3DTractorBeam ('3TRC') - the Yokian tractor beam of level 4c: a hidden morph object
    ("yrayupdown.ASE" HIUP / "yraystop.ASE" HISTOP, textures "yray1.png" / "yray2.png", radius 10,
    played as STOP at alpha 0.5, no collision response) that, once shown (NextState 1), flips
    between its two textures every 0.15 s and pulses the colour of the material it shows
    (sin of the time, sin of the time, 1, alpha 0.2).
    Reconstructed from Neutron.exe: constructor 0x4462c0 (factory '3TRC', 0x6d0 bytes),
    destructor 0x446420 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4463f0 /
    0x446660 / 0x446670 / 0x446680). Vtables: element 0x4bbad8 (C3DAnimated's 72 slots), game
    object (+0xc0) (0, 7, 241 overridden). Own members 0x6bc..0x6c4, vtordisp 0x6c8.     */
#pragma once
#include "../C3DAnimated.h"

class C3DTractorBeam : public C3DAnimated
{
	public:
	C3DTractorBeam();														// 0x4462c0
	virtual ~C3DTractorBeam();												// 0x446420

	virtual void InitObject(void);											// SLOT 7   0x446470 animations, textures
	virtual void Process(float dt);											// SLOT 241 0x446540 texture flip, colour pulse

	// ---- data members (object offsets)
	int					f_6bc;			// 0x6bc  texture shown (0 / 1)
	float				f_6c0;			// 0x6c0  time since the last flip
	float				f_6c4;			// 0x6c4  running time
};

/*  C3DShrinkRay ('3SHR') - the beam of Jimmy's shrink ray (C3DJimmy f_a10): the morph object
    "ray.ase" with three half-transparent textures (ray0000..0002.png) that it cycles through
    every 0.1 s. Jimmy shows it in front of him while he fires (slot 110) and hides it after.
    Reconstructed from Neutron.exe: constructor 0x43fe20 (0x6d0 bytes), destructor 0x43ff70
    (deleting thunks 0x440260 / 0x440250 / 0x43ff40 / 0x440270..0x440290). Vtables 0x4b47f0
    (element, 72 slots: C3DAnimated's) / 0x4b4350 (game object, at +0xc0; 0, 7, 241 own,
    259 C3DAnimated's) / 0x4b47a0 (OMediaAnim) / 0x4b47dc (listener) / 0x4b4340 (streamer).
    Own members 0x6bc..0x6c8, vtordisp 0x6c8, streamer 0x6cc.                                  */
#pragma once
#include "../C3DAnimated.h"

class C3DShrinkRay : public C3DAnimated
{
	public:
	C3DShrinkRay();															// 0x43fe20
	virtual ~C3DShrinkRay();												// 0x43ff70

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x440000 the animation and the three textures
	virtual void Process(float dt);											// SLOT 241 0x440180 the next texture every 0.1 s

	short				f_6bc;			// 0x6bc  the texture shown (0..2)
	float				f_6c0;			// 0x6c0  time since the last change
	unsigned char		f_6c4;			// 0x6c4  InitObject has run again from Process
};

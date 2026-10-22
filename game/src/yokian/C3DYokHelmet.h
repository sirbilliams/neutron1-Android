/*  C3DYokHelmet ('3YHE') - the Yokian captain's helmet (the dome over the spy's head): a morph
    object ("yokcaphelm.ase", texture "yokcaptn.png", loaded on the first ev69), radius 10,
    no collision, alpha blended (0.8; 0.2 when g_windowed is set). The C3DYokianSpy makes one
    and moves it with itself (C3DYokian keeps it at +0x9a4).
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44a440 (no factory entry:
    made in code, 0x6c4 bytes), destructor 0x44a590 (only resets the vtables, then
    ~C3DAnimated; deleting thunks 0x44a560 / 0x44a700 / 0x44a710 / 0x44a720). Element vtable:
    67 (Negatron's ev69) overridden; game object vtable (+0xc0): 0, 7. No members of its own
    (vtordisp 0x6bc, streamer 0x6c0).                                                         */
#pragma once
#include "../C3DAnimated.h"

class C3DYokHelmet : public C3DAnimated
{
	public:
	C3DYokHelmet();															// 0x44a440
	virtual ~C3DYokHelmet();												// 0x44a590

	virtual void ev69(void);												// ESLOT 67 0x44a5e0 (BG numbering) load the animation (once)
	virtual void InitObject(void);											// SLOT 7   0x44a640
};

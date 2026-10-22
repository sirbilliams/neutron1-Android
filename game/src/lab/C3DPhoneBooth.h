/*  C3DPhoneBooth ('3PHO') - Jimmy's phone booth (in Retroville and in the lab): shape 0 of
    "phone.omt", unlit, no animation, radius 40. Touched by Jimmy it calls his element slot 117
    (0x42abb0), which handles the booth (the level-to-level trip).
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x4355a0 (factory '3PHO', 0x6cc
    bytes), destructor 0x435710 (only resets the vtables, then ~C3DAnimated; deleting thunks
    0x4356e0 / 0x435880 / 0x435890 / 0x4358a0). Game object vtable (+0xc0): 0, 7, 10, 16, 241,
    259 overridden (241 is code shared at 0x448bc0 that calls C3DAnimated's Process; 259 is
    shared too, at C3DAbductee's address); streamer: C3DHydrant's (OMedia3DShapeElement's
    read_class / write_class). Own member 0x6bc (0 in the constructor), vtordisp 0x6c4,
    streamer 0x6c8.                                                                           */
#pragma once
#include "../C3DAnimated.h"

class C3DPhoneBooth : public C3DAnimated
{
	public:
	C3DPhoneBooth();														// 0x4355a0
	virtual ~C3DPhoneBooth();												// 0x435710

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x435780 the phone.omt shape
	virtual void ResetObject(void);											// SLOT 10  0x435760 radius 40
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x435800 Jimmy: his slot 117
	virtual void Process(float dt);											// SLOT 241 0x448bc0 (shared) C3DAnimated's

	int					f_6bc;			// 0x6bc  (0; not used here)
};

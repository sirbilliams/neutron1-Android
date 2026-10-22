/*  C3DFerris ('3FER') - the Ferris wheel of Retroland ("wheel.ase", "wheel.png") and its four
    chairs, C3DFerChair ("chair.ase", "wheel.png"; no factory ID, made by the wheel's
    constructor). The wheel turns about its z axis at 20 degrees / s; every frame each chair is
    put 432 from its centre (+x, -x, +y, -y) and kept hanging upright (rotation (0, the wheel's
    y, 0)).
    Reconstructed from Neutron.exe: C3DFerris constructor 0x418cc0 (factory '3FER', 0x6ec
    bytes), destructor 0x418f10 (only resets the vtables, then ~C3DAnimated; deleting thunks
    0x418ee0 / 0x419290 / 0x4192a0 / 0x4192b0); C3DFerChair constructor 0x418a20 (0x6c4 bytes),
    destructor 0x418b80 (deleting thunks 0x418b50 / 0x418c90 / 0x418ca0 / 0x418cb0).

    Original layout: both as C3DAnimated (element vtable 72 slots, all C3DAnimated's;
    CLocalGameObject vtable at +0xc0: 0, 7, 241 overridden in the wheel, 0, 7 in the chair), the
    wheel's own members 0x6bc..0x6cc (the four chairs), vtordisp 0x6cc, streamer 0x6d0.    */
#pragma once
#include "../C3DAnimated.h"

class C3DFerChair : public C3DAnimated
{
	public:
	C3DFerChair();															// 0x418a20
	virtual ~C3DFerChair();													// 0x418b80

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x418bd0 animation and texture right away
};

class C3DFerris : public C3DAnimated
{
	public:
	C3DFerris();															// 0x418cc0
	virtual ~C3DFerris();													// 0x418f10

	// ---- element-family overrides
	virtual void ev69(void);												// ESLOT 69 0x418f60 load the model once

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x418fc0 C3DAnimated's, then InitAnim
	virtual void Process(float dt);											// SLOT 241 0x419000 turn, place the chairs

	C3DFerChair		*m_chair[4];			// 0x6bc  the chairs (+x, -x, +y, -y)
};

/*  C3DRocket ('3RCK', named "C3DYOKIANSHIP") - a rocket on a launch pad (objects.omt shape 15,
    unlit): an AI that waits (AIState 3) and, set flying by the cut scenes, puffs smoke 10 below
    itself every 0.1 s (ten C3DNewSmokePuffs in turn) while it keeps its initial rotation.
    Reconstructed from Neutron.exe: constructor 0x43d090 (factory '3RCK', 0x9cc bytes),
    destructor 0x43d330 (then ~C3DAI; deleting thunks 0x43d300 / 0x43d4e0 / 0x43d4f0 /
    0x43d500). Vtables 0x4b1ec0 (element, C3DAI's 95 slots) / 0x4b1eac (listener) / 0x4b1e70
    (OMediaAnim) / 0x4b1a20 (game object at +0xc0: 241, 259 overridden) / 0x4b1a10 (streamer
    at +0x9c8). Own members 0x994..0x9c4.                                                    */
#pragma once
#include "../C3DAI.h"

class C3DNewSmokePuff;

class C3DRocket : public C3DAI
{
	public:
	C3DRocket();															// 0x43d090
	virtual ~C3DRocket();													// 0x43d330

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x43d380 smoke while flying (state 3)
	virtual void v260(void);												// SLOT 259 0x43d490 radius 1

	// ---- data members (object offsets)
	C3DNewSmokePuff		*m_puffs[10];	// 0x994  the smoke (the original keeps their CGameObject parts)
	int					m_next_puff;	// 0x9bc  0..9
	float				m_puff_time;	// 0x9c0  time since the last puff
};

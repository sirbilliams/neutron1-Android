/*  C3DSailBoat ('3SAI') - a toy sailboat (objects.omt shape 13, unlit): an AI waiting (AIState
    1) that, while moving (states 2 and 3), floats (no gravity) and rocks (x 5 sin 4t, z 5 sin
    3.5t degrees); the first time it sails it gives 50 points.
    Reconstructed from Neutron.exe: constructor 0x43ecc0 (factory '3SAI', 0x9a4 bytes),
    destructor 0x43eef0 (then ~C3DAI; deleting thunks 0x43eec0 / 0x43f040 / 0x43f050 /
    0x43f060). Vtables 0x4b303c (element, C3DAI's 95 slots) / 0x4b3028 (listener) / 0x4b2fec
    (OMediaAnim) / 0x4b2b9c (game object at +0xc0: 241, 259 overridden; 259 is 0x42d010, a
    jump to C3DAI's) / 0x4b2b8c (streamer at +0x9a0). Own members 0x994..0x99c.               */
#pragma once
#include "../C3DAI.h"

class C3DSailBoat : public C3DAI
{
	public:
	C3DSailBoat();															// 0x43ecc0
	virtual ~C3DSailBoat();													// 0x43eef0

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x43ef40 floating and rocking

	// ---- data members (object offsets)
	float				m_time;			// 0x994  rocking time
	unsigned char		m_sailed;		// 0x998  the 50 points given
};

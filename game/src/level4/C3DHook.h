/*  C3DHook ('3HOO') - the hook of level 4b: a patrolling morph object ("hook.ase" with the
    "fan.png" texture, radius 100) that follows its patrol points (C3DAI state 3, AIState 3, speed
    250), turning at 1 per second about each axis, with Process cancelling gravity on it.
    Reconstructed from Neutron.exe: constructor 0x420060 (factory '3HOO', 0x99c bytes),
    destructor 0x4201f0 (only resets the vtables, then ~C3DAI; deleting thunks 0x4201c0 /
    0x420360 / 0x420370 / 0x420380). Vtables: element 0x4a16f4 (C3DAI's 95 slots), game object
    (+0xc0) 0x4a1254 (0, 7, 241 overridden). No members of its own.                        */
#pragma once
#include "../C3DAI.h"

class C3DHook : public C3DAI
{
	public:
	C3DHook();																// 0x420060
	virtual ~C3DHook();														// 0x4201f0

	virtual void InitObject(void);											// SLOT 7   0x420240 animation, texture
	virtual void Process(float dt);											// SLOT 241 0x4202d0 + gravity cancelled
};

/*  C3DCamel ('3CML') - the camel ride of Retroland (shape 0 of "camel.omt"): a C3DAI in state 3
    (patrol) that walks its points at a speed set by the slope under it: going down it speeds
    up (the downward velocity counts at least 340 / s, up to 3300), going up it slows down
    (the upward velocity counts at least 240 / s, down to 630), and its turn rates follow the
    speed (speed / 1100 x 1.8). Gravity is cancelled (as C3DEye's). Jimmy touching it is put
    away (he is hidden, the camel hidden too, gadgets locked, 25 points), the game type's camera
    becomes the camel and a sound (index 0xd1, looped) plays. v243 then drags the viewport
    (2.5 / 1.8 / 2.5 x dt) to the point (0, 20, 700) from the camel and turns it towards the point
    (0, 0, 1200) from it. A fire or jump key stops the sound and gives Jimmy ("JIM1") the camera
    back at his start position.
    Reconstructed from Neutron.exe: constructor 0x412150 (factory '3CML', 0x9a0 bytes),
    destructor 0x412360 (only resets the vtables, then ~C3DAI; deleting thunks 0x412330 /
    0x412820 / 0x412830 / 0x412840).

    Original layout: as C3DAI (element vtable 95 slots, all C3DAI's; CLocalGameObject vtable at
    +0xc0: 0, 16, 241, 243, 259 overridden), own member 0x994, vtordisp 0x998, streamer 0x99c. */
#pragma once
#include "../C3DAI.h"

class C3DCamel : public C3DAI
{
	public:
	C3DCamel();																// 0x412150
	virtual ~C3DCamel();													// 0x412360

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x4124e0 Jimmy touches: he rides
	virtual void Process(float dt);											// SLOT 241 0x4123d0 C3DAI's, gravity cancelled, speed by the slope
	virtual void v243(float dt);											// SLOT 243 0x4125d0 the camera follows; a key ends the ride
	virtual void v260(void);												// SLOT 259 0x4123b0 C3DAI's, then v152(250)

	int				m_sound;				// 0x994  channel of the ride's sound (-1: none)
};

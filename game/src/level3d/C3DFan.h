/*  C3DFan ('3FAN') - a big fan ("fan.ase", "fan.png") that blows Jimmy away. Properties
    FanSpeed (= 400), FanRange (= 1500), FanOn (= 1). Its blades turn about z at 0.35 x the
    current speed, which runs up (130 / s) or down (200 / s) to FanSpeed while on, 0 while off.
    Within FanRange of the camera (Jimmy) it pushes him (velocity) by (1 - f) x (position - the
    point `speed` ahead on its local z), f = distance / FanRange + (1 - cos of the angle between
    its local z and the line to Jimmy), capped at 1. ToggleObject (v268): 0 off, else on.
    Reconstructed from Neutron.exe: constructor 0x418390 (factory '3FAN', 0x940 bytes),
    destructor 0x4184f0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4189e0 /
    0x4184c0 / 0x4189d0 / 0x4189f0).

    Original layout: as C3DAnimated (element vtable 0x49b0d0, 94 slots, all C3DAnimated's;
    OMediaAnim vtable 0x49b080 at +0x90; listener 0x49b0bc at +0x30; CLocalGameObject vtable
    0x49ac30 at +0xc0), own members 0x924..0x938, vtordisp 0x938, streamer 0x93c. Slot 260 is
    a jump to C3DAnimated's (0x418950), so it is not overridden here.                        */
#pragma once
#include "../C3DAnimated.h"

class C3DFan : public C3DAnimated
{
	public:
	C3DFan();																// 0x418390
	virtual ~C3DFan();														// 0x4184f0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x418540 properties, animation, texture
	virtual void ResetObject(void);											// SLOT 10  0x418920 + the target speed
	virtual void Process(float dt);											// SLOT 240 0x418610 spin, blow the camera
	virtual void v268(int state);											// SLOT 268 N2:0x431ab0 ToggleObject: 0 off, else on

	float		m_fan_speed;			// 0x924  "FanSpeed" (= 400)
	float		m_fan_range;			// 0x928  "FanRange" (= 1500)
	float		m_speed;				// 0x92c  current speed (= 0)
	float		m_target;				// 0x930  speed it runs to (= 0)
	int			m_fan_on;				// 0x934  "FanOn" (= 1)
};

/*  C3DAICar - a car that drives along its patrol points (a C3DAI in state 3, speed 400, turn
    rates 1, 2, 2) with an engine sound (index 6, looped, started once the properties are
    loaded). Hit by Jimmy, the jeep or Goddard it honks ("horn") and stops where it is for 8 s
    (speed 0.0001, held at that point while touched), then drives on at 400. Something more than
    480 above it is thrown off (velocity 700, 700, 200); Jimmy also loses his jump state. The
    base of C3DNeuCar ('3NCA'), C3DNeuCar2 ('3NC2') and C3DBus ('3SBU'), and of C3DEye ('3EYE');
    no factory ID of its own.
    Reconstructed from Neutron.exe: constructor 0x40a8a0, destructor 0x40aa00 (deleting thunks
    0x40a9d0 / 0x40adb0 / 0x40adc0 / 0x40add0).

    Original layout: as C3DAI (element vtable 95 slots, all C3DAI's; CLocalGameObject vtable at
    +0xc0: 0, 16, 17, 241, 259, 272, 273 overridden), own members 0x994..0x9b0, vtordisp 0x9b0,
    streamer 0x9b4.                                                                       */
#pragma once
#include "../C3DAI.h"

class C3DAICar : public C3DAI
{
	public:
	C3DAICar();																// 0x40a8a0
	virtual ~C3DAICar();													// 0x40aa00

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x40aad0 honk and stop; throw off what is on top
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x40ac10 no longer held
	virtual void v260(void);												// SLOT 259 0x40ac30 properties loaded: the engine sound
	virtual void Process(float dt);											// SLOT 241 0x40aa50 held in place; drive on when the time is up
	virtual void v277(void);												// SLOT 272 0x40ac60 PauseAnim(1), the engine sound off
	virtual void v278(void);												// SLOT 273 0x40ac80 PauseAnim(0), the engine sound on again

	float				m_stop_time;	// 0x994  time left standing after a hit (= 0)
	OMedia3DVector		m_hold_pos;		// 0x998  where it was hit (0, 0, 0, 1)
	unsigned char		m_held;			// 0x9a8  touched: kept at m_hold_pos (= 0)
	int					m_sound;		// 0x9ac  channel of the engine sound (-1: none)
};

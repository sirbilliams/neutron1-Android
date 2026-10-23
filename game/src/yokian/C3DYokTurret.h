/*  C3DYokTurret ('3TUR') - a Yokian missile turret (shape 5 of "objectslevel5a.omt", unlit,
    radius 150, FOV 360, sees 15000 units): a C3DAI in state 6 that every 4.5 s (while the game
    runs) aims for the camera object (Jimmy: 200 units, the rocket ship: 0.35 of its distance,
    the jeep: 400) and, in AI state 0, launches the next of its three C3DMissiles (element slot
    95, sound 0x89). From 1.8 s on it is put back to its saved position every frame.
    Reconstructed from Neutron.exe: constructor 0x44c6f0 (factory '3TUR', 0x9bc bytes; its log
    says "C3DHarrier()"), destructor 0x44ca20 (only resets the vtables, then ~C3DAI; deleting
    thunks 0x44c9f0 / 0x44d050 / 0x44d060 / 0x44d070). Element vtable: 95 is new; game object
    vtable (+0xc0): 0, 7, 241, 257 overridden. Own members 0x994..0x9b2, vtordisp 0x9b4,
    streamer 0x9b8. Slot numbers are Boy Genius's.                                        */
#pragma once
#include "../C3DAI.h"

class C3DMissile;

class C3DYokTurret : public C3DAI
{
	public:
	C3DYokTurret();															// 0x44c6f0
	virtual ~C3DYokTurret();												// 0x44ca20

	// ---- element-family slots
	virtual void ev95(float a, float b, float c);							// ESLOT 95 0x44cd50 launch the next missile (the arguments are not read)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x44ca70 shape 5, radius
	virtual void Process(float dt);											// SLOT 241 0x44cb00 aim and fire
	virtual void v258(void);												// SLOT 257 0x44ccf0 remember the position

	// ---- data members (object offsets)
	float				m_home[3];		// 0x994  position kept by v258 (the start position)
	float				f_9a0;			// 0x9a0  (1, not used)
	C3DMissile			*m_missile[3];	// 0x9a4  the missiles
	short				m_next;			// 0x9b0  the next missile to launch (0..2)
	unsigned char		m_toggle;		// 0x9b2  flips with every cycle (1, not read)
};

/*  C3DJeep ('3JEE') - Goddard's scooter (level1_a): a C3DCar whose body is chunk 0 of
    "omt\scooter.omt" (a plain shape, f_6d0 = 2). Jimmy touching it gets on (his ActivateMenuItem
    item 4: Goddard turns into it, Jimmy rides as its driver f_a84, SetupCar4(73, -30, 3, 3, 1,
    1, 1)); while ridden the driver is kept on it, plays SCOOT / SCOOTSTOP (by speed, every 0.3 s)
    and Jump hops it (700 up, once per press, on the floor). Steered and driven by the player's
    key lists (PlayerKeyLeft / Right / Go / Action / Back; no joystick slot, no camera keys);
    walls do not crash it; hitting one while holding Go slows it to 230 at the least. Power-up 0
    gives it 1.4 times the top speed (ev107). Pause / resume stop and restart the engine sound.
    Reconstructed from Neutron.exe: constructor N2:0x4420d0 (factory '3JEE', 0xa94 bytes),
    destructor 0x421330 (deleting thunks 0x421d40 / 0x421d30 / N2:0x442340 / 0x421d50).
    Vtables: element 0x4a2f8c (C3DCar's 144 slots; 54, 107, 112, 123..126 overridden), game
    object (+0xc0) 0x4a2aec (0, 7, 8, 11, 16, 220, 240, 243, 260, 275, 277, 278, 284, 285
    overridden), streamer 0x4a2adc. Own members 0xa84..0xa8c, vtordisp 0xa8c, streamer 0xa90.
    Element-family slots are `ev<n>` (`// ESLOT n`); members are f_<offset in the original
    object> (CGameObject-family code sees them at offset - 0xc0).                             */
#pragma once
#include "C3DCar.h"

class C3DAnimated;
class OMediaStreamOperators;

class C3DJeep : public C3DCar
{
	public:
	C3DJeep();																// N2:0x4420d0
	virtual ~C3DJeep();														// 0x421330

	// ---- element-family slots
	virtual void ShowAgain(void);											// ESLOT 54  0x464f30 + the wheels
	virtual void ev107(float power);										// ESLOT 107 N2:0x4428f0 + power-up 0: top speed x1.4
	virtual void ev112(void) {}												// ESLOT 112 0x472970 (no joystick)
	virtual unsigned char ev123(void);										// ESLOT 123 N2:0x442da0 PlayerKeyLeft
	virtual unsigned char ev124(void);										// ESLOT 124 N2:0x442db0 PlayerKeyRight
	virtual unsigned char ev125(void);										// ESLOT 125 N2:0x442dc0 Go / Action / button 0 (rear wheels down)
	virtual unsigned char ev126(void);										// ESLOT 126 N2:0x442df0 PlayerKeyBack (not with button 0)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4213f0 the scooter shape
	virtual void UnInitObject(void);										// SLOT 8   0x44bbb0 (C3DAnimated's)
	virtual void InitPhysics(void);											// SLOT 11  N2:0x442550 once
	virtual void AddTouching(CGameObject *other);							// SLOT 16  N2:0x4429c0 Jimmy gets on (menu item 4)
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 220 0x421910 walls slow it
	virtual void Process(float dt);											// SLOT 240 N2:0x442560 the driver, his animation, Jump
	virtual void v243(float dt);											// SLOT 243 0x421880 controls while the game runs
	virtual void v260(void);												// SLOT 260 0x4217d0 power f_9f0
	virtual void v275(void);												// SLOT 275 N2:0x442a50 Jimmy's v275, mirror off
	virtual void v277(void);												// SLOT 277 0x421bb0 pause: engine sound off
	virtual void v278(void);												// SLOT 278 N2:0x442d40 resume: engine sound on
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 N2:0x46cc40 (C3DRocketShip's) position, rotation
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 N2:0x442e10 position, rotation

	// ---- data members (object offsets)
	C3DAnimated			*f_a84;			// 0xa84  the driver (Jimmy; set by his ActivateMenuItem 4)
	float				f_a88;			// 0xa88  time since the driver's animation was set
};

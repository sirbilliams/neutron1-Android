/*  C3DJeep ('3JEE') - Goddard's scooter (level1_a): a C3DCar whose body is chunk 0 of
    "omt\scooter.omt" (a plain shape). Jimmy riding it (the gadget 4: Goddard turns into it and
    Jimmy sits on it, C3DJimmy sets the driver f_a84) is kept on it; the driver plays SCOOT /
    SCOOTSTOP (by speed, every 0.3 s) and Jump hops it (700 up, once per press, on the floor).
    Steered and driven by the keys (Left / Right arrows, Up / left mouse / button 0 to go, Down /
    right mouse to brake, only with the rear wheels on the floor); walls do not crash it, hitting
    one while going slows it to 230 at the least. Pause / resume stop and restart the engine sound.
    Reconstructed from Neutron.exe: constructor 0x4211a0 (factory '3JEE', 0x6ac bytes),
    destructor 0x421330 (deleting thunks 0x421d40 / 0x421d30 / 0x421300 / 0x421d50).
    Vtables: element 0x4a2f8c (C3DCar's 93 slots + 93; 61, 72..75 overridden), game object
    (+0xc0) 0x4a2aec (0, 7, 8, 11, 221, 241, 243, 259, 270, 272, 273 overridden), streamer
    0x4a2adc. Own members 0x69c, 0x6a0, vtordisp 0x6a4, streamer 0x6a8.
    Element-family slots are `ev<n>` (`// ESLOT n`: the names of the base classes); members keep
    the names other classes use (f_<n>, n = the Negatron offset = the Neutron.exe offset +
    0x3e8); the comments give the Neutron.exe object offsets (CGameObject-family code sees them
    at offset - 0xc0).                                                                         */
#pragma once
#include "C3DCar.h"

class C3DAnimated;

class C3DJeep : public C3DCar
{
	public:
	C3DJeep();																// 0x4211a0
	virtual ~C3DJeep();														// 0x421330

	// ---- element-family slots
	virtual void ev112(void) {}												// ESLOT 112 0x472970 (slot 61: no joystick)
	virtual unsigned char ev123(void);										// ESLOT 123 0x421c10 (slot 72) Left
	virtual unsigned char ev124(void);										// ESLOT 124 0x421c40 (slot 73) Right
	virtual unsigned char ev125(void);										// ESLOT 125 0x421c70 (slot 74) Go (rear wheels down)
	virtual unsigned char ev126(void);										// ESLOT 126 0x421cb0 (slot 75) Back (rear wheels down)
	virtual void ev106(void);												// ESLOT 106 0x421800 (slot 93) shown again (hide count 1, hide(false)), with the wheels

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4213f0 the scooter shape
	virtual void UnInitObject(void);										// SLOT 8   0x4213e0
	virtual void InitPhysics(void);											// SLOT 11  0x421570 once
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x421910 walls slow it
	virtual void Process(float dt);											// SLOT 241 0x421580 the driver, his animation, Jump
	virtual void v243(float dt);											// SLOT 243 0x421880 controls while the game runs
	virtual void v260(void);												// SLOT 259 0x4217d0 power f_9f0
	virtual void v275(void);												// SLOT 270 0x4218c0 Jimmy's v275, mirror off
	virtual void v277(void);												// SLOT 272 0x421bb0 pause: engine sound off
	virtual void v278(void);												// SLOT 273 0x421bd0 resume: engine sound on

	// ---- data members (object offsets)
	C3DAnimated			*f_a84;			// 0x69c  the driver (Jimmy; set by his gadget 4)
	float				f_a88;			// 0x6a0  time since the driver's animation was set
};

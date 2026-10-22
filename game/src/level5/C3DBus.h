/*  C3DBus ('3SBU') - the school bus (Level3b/3e, Level5c): a C3DAICar whose morph model
    "bus.ase" ("HIDEFAULT", texture "bus.png") is only loaded once its animations are set up
    (ev69), and then plays "STOP" (looped). Speed 400, v64 0.05, v66 1000, v95 2, v34 0. Once
    the properties are loaded it also plays sound 0xe0 (looped) while the task DINO is below 10
    and the SCENE beyond 259 (on top of C3DAICar's engine sound); the sound stops for good once
    DINO reaches 10 (v267) and pauses with the animation.
    Reconstructed from Neutron.exe: constructor 0x411060 (factory '3SBU', 0x9bc bytes; it
    inlines InitObject), destructor 0x4111d0 (vtable resets only, then ~C3DAICar; deleting
    thunks 0x4111a0 / 0x4113c0 / 0x4113d0 / 0x4113e0).

    Original layout: as C3DAICar (element vtable 144 slots; CLocalGameObject vtable at +0xc0: 0,
    7, 259, 265, 272, 273 overridden), own member 0x9b0, vtordisp 0x9b4, streamer 0x9b8. */
#pragma once
#include "C3DAICar.h"

class C3DBus : public C3DAICar
{
	public:
	C3DBus();																// 0x411060
	virtual ~C3DBus();														// 0x4111d0

	// ---- element-family overrides
	virtual void ev69(void);												// ESLOT 69 0x411220 load the model once

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x418290 C3DAI's, then InitAnim
	virtual void v260(void);												// SLOT 259 0x411280 properties loaded: the bus sound
	virtual void v267(int level);											// SLOT 265 0x4112e0 C3DAnimated's, then silent once DINO >= 10
	virtual void v277(void);												// SLOT 272 0x411330 C3DAICar's, then the bus sound off
	virtual void v278(void);												// SLOT 273 0x411350 C3DAICar's, then the bus sound on again

	int					m_bus_sound;	// 0x9b0  channel of the bus sound (-1: none)
};

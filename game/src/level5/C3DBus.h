/*  C3DBus ('3SBU') - the school bus (Level5c): a C3DAICar whose morph model "bus.ase"
    ("HIDEFAULT", texture "bus.png") is only loaded once its animations are set up (ev69), and
    then plays "STOP" (looped). Speed 400, v64 0.05, v66 1000, v94 2, v34 0.
    Reconstructed from Neutron.exe: constructor 0x411060 (factory '3SBU', 0x1b78 bytes; it
    inlines InitObject), destructor 0x4111d0 (vtable resets only, then ~C3DAICar; deleting
    thunks 0x4111a0 / 0x4113c0 / 0x4113d0 / 0x4113e0).

    Original layout: as C3DAICar (element vtable 0x493474, 144 slots; CLocalGameObject vtable
    0x492fd4 at +0xc0), own member 0x1b6c, vtordisp 0x1b70, streamer 0x1b74. Slots 8
    (0x44bbb0), 260 (N2:0x423840) and 267 (N2:0x441fe0) are jumps to / copies of the inherited code,
    so they are not overridden.                                                               */
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
	virtual void v277(void);												// SLOT 277 N2:0x423850 ev79(1)
	virtual void v278(void);												// SLOT 278 N2:0x423860 ev79(0)

	int					f_1b6c;			// 0x1b6c (= -1, not used)
};

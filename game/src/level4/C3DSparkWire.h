/*  C3DSparkWire ('3SPA') - a live power cable of the Downtown levels (and Level1a): a Tesla coil
    (see C3DTesla: the same shock on touching Jimmy, the same texture cycle) whose morph objects
    are "powerline.ase" (HIWALK, played as WALK) and "powerlinestop.ase" (HISTOP), with textures
    "powerline01.png" / "powerline02.png". "ItemActive" (1) is whether the cable is live: reset
    turns it off (hidden) when 0.
    Reconstructed from Neutron.exe: constructor 0x441260 (factory '3SPA', 0x6d8 bytes),
    destructor 0x4413b0 (only resets the vtables, then ~C3DTesla; deleting thunks 0x441380 /
    0x441570 / 0x441580 / 0x441590). Vtables: element 0x4b63a4 (C3DTesla's 74 slots), game
    object (+0xc0) 0x4b5f04 (0, 7, 10 overridden). No members of its own.              */
#pragma once
#include "../lab/C3DTesla.h"

class C3DSparkWire : public C3DTesla
{
	public:
	C3DSparkWire();															// 0x441260
	virtual ~C3DSparkWire();												// 0x4413b0

	virtual void InitObject(void);											// SLOT 7   0x441440 property, animations, textures
	virtual void ResetObject(void);											// SLOT 10  0x441400 + hidden when not live
};

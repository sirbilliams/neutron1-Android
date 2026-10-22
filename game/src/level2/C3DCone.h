/*  C3DCone ('3CON') - a traffic cone: a plain C3DSpriteType billboard ("sprites.omt" canvas 41,
    size 70) with no behaviour of its own (cf. C3DTree).
    Reconstructed from Neutron.exe: constructor 0x4152a0 (factory '3CON', 0x5f0 bytes), destructor
    0x415420 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x4153f0 / 0x415490 /
    0x4154a0). Vtables 0x496fdc (element, 53 slots, all C3DSprite's) / 0x496fc8 (listener) /
    0x496b78 (game object at +0xc8, only slot 0 overridden) / 0x496b68 (streamer at +0x5ec).   */
#pragma once
#include "../C3DSpriteType.h"

class C3DCone : public C3DSpriteType
{
	public:
	C3DCone();																// 0x4152a0
	virtual ~C3DCone();														// 0x415420
};

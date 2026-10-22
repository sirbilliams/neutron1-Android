/*  C3DTree ('3TRE') - a tree: a plain C3DSpriteType billboard ("sprites.omt" canvas 16, size
    150) with no behaviour of its own.
    Reconstructed from Neutron.exe: constructor 0x446c50 (factory '3TRE', 0x5f0 bytes), destructor
    0x446dc0 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x446e30 / 0x446d90 /
    0x446e40). Vtables 0x4bcc38 (element, 53 slots, all C3DSprite's) / 0x4bcc24 (listener) /
    0x4bc7d4 (game object at +0xc8, only slot 0 overridden) / 0x4bc7c4 (streamer at +0x5ec).    */
#pragma once
#include "../C3DSpriteType.h"

class C3DTree : public C3DSpriteType
{
	public:
	C3DTree();																// 0x446c50
	virtual ~C3DTree();														// 0x446dc0
};

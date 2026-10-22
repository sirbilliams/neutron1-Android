/*  C3DTree ('3TRE') - a tree: a plain C3DSpriteType billboard ("sprites.omt" canvas 16, size
    150) with no behaviour of its own.
    Reconstructed from Neutron.exe: constructor 0x4154b0, destructor 0x446dc0 (only resets the
    vtables, then ~C3DSpriteType; deleting thunks 0x446e30 / 0x446d90 / 0x446e40). Vtables
    N2:0x501634 (element, 55 slots, all C3DSprite's) / N2:0x501620 (listener) / N2:0x5011a4 (game object,
    only slot 0 overridden) / N2:0x501194 (streamer). 0x670 bytes: C3DSpriteType's 0x668, vtordisp
    0x668, the streamer at 0x66c.                                                            */
#pragma once
#include "../C3DSpriteType.h"

class C3DTree : public C3DSpriteType
{
	public:
	C3DTree();																// 0x4154b0
	virtual ~C3DTree();														// 0x446dc0
};

/*  C3DBaseballPickup ('3BPU') - the baseball launcher lying in a level: sprite 32 of
    "sprites.omt" (50 units). Touched by Jimmy while the gadget is not yet his (gadget page
    item 6 = 0) it hides, makes the launcher available (type 1) and plays sound 18.
    Reconstructed from Neutron.exe: factory '3BPU', constructor 0x410090 (0x5f0 bytes),
    destructor 0x410220 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x410300
    / 0x4101f0 / 0x410310). Only AddTouching (slot 16, 0x410260) is its own.                    */
#pragma once
#include "../C3DSpriteType.h"

class C3DBaseballPickup : public C3DSpriteType
{
	public:
	C3DBaseballPickup();													// 0x410090
	virtual ~C3DBaseballPickup();											// 0x410220

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x410260 Jimmy picks it up
};

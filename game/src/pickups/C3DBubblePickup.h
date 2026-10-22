/*  C3DBubblePickup ('3BUP') - the bubble gadget lying in a level: sprite 26 of "sprites.omt"
    (50 units). Touched by Jimmy it hides, makes the bubble (gadget 0) available in the PDA
    menu (pickup menu item 0 / gadget page item 0 = type 1) and asks "try it in VR?" (box 10).
    Reconstructed from Neutron.exe: factory '3BUP', constructor 0x410d90 (0x6d4 bytes),
    destructor 0x410f20 (only resets the vtables, then ~CPickupType; deleting thunks 0x410ff0 /
    0x410ef0 / 0x411000). Only AddTouching (slot 16, 0x410f60) is its own.                       */
#pragma once
#include "CPickupType.h"

class C3DBubblePickup : public CPickupType
{
	public:
	C3DBubblePickup();														// 0x410d90
	virtual ~C3DBubblePickup();												// 0x410f20

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x410f60 Jimmy picks it up
};

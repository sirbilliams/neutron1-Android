/*  C3DRocketFuel ('3FUE') - a rocket fuel canister of level 4b (sprites.omt canvas 40, size 50,
    radius 150): touched by Jimmy it hides, switches the PDA's rocket-ship item (page 2, item
    26) on, announces the pickup (picture 26), gives 100 points and stops colliding. When the
    story is at SCENE 470 it moves it on to 480 and takes one of the pickup picture 25 away
    (clearing its item when none are left).
    Reconstructed from Neutron.exe: constructor 0x43d530 (factory '3FUE', 0x5f0 bytes),
    destructor 0x43d6c0 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x43d690 /
    0x43d820 / 0x43d830). Vtables 0x4b24bc (element, 53 slots, none overridden) / 0x4b24a8
    (listener) / 0x4b2058 (game object at +0xc8: 16 and 264 overridden) / 0x4b2048 (streamer).
    No members of its own.                                                                    */
#pragma once
#include "../C3DSpriteType.h"

class C3DRocketFuel : public C3DSpriteType
{
	public:
	C3DRocketFuel();														// 0x43d530
	virtual ~C3DRocketFuel();												// 0x43d6c0

	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x43d700 Jimmy: collected
	virtual void v266(void);												// SLOT 264 0x43d7b0 shown again, collides, not "v132"
};

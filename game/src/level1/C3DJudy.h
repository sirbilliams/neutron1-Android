/*  C3DJudy ('3MOM') - Judy Neutron, Jimmy's mom: a morph friend (.ase animations FIX, TALK,
    WALK, STOP; texture "judy.png"), patrolling (AIState 2) at 160, VisibleRange 500; she walks
    where the others run. In Retroville she shows while SCENE is 200..259, in LV4A from 500,
    elsewhere always. Talking to her (ev96, only while she collides): SCENE 200 -> 205 (gadget
    item 3), 210 -> 220, 230 -> 250 (uses up one pickup 18). Reset puts her back at her start.
    Reconstructed from Neutron.exe: constructor 0x42b2d0 (factory '3MOM', 0xbb0 bytes),
    destructor 0x42b470 (deleting thunks 0x42b440 / 0x42b7d0 / 0x42b7e0 / 0x42b7f0).

    Original layout: as C3DFriends (element vtable 0x4a4348, 98 slots, 67 and 96 overridden;
    listener 0x4a4334; OMediaAnim 0x4a42f8; CLocalGameObject vtable 0x4a3ea8 at +0xc0: 7, 10,
    264, 265 overridden), no members of its own. Element slots are named by their Negatron
    numbers (67 = ev69, 96 = ev96).                                                         */
#pragma once
#include "../friends/C3DFriends.h"

class C3DJudy : public C3DFriends
{
	public:
	C3DJudy();																// 0x42b2d0
	virtual ~C3DJudy();														// 0x42b470

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x42b4c0 load the animations and the texture (once)
	virtual void ev96(void);												// ESLOT 96  0x42b590 talked to: the next SCENE step

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void ResetObject(void);											// SLOT 10  0x42b630 back to the start position
	virtual void v266(void);												// SLOT 264 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x42b6f0 shown by level and SCENE
};

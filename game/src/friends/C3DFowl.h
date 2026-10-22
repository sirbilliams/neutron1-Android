/*  C3DFowl ('3FOW') - Miss Fowl, Jimmy's teacher: a morph friend (.ase animations WALK, TALK,
    TELE (teleport), CHEER, STOP; texture "fowl.png"), patrolling (AIState 2) at 160,
    VisibleRange 500. Where she shows depends on the level and the story (v267): in LEV4 while
    SCENE is 290..309, in LV4A from 500, in LV4C while SCENE is 460..469; in other levels she
    is left as she is. Talking to her (ev96) is C3DFriends' (100 points, then stop).
    Reconstructed from Neutron.exe: constructor 0x41b0a0 (factory '3FOW'),
    destructor 0x41b250 (deleting thunks 0x41b220 / 0x41b460 / 0x41b470 / 0x41b480).

    Original layout: as C3DFriends (element vtable 0x49daa0: 67 and 96 overridden; listener
    0x49da8c; OMediaAnim 0x49da50; CLocalGameObject vtable 0x49d600 at +0xc0: 7, 264, 265
    overridden), no members of its own.                                                     */
#pragma once
#include "C3DFriends.h"

class C3DFowl : public C3DFriends
{
	public:
	C3DFowl();																// 0x41b0a0
	virtual ~C3DFowl();														// 0x41b250

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x41b2a0 load the animations and the texture (once)
	virtual void ev96(void);												// ESLOT 96  0x4206c0 talked to

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v266(void);												// SLOT 264 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x41b380 shown by level and SCENE
};

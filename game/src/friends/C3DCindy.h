/*  C3DCindy ('3CIN') - Cindy Vortex: a morph friend (.ase animations WAVE, WALK, TALK, CHEER,
    TELE (teleport), STOP; texture "cindy.png"), patrolling (AIState 2) at 160, VisibleRange 500.
    Where she shows depends on the level and the story (v267): in LEV4 while SCENE is 260..279,
    in LV3D while SCENE is 400..419, in LV4A from 500, always in other levels. Talking to her
    (ev96) at SCENE 260 gives the menu item and moves SCENE on to 270; at 270 her ev90 turns
    the pickup count of item 7 into points (100 each) and opens the confirm box 4.
    Reconstructed from Neutron.exe: constructor 0x414db0 (factory '3CIN'),
    destructor 0x414f50 (deleting thunks 0x414f20 / 0x415270 / 0x415280 / 0x415290).

    Original layout: as C3DFriends (element vtable 0x4969d4: 67, 90 and 96 overridden; listener
    0x4969c0; OMediaAnim 0x496984; CLocalGameObject vtable 0x496534 at +0xc0: 7, 264, 265
    overridden), no members of its own.                                                     */
#pragma once
#include "C3DFriends.h"

class C3DCindy : public C3DFriends
{
	public:
	C3DCindy();																// 0x414db0
	virtual ~C3DCindy();													// 0x414f50

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x414fa0 load the animations and the texture (once)
	virtual void ev90(void);												// ESLOT 90  0x415120
	virtual void ev96(void);												// ESLOT 96  0x4150d0 talked to

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v266(void);												// SLOT 264 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x415180 shown by level and SCENE
};

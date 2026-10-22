/*  C3DHugh ('3HUG') - Hugh Neutron, Jimmy's dad: an OMT morph friend (hughtalk / hughwalk /
    hughcount / hughstop .ase, texture "hugh.png"), patrolling (AIState 2) at 160, VisibleRange 500. His
    animations (and hughcount.ase, HICOUNT) are loaded by InitObject (his ev69 is the empty
    one); he is shown everywhere but level4a, where he is hidden until SCENE passes 204 (v267).
    Reconstructed from Neutron.exe: constructor 0x420390 (factory '3HUG', 0xbb0 bytes),
    destructor 0x420520 (deleting thunks 0x4204f0 / 0x420700 / 0x420710 / 0x420720).

    Original layout: as C3DFriends (element vtable: 145 is 0x41ee20, a jump to the empty
    0x472970; CLocalGameObject vtable at +0xc0: 7, 266, 267 overridden), no members of its own. */
#pragma once
#include "../friends/C3DFriends.h"

class C3DHugh : public C3DFriends
{
	public:
	C3DHugh();																// 0x420390
	virtual ~C3DHugh();														// 0x420520

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x420570 load the morph animations and the texture
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 0x420650 shown by level and SCENE
};

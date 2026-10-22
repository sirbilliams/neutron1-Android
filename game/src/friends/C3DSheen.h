/*  C3DSheen ('3SHE') - Sheen: a morph friend (.ase animations PICK, WAVE, WALK, TALK, STOP;
    texture "sheen.png"), patrolling (AIState 2) at 160, VisibleRange 500; he walks where the
    others run. Reset puts him back at his start position. He shows in Retroville while SCENE
    is 280..299, in LEV2 at 310..329, in LEV4 at 360..389, and in every other level. Talking
    to him (ev145) moves SCENE on (280 -> 290, 310 -> 320 with gadget item 4, 360 -> 380).
    Reconstructed from Neutron.exe: constructor 0x43f920 (factory '3SHE', 0xbb0 bytes),
    destructor 0x43fac0 (deleting thunks 0x43fa90 / 0x43fdf0 / 0x43fe00 / 0x43fe10).

    Original layout: as C3DFriends (element vtable 0x4b41ac, 98 slots, 67 and 96 overridden;
    listener 0x4b4198; OMediaAnim 0x4b415c; CLocalGameObject vtable 0x4b3d0c at +0xc0: 7, 10,
    11, 259, 264, 265 overridden), no members of its own. Slot 259 is 0x42d010, a jump to
    C3DAI's. Element slots are named by their Negatron numbers (67 = ev69, 96 = ev145).      */
#pragma once
#include "C3DFriends.h"

class C3DSheen : public C3DFriends
{
	public:
	C3DSheen();																// 0x43f920
	virtual ~C3DSheen();													// 0x43fac0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x43fb10 load the animations and the texture (once)
	virtual void ev145(void);												// ESLOT 96  0x43fca0 talked to: the next SCENE step

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void ResetObject(void);											// SLOT 10  0x43fd10 back to the start position
	virtual void InitPhysics(void);											// SLOT 11  0x42ab50 (unless f_4aa)
	virtual void v266(void);												// SLOT 264 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x43fbf0 shown by level and SCENE
};

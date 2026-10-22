/*  C3DCarl ('3CAR') - Carl: a morph friend (.ase animations WALK, TALK, TELE, CHEER, INHALE,
    STOP; texture "carl.png"), waiting (AIState 1) at speed 200, VisibleRange 500; he walks
    where the others run. Where he shows depends on the level and the story (v267): in
    Retroville while SCENE is 60..89, in LEV2 at 270..289, in LEV3 at 380..389, in the lab
    (LV1B) up to 59 (from 31 at "CARLESC3", state 6), in LV4A from 500, elsewhere always.
    Talking to him (ev96): SCENE 50 -> 60, 65 -> 70 (and he moves to "CARL8"), 270 -> 280.
    Reconstructed from Neutron.exe: constructor 0x413af0 (factory '3CAR', 0xbb0 bytes),
    destructor 0x413ca0 ("Delete C3DCarl" (debug log), then ~C3DFriends; deleting thunks
    0x413c70 / 0x414160 / 0x414170 / 0x414180).

    Original layout: as C3DFriends (element vtable 0x495804, 98 slots, 67 and 96 overridden;
    listener 0x4957f0; OMediaAnim 0x4957b4; CLocalGameObject vtable 0x495364 at +0xc0: 7, 8,
    259, 264, 265 overridden), no members of its own. Slot 259 is 0x42d010, a jump to
    C3DAI's. Element slots are named by their Negatron numbers (67 = ev69, 96 = ev96).      */
#pragma once
#include "C3DFriends.h"

class C3DCarl : public C3DFriends
{
	public:
	C3DCarl();																// 0x413af0
	virtual ~C3DCarl();														// 0x413ca0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x413d60 load the animations and the texture (once)
	virtual void ev96(void);												// ESLOT 96  0x413e50 talked to: the next SCENE step

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void UnInitObject(void);										// SLOT 8   0x413d30
	virtual void v266(void);												// SLOT 264 0x413eb0 task state; at SCENE 70 to "CARL8"
	virtual void v267(int level);											// SLOT 265 0x413f30 shown by level and SCENE
};

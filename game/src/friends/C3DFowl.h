/*  C3DFowl ('3FOW') - Miss Fowl, Jimmy's teacher (in the museum): a Granny friend (model
    "fowlbase.grn"; TALK, YELL (fowldino), STOP, IDLE, TALK2 (fowltalknorm)), patrolling
    (AIState 2) at 160, VisibleRange 500. A cut scene that finds her stopped makes her TALK.
    Reconstructed from Neutron.exe: constructor N2:0x436540 (factory '3FOW', 0x1d7c bytes),
    destructor 0x41b250 (deleting thunks 0x41b220 / 0x41b460 / 0x41b470 / 0x41b480).

    Original layout: as C3DFriends (element vtable 0x49daa0, 147 slots; listener 0x49da8c;
    OMediaAnim 0x49da50; CLocalGameObject vtable 0x49d600 at +0xc0), no members of its own.
    Element slot 145 is 0x41ee20 (empty, as C3DFriends').                                    */
#pragma once
#include "C3DFriends.h"

class C3DFowl : public C3DFriends
{
	public:
	C3DFowl();																// N2:0x436540
	virtual ~C3DFowl();														// 0x41b250

	// ---- element-family slots
	virtual void ev60(void);												// ESLOT 60 N2:0x4369f0 no such animation: IDLE
	virtual void ev69(void);												// ESLOT 69 N2:0x436760 load the model and the animations (once)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v260(void);												// SLOT 260 N2:0x417110
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 N2:0x436900
	virtual void v270(int old_mode, int new_mode);							// SLOT 270 N2:0x436920 "C3DFowl::ChangeState": TALK
};

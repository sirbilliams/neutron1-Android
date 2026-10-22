/*  C3DCindy ('3CIN') - Cindy: a Granny friend (model "cindybase.grn"; WALK, TALK, STOP, LOVE,
    LOVETALK, LOVEIDLE, DISGTALK, IDLE, TALKFLURP, DRINKFLURP, HOLDFLURP), patrolling
    (AIState 2) at 160, VisibleRange 500, shadow 80 below her origin. A cut scene that finds her
    stopped makes her IDLE.
    Reconstructed from Neutron.exe: constructor N2:0x427600 (factory '3CIN', 0x1d7c bytes),
    destructor 0x414f50 (deleting thunks 0x414f20 / 0x415270 / 0x415280 / 0x415290).

    Original layout: as C3DFriends (element vtable 0x4969d4, 147 slots; listener 0x4969c0;
    OMediaAnim 0x496984; CLocalGameObject vtable 0x496534 at +0xc0), no members of its own.  */
#pragma once
#include "C3DFriends.h"

class C3DCindy : public C3DFriends
{
	public:
	C3DCindy();																// N2:0x427600
	virtual ~C3DCindy();													// 0x414f50

	// ---- element-family slots
	virtual void ev60(void);												// ESLOT 60 N2:0x4369f0 no such animation: IDLE
	virtual void ev69(void);												// ESLOT 69 N2:0x427830 load the model and the animations (once)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v260(void);												// SLOT 260 N2:0x417110
	virtual void v270(int old_mode, int new_mode);							// SLOT 270 N2:0x427a40 "C3DCindy::ChangeState": IDLE
};

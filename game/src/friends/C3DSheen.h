/*  C3DSheen ('3SHE') - Sheen: a Granny friend (model "sheenbase.grn", one animation IDLE
    (sheenidle) used for running, waiting and sitting too), patrolling (AIState 2) at 160,
    VisibleRange 500. Reset puts him back at his start position.
    Reconstructed from Neutron.exe: constructor N2:0x46ecd0 (factory '3SHE', 0x1d7c bytes),
    destructor 0x43fac0 (deleting thunks 0x43fa90 / 0x43fdf0 / 0x43fe00 / 0x43fe10).

    Original layout: as C3DFriends (element vtable 0x4b41ac, 147 slots; listener 0x4b4198;
    OMediaAnim 0x4b415c; CLocalGameObject vtable 0x4b3d0c at +0xc0), no members of its own.
    Slot 260 is N2:0x46e3c0, a jump to C3DAI's (so not overridden here).                        */
#pragma once
#include "C3DFriends.h"

class C3DSheen : public C3DFriends
{
	public:
	C3DSheen();																// N2:0x46ecd0
	virtual ~C3DSheen();													// 0x43fac0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  N2:0x46eef0 load the model and the animation (once)
	virtual void ev145(void);												// ESLOT 145 N2:0x44fa60

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void ResetObject(void);											// SLOT 10  0x43fd10 back to the start position
	virtual void InitPhysics(void);											// SLOT 11  0x42ab50 (unless f_4aa)
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 0x4418e0
};

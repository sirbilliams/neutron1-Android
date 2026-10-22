/*  C3DBenny ('3BEN') - Benny (the museum janitor): a morph friend (.ase animations WIPE, STOP;
    texture "benny.png"), patrolling (AIState 2) at 160, VisibleRange 500, wiping from the
    start; shadow 5 below his origin.
    Reconstructed from Neutron.exe: constructor 0x410340 (factory '3BEN', 0x1d7c bytes),
    destructor 0x4104f0 (deleting thunks 0x4104c0 / 0x4107f0 / 0x410800 / 0x410810).

    Original layout: as C3DFriends (element vtable 0x492368, 147 slots; listener 0x492354;
    OMediaAnim 0x492318; CLocalGameObject vtable 0x491ec8 at +0xc0), no members of its own.  */
#pragma once
#include "C3DFriends.h"

class C3DBenny : public C3DFriends
{
	public:
	C3DBenny();																// 0x410340
	virtual ~C3DBenny();													// 0x4104f0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x4105b0 load the animations and the texture (once)
	virtual void ev145(void);												// ESLOT 145 N2:0x44fa60

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void UnInitObject(void);										// SLOT 8   0x410580 "C3DBenny::UnInitObject()"
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 0x4418e0
};

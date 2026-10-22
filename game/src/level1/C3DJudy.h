/*  C3DJudy ('3MOM') - Judy Neutron, Jimmy's mom: an OMT morph friend (judytalk / judywalk /
    judystop .ase, texture "judy.png"), patrolling (AIState 2) at 160, VisibleRange 500.
    Reconstructed from Neutron.exe: constructor 0x432730 (factory '3MOM', 0x1d7c bytes),
    destructor 0x42b470 (deleting thunks 0x42b440 / 0x42b7d0 / 0x42b7e0 / 0x42b7f0).

    Original layout: as C3DFriends (element vtable: 69, 145 overridden; CLocalGameObject vtable
    N2:0x4e5644 at +0xc0: 7, 266, 267 overridden), no members of its own.                       */
#pragma once
#include "../friends/C3DFriends.h"

class C3DJudy : public C3DFriends
{
	public:
	C3DJudy();																// 0x432730
	virtual ~C3DJudy();														// 0x42b470

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x42b4c0 load the morph animations and the texture (once)
	virtual void ev145(void);												// ESLOT 145 N2:0x44ec00 talked to (nothing happens)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 N2:0x47a660
};

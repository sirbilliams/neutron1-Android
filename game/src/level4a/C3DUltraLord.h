/*  C3DUltraLord ('3ULT') - Ultra Lord (level4a): an OMT morph friend (ultrawalk, ultraflex 1..3,
    ultratalk, ultragive, ultrawhisper, ultrastop .ase; texture "ultralord.png"; radius 150,
    scale 0.5), patrolling (AIState 2) at 160, VisibleRange 500.
    Reconstructed from Neutron.exe: constructor 0x448310 (factory '3ULT', 0x1d7c bytes),
    destructor 0x4484b0 (deleting thunks 0x448760 / 0x448750 / 0x448480 / 0x448770).

    Original layout: as C3DFriends (element vtable 0x4bdc7c: 69, 126 (N2:0x45b110, a jump to the
    empty 0x41ee20) and 145 (N2:0x44fa60) overridden; CLocalGameObject vtable 0x4bd7dc at +0xc0: 7,
    266, 267 overridden, all shared code), no members of its own.                             */
#pragma once
#include "../friends/C3DFriends.h"

class C3DUltraLord : public C3DFriends
{
	public:
	C3DUltraLord();															// 0x448310
	virtual ~C3DUltraLord();												// 0x4484b0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x448500 load the morph animations and the texture (once)
	virtual void ev145(void);												// ESLOT 145 N2:0x44fa60 talked to (nothing happens)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 N2:0x47a660
};

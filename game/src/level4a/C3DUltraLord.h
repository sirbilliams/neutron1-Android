/*  C3DUltraLord ('3ULT') - Ultra Lord (level4a, and Retroland's level3d): an OMT morph friend
    (ultrawalk, ultraflex 1..3, ultratalk, ultragive, ultrawhisper, ultrastop .ase; texture
    "ultralord.png"; radius 150, scale 0.5), patrolling (AIState 2) at 160, VisibleRange 500.
    Where he shows depends on the level and the story (v267): in LEV3 while SCENE is 320..409,
    in LV4A from SCENE 500, in every other level always. Talked to (ev96) at SCENE 390 he
    makes the first PDA gadget usable; ev90 sets the AI state to 3.
    Reconstructed from Neutron.exe: constructor 0x448310 (factory '3ULT', 0x1d7c bytes),
    destructor 0x4484b0 (deleting thunks 0x448760 / 0x448750 / 0x448480 / 0x448770).

    Original layout: as C3DFriends (element vtable 0x4bdc7c: 69 (0x448500), 90 (0x448660),
    96 (0x448620) overridden; CLocalGameObject vtable 0x4bd7dc at +0xc0: 7, 265 (0x448670), 266
    overridden), no members of its own.                                                      */
#pragma once
#include "../friends/C3DFriends.h"

class C3DUltraLord : public C3DFriends
{
	public:
	C3DUltraLord();															// 0x448310
	virtual ~C3DUltraLord();												// 0x4484b0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x448500 load the morph animations and the texture (once)
	virtual void ev90(void);												// ESLOT 90  0x448660 AI state 3
	virtual void ev96(void);												// ESLOT 96  0x448620 talked to

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x448670 shown by level and SCENE
};

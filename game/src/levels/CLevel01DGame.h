/*  CLevel01DGame ('LV1D') - the game type of level1d.gam, Retroville by night: the first level a
    new game loads. Sky "skylevel1d", terrain level1d.omt unlit, black fog from 120000.
    Reconstructed from Neutron.exe: constructor 0x44f920, destructor 0x44fa40 (deleting
    thunks 0x44fcb0 / 0x44fa10 / 0x44fcc0), vtables 0x4c48b0 (element) / 0x4c489c / 0x4c444c
    (game object) / 0x4c443c. 0x68c bytes: CJimmyGame's layout, no members of its own.

    The constructor of every level game type does the same: name, InitObject (which runs
    InitGame through the vtable), the level's 4cc and the pickup index base (NextPickupIndex).
    It sets the base after InitGame, so its value wins over CJimmyGame::InitGame's 100.      */
#pragma once
#include "../CJimmyGame.h"

class CLevel01DGame : public CJimmyGame
{
	public:
	CLevel01DGame();														// 0x44f920
	virtual ~CLevel01DGame();												// 0x44fa40

	virtual void UnInitObject(void);										// SLOT 8   0x44fad0
	virtual void InitGame(void);											// ESLOT 45 0x44fb00
};

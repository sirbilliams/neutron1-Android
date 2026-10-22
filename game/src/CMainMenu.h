/*  CMainMenu ('MAIN') - the game type that LoadNewLevel creates for every level id (0x44d0e0);
    the main menu runs on it. A CJimmyGame that overrides nothing. (Negatron's CDefaultGame, a
    plain CGameType, has its place there.)
    Reconstructed from Neutron.exe: constructor 0x459110 (0x5c0 bytes), destructor 0x459230
    (deleting thunks 0x4592b0 / 0x459200 / 0x4592a0), vtables CMainMenu.0..3.

    The level's real game type (CLevelXXGame) is not made here: CEditor's LoadGame reads the
    level .gam file, whose first object id is the game type, creates it through the object
    factory and sets g_gametype to it. Jimmy ('3JIM') is an object of every .gam (Neutron.exe
    has no CreateStaticObjects).                                                              */
#pragma once
#include "CJimmyGame.h"
#include <list>

class CWayPoint;

class CMainMenu : public CJimmyGame
{
	public:
	CMainMenu();															// 0x459110
	virtual ~CMainMenu();													// 0x459230
};

namespace neg {
extern std::list<CWayPoint *> g_waypoints;				// 0x696044  every CWayPoint

CGameType *CreateGameType(unsigned int level);			// 0x44d0e0  (level is not used)
void ClearWayPoints(void);								// 0x47ec30
}

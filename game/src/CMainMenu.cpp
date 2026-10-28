/*  Reconstructed from Neutron.exe. CMainMenu and the level-load helper next to it in Neutron.exe (0x44d0e0), and the waypoint list
    reset (0x47ec30). See CMainMenu.h.                                                          */

#include "CMainMenu.h"
#include "CEditor.h"
#include "Game.h"
#include "Todo.h"
#include "gen/globals.h"

namespace neg {
std::list<CWayPoint *>	g_waypoints;
}
using namespace neg;

// CEditor element vtable +0x118: opens omt\Editor.omt.
static void EditorOpenDatabase(void)		{ g_editor->OpenEditorDatabase(); }
// CEditor element vtable +0x12c: "DrawSplash" - shows splash canvas `id` of Editor.omt.
static void EditorDrawSplash(int id)		{ g_editor->DrawSplash(id); }

// --------------------------------------------------------------------------- CMainMenu

CMainMenu::CMainMenu()														// 0x459110
{
	static char name[] = "CMAINMENU";			// SetName upper-cases in place
	SetName(name);
	Log("CMainMenu()");
	CGameType::InitObject();					// (direct call) -> InitGame
	CGameType::SetID('MAIN');
	v128(0);									// 0x4719c0: byte +0x139 = 0
	// (debug log, empty in the shipped build)
}

CMainMenu::~CMainMenu()														// 0x459230
{
}

// --------------------------------------------------------------------------- 0x44d0e0

// Called by LoadNewLevel with the level id ('MAIN' at start-up, the AWEMSG_LOAD parameter later),
// which it does not use: it always makes a CMainMenu, then has the editor open its database and
// show splash 3.
CGameType *neg::CreateGameType(unsigned int level)
{
	(void)level;
	CGameType *game = NULL;
	if (g_editor)
	{
		game = new CMainMenu;					// 0x5c0 bytes
		EditorOpenDatabase();					// g_editor vtable +0x118
		EditorDrawSplash(3);					// g_editor vtable +0x12c
	}
	return game;
}

// --------------------------------------------------------------------------- 0x47ec30

void neg::ClearWayPoints(void)
{
	g_waypoints.clear();						// the nodes only; the waypoints are game objects (DeleteAllObjects)
	g_69604c = 0;
}

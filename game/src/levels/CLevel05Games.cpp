/*  The level 5 game types - see CLevel05Games.h. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel05Games.h"
#include "LevelHelpers.h"
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

// --------------------------------------------------------------------------- CLevel05Game 'LEV5'

CLevel05Game::CLevel05Game()												// 0x454f00
{
	static char name[] = "CLEVEL05GAME";
	SetName(name);
	Log("CLevel05Game()");
	CGameType::InitObject();
	CGameType::SetID('LEV5');
	m_pickup_index_base = 2700;
}

CLevel05Game::~CLevel05Game()												// 0x455020
{
	Log("~CLevel05Game()");
}

void CLevel05Game::InitGame(void)											// 0x4550b0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// not written to the .gam (f_00b)
	LocalSky("omt\\skylevel5.omt");							// a local: m_sky stays NULL
	InitTerrainDatabase("level5.omt");
	CreateTerrainUnLit();									// element +0xf8
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.646f, 0.466f, 0.0f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 0x509a38 +0xc8
	g_layer_clear->set_clear_color(c);						// +0x10c
	WorldFog(g_layer_world, c, 12000.0f, 5000.0f, 10000.0f);	// (the same through 0x509a30 and 0x509a3c)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel05AGame 'LV5A'

CLevel05AGame::CLevel05AGame()												// 0x4547b0
{
	static char name[] = "CLEVEL05AGAME";
	SetName(name);
	Log("CLevel05AGame()");
	CGameType::InitObject();
	CGameType::SetID('LV5A');
	m_pickup_index_base = 2500;
}

CLevel05AGame::~CLevel05AGame()												// 0x4548d0
{
	Log("~CLevel05aGame()");
}

void CLevel05AGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

// LV5A and LV5B: black fog, one copy of the function each.
static void InitLevel5Black(CJimmyGame *game, const char *omt)
{
	game->Log("InitGame()");
	game->CJimmyGame::InitGame();
	game->InitTerrainDatabase(omt);
	game->v107(0);
	game->CreateTerrainUnLit();								// element +0xf8
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.0f, 0.0f, 0.0f, 0.0f);
	g_layer_clear->set_clear_color(c);						// 0x509a38 +0x10c
	WorldFog(g_layer_world, c, 16000.0f, 6000.0f, 16000.0f);	// (the same through 0x509a30 and 0x509a3c)
	CaptureMouse(true);
}

void CLevel05AGame::InitGame(void)											// 0x454960
{
	InitLevel5Black(this, "level5a.omt");
}

// --------------------------------------------------------------------------- CLevel05BGame 'LV5B'

CLevel05BGame::CLevel05BGame()												// 0x454b80
{
	static char name[] = "CLEVEL05BGAME";
	SetName(name);
	Log("CLevel05BGame()");
	CGameType::InitObject();
	CGameType::SetID('LV5B');
	m_pickup_index_base = 2600;
}

CLevel05BGame::~CLevel05BGame()												// 0x454ca0
{
}

void CLevel05BGame::InitGame(void)											// 0x454ce0
{
	InitLevel5Black(this, "level5b.omt");
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevel05Game(void)		{ return new CLevel05Game; }		// 'LEV5' (0x5c0 bytes)
CGameObject *New_CLevel05AGame(void)	{ return new CLevel05AGame; }		// 'LV5A'
CGameObject *New_CLevel05BGame(void)	{ return new CLevel05BGame; }		// 'LV5B'
}

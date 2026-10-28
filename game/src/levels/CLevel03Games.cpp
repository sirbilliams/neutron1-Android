/*  The level 3 game types - see CLevel03Games.h.
    Reconstructed from Neutron.exe: every constructor, destructor, InitGame and other slot
    checked against raw/ (including _free.c); compiler thunks and factories checked.
 */
// port: native C++ inheritance supplies ABI thunks; empty 0x472970 debug calls omitted.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel03Games.h"
#include "../port/Lod.h"		// port: draw distance: far plane / fog range scaled (NEG_LOD), 1 = the exe
#include "LevelHelpers.h"
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

// --------------------------------------------------------------------------- CLevel03Game 'LEV3'

CLevel03Game::CLevel03Game()												// 0x452a70
{
	static char name[] = "CLEVEL03GAME";
	SetName(name);
	Log("CLevel03Game()");
	CGameType::InitObject();
	CGameType::SetID('LEV3');
	m_pickup_index_base = 1900;
}

CLevel03Game::~CLevel03Game()												// 0x452b90
{
	Log("~CLevel03Game()");
}

void CLevel03Game::InitGame(void)											// 0x452c20
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// not written to the .gam (f_00b)
	LocalSky("omt\\nightsky.omt");							// a local: m_sky stays NULL
	InitTerrainDatabase("level3.omt");
	v107(0);												// (again)
	CreateTerrain();										// element +0xf4 (lit)
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.24f, 0.56f, 0.75f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 0x509a38 +0xc8
	neg::LodFarClip(g_layer_world, 40000.0f);					// 0x509a30
	g_layer_world->set_fog_color(c);
	neg::LodFarClip(g_layer_world, 40000.0f);					// 0x509a3c
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_clear, false);						// 0x476910(layer, 0x800)
	SetLayerFog(g_layer_world, false);						// (0x509a3c)
	SetLayerFog(g_layer_world, false);						// (0x509a30)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel03AGame 'LV3A'

CLevel03AGame::CLevel03AGame()												// 0x451730
{
	static char name[] = "CLEVEL03AGAME";
	SetName(name);
	Log("CLevel03AGame()");
	CGameType::InitObject();
	CGameType::SetID('LV3A');
	m_pickup_index_base = 1400;
}

CLevel03AGame::~CLevel03AGame()												// 0x451850
{
}

void CLevel03AGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

void CLevel03AGame::InitGame(void)											// 0x451890
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	InitTerrainDatabase("level3A.omt");
	v107(0);
	CreateTerrainUnLit();									// element +0xf8
	LocalSky("omt\\nightsky.omt");
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.1f, 0.1f, 0.1f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 0x509a38 (no clear colour here)
	neg::LodFarClip(g_layer_world, 80000.0f);
	g_layer_world->set_fog_color(c);
	neg::LodFogRange(g_layer_world, 30000.0f, 70000.0f);
	SetLayerFog(g_layer_world, true);
	CaptureMouse(true);
}

// LV3B..LV3E: one copy of the same function each, differing in the terrain database only.
// Fog colour only (the clear layer's and both world-layer globals'), far plane 40000, fog off.
static void InitLevel3Night(CJimmyGame *game, const char *omt)
{
	game->Log("InitGame()");
	game->CJimmyGame::InitGame();
	game->InitTerrainDatabase(omt);
	game->v107(0);
	game->CreateTerrain();									// element +0xf4 (lit)
	LocalSky("omt\\nightsky.omt");
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.14f, 0.16f, 0.75f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 0x509a38 +0xc8
	neg::LodFarClip(g_layer_world, 40000.0f);					// 0x509a30
	g_layer_world->set_fog_color(c);
	neg::LodFarClip(g_layer_world, 40000.0f);					// 0x509a3c
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_world, false);						// 0x476910(0x509a3c, 0x800)
	SetLayerFog(g_layer_world, false);	// 0x509a30
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel03BGame 'LV3B'

CLevel03BGame::CLevel03BGame()												// 0x451af0
{
	static char name[] = "CLEVEL03BGAME";
	SetName(name);
	Log("CLevel03BGame()");
	CGameType::InitObject();
	CGameType::SetID('LV3B');
	m_pickup_index_base = 1500;
}

CLevel03BGame::~CLevel03BGame()												// 0x451c10
{
}

void CLevel03BGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

void CLevel03BGame::InitGame(void)											// 0x451c50
{
	InitLevel3Night(this, "level3B.omt");
}

// --------------------------------------------------------------------------- CLevel03CGame 'LV3C'

CLevel03CGame::CLevel03CGame()												// 0x451ed0
{
	static char name[] = "CLEVEL03CGAME";
	SetName(name);
	Log("CLevel03CGame()");
	CGameType::InitObject();
	CGameType::SetID('LV3C');
	m_pickup_index_base = 1600;
}

CLevel03CGame::~CLevel03CGame()												// 0x451ff0
{
}

void CLevel03CGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

void CLevel03CGame::InitGame(void)											// 0x452030
{
	InitLevel3Night(this, "level3C.omt");
}

// --------------------------------------------------------------------------- CLevel03DGame 'LV3D'

CLevel03DGame::CLevel03DGame()												// 0x4522b0
{
	static char name[] = "CLEVEL03DGAME";
	SetName(name);
	Log("CLevel03DGame()");
	CGameType::InitObject();
	CGameType::SetID('LV3D');
	m_pickup_index_base = 1700;
}

CLevel03DGame::~CLevel03DGame()												// 0x4523d0
{
}

void CLevel03DGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

void CLevel03DGame::InitGame(void)											// 0x452410
{
	InitLevel3Night(this, "level3D.omt");
}

// --------------------------------------------------------------------------- CLevel03EGame 'LV3E'

CLevel03EGame::CLevel03EGame()												// 0x452690
{
	static char name[] = "CLEVEL03EGAME";
	SetName(name);
	Log("CLevel03EGame()");
	CGameType::InitObject();
	CGameType::SetID('LV3E');
	m_pickup_index_base = 1800;
}

CLevel03EGame::~CLevel03EGame()												// 0x4527b0
{
}

void CLevel03EGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

void CLevel03EGame::InitGame(void)											// 0x4527f0
{
	InitLevel3Night(this, "level3E.omt");
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevel03Game(void)		{ return new CLevel03Game; }		// 'LEV3' (0x5c0 bytes)
CGameObject *New_CLevel03AGame(void)	{ return new CLevel03AGame; }		// 'LV3A'
CGameObject *New_CLevel03BGame(void)	{ return new CLevel03BGame; }		// 'LV3B'
CGameObject *New_CLevel03CGame(void)	{ return new CLevel03CGame; }		// 'LV3C'
CGameObject *New_CLevel03DGame(void)	{ return new CLevel03DGame; }		// 'LV3D'
CGameObject *New_CLevel03EGame(void)	{ return new CLevel03EGame; }		// 'LV3E'
}

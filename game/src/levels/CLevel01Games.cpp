/*  The Retroville level game types - see CLevel01Games.h.
    Reconstructed from Neutron.exe: every constructor, destructor, InitGame and other slot
    checked against raw/ (including _free.c); compiler thunks and factories checked.
 */
// port: native C++ inheritance supplies ABI thunks; empty 0x472970 debug calls omitted.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel01Games.h"
#include "../port/Lod.h"		// port: draw distance: far plane / fog range scaled (NEG_LOD), 1 = the exe
#include "LevelHelpers.h"
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

// --------------------------------------------------------------------------- CLevel01Game 'LEV1'

CLevel01Game::CLevel01Game()												// 0x4503c0
{
	static char name[] = "CLEVEL01GAME";
	SetName(name);
	Log("CLevel01Game()");
	CGameType::InitObject();
	CGameType::SetID('LEV1');
	f_684 = 0;
	m_pickup_index_base = 800;
}

CLevel01Game::~CLevel01Game()												// 0x4504f0
{
	Log("~CLevel01Game()");
}

// 0x4508d0: only CJimmyGame's.
void CLevel01Game::Process(float dt)	{ CJimmyGame::Process(dt); }

void CLevel01Game::InitGame(void)											// 0x450580
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// slot 108: not written to the .gam (f_00b)
	// 0x4505bd..0x4506ba and 0x4507ca..0x4507ef: disassembly confirms RGBA, alpha 1.
	float tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };				// the terrain colour for the time of day
	m_sky = CreateSky();
	if (m_sky)
	{
		switch (GetSceneStage())							// element +0x15c
		{
			case 0:
				SkyLoad(m_sky, "omt\\bluesky3.omt", false);	// sky element +0xe0
				m_sky->ev51();								// element +0xcc
				break;
			case 1:
				SkyLoad(m_sky, "omt\\dusksky.omt", false);
				m_sky->ev51();
				tint[0] = 1.0f; tint[1] = 0.9f; tint[2] = 0.9f;
				break;
			case 2:
				m_sky_rotates = 0;
				SkyLoad(m_sky, "omt\\nightsky.omt", false);
				m_sky->ev51();
				tint[0] = 0.7f; tint[1] = 0.7f; tint[2] = 0.8f;
				break;
		}
		m_sky->place(0.0f, 0.0f, 0.0f);						// (+0x44..+0x4c written directly)
		m_sky->InitPhysics();								// (empty for a C3DSky)
		m_sky->v67(1.0f);									// radius
		m_sky->v152(1.0f);
	}
	InitTerrainDatabase("level1.omt");						// element +0xe4
	CreateTerrainTinted(tint[0], tint[1], tint[2], tint[3]);	// element +0x158
	ViewportToOrigin();
	const OMediaFARGBColor fog = RGBA(0.24f, 0.56f, 0.75f, 0.0f);
	g_layer_clear->set_fog_color(fog);						// 0x509a38 +0xc8
	neg::LodFarClip(g_layer_world, 28000.0f);					// 0x509a3c +0xc0
	g_layer_world->set_fog_color(fog);						// +0xc8
	neg::LodFogRange(g_layer_world, 13000.0f, 17000.0f);		// +0xd8 / +0xdc
	SetLayerFog(g_layer_world, false);						// 0x476910(0x509a3c, 0x800)
	CaptureMouse(true);										// 0x46a400
}

// --------------------------------------------------------------------------- CLevel01AGame 'LV1A'

CLevel01AGame::CLevel01AGame()												// 0x44ef30
{
	static char name[] = "CLEVEL01AGAME";
	SetName(name);
	Log("CLevel01AGame()");
	CGameType::InitObject();
	CGameType::SetID('LV1A');
	m_pickup_index_base = 200;
}

CLevel01AGame::~CLevel01AGame()												// 0x44f050
{
	Log("~CLevel01AGame()");
}

void CLevel01AGame::InitGame(void)											// 0x44f0e0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	InitTerrainDatabase("level1a.omt");
	CreateTerrain();										// element +0xf4 (lit)
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.28f, 0.3f, 0.4f, 0.0f);
	g_layer_clear->set_clear_color(c);						// 0x509a38 +0x10c
	WorldFog(g_layer_world, c, 5000.0f, 600.0f, 4500.0f);	// 0x509a30
	WorldFog(g_layer_world, c, 5000.0f, 600.0f, 4500.0f);	// 0x509a3c
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01BGame 'LV1B'

CLevel01BGame::CLevel01BGame()												// 0x44f300
{
	static char name[] = "CLEVEL01AGAME";		// sic: level 1a's name
	SetName(name);
	Log("CLevel01BGame()");
	CGameType::InitObject();
	CGameType::SetID('LV1B');
	m_pickup_index_base = 300;
}

CLevel01BGame::~CLevel01BGame()												// 0x44f420
{
	Log("~CLevel01BGame()");
}

void CLevel01BGame::InitGame(void)											// 0x44f4b0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	InitTerrainDatabase("level1b.omt");					// element +0xe4
	CreateTerrainTinted(1.0f, 1.0f, 1.0f, 1.0f);			// element +0x158 (BG slot 86)
	ViewportToOrigin();
	// Far plane 10000 (through both world-layer globals), fog off (through 0x509a3c only: the
	// same layer).
	neg::LodFarClip(g_layer_world, 10000.0f);	// 0x509a30
	neg::LodFarClip(g_layer_world, 10000.0f);					// 2002 +0xc0
	SetLayerFog(g_layer_world, false);						// 0x476910(layer, 0x800)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01CGame 'LV1C'

CLevel01CGame::CLevel01CGame()												// 0x44f630
{
	static char name[] = "CLEVEL01CGAME";
	SetName(name);
	Log("CLevel01CGame()");
	CGameType::InitObject();
	CGameType::SetID('LV1C');
	m_pickup_index_base = 400;
}

CLevel01CGame::~CLevel01CGame()												// 0x44f750
{
	Log("~CLevel01Game()");						// sic
}

void CLevel01CGame::InitGame(void)											// 0x44f7e0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	InitTerrainDatabase("level1c.omt");
	CreateTerrainUnLit();									// element +0xf8
	ViewportToOrigin();
	neg::LodFarClip(g_layer_world, 10000.0f);	// 0x509a30
	neg::LodFarClip(g_layer_world, 10000.0f);					// as level 1b: far plane only, fog off
	SetLayerFog(g_layer_world, false);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01EGame 'LV1E'

CLevel01EGame::CLevel01EGame()												// 0x44fcd0
{
	static char name[] = "CLEVEL01EGAME";
	SetName(name);
	Log("CLevel01EGame()");
	CGameType::InitObject();
	CGameType::SetID('LV1E');
	m_pickup_index_base = 600;
}

CLevel01EGame::~CLevel01EGame()												// 0x44fdf0
{
}

// The same as CLevel01DGame::InitGame with level1e.omt.
void CLevel01EGame::InitGame(void)											// 0x44fe30
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	LocalSky("omt\\space.omt");								// a local: m_sky stays NULL
	InitTerrainDatabase("level1e.omt");
	CreateTerrain();										// element +0xf4 (lit)
	ViewportToOrigin();
	neg::LodFarClip(g_layer_world, 120000.0f);	// 0x509a30
	neg::LodFarClip(g_layer_world, 120000.0f);					// 0x509a3c
	SetLayerFog(g_layer_world, false);
	SetLayerFog(g_layer_world, false);	// 0x509a30, after 0x509a3c
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01FGame 'LV1F'

CLevel01FGame::CLevel01FGame()												// 0x450000
{
	static char name[] = "CLEVEL01DGAME";		// sic: level 1d's name and log
	SetName(name);
	Log("CLevel01DGame()");
	CGameType::InitObject();
	CGameType::SetID('LV1F');
	m_pickup_index_base = 700;
}

CLevel01FGame::~CLevel01FGame()												// 0x450120
{
}

void CLevel01FGame::InitGame(void)											// 0x450160
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	LocalSky("omt\\space.omt");								// a local: no sky rotation
	InitTerrainDatabase("level1f.omt");
	CreateTerrain();
	ViewportToOrigin();
	neg::LodFarClip(g_layer_world, 15000.0f);	// 0x509a30
	neg::LodFarClip(g_layer_world, 15000.0f);	// 0x509a3c
	const OMediaFARGBColor c = RGBA(0.0f, 0.0f, 0.0f, 0.0f);
	g_layer_clear->set_clear_color(c);						// 0x509a38 +0x10c
	neg::LodFogRange(g_layer_world, 2000.0f, 14000.0f);
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_world, true);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevel01Game(void)		{ return new CLevel01Game; }		// 'LEV1' (0x5c4 bytes)
CGameObject *New_CLevel01AGame(void)	{ return new CLevel01AGame; }		// 'LV1A' (0x5c0 bytes)
CGameObject *New_CLevel01BGame(void)	{ return new CLevel01BGame; }		// 'LV1B'
CGameObject *New_CLevel01CGame(void)	{ return new CLevel01CGame; }		// 'LV1C'
CGameObject *New_CLevel01EGame(void)	{ return new CLevel01EGame; }		// 'LV1E'
CGameObject *New_CLevel01FGame(void)	{ return new CLevel01FGame; }		// 'LV1F'
}

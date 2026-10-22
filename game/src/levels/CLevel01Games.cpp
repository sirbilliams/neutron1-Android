/*  The Retroville level game types - see CLevel01Games.h. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel01Games.h"
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

// 0x4508d0 (shared with L1_A, L1_B, L1_C, LV4B): only CJimmyGame's.
void CLevel01Game::Process(float dt)	{ CJimmyGame::Process(dt); }

void CLevel01Game::InitGame(void)											// 0x450580
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// slot 108: not written to the .gam (f_00b)
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
	g_layer_world->set_far_clip(28000.0f);					// 0x509a3c +0xc0
	g_layer_world->set_fog_color(fog);						// +0xc8
	g_layer_world->set_fog_range(13000.0f, 17000.0f);		// +0xd8 / +0xdc
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
	SetFog(0.28f, 0.3f, 0.4f, 0.0f, 600.0f, 4500.0f, 5000.0f, true);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01BGame 'LV1B'

CLevel01BGame::CLevel01BGame()												// 0x450000
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
	InitTerrainDatabase("level1b.omt");
	CreateTerrainUnLit();
	ViewportToOrigin();
	SetFog(0.0f, 0.0f, 0.0f, 1.0f, 10000.0f, 12000.0f, 12000.0f, true);
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
	CreateTerrainUnLit();
	ViewportToOrigin();
	SetFog(0.0f, 0.0f, 0.0f, 1.0f, 10000.0f, 12000.0f, 12000.0f, true);
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
	m_pickup_index_base = 1900;					// (the same as CLevel03Game's)
}

CLevel01EGame::~CLevel01EGame()												// 0x44fdf0
{
}

// 0x44fe30. Quirk: it loads level 3's terrain, and sets no fog.
void CLevel01EGame::InitGame(void)
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	InitTerrainDatabase("level3.omt");
	CreateTerrainUnLit();
	ViewportToOrigin();
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01FGame 'LV1F'

CLevel01FGame::CLevel01FGame()												// 0x454f00
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
	C3DObject *sky = CreateSky();							// a local: no sky rotation
	SkyLoad(sky, "omt\\space.omt", false);
	sky->place(0.0f, 0.0f, 0.0f);
	sky->InitPhysics();
	sky->v67(1.0f);
	sky->v152(1.0f);
	sky->ev51();
	InitTerrainDatabase("level1f.omt");
	CreateTerrain();
	ViewportToOrigin();
	SetFog(0.0f, 0.0f, 0.0f, 0.0f, 2000.0f, 14000.0f, 15000.0f, true);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01_AGame 'L1_A'

CLevel01_AGame::CLevel01_AGame()											// 0x455330
{
	static char name[] = "CLEVEL01_AGAME";
	SetName(name);
	Log("CLevel01_AGame()");
	CGameType::InitObject();
	CGameType::SetID('L1_A');
	m_pickup_index_base = 800;
}

CLevel01_AGame::~CLevel01_AGame()											// N2:0x4866a0
{
	Log("~CLevel01_AGame()");
}

void CLevel01_AGame::Process(float dt)	{ CJimmyGame::Process(dt); }		// 0x4508d0

void CLevel01_AGame::InitGame(void)											// N2:0x486730
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	m_sky = CreateSky();
	SkyLoad(m_sky, "omt\\wintersky.omt", false);
	m_sky->ev51();
	m_sky->place(0.0f, 0.0f, 0.0f);
	m_sky->InitPhysics();
	m_sky->v67(1.0f);
	m_sky->v152(1.0f);
	m_sky_rotates = 0;
	InitTerrainDatabase("level1_A.omt");
	CreateTerrainTinted(1.0f, 1.0f, 1.0f, 1.0f);			// element +0x168
	ViewportToOrigin();
	SetFog(0.2f, 0.2f, 0.2f, 0.0f, 13000.0f, 17000.0f, 28000.0f, false);	// (fog off)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01_BGame 'L1_B'

CLevel01_BGame::CLevel01_BGame()											// 0x454b80
{
	static char name[] = "CLEVEL01_BGAME";
	SetName(name);
	Log("CLevel01_BGame()");
	CGameType::InitObject();
	CGameType::SetID('L1_B');
	m_pickup_index_base = 800;
}

CLevel01_BGame::~CLevel01_BGame()											// N2:0x486ab0
{
	Log("~CLevel01_BGame()");
}

void CLevel01_BGame::Process(float dt)	{ CJimmyGame::Process(dt); }		// 0x4508d0

void CLevel01_BGame::InitGame(void)											// N2:0x486b40
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	m_sky = CreateSky();
	SkyLoad(m_sky, "omt\\dusksky.omt", false);
	m_sky->ev51();
	m_sky->place(0.0f, 0.0f, 0.0f);
	m_sky->InitPhysics();
	m_sky->v67(1.0f);
	m_sky->v152(1.0f);
	InitTerrainDatabase("level1_B.omt");
	CreateTerrainTinted(1.0f, 1.0f, 1.0f, 1.0f);
	ViewportToOrigin();
	SetFog(0.6f, 0.37f, 0.25f, 0.0f, 1000.0f, 5000.0f, 7500.0f, true);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel01_CGame 'L1_C'

CLevel01_CGame::CLevel01_CGame()											// 0x44f300
{
	static char name[] = "CLEVEL01_CGAME";
	SetName(name);
	Log("CLevel01_CGame()");
	CGameType::InitObject();
	CGameType::SetID('L1_C');
	m_pickup_index_base = 800;
}

CLevel01_CGame::~CLevel01_CGame()											// N2:0x486eb0
{
	Log("~CLevel01_CGame()");
}

void CLevel01_CGame::Process(float dt)	{ CJimmyGame::Process(dt); }		// 0x4508d0

void CLevel01_CGame::InitGame(void)											// N2:0x486f40
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	m_sky = CreateSky();
	m_sky_rotates = 0;
	SkyLoad(m_sky, "omt\\nightsky.omt", false);
	m_sky->ev51();
	m_sky->place(0.0f, 0.0f, 0.0f);
	m_sky->InitPhysics();
	m_sky->v67(1.0f);
	m_sky->v152(1.0f);
	InitTerrainDatabase("level1_C.omt");
	CreateTerrainTinted(0.7f, 0.7f, 0.8f, 1.0f);
	ViewportToOrigin();
	SetFog(0.2f, 0.2f, 0.2f, 0.0f, 13000.0f, 17000.0f, 28000.0f, true);
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevel01Game(void)		{ return new CLevel01Game; }		// 'LEV1' (0x690 bytes)
CGameObject *New_CLevel01AGame(void)	{ return new CLevel01AGame; }		// 'LV1A' (0x68c bytes)
CGameObject *New_CLevel01BGame(void)	{ return new CLevel01BGame; }		// 'LV1B'
CGameObject *New_CLevel01CGame(void)	{ return new CLevel01CGame; }		// 'LV1C'
CGameObject *New_CLevel01EGame(void)	{ return new CLevel01EGame; }		// 'LV1E'
CGameObject *New_CLevel01FGame(void)	{ return new CLevel01FGame; }		// 'LV1F'
CGameObject *New_CLevel01_AGame(void)	{ return new CLevel01_AGame; }		// 'L1_A'
CGameObject *New_CLevel01_BGame(void)	{ return new CLevel01_BGame; }		// 'L1_B'
CGameObject *New_CLevel01_CGame(void)	{ return new CLevel01_CGame; }		// 'L1_C'
}

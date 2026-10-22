/*  The level 4 game types - see CLevel04Games.h. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel04Games.h"
#include "LevelHelpers.h"
#include <string.h>
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

// The sky of LEV4 and LV4C by the story stage; the night sky stands still. LV4C also tints its
// terrain by the stage (as LEV1 does; `tint` is r, g, b, NULL for LEV4). (Inline in both; the compiler shares
// the SkyLoad call between the cases.)
static void LoadStageSky(CJimmyGame *game, char stage, float *tint)
{
	switch (stage)
	{
		case 0:	SkyLoad(game->m_sky, "omt\\bluesky3.omt", false); break;
		case 1:
			SkyLoad(game->m_sky, "omt\\dusksky.omt", false);
			if (tint) { tint[0] = 1.0f; tint[1] = 0.9f; tint[2] = 0.9f; }
			break;
		case 2:
			game->m_sky_rotates = 0;
			SkyLoad(game->m_sky, "omt\\nightsky.omt", false);
			if (tint) { tint[0] = 0.7f; tint[1] = 0.7f; tint[2] = 0.8f; }
			break;
	}
}

// --------------------------------------------------------------------------- CLevel04Game 'LEV4'

CLevel04Game::CLevel04Game()												// 0x4542f0
{
	static char name[] = "CLEVEL04GAME";
	SetName(name);
	Log("CLevel04Game()");
	CGameType::InitObject();
	CGameType::SetID('LEV4');
	m_pickup_index_base = 2400;
}

CLevel04Game::~CLevel04Game()												// 0x454410
{
	Log("~CLevel04Game()");
}

void CLevel04Game::InitGame(void)											// 0x4544a0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// not written to the .gam (f_00b)
	m_sky = CreateSky();
	if (m_sky)
	{
		LoadStageSky(this, GetSceneStage(), NULL);			// element +0x15c, sky element +0xe0
		m_sky->ev51();										// element +0xcc
		m_sky->place(0.0f, 0.0f, 0.0f);
		m_sky->InitPhysics();								// (empty for a C3DSky)
		m_sky->v67(1.0f);									// radius
		m_sky->v152(1.0f);
	}
	InitTerrainDatabase("level4.omt");
	CreateTerrain();										// element +0xf4 (lit)
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.24f, 0.56f, 0.75f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 2002 +0xc8
	g_layer_world->set_far_clip(30000.0f);					// (0x509a30) 2002 +0xc0
	g_layer_world->set_fog_color(c);
	g_layer_world->set_far_clip(30000.0f);					// (0x509a3c)
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_clear, false);						// 0x476910(layer, 0x800)
	SetLayerFog(g_layer_world, false);						// (0x509a3c)
	SetLayerFog(g_layer_world, false);						// (0x509a30)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel04AGame 'LV4A'

CLevel04AGame::CLevel04AGame()												// 0x452eb0
{
	static char name[] = "CLEVEL04AGAME";
	SetName(name);
	Log("CLevel04AGame()");
	CGameType::InitObject();
	CGameType::SetID('LV4A');
	m_pickup_index_base = 2000;
}

CLevel04AGame::~CLevel04AGame()												// 0x452fd0
{
	Log("~CLevel04aGame()");
}

// 0x453060. Quirk (as LV4D): the fog range is set (on 0x509a3c only), then the fog is
// turned off.
void CLevel04AGame::InitGame(void)
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	LocalSky("omt\\nightsky.omt");							// a local: m_sky stays NULL
	InitTerrainDatabase("level4a.omt");
	CreateTerrainUnLit();									// element +0xf8
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.24f, 0.96f, 0.25f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 2002 +0xc8
	g_layer_world->set_far_clip(10000.0f);					// (0x509a30)
	g_layer_world->set_fog_color(c);
	g_layer_world->set_far_clip(10000.0f);					// (0x509a3c)
	g_layer_world->set_fog_color(c);
	g_layer_world->set_fog_range(1000.0f, 9600.0f);			// (0x509a3c) 2002 +0xd8 / +0xdc
	SetLayerFog(g_layer_world, false);						// (0x509a30) 0x476910(layer, 0x800)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel04BGame 'LV4B'

CLevel04BGame::CLevel04BGame()												// 0x4532e0
{
	static char name[] = "CLEVEL04BGAME";
	SetName(name);
	Log("CLevel04BGame()");
	CGameType::InitObject();
	CGameType::SetID('LV4B');
	strcpy(m_task_name, "POWER");
	m_sweep_time = 0.0f;
	m_sweeping = 0;
	m_pickup_index_base = 2100;
}

CLevel04BGame::~CLevel04BGame()												// 0x453430
{
	Log("~CLevel04bGame()");
}

void CLevel04BGame::UnInitObject(void)	{ CGameType::UnInitObject(); }		// 0x4513d0

// 0x4536c0. Once the power task has started (v267) the fog sweeps outwards for 3 seconds
// (19000 units a second), then it is switched off with the far plane at 22000. Every layer is
// written the way InitGame writes them (level 4b's fog colour, 0.1 / 0.56 / 0.1).
void CLevel04BGame::Process(float dt)
{
	CJimmyGame::Process(dt);
	if (!m_sweeping) return;
	m_sweep_time += dt;
	const OMediaFARGBColor c = RGBA(0.1f, 0.56f, 0.1f, 0.0f);
	const float t = m_sweep_time * 19000.0f;
	g_layer_clear->set_fog_color(c);						// 0x509a38
	g_layer_clear->set_fog_range(t + 11000.0f, t + 12000.0f);
	SetLayerFog(g_layer_clear, true);
	g_layer_world->set_far_clip(t + 12000.0f);				// 0x509a30
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_world, true);
	g_layer_world->set_fog_range(500.0f, t + 10000.0f);
	g_layer_world->set_fog_range(500.0f, t + 10000.0f);		// 0x509a3c
	g_layer_world->set_far_clip(t + 12000.0f);
	g_layer_world->set_fog_color(c);
	if (m_sweep_time >= 3.0f)
	{
		m_sweeping = 0;
		SetLayerFog(g_layer_world, false);					// 0x509a3c, then 0x509a30
		g_layer_world->set_far_clip(22000.0f);
	}
}

// 0x4538b0: CJimmyGame's, then the task name again (a level load may have replaced it).
void CLevel04BGame::v260(void)
{
	CJimmyGame::v260();
	strcpy(m_task_name, "POWER");
}

// 0x453690: CLocalGameObject's (the task state), then the sweep starts when the state is 1
// ("Calling Settaskstate for Level04B  %d" is a debug log, empty in the shipped build).
void CLevel04BGame::v267(int state)
{
	CLocalGameObject::v267(state);
	if (state == 1) m_sweeping = 1;
}

void CLevel04BGame::InitGame(void)											// 0x4534c0
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	InitTerrainDatabase("level4b.omt");
	CreateTerrainUnLit();									// element +0xf8
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.1f, 0.56f, 0.1f, 0.0f);
	WorldFog(g_layer_world, c, 7200.0f, 500.0f, 7000.0f);	// (0x509a30)
	g_layer_clear->set_clear_color(c);						// 0x509a38 +0x10c
	WorldFog(g_layer_world, c, 7200.0f, 500.0f, 7000.0f);	// (0x509a3c)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel04CGame 'LV4C'

CLevel04CGame::CLevel04CGame()												// 0x453960
{
	static char name[] = "CLEVEL04CGAME";
	SetName(name);
	Log("CLevel04CGame()");
	CGameType::InitObject();
	CGameType::SetID('LV4C');
	m_pickup_index_base = 2200;
}

CLevel04CGame::~CLevel04CGame()												// 0x453a80
{
	Log("~CLevel04cGame()");
}

void CLevel04CGame::InitGame(void)											// 0x453b10
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	float tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };				// the terrain colour for the stage
	m_sky = CreateSky();
	if (m_sky)
	{
		LoadStageSky(this, GetSceneStage(), tint);
		m_sky->ev51();
		m_sky->place(0.0f, 0.0f, 0.0f);
		m_sky->InitPhysics();
		m_sky->v67(1.0f);
		m_sky->v152(1.0f);
	}
	InitTerrainDatabase("level4c.omt");
	CreateTerrainTinted(tint[0], tint[1], tint[2], tint[3]);	// element +0x158
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.031f, 0.34f, 0.58f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 2002 +0xc8
	g_layer_clear->set_clear_color(c);						// 2002 +0x10c
	g_layer_world->set_far_clip(30000.0f);					// (0x509a30)
	g_layer_world->set_fog_color(c);
	g_layer_world->set_far_clip(30000.0f);					// (0x509a3c)
	g_layer_world->set_fog_color(c);
	SetLayerFog(g_layer_world, false);						// (0x509a3c) 0x476910(layer, 0x800)
	SetLayerFog(g_layer_world, false);						// (0x509a30)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevel04DGame 'LV4D'

CLevel04DGame::CLevel04DGame()												// 0x453ec0
{
	static char name[] = "CLEVEL04DGAME";
	SetName(name);
	Log("CLevel04DGame()");
	CGameType::InitObject();
	CGameType::SetID('LV4D');
	m_pickup_index_base = 2300;
}

CLevel04DGame::~CLevel04DGame()												// 0x453fe0
{
	Log("~CLevel04DGame()");
}

// 0x454070. Quirk: the fog range is set (on 0x509a3c only), then the fog is turned off.
void CLevel04DGame::InitGame(void)
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);
	C3DObject *sky = CreateSky();							// a local: m_sky stays NULL
	SkyLoad(sky, "omt\\nightsky.omt", false);
	sky->place(0.0f, 0.0f, 0.0f);
	sky->InitPhysics();
	sky->v67(1.0f);
	sky->v152(1.0f);
	sky->ev51();
	InitTerrainDatabase("level4d.omt");
	CreateTerrainUnLit();
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.24f, 0.96f, 0.25f, 0.0f);
	g_layer_clear->set_fog_color(c);						// 2002 +0xc8
	g_layer_world->set_far_clip(10000.0f);					// (0x509a30)
	g_layer_world->set_fog_color(c);
	g_layer_world->set_far_clip(10000.0f);					// (0x509a3c)
	g_layer_world->set_fog_color(c);
	g_layer_world->set_fog_range(1000.0f, 9600.0f);			// (0x509a3c) 2002 +0xd8 / +0xdc
	SetLayerFog(g_layer_world, false);						// (0x509a30) 0x476910(layer, 0x800)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevel04Game(void)		{ return new CLevel04Game; }		// 'LEV4' (0x68c bytes)
CGameObject *New_CLevel04AGame(void)	{ return new CLevel04AGame; }		// 'LV4A'
CGameObject *New_CLevel04BGame(void)	{ return new CLevel04BGame; }		// 'LV4B' (0x694 bytes)
CGameObject *New_CLevel04CGame(void)	{ return new CLevel04CGame; }		// 'LV4C'
CGameObject *New_CLevel04DGame(void)	{ return new CLevel04DGame; }		// 'LV4D'
}

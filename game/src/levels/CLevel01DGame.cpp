/*  CLevel01DGame - see CLevel01DGame.h. Reconstructed from Neutron.exe 0x44f920..0x44fcc0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel01DGame.h"
#include "LevelHelpers.h"
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

CLevel01DGame::CLevel01DGame()												// 0x44f920
{
	static char name[] = "CLEVEL01DGAME";		// SetName upper-cases in place
	SetName(name);
	Log("CLevel01DGame()");
	CGameType::InitObject();					// (direct call) -> InitGame
	CGameType::SetID('LV1D');
	m_pickup_index_base = 500;
	// (debug log, empty in the shipped build)
}

CLevel01DGame::~CLevel01DGame()												// 0x44fa40
{
	Log("~CLevel01DGame()");
}

// 0x44fad0. The original then logs "UI L2 dest 1", "UI L2 dest 2", "UI L2 dest 3" (debug
// calls, empty in the shipped build).
void CLevel01DGame::UnInitObject(void)
{
	CGameType::UnInitObject();
}

void CLevel01DGame::InitGame(void)											// 0x44fb00
{
	Log("InitGame()");
	CJimmyGame::InitGame();
	v107(0);												// not written to the .gam (f_00b)

	C3DObject *sky = CreateSky();							// a local: m_sky stays NULL (no sky rotation)
	SkyLoad(sky, "omt\\space.omt", false);				// sky element +0xe0 (BG slot 56)
	sky->place(0.0f, 0.0f, 0.0f);
	sky->InitPhysics();										// (empty for a C3DSky)
	sky->v67(1.0f);											// radius (BG +0x110)
	sky->v152(1.0f);										// (BG +0x264)
	sky->ev51();											// element +0xcc

	InitTerrainDatabase("level1d.omt");						// element +0xe4
	CreateTerrain();										// element +0xf4 (lit)
	ViewportToOrigin();
	// Far plane 120000 and no fog (0x476910(layer, 0x800)), each set through both world-layer
	// globals (0x509a30, 0x509a3c: the same layer).
	g_layer_world->set_far_clip(120000.0f);					// 2002 +0xc0
	SetLayerFog(g_layer_world, false);
	CaptureMouse(true);										// 0x46a400
	// (debug log)
}

namespace neg {
CGameObject *New_CLevel01DGame(void)	{ return new CLevel01DGame; }		// factory 'LV1D' (0x68c bytes)
}

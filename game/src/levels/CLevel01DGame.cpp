/*  CLevel01DGame - see CLevel01DGame.h. Reconstructed from Neutron.exe 0x44f920..0x44fcc0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevel01DGame.h"
#include "../port/Lod.h"		// port: draw distance: far plane / fog range scaled (NEG_LOD), 1 = the exe
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
	v107(0);												// slot 108: not written to the .gam (f_00b)

	C3DObject *sky = CreateSky();							// a local: m_sky stays NULL (no sky rotation)
	SkyLoad(sky, "omt\\space.omt", false);				// sky element +0xe0
	sky->place(0.0f, 0.0f, 0.0f);							// (+0x44..+0x4c written directly)
	sky->InitPhysics();										// (empty for a C3DSky)
	sky->v67(1.0f);											// radius
	sky->v152(1.0f);
	sky->ev51();											// element +0xcc

	InitTerrainDatabase("level1d.omt");						// element +0xe4
	CreateTerrain();										// element +0xf4 (lit; Negatron's level1d was unlit)
	ViewportToOrigin();
	neg::LodFarClip(g_layer_world, 120000.0f);					// 0x509a30 +0xc0, then the same through 0x509a3c
	SetLayerFog(g_layer_world, false);						// 0x476910(0x509a3c, 0x800), then 0x509a30
	CaptureMouse(true);										// 0x46a400
	// (debug log)
}

namespace neg {
CGameObject *New_CLevel01DGame(void)	{ return new CLevel01DGame; }		// factory 'LV1D' (0x68c bytes)
}

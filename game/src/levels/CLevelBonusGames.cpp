/*  The bonus level game types - see CLevelBonusGames.h. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLevelBonusGames.h"
#include "LevelHelpers.h"
#pragma GCC diagnostic pop

using namespace neg;
using namespace levels;

// The InitGame of every VR level. Quirk: the fog ends at 33000 through one world-layer global
// and at 30000 through the other; both are the same layer, so the second (30000) stays.
static void InitVR(CJimmyGame *game, const char *omt)
{
	game->Log("InitGame()");
	game->CJimmyGame::InitGame();
	game->InitTerrainDatabase(omt);
	game->v107(0);											// not written to the .gam (f_00b)
	game->CreateTerrainUnLit();								// element +0xf8
	ViewportToOrigin();
	const OMediaFARGBColor c = RGBA(0.031f, 0.34f, 0.58f, 0.0f);
	g_layer_clear->set_clear_color(c);						// 0x509a38 +0x10c
	WorldFog(g_layer_world, c, 50000.0f, 20000.0f, 33000.0f);	// (0x509a30)
	WorldFog(g_layer_world, c, 50000.0f, 20000.0f, 30000.0f);	// (0x509a3c)
	CaptureMouse(true);
}

// --------------------------------------------------------------------------- CLevelVR01 'VR01'

CLevelVR01::CLevelVR01()													// 0x455ef0
{
	static char name[] = "CLEVELVR01";
	SetName(name);
	Log("CLevelVR01()");
	CGameType::InitObject();
	CGameType::SetID('VR01');
	m_pickup_index_base = 3100;
}

CLevelVR01::~CLevelVR01()													// 0x456010
{
}

void CLevelVR01::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR01::InitGame(void)												// 0x456050
{
	InitVR(this, "vr01.omt");
}

// --------------------------------------------------------------------------- CLevelVR02 'VR02'

CLevelVR02::CLevelVR02()													// 0x456270
{
	static char name[] = "CLEVELVR02";
	SetName(name);
	Log("CLevelVR02()");
	CGameType::InitObject();
	CGameType::SetID('VR02');
	m_pickup_index_base = 3200;
}

CLevelVR02::~CLevelVR02()													// 0x456390
{
}

void CLevelVR02::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR02::InitGame(void)												// 0x4563d0
{
	InitVR(this, "vr02.omt");
}

// --------------------------------------------------------------------------- CLevelVR03 'VR03'

CLevelVR03::CLevelVR03()													// 0x4565f0
{
	static char name[] = "CLEVELVR03";
	SetName(name);
	Log("CLevelVR03()");
	CGameType::InitObject();
	CGameType::SetID('VR03');
	m_pickup_index_base = 3300;
}

CLevelVR03::~CLevelVR03()													// 0x456710
{
}

void CLevelVR03::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR03::InitGame(void)												// 0x456750
{
	InitVR(this, "vr03.omt");
}

// --------------------------------------------------------------------------- CLevelVR04 'VR04'

CLevelVR04::CLevelVR04()													// 0x456970
{
	static char name[] = "CLEVELVR04";
	SetName(name);
	Log("CLevelVR04()");
	CGameType::InitObject();
	CGameType::SetID('VR04');
	m_pickup_index_base = 3400;
}

CLevelVR04::~CLevelVR04()													// 0x456a90
{
}

void CLevelVR04::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR04::InitGame(void)												// 0x456ad0
{
	InitVR(this, "vr04.omt");
}

// --------------------------------------------------------------------------- CLevelVR05 'VR05'

CLevelVR05::CLevelVR05()													// 0x456cf0
{
	static char name[] = "CLEVELVR05";
	SetName(name);
	Log("CLevelVR05()");
	CGameType::InitObject();
	CGameType::SetID('VR05');
	m_pickup_index_base = 3500;
}

CLevelVR05::~CLevelVR05()													// 0x456e10
{
}

void CLevelVR05::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR05::InitGame(void)												// 0x456e50
{
	InitVR(this, "vr05.omt");
}

// --------------------------------------------------------------------------- CLevelVR06 'VR06'

CLevelVR06::CLevelVR06()													// 0x457070
{
	static char name[] = "CLEVELVR06";
	SetName(name);
	Log("CLevelVR06()");
	CGameType::InitObject();
	CGameType::SetID('VR06');
	m_pickup_index_base = 3600;
}

CLevelVR06::~CLevelVR06()													// 0x457190
{
}

void CLevelVR06::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR06::InitGame(void)												// 0x4571d0
{
	InitVR(this, "vr06.omt");
}

// --------------------------------------------------------------------------- CLevelVR07 'VR07'

CLevelVR07::CLevelVR07()													// 0x4573f0
{
	static char name[] = "CLEVELVR07";
	SetName(name);
	Log("CLevelVR07()");
	CGameType::InitObject();
	CGameType::SetID('VR07');
	m_pickup_index_base = 3700;
}

CLevelVR07::~CLevelVR07()													// 0x457510
{
}

void CLevelVR07::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR07::InitGame(void)												// 0x457550
{
	InitVR(this, "vr07.omt");
}

// --------------------------------------------------------------------------- CLevelVR08 'VR08'

CLevelVR08::CLevelVR08()													// 0x457770
{
	static char name[] = "CLEVELVR08";
	SetName(name);
	Log("CLevelVR08()");
	CGameType::InitObject();
	CGameType::SetID('VR08');
	m_pickup_index_base = 3800;
}

CLevelVR08::~CLevelVR08()													// 0x457890
{
}

void CLevelVR08::UnInitObject(void)	{ CGameType::UnInitObject(); }			// 0x4513d0

void CLevelVR08::InitGame(void)												// 0x4578d0
{
	InitVR(this, "vr08.omt");
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLevelVR01(void)		{ return new CLevelVR01; }			// 'VR01' (0x5c0 bytes)
CGameObject *New_CLevelVR02(void)		{ return new CLevelVR02; }			// 'VR02'
CGameObject *New_CLevelVR03(void)		{ return new CLevelVR03; }			// 'VR03'
CGameObject *New_CLevelVR04(void)		{ return new CLevelVR04; }			// 'VR04'
CGameObject *New_CLevelVR05(void)		{ return new CLevelVR05; }			// 'VR05'
CGameObject *New_CLevelVR06(void)		{ return new CLevelVR06; }			// 'VR06'
CGameObject *New_CLevelVR07(void)		{ return new CLevelVR07; }			// 'VR07'
CGameObject *New_CLevelVR08(void)		{ return new CLevelVR08; }			// 'VR08'
}

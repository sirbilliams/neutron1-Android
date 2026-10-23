/*  C3DGeyser - see C3DGeyser.h. Reconstructed from Neutron.exe 0x41bfd0..0x41c2a0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DGeyser.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "OMedia3DShape.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DGeyser::C3DGeyser()										// 0x41bfd0
{
	SetName((char *)"C3DGEYSER");							// port: the original calls SetName (0x4702d0) without pushing an argument (it gets a stray stack value, the caller's edi)
	C3DGeyser::InitObject();								// 0x41c1a0 (called directly)
	C3DObject::SetObjectID('3GEY');							// 0x462220
	v52(0);
	v46(0);
	v34(1);
	m_puffs = 5;
	for (int i = 0; i < m_puffs; i++) m_puff[i] = NULL;		// (the valve's puffs are left behind)
	m_enabled = 1;
	m_steam_period = 5.0f;
	f_734 = 0;
	m_sound = -1;
	v107(0);												// +0x1b0
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DGeyser::~C3DGeyser()										// 0x41c150
{
}

namespace neg {
CGameObject *New_C3DGeyser(void) { return new C3DGeyser; }				// '3GEY' (0x748 bytes)
}

void C3DGeyser::InitObject(void)							// 0x41c1a0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("SteamPeriod", &m_steam_period, PROP_FLOAT, 0);
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 8));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// 0x477550
	v66(115.0f);											// +0x108
	ev53();													// element +0xcc: v64 from the shape
}

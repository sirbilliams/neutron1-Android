/*  C3DHook - see C3DHook.h. Reconstructed from Neutron.exe 0x420060..0x420380. */
#include "C3DHook.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"

using namespace neg;

C3DHook::C3DHook()											// 0x420060
{
	SetName((char *)"C3DHOOK");
	Log("C3DHOOK()");
	C3DHook::InitObject();									// 0x420240 (called directly)
	C3DObject::SetObjectID('3HOO');							// 0x462220
	m_speed = 250.0f;
	m_state = 3;
	m_ai_state = 3;
	v46(1);
	v52(1);
	v42(0);
	v40(0);
	v94(2);
	m_turn_rates.x = 1.0f;
	m_turn_rates.y = 1.0f;
	m_turn_rates.z = 1.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DHook::~C3DHook()											// 0x4201f0
{
}

namespace neg {
CGameObject *New_C3DHook(void) { return new C3DHook; }					// '3HOO' (0x99c bytes)
}

void C3DHook::InitObject(void)								// 0x420240
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"hook.ase");	// +0xd8
	CreateTexture((char *)"fan.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(100.0f);											// radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DHook::Process(float dt)								// 0x4202d0
{
	C3DAI::Process(dt);										// 0x408000
	float a[4], b[4];
	v155(a);												// +0x270 constant acceleration (BG slot 156)
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
}

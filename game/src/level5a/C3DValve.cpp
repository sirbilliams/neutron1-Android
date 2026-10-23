/*  C3DValve - see C3DValve.h. Reconstructed from Neutron.exe 0x448780..0x448c60. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DValve.h"
#include "../objects/C3DSmokePuff.h"
#include "../Game.h"
#include "../Globals.h"
#include "../JimmySlots.h"
#include "../Sound.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DValve::C3DValve()										// 0x448780
{
	SetName((char *)"C3DVALVE");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DValve::InitObject();									// 0x448a30 (called directly)
	C3DObject::SetObjectID('3VAL');							// 0x462220
	v52(1);
	v46(0);
	v34(1);
	m_puffs = 5;
	for (int i = 0; i < m_puffs; i++)
	{
		m_puff[i] = new C3DSmokePuff;						// 0x440980 (0x62c bytes)
		m_puff[i]->force_show();							// inline: hide_count = 1, hide(false)
	}
	m_enabled = 1;
	m_steam_period = 5.0f;
	f_734 = 0;
	m_sound = -1;
	m_jimmy = NULL;
	f_728 = 0;
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DValve::~C3DValve()										// 0x448980
{
}

namespace neg {
CGameObject *New_C3DValve(void) { return new C3DValve; }				// '3VAL' (0x748 bytes)
}

void C3DValve::InitObject(void)								// 0x448a30
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("SteamPeriod", &m_steam_period, PROP_FLOAT, 0);
	InitAnim();												// element +0x108
	f_63c = 0;												// (BG +0x570) InitAnim done: cleared again
	v67(80.0f);												// +0x110: radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DValve::ev69(void)									// 0x4489d0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"valve.ase");	// element +0xd8
	CreateTexture((char *)"valve.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
}

void C3DValve::ResetObject(void)							// 0x448ba0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	f_728 = 0;
}

void C3DValve::AddTouching(CGameObject *other)				// 0x448ab0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_enabled == 0) return;
	if (!other->NameContains((char *)"C3DJIMMY")) return;
	JimmyElementSlot104(other, -10.0f, -10, 5.0f);			// Jimmy's element +0x1a0: shocked
	PlayGameSoundIndex(-1, 0xbc, false);					// 0x458980
	m_jimmy = other;
}

void C3DValve::RemoveTouching(CGameObject *other)			// 0x448b20
{
	CGameObject::RemoveTouching(other);						// 0x470b20
	if (other->NameContains((char *)"C3DJIMMY")) m_jimmy = NULL;
}

void C3DValve::v260(void)									// 0x448b50
{
	C3DAnimated::v260();									// 0x40e7b0
	if (m_enabled != 0) ev72();								// element +0x120
	else ev73();											// element +0x124
	v67(140.0f);											// +0x110: radius
}

void C3DValve::ev73(void)									// 0x448bd0
{
	if (m_sound != -1)
	{
		StopGameSound(m_sound, false);						// 0x458a00
		m_sound = -1;
	}
}

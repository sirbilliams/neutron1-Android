/*  C3DDarwinFish - see C3DDarwinFish.h. Reconstructed from Neutron.exe 0x416ac0..0x416ea0
    (and 0x4173c0). */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DDarwinFish.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DDarwinFish::C3DDarwinFish()								// 0x416ac0
{
	SetName((char *)"C3DDARWINFISH");						// 0x4702d0
	Log("C3DDarwinFish");
	f_649 = 0;												// ev69 loads the animations later
	m_is_pickup = 1;												// a pickup
	C3DDarwinFish::InitObject();							// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev106(f_a9c, (char *)"STOP");
	C3DAI::ev106(f_ac4, (char *)"WALK");
	C3DAI::ev106(f_b14, (char *)"STOP");
	C3DAI::ev106(f_b3c, (char *)"WALK");
	f_9a8 = 2000.0f;										// VisibleRange
	f_b98 = 90.0f;											// FOV
	f_c1c = 0.5f;
	C3DObject::SetObjectID('3FIS');							// 0x462220
	f_bd4 = 0;												// no hovering
	f_c28 = 1;
	m_pic_number = 12;												// PIC_NUMBER
}

C3DDarwinFish::~C3DDarwinFish()								// 0x416c70
{
	// ("Deleting DarwinFish": debug log)
}

namespace neg {
CGameObject *New_C3DDarwinFish(void) { return new C3DDarwinFish; }		// '3FIS' (0x9a8 bytes)
}

void C3DDarwinFish::InitObject(void)						// 0x4173c0
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
}

void C3DDarwinFish::ev69(void)								// 0x416d00
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"darwinwalk.ase");	// +0xd8
	CreateAnim((char *)"HISHRINK", (char *)"darwinshrink.ASE");
	CreateAnim((char *)"HISTOP", (char *)"darwinstop.ase");
	CreateTexture((char *)"darwin.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
}

void C3DDarwinFish::AddTouching(CGameObject *other)			// 0x416d90
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (!f_c40 || other != g_camera || g_object_states[m_pickup_index] != 0) return;
	OMediaElement::hide(true);								// element +0x58
	v232();													// +0x3a4
	// 0x42adc0: 50 points (no difficulty scaling; the pop-up shows the award)
	g_score += 50;
	g_score_popup = 50;
	g_score_popup_time = 1.2f;
	if (m_pic_number < 0) return;
	// 0x4061d0: the pickup pop-up (3 s, or queued behind the one showing), counted
	if (g_4f815c <= 0)
	{
		g_4f8158 = -1;
		g_4f8154 = m_pic_number;
		g_4f815c = 3.0f;
		g_4f8160 = (unsigned int)m_pickup_index;
	}
	else if (g_4f8154 != -1) g_4f8158 = m_pic_number;
	g_pickup_counts[2 * m_pic_number]++;							// 0x4f7db8
	if (m_pic_number < 29)											// 0x4038c0(2, pic, 1)
	{
		C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(2);
		if (p) p->items[m_pic_number].type = 1;
	}
	g_object_states[m_pickup_index] = 1;
}

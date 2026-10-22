/*  C3DDino - see C3DDino.h. Reconstructed from Neutron.exe 0x417100..0x4174f0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DDino.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DDino::C3DDino()											// 0x417100
{
	SetName((char *)"C3DDINO");
	Log("C3DDino");
	f_649 = 0;												// ev69 loads the animations later
	m_is_pickup = 1;										// a pickup
	C3DDino::InitObject();									// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_wag, (char *)"STOP");
	C3DAI::ev72(m_anim_run, (char *)"WALK");
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	C3DAI::ev72(m_anim_walk, (char *)"WALK");
	m_keep_fleeing = 0.5f;
	C3DObject::SetObjectID('3DIN');							// 0x462220
	m_hover = 0;											// no hovering
	m_958 = 1;
	m_is_pickup = 1;
	m_pic_number = 14;										// PIC_NUMBER
	m_speed = 400.0f;
}

C3DDino::~C3DDino()											// 0x4172b0
{
}

namespace neg {
CGameObject *New_C3DDino(void) { return new C3DDino; }		// '3DIN'
}

void C3DDino::InitObject(void)								// 0x4173c0
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
}

void C3DDino::ev69(void)									// 0x417300
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"dinowalk.ase");	// +0xd8
	CreateAnim((char *)"HISHRINK", (char *)"dinoshrink.ASE");
	CreateAnim((char *)"HISTOP", (char *)"dinostop.ase");
	CreateTexture((char *)"dino.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(400.0f);											// CGameObject slot 68 (+0x110) (field +0x34)
	v64(0.25f);												// CGameObject +0x100 (field +0x2c)
}

void C3DDino::AddTouching(CGameObject *other)				// 0x417400
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (!m_state_locked || other != g_camera || g_object_states[m_pickup_index] != 0) return;
	OMediaElement::hide(true);								// element +0x58
	v232();													// +0x3a4
	AddScore(50);											// 0x42adc0
	g_object_states[m_pickup_index] = 1;
	MenuSetItem(2, 14, 1);									// 0x4038c0
	AnnouncePickup(m_pic_number, (unsigned int)m_pickup_index);	// 0x4061d0
}

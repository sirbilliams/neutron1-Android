/*  C3DYokianShield - see C3DYokianShield.h. Reconstructed from Neutron.exe 0x44b510..0x44b7d0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokianShield.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DYokianShield::C3DYokianShield()							// 0x44b510
{
	SetName((char *)"C3DBUBBLE");
	Log("C3DBubble()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3YSH');									// element slot 48
	v36(1);
	v107(0);												// (BG slot 108)
	strcpy(m_sprite_db, "sprites.omt");						// (BG +0x584)
	m_sprite_index = 28;
	m_sprite_size = 300;
	v232();													// (BG slot 233)
	m_time = 0;
	// (then a call to the empty 0x472970)
}

C3DYokianShield::~C3DYokianShield()							// 0x44b680
{
}

void C3DYokianShield::ShowShield(void)						// 0x44b750
{
	m_time = 0.1f;
	force_show();											// (2002 +0x70 hide count: 1, then hide(false))
}

void C3DYokianShield::HideShield(void)						// 0x44b770
{
	hide(true);												// element +0x58
}

void C3DYokianShield::Process(float dt)						// 0x44b6f0
{
	C3DSpriteType::Process(dt);								// 0x40ff40 (C3DSprite's)
	if (m_time > 0)
	{
		m_time -= dt;
		if (m_time < 0) HideShield();						// element +0xd8
	}
}

void C3DYokianShield::v260(void)							// 0x44b6c0
{
	C3DSprite::v260();										// (BG slot 259)
	set_blend(omblendfc_Src_Alpha, omblendfc_One);			// 2002 +0xb4 = 6, +0xb8 = 1
	hide(true);												// element +0x58
}

/*  C3DLightObj - see C3DLightObj.h. Reconstructed from Neutron.exe 0x42dba0..0x41f1a0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DLightObj.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- construction

C3DLightObj::C3DLightObj()									// 0x42dba0
{
	SetName((char *)"C3DLIGHTOBJ");
	Log("C3DLightObj()");
	C3DLightObj::InitObject();								// 0x42ddc0 (called directly)
	SetObjectID('3LIO');									// 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 36;
	m_sprite_size = 100;
	v42(0);
	v40(0);
	v52(0);
	v107(1);
	v46(0);
	v232();
	m_alpha = 1.0f;
	m_red = 1.0f;
	m_green = 1.0f;
	m_blue = 1.0f;
	m_activated = 1;
	m_offset_time = 0;
	m_time = 0;
	m_pulse = 0;
	m_on_sound = -1;
	m_off_sound = -1;
	m_toggle_time = 0;
	m_pulsate = 0;
	m_period = 1.0f;
	// (then a call to the empty 0x472970)
}

C3DLightObj::~C3DLightObj() {}								// 0x42dd80

namespace neg {
CGameObject *New_C3DLightObj(void) { return new C3DLightObj; }			// '3LIO' (0x6a4 bytes)
}

// --------------------------------------------------------------------------- slots

void C3DLightObj::InitObject(void)							// 0x42ddc0
{
	Log("InitObject()");
	C3DSprite::InitObject();								// 0x464130
	RegisterProperty("Alpha", &m_alpha, PROP_FLOAT, 0);
	RegisterProperty("Red", &m_red, PROP_FLOAT, 0);
	RegisterProperty("Green", &m_green, PROP_FLOAT, 0);
	RegisterProperty("Blue", &m_blue, PROP_FLOAT, 0);
	RegisterProperty("Activated", &m_activated, PROP_LONG, 0);
	RegisterProperty("Pulsate", &m_pulsate, PROP_LONG, 0);
	RegisterProperty("Period", &m_period, PROP_FLOAT, 0);
	RegisterProperty("OffsetTime", &m_offset_time, PROP_FLOAT, 0);
	RegisterProperty("OnSoundIndex", &m_on_sound, PROP_LONG, 0);
	RegisterProperty("OffSoundIndex", &m_off_sound, PROP_LONG, 0);
	v67(10.0f);												// radius
}

void C3DLightObj::Process(float dt)							// 0x42df00
{
	C3DSprite::Process(dt);									// (no pause check)									// 0x463090
	m_time += dt;
	m_toggle_time += dt;
	if (!m_activated || !m_pulsate) return;
	const float half = m_period * 0.5f;						// 0x48d948
	float v = m_time;
	if (half <= m_time) v = half - (m_time - half);			// up, then down again
	m_pulse = v / half;
	if (m_period < m_time)
	{
		m_time = 0;
		set_diffuse(OMediaFARGBColor(0.0f, m_red, m_green, m_blue));	// +0x90..+0x9c
		return;
	}
	set_diffuse(OMediaFARGBColor(m_pulse, m_red, m_green, m_blue));
}

void C3DLightObj::v260(void)								// 0x42e030
{
	C3DSprite::v260();										// (C3DSprite::v259, not C3DSpriteType's)
	set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// +0xb4 = 6, +0xb8 = 7 / 1
	m_time = m_offset_time;
	if (m_activated)
	{
		set_diffuse(OMediaFARGBColor(m_alpha, m_red, m_green, m_blue));
		force_show();										// inline: hide_count (+0x70) = 1, hide(false)
		return;
	}
	OMediaElement::hide(true);								// element +0x58
}

void C3DLightObj::v268(int a2)								// 0x42e0f0
{
	if (a2 == -1)
	{
		if (m_toggle_time < 1.0f) return;					// 0x48d924
		m_toggle_time = 0;
		if (m_activated) goto off;
	}
	else if (a2 == 0) goto off;
	else if (a2 != 1) return;
	if (m_on_sound != -1) PlayGameSoundIndex(-1, m_on_sound, false);	// 0x458980
	m_activated = 1;
	force_show();
	return;
off:
	if (m_off_sound != -1) PlayGameSoundIndex(-1, m_off_sound, false);
	m_activated = 0;
	OMediaElement::hide(true);
}

/*  C3DNewSmokePuff - see C3DNewSmokePuff.h. Reconstructed from Neutron.exe 0x433070..0x433480. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DNewSmokePuff.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DNewSmokePuff::C3DNewSmokePuff()							// 0x433070
{
	f_608 = 0;
	f_618 = 1.0f;
	SetName((char *)"C3DNEWSMOKEPUFF");
	Log("C3DNEWSmokePuff()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3NSM');									// 0x462d80
	v107(0);												// +0x1b0
	v36(1);
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 37;
	m_sprite_size = 40;
	v52(0);
	v46(0);
	v42(0);
	v40(0);
	v232();
	C3DSprite::InitPhysics();								// 0x464210
	m_time = 0;
	m_life = 1.0f;
	m_red = 1.0f;
	m_green = 1.0f;
	m_blue = 1.0f;
	f_5fc = 1.0f;
	f_600 = 2.0f;
	f_604 = 0;
	f_60c = 0;
	f_610 = 0;
	f_614 = 0;
	m_grow = 100.0f;
	m_active = 1;
}

C3DNewSmokePuff::~C3DNewSmokePuff()							// 0x433270
{
}

namespace neg {
CGameObject *New_C3DNewSmokePuff(void) { return new C3DNewSmokePuff; }	// '3NSM' (0x630 bytes)
}

void C3DNewSmokePuff::Start(float x, float y, float z, float w)	// 0x4332d0
{
	m_active = 1;
	m_time = 0;
	m_size = (float)m_sprite_size;
	v196(x, y, z, w);										// CGameObject +0x314
	force_show();											// inline: hide_count = 1, hide(false)
}

void C3DNewSmokePuff::Process(float dt)						// 0x433340
{
	C3DSprite::Process(dt);									// 0x463090
	if (!m_active) return;
	m_time += dt;
	m_size = dt * m_grow + m_size;
	set_size(m_size, m_size);								// +0xac / +0xb0
	if (m_time < m_life)
	{
		set_diffuse(OMediaFARGBColor(1.0f - m_time / m_life, m_red, m_green, m_blue));	// +0x90..+0x9c
		return;
	}
	OMediaElement::hide(true);								// +0x58
	m_active = 0;
}

void C3DNewSmokePuff::v260(void)							// 0x4332b0
{
	C3DSprite::v260();										// 0x464810
	set_blend(omblendfc_Src_Alpha, omblendfc_One);			// +0xb4 = 6, +0xb8 = 1
}

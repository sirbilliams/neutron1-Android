/*  C3DSmokePuff - see C3DSmokePuff.h. Reconstructed from Neutron.exe 0x440980..0x440e20. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSmokePuff.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// The game's random number generator as the integer 0..1000 (the seed's high half * 1001 >> 16).
static int RandomThousand(void)
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	return (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
}

C3DSmokePuff::C3DSmokePuff()								// 0x440980
{
	f_608 = 0;
	m_pos[3] = 1.0f;
	SetName((char *)"C3DSMOKEPUFF");
	Log("C3DSmokePuff()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3SMO');									// 0x462d80
	v107(0);												// +0x1b0
	v36(1);
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 37;
	m_sprite_size = 40;
	v52(0);
	v46(1);
	v42(0);
	v40(0);
	v232();
	C3DSprite::InitPhysics();								// 0x464210
	m_time = 0;
	m_life = 10.0f;
	m_red = 1.0f;
	m_green = 1.0f;
	m_blue = 1.0f;
	f_5fc = 1.0f;
	f_600 = 2.0f;
	f_604 = 0;
	m_pos[0] = 0;
	m_pos[1] = 0;
	m_pos[2] = 0;
	m_grow = 80.0f;
	m_active = 1;
}

C3DSmokePuff::~C3DSmokePuff()								// 0x440b80
{
}

namespace neg {
CGameObject *New_C3DSmokePuff(void) { return new C3DSmokePuff; }		// '3SMO' (0x62c bytes)
}

void C3DSmokePuff::Process(float dt)						// 0x440bc0
{
	C3DSprite::Process(dt);									// 0x463090
	if (!m_active) return;
	m_time = dt + m_time;
	float a[4], b[4];
	v155(a);												// +0x270: the acceleration
	v155(b);
	v157(b[0], 25.0f - g_gravity.y, a[2]);					// +0x278: up, against gravity (0x48fe04, 0x5cfc04)
	const int r1 = RandomThousand();
	(void)RandomThousand();									// (a draw that is not used)
	const int r2 = RandomThousand();
	v178((float)(((double)r1 * (double)0.001f - 0.5) * 0.2), 0,
		 (float)(((double)r2 * (double)0.001f - 0.5) * 0.2));	// +0x2cc: a random jolt (0x48e5d8, 0x48e5d0, 0x4a2ad0)
	m_sprite_size = (int)((double)dt * m_grow + m_sprite_size);	// 0x47f940 (_ftol)
	if (m_sprite_size > 400) m_sprite_size = 40;
	set_size((float)m_sprite_size, (float)m_sprite_size);	// +0xac / +0xb0
	if (m_time < m_life)
	{
		set_diffuse(OMediaFARGBColor(1.0f - m_time / m_life, m_red, m_green, m_blue));	// +0x90..+0x9c
		return;
	}
	OMediaElement::hide(true);								// +0x58
}

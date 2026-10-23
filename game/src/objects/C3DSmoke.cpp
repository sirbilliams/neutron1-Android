/*  C3DSmoke - see C3DSmoke.h. Reconstructed from Neutron.exe 0x440530..0x440970. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSmoke.h"
#include "C3DSmokePuff.h"
#include "../Game.h"
#include "../Globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// The game's random number generator as a fraction: 0..1 in steps of 0.001 (0x48e5d8).
static float RandomFraction(void)
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	return (float)((double)r * (double)0.001f);
}

C3DSmoke::C3DSmoke()										// 0x440530
{
	SetName((char *)"C3DSMOKE");
	Log("C3DSmoke()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3SM1');									// 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 37;
	m_sprite_size = 40;
	v36(1);
	v52(0);
	v46(0);
	v42(0);
	v40(0);
	v232();
	OMediaElement::hide(true);								// element +0x58
	m_colour[0] = 1.0f;
	m_colour[1] = 1.0f;
	m_colour[2] = 1.0f;
	m_count = 15;
	for (int i = 0; i < m_count; i++)
	{
		m_puff[i] = new C3DSmokePuff;						// 0x440980 (0x62c bytes)
		m_puff[i]->hide(true);								// element +0x58
		m_puff[i]->m_active = 0;							// 0x620
	}
	m_clock = 0;
	m_interval = RandomFraction();
	m_active = 1;
}

C3DSmoke::~C3DSmoke()										// 0x4407b0
{
}

namespace neg {
CGameObject *New_C3DSmoke(void) { return new C3DSmoke; }				// '3SM1' (0x648 bytes)
}

void C3DSmoke::Process(float dt)							// 0x4407f0
{
	CGameObject::Process(dt);								// 0x473240 (not the sprite's)
	if (!m_active) return;
	const float t = m_clock;
	m_clock = dt + t;
	if (!((double)m_interval + 0.6 < (double)(dt + t))) return;	// 0x48e5f8
	int i = 0;
	if (0 < m_count)
	{
		while (m_puff[i]->m_time <= m_puff[i]->m_life)		// still alive: the next one
		{
			i++;
			if (m_count <= i) return;
		}
		m_interval = RandomFraction();
		float p[4];
		v195(p);											// +0x310: our position
		m_puff[i]->m_pos[0] = p[0];
		m_puff[i]->m_pos[1] = p[1];
		m_puff[i]->m_pos[2] = p[2];
		m_puff[i]->m_pos[3] = p[3];
		m_puff[i]->force_show();							// inline: hide_count = 1, hide(false)
		m_puff[i]->m_time = 0;
		m_clock = 0;
	}
}

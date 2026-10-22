/*  C3DYokian - see C3DYokian.h. Reconstructed from Neutron.exe 0x44a730..0x44b220. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokian.h"
#include "C3DYokianShield.h"
#include "C3DYokHelmet.h"
#include "../C3DShadow.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../Physics.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- construction

C3DYokian::C3DYokian()										// 0x44a730
{
	f_6bc = 0;
	f_6c4 = 200.0f;											// speed
	f_974 = 0;												// hit time
	f_9a8 = 0;
	f_6c8 = 2;												// AI state: patrol
	f_93c = 2;												// the state to go back to
	SetName((char *)"C3DYOKIAN");
	m_shield = new C3DYokianShield;							// 0x5f4 bytes, 0x44b510
	m_helmet = NULL;
	if (!g_windowed)										// 0x509a13
	{
		m_shadow = new C3DShadow;							// 0x5f4 bytes, 0x43f640
	}
	else m_shadow = NULL;
	if (m_shadow)
	{
		m_shadow->v36(1);									// (its game object part, +0x90)
		m_shadow->SetSize(140, 50);							// 0x43f870
	}
	f_970 = 0;												// not hit
	strcpy(f_898, "STOP");									// 0x407fa0 (BG C3DAI element slot 72)
	strcpy(f_820, "WALK");
	strcpy(f_870, "STOP");
	v50(0);
	f_908 = 1;
	f_9b0 = 0;
	f_958 = 1;
	m_loop_sound = -1;
	f_990 = 900.0f;
}

C3DYokian::~C3DYokian()										// 0x44a9b0
{
}

// --------------------------------------------------------------------------- element family

void C3DYokian::ev70(unsigned char show)					// 0x44afa0 (BG ESLOT 68; C3DAI's is not called)
{
	if (!show)
	{
		hide(true);											// element +0x58
		v232();												// +0x3a4 (BG slot 233): no collision
		v132(0);											// +0x214 (BG slot 133)
		if (m_shadow) m_shadow->hide(true);
		return;
	}
	force_show();											// (2002 +0x70 hide count: 1, then hide(false))
	v234();													// +0x3ac (BG slot 235): collision
	v132(1);
	if (m_helmet) m_helmet->force_show();
}

void C3DYokian::ev91_bg(void)								// 0x44b100
{
	ev58((char *)"ATTACK", 1);								// element +0xe0
	if (m_loop_sound == -1) m_loop_sound = PlayGameSound3DIndex(this, -1, 0x38, true);	// 0x4589c0
}

// --------------------------------------------------------------------------- CGameObject family

void C3DYokian::AddTouching(CGameObject *other)				// 0x44b070
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DBASEBALL") && !other->v133())	// +0x218 (BG slot 134): it has not hit yet
	{
		PlayGameSoundIndex(-1, 0x2f, false);				// 0x458980
		ev107(5);											// element +0x124 (BG ESLOT 73): AI state 5
		f_940 = 2200.0f;
		f_96c = 5.0f;
		AddScore(10);										// BG 0x42adc0
	}
	if (other->NameContains((char *)"C3DJIMMY")) f_934 = 2.0f;
}

void C3DYokian::Process(float dt)							// 0x44aa00
{
	C3DAI::Process(dt);										// 0x408000
	f_278 = f_278;											// (a float reload in the original)
	f_274 = 0;
	f_27c = 0;

	// The shadow on the ground below (a ray 2000 down).
	if (!g_windowed)
	{
		float p[4];
		v195(p);											// +0x310 position (BG slot 196)
		OMedia3DVector a(p[0], p[1], p[2], p[3]);
		OMedia3DVector b(p[0], p[1] - 2000.0f, p[2], p[3]);	// 0x48e5a4
		if (PhysicsClosestHit(a, b, false) && m_shadow)		// 0x47c6e0 (the end lands in g_phys_ray.end)
		{
			const float y = g_phys_ray.end.y + 30.0f;		// 0x5cfc64; 0x48d404
			v195(p);
			m_shadow->v197(p[0], y, p[2]);					// +0x318: position
		}
	}

	// Patrolling, waiting or scanning: the attack loop sound off.
	if ((f_6c8 == 2 || f_6c8 == 3 || f_6c8 == 6) && m_loop_sound != -1)
	{
		StopGameSound(m_loop_sound, false);					// 0x458a00
		m_loop_sound = -1;
	}

	// The helmet follows.
	if (m_helmet)
	{
		float p[4], r[4];
		v195(p);
		m_helmet->v196(p[0], p[1], p[2], p[3]);				// +0x314
		v201(r);											// +0x328 rotation
		m_helmet->v202(r[0], r[1], r[2], r[3]);				// +0x32c
	}

	// The bubble, while it flashes: 100 towards the camera from us, 100 up.
	if (m_shield && m_shield->m_time > 0 && f_6c0 != 0)
	{
		float p[4];
		v195(p);
		OMedia3DVector d(p[0] - g_viewport->getx(), p[1] - g_viewport->gety(), p[2] - g_viewport->getz(), 0);
		const float k = 1.0f / d.quick_magnitude();			// 0x48d924
		d.x *= k; d.y *= k; d.z *= k;
		d.x *= -100.0f; d.y *= -100.0f; d.z *= -100.0f;		// 0x49fac4
		m_shield->v196(p[0] + d.x, p[1] + d.y + 100.0f, p[2] + d.z, p[3]);	// +0x314; 0x48d96c
	}

	// Hit.
	if (f_970)
	{
		if (m_loop_sound != -1)
		{
			StopGameSound(m_loop_sound, false);
			m_loop_sound = -1;
			*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;	// 0x509a40: the random state
			const float r = (float)(int)(((int)((*g_random_seed >> 16) * 1001) >> 16) & 0xffff) * 0.001f;	// 0..1 (0x48e5d8)
			int id;
			if (r < 0.25f) id = 0xf3;						// 0x49fad0
			else if (r < 0.5f) id = 0xf4;					// 0x48d948
			else if (r < 0.75f) id = 0xf5;					// 0x49d5ac
			else id = 0xf6;
			PlayGameSoundIndex(-1, id, false);				// 0x458980
		}
		if (m_shadow) m_shadow->SetSize(40, 15);			// 0x43f870
		if (m_helmet) m_helmet->hide(true);					// element +0x58
		if (f_6c8 == 0)
		{
			ev107(5);										// element +0x124 (BG ESLOT 73)
			f_940 = 1500.0f;
			f_6c4 = 400.0f;									// speed
		}
		if (f_974 > 0)
		{
			f_974 -= dt;
			if (f_974 < 0)
			{
				f_974 = 0;
				f_970 = 0;
				f_94c = 0;
				ev107(f_93c);								// back to the previous state
				ev58((char *)"WALK", 1);					// element +0xe0
				f_6c4 = 200.0f;
				if (m_shadow) m_shadow->SetSize(140, 50);
				if (m_helmet) m_helmet->force_show();
			}
		}
	}

	if (f_13e)												// CGameObject +0x13e: shown
	{
		if (m_shadow) m_shadow->force_show();
	}
	else if (m_shadow) m_shadow->hide(true);

	float v[4];
	v174(v);												// +0x2bc velocity (BG slot 175)
	if (v[1] > 0) ev130(dt, 250.0f);						// element +0x178 (BG ESLOT 94)
}

void C3DYokian::v277(void)									// 0x44b140
{
	C3DAnimated::v277();									// (BG slot 272)
	if (m_loop_sound != -1) StopChannel(m_loop_sound, false);	// 0x47d7a0 (the channel is kept)
}

void C3DYokian::v278(void)									// 0x44b160
{
	C3DAnimated::v278();									// (BG slot 273)
	if (m_loop_sound != -1) m_loop_sound = PlayGameSound3DIndex(this, -1, 0x38, true);
}

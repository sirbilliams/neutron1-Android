/*  C3DYokTurret - see C3DYokTurret.h. Reconstructed from Neutron.exe 0x44c6f0..0x44d070. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokTurret.h"
#include "C3DMissile.h"
#include "../Bridge.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include "OMedia3DShape.h"
#pragma GCC diagnostic pop
#include <math.h>
#include <string.h>

using namespace neg;

C3DYokTurret::C3DYokTurret()								// 0x44c6f0
{
	f_9a0 = 1.0f;
	SetName((char *)"C3DYOKTURRET");
	Log("C3DHarrier()");									// (sic)
	v67(150.0f);											// radius
	v62(0.8f);
	v64(0.7f);
	m_speed = 0;
	strcpy(m_anim_wag, "none");								// 0x407fa0
	strcpy(m_anim_run, "none");
	strcpy(m_anim_fly, "none");
	strcpy(m_anim_sit, "none");
	strcpy(m_anim_none, "none");
	strcpy(m_anim_walk, "none");
	m_state = 6;
	m_ai_state = 6;
	C3DYokTurret::InitObject();								// 0x44ca70 (called directly)
	float p[4];
	v195(p);												// +0x310: where it stands
	m_home[0] = p[0];
	v195(p);
	m_home[1] = p[1];
	v195(p);
	m_home[2] = p[2];
	C3DObject::SetObjectID('3TUR');							// 0x462220
	C3DObject::InitPhysics();								// 0x4623d0
	m_turn_rates.x = 8.0f;
	m_turn_rates.y = 4.0f;
	m_turn_rates.z = 4.0f;
	m_offset.x = 0;
	m_offset.y = -500.0f;
	m_offset.z = 2000.0f;
	MakeShapeUnLit(get_shape());							// 0x477550
	v50(0);
	v42(0);
	v40(0);
	v232();
	f_4a8 = 0;												// (BG +0x49c)
	m_next = 0;
	m_toggle = 1;
	m_time = 0;
	m_fov = 360.0f;
	m_visible_range = 15000.0f;
	for (int i = 0; i < 3; i++)
	{
		m_missile[i] = new C3DMissile;						// 0x42f500 (0x6d4 bytes)
		if (m_missile[i])
		{
			m_missile[i]->hide(true);						// element +0x58
			m_missile[i]->v52(0);							// +0xd0
		}
	}
}

C3DYokTurret::~C3DYokTurret()								// 0x44ca20
{
}

namespace neg {
CGameObject *New_C3DYokTurret(void) { return new C3DYokTurret; }		// '3TUR' (0x9bc bytes)
}

void C3DYokTurret::InitObject(void)							// 0x44ca70
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 5));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// 0x477550
	v66(115.0f);											// +0x108
	ev53();													// element +0xcc: v64 from the shape
}

void C3DYokTurret::Process(float dt)						// 0x44cb00
{
	C3DAI::Process(dt);										// 0x408000
	if (!GameIsRunning()) return;							// 0x475ca0
	const float t = m_time;
	m_time = dt + t;
	float p[4];
	if (1.8 < (double)m_time)								// 0x494cb8
	{
		v88(p);												// +0x164: the saved start position
		v196(p[0], p[1], p[2], p[3]);						// +0x314: back to it
	}
	float a[4], b[4];
	v155(a);												// +0x270: the acceleration
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	if (!(4.5 < (double)m_time)) return;					// 0x4afd38
	m_time = 0;
	if (g_camera)
	{
		if (g_camera->NameContains((char *)"C3DJIMMY"))
		{
			m_offset.x = 0;
			m_offset.y = 0;
			m_offset.z = 200.0f;
		}
		else if (g_camera->NameContains((char *)"C3DROCKET"))
		{
			float q[4];
			v195(p);										// +0x310
			g_camera->v195(q);
			m_offset.x = 0;
			m_offset.y = 0;
			const float dz = p[2] - q[2];
			m_offset.z = (float)(sqrtf((p[0] - q[0]) * (p[0] - q[0]) + (p[1] - q[1]) * (p[1] - q[1]) + dz * dz) * 0.35);	// 0x49b200
		}
		else if (g_camera->NameContains((char *)"C3DJEEP"))
		{
			m_offset.x = 0;
			m_offset.y = 0;
			m_offset.z = 400.0f;
		}
	}
	if (m_state == 0) ev95(0, 0, 320.0f);					// element +0x17c
	m_toggle = (m_toggle == 0);
}

void C3DYokTurret::v258(void)								// 0x44ccf0
{
	float p[4];
	v195(p);												// +0x310
	m_home[0] = p[0];
	v195(p);
	m_home[1] = p[1];
	v195(p);
	m_home[2] = p[2];
}

void C3DYokTurret::ev95(float a, float b, float c)			// 0x44cd50
{
	C3DMissile *m = m_missile[m_next];
	if (!m) return;
	float p[4];
	// A point 100 units behind the turret, in world space; the original then moves THE TURRET
	// (not the missile) there (a quirk: Process puts it back).
	OMedia3DVector p0 = v224(0, 0, -100.0f);				// +0x384
	v196(p0.x, p0.y, p0.z, p0.w);							// +0x314
	m->force_show();										// inline: hide_count = 1, hide(false)
	m->v132(1);												// +0x214
	OMedia3DVector d = v226(0, 0, 1.0f);					// +0x38c: the way it faces
	PlayGameSoundIndex(-1, 0x89, false);					// 0x458980
	m->v175(d.x * 1000.0f, d.y * 1000.0f, d.z * 1000.0f, d.w);	// +0x2c0: velocity (0x48e5ac)
	m->v42(0);												// +0xa8
	m->v40(0);												// +0xa0
	OMedia3DVector p1 = v224(a, b, c);						// +0x384: the stack arguments (0x44cedc), where the missile starts (Process: 0, 0, 320)
	m->v196(p1.x, p1.y, p1.z, p1.w);						// +0x314
	v201(p);												// +0x328: the turret's rotation
	m->v202(p[0], p[1], p[2], p[3]);						// +0x32c
	m->f_4a8 = 0;											// (BG +0x49c)
	m->m_flying = 1;
	m->m_timer = 0;
	m->v52(1);												// +0xd0
	m->v67(50.0f);											// +0x110: radius
	m_next++;
	if (m_next > 2) m_next = 0;
}

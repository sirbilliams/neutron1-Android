/*  C3DMovingTarget - see C3DMovingTarget.h. Reconstructed from Neutron.exe 0x430220..0x4306b0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMovingTarget.h"
#include "../level4/C3DVRTrophy.h"
#include "../pickups/C3DPickupItem.h"
#include "../CGameType.h"
#include "../InterfaceElement.h"
#include "../Sound.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMedia3DVector.h"
#include <math.h>
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// --------------------------------------------------------------------------- construction

C3DMovingTarget::C3DMovingTarget()							// 0x430220
{
	m_start[3] = 1.0f;
	m_dest[3] = 1.0f;
	SetName((char *)"C3DMOVINGTARGET");
	Log("C3DMovingTarget()");
	C3DSprite::InitObject();								// 0x464130 (it has no InitObject of its own)
	m_speed = 100.0f;
	m_hits_required = 5;
	m_respawn_time = 15.0f;
	m_num_points = 10;
	RegisterProperty("StartPosX",      &m_start[0], PROP_FLOAT, 0);	// 0x473fa0
	RegisterProperty("StartPosY",      &m_start[1], PROP_FLOAT, 0);
	RegisterProperty("StartPosZ",      &m_start[2], PROP_FLOAT, 0);
	RegisterProperty("DestPosX",       &m_dest[0],  PROP_FLOAT, 0);
	RegisterProperty("DestPosY",       &m_dest[1],  PROP_FLOAT, 0);
	RegisterProperty("DestPosZ",       &m_dest[2],  PROP_FLOAT, 0);
	RegisterProperty("Speed",          &m_speed,    PROP_FLOAT, 0);
	RegisterProperty("ActivateObject", m_activate,  PROP_ALPHA, 0);
	RegisterProperty("HitsRequired",   &m_hits_required, PROP_LONG, 0);
	RegisterProperty("NumPoints",      &m_num_points,    PROP_LONG, 0);
	RegisterProperty("RespawnTime",    &m_respawn_time,  PROP_FLOAT, 0);
	SetObjectID('3TAR');									// 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	strcpy(m_activate, "none");
	m_sprite_index = 98;
	m_sprite_size = 100;
	v42(0);
	v40(0);
	v67(75.0f);												// radius
	v46(1);
	v34(1);
	C3DSprite::InitPhysics();								// 0x464210
	f_5e8 = 0;
	m_dest[0] = m_dest[1] = m_dest[2] = 0;
	m_start[0] = m_start[1] = m_start[2] = 0;
	m_respawn_left = 0;
	// (then a call to the empty UnInitDatabases 0x472970)
}

C3DMovingTarget::~C3DMovingTarget() {}						// 0x430530

namespace neg {
CGameObject *New_C3DMovingTarget(void) { return new C3DMovingTarget; }	// '3TAR' (0x68c bytes)
}

// --------------------------------------------------------------------------- slots

void C3DMovingTarget::AddTouching(CGameObject *other)		// 0x430570
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DBASEBALL") || m_respawn_left > 0) return;
	hide(true);												// element +0x58
	m_respawn_left = m_respawn_time;
	AddScore(m_num_points);									// 0x42adc0
	CGameObject *act = FindObjectByTag(m_activate);			// 0x474070
	if (!act)
	{
		PlayGameSoundIndex(-1, 0xc6, false);				// 0x458980
		return;
	}
	if (act->NameContains((char *)"C3DTROPHY"))
	{
		C3DVRTrophy *t = static_cast<C3DVRTrophy *>(act);
		if (m_hits_required < ++t->f_6bc)					// the hits it has counted
		{
			t->v268(1);										// +0x428
			t->f_6bc = 0;
			PlayGameSoundIndex(-1, 199, false);
		}
	}
	if (act->NameContains((char *)"C3DPICKUPITEM"))
	{
		C3DPickupItem *p = static_cast<C3DPickupItem *>(act);
		if (m_hits_required < ++p->f_760)
		{
			p->v268(1);
			p->f_760 = 0;
			PlayGameSoundIndex(-1, 199, false);
		}
	}
}

void C3DMovingTarget::Process(float dt)						// 0x4306b0
{
	C3DSprite::Process(dt);									// 0x463090
	float acc[4];
	v155(acc);												// slot 156 (read twice by the original)
	v157(acc[0], -g_gravity.y, acc[2]);						// +0x278: gravity cancelled (0x5cfc04)
	if (m_respawn_left > 0)
	{
		m_respawn_left -= dt;
		if (m_respawn_left <= 0)
		{
			m_respawn_left = 0;
			if (!getvisible()) force_show();				// the hit target shows again (hide count 1, hide(false))
		}
	}
	float p[4];
	v195(p);												// +0x310
	OMedia3DVector d(m_dest[0] - p[0], m_dest[1] - p[1], m_dest[2] - p[2], 0);
	const float dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
	const float inv = 1.0f / d.quick_magnitude();
	v176(d.x * inv * m_speed, d.y * inv * m_speed, d.z * inv * m_speed);	// +0x2c4: the velocity
	if (dist < 100.0f) v196(m_start[0], m_start[1], m_start[2], m_start[3]);	// +0x314: back to the start
}

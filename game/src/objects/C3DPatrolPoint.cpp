/*  C3DPatrolPoint - see C3DPatrolPoint.h. Reconstructed from Neutron.exe 0x434b30..0x434f50. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DPatrolPoint.h"
#include "../C3DAI.h"
#include "../Game.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- construction

C3DPatrolPoint::C3DPatrolPoint()							// 0x434b30
{
	SetName((char *)"C3DPATROLPOINT");
	Log("C3DPatrolPoint()");
	C3DPatrolPoint::InitObject();							// (called directly, as in the original)
	SetObjectID('3PAT');									// element slot 48
	m_sprite_index = 6;
	m_sprite_size = 50;
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_call_object_tag, "none");
	strcpy(m_activate_anim, "none");
	strcpy(m_wait_anim, "none");
	m_wait_time = 0;
	strcpy(m_sound_db, "none");
	m_sound_index = -1;
	strcpy(m_next_patrol_point, "none");
	v67(250.0f);											// slot 68: radius
	v46(0);
	v42(0);
	v40(0);
	v52(0);
	f_008 = 1;												// element +0xd0
	C3DSprite::InitPhysics();
	// (debug log)
}

C3DPatrolPoint::~C3DPatrolPoint() {}						// 0x434da0

// --------------------------------------------------------------------------- CGameObject-family slots

void C3DPatrolPoint::InitObject(void)						// 0x434de0
{
	C3DSprite::InitObject();
	RegisterProperty("CallObjectTag",   m_call_object_tag,   PROP_ALPHA, 0);
	RegisterProperty("ActivateAnim",    m_activate_anim,     PROP_ALPHA, 0);
	RegisterProperty("SoundDatabase",   m_sound_db,          PROP_ALPHA, 0);
	RegisterProperty("SoundIndex",      &m_sound_index,      PROP_LONG,  0);
	RegisterProperty("NextPatrolPoint", m_next_patrol_point, PROP_ALPHA, 0);
	RegisterProperty("WaitAnim",        m_wait_anim,         PROP_ALPHA, 0);
	RegisterProperty("WaitTime",        &m_wait_time,        PROP_FLOAT, 0);
}

void C3DPatrolPoint::AddTouching(CGameObject *other)		// 0x434ea0
{
	CGameObject::AddTouching(other);
	if (other->NameContains((char *)"C3DAI"))
	{
		// (the original then reads the AI's state, +0x6c8 of its element, as if every "C3DAI..."
		// were a C3DAI, and logs "PatrolPoint %s hit" when it patrols (2): debug only)
	}
}

namespace neg {
CGameObject *New_C3DPatrolPoint(void)	{ return new C3DPatrolPoint; }		// factory '3PAT' (0x7ec bytes)
}

/*  C3DAICar - see C3DAICar.h. Reconstructed from Neutron.exe 0x40a8a0..0x40ae00. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DAICar.h"
#include "../C3DPlayer.h"
#include "../Sound.h"
#pragma GCC diagnostic pop
#include <string>

using namespace neg;

C3DAICar::C3DAICar()										// 0x40a8a0
{
	m_speed = 400.0f;											// speed
	m_state = 3;												// state: patrol only
	m_ai_state = 3;												// AIState
	CGameObject::v46(1);									// 0x471030 (direct call)
	m_turn_rates.y = 2.0f;											// turn rates
	m_turn_rates.z = 2.0f;
	m_turn_rates.x = 1.0f;
	m_stop_time = 0;
	m_hold_pos.x = 0;
	m_hold_pos.y = 0;
	m_hold_pos.z = 0;
	m_hold_pos.w = 1.0f;
	m_held = 0;
	m_check_stuck = 0;
	m_sound = -1;
}

C3DAICar::~C3DAICar()										// 0x40aa00
{
}

void C3DAICar::Process(float dt)							// 0x40aa50
{
	C3DAI::Process(dt);										// 0x408000
	if (m_held)
		v196(m_hold_pos.x, m_hold_pos.y, m_hold_pos.z, m_hold_pos.w);	// +0x310 position
	m_stop_time = m_stop_time - dt;
	if (m_stop_time < 0.0f)									// 0x48d914
		m_speed = 400.0f;										// (every frame once the time is up)
}

void C3DAICar::AddTouching(CGameObject *other)				// 0x40aad0
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (!other->NameContains((char *)"C3DJIMMY") &&			// +0x18
		!other->NameContains((char *)"C3DJEEP") &&
		!other->NameContains((char *)"C3DGODDARD"))			// (Negatron's rocket ship is not in the list)
		return;
	float o[4], p[4];
	other->v195(o);											// +0x30c
	v195(p);
	if (o[1] > p[1] + 480.0f)								// 0x48ec38: on top of us
	{
		other->v175(700.0f, 700.0f, 200.0f, 0);				// +0x2bc thrown off
		if (other->NameContains((char *)"C3DJIMMY"))
		{
			C3DPlayer *j = dynamic_cast<C3DPlayer *>(other);	// (other - 0xc0)
			if (j) j->f_7e8 = 0;							// port: checked (not in jump any more)
		}
	}
	PlayGameSound(-1, std::string("horn"), false);			// 0x40acc0, 0x458b40
	m_speed = 0.0001f;										// stop (0x38d1b717)
	m_stop_time = 8.0f;
	m_held = 1;
	v195(p);
	m_hold_pos.x = p[0];
	m_hold_pos.y = p[1];
	m_hold_pos.z = p[2];
	m_hold_pos.w = p[3];
}

void C3DAICar::RemoveTouching(CGameObject *other)			// 0x40ac10
{
	C3DAI::RemoveTouching(other);							// 0x40a390
	m_held = 0;
}

void C3DAICar::v260(void)									// 0x40ac30
{
	C3DAI::v260();											// 0x409480
	m_sound = PlayGameSound3DIndex(this, -1, 6, true);		// 0x4589c0 the engine
}

void C3DAICar::v277(void)									// 0x40ac60
{
	C3DAnimated::v277();									// 0x40e770
	if (m_sound != -1) StopChannel(m_sound, false);			// 0x47d7a0
}

void C3DAICar::v278(void)									// 0x40ac80
{
	C3DAnimated::v278();									// 0x40e790
	if (m_sound != -1)
		m_sound = PlayGameSound3DIndex(this, -1, 6, true);
}

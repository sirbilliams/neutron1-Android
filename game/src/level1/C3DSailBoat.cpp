/*  C3DSailBoat - see C3DSailBoat.h. Reconstructed from Neutron.exe 0x43ecc0..0x43f060. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSailBoat.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../InterfaceElement.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include <math.h>

using namespace neg;

C3DSailBoat::C3DSailBoat()									// 0x43ecc0
{
	SetName((char *)"C3DSAILBOAT");							// 0x4702d0
	Log("C3DSAILBOAT");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3SAI');							// 0x462220
	m_speed = 200.0f;											// speed
	m_state = 1;												// waiting
	m_ai_state = 1;												// AIState
	C3DAI::ev72(m_anim_wag, (char *)"none");
	C3DAI::ev72(m_anim_run, (char *)"none");
	C3DAI::ev72(m_anim_fly, (char *)"none");
	C3DAI::ev72(m_anim_sit, (char *)"none");
	C3DAI::ev72(m_anim_none, (char *)"none");
	C3DAI::ev72(m_anim_walk, (char *)"none");
	v67(100.0f);											// radius
	v152(100.0f);
	m_turn_rates.y = 0.3f;											// turn rate y (0x8d4)
	v42(1);
	v40(1);
	v52(1);
	f_648 = 0;												// animations off
	m_time = 0;
	m_hover = 0;												// not floating
	m_sailed = 0;
	m_db = GetDatabase("objects.omt");						// 0x46a910
	set_shape(Load3DShape(m_db, 13));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	m_check_stuck = 0;												// (0x978)
}

C3DSailBoat::~C3DSailBoat()									// 0x43eef0
{
}

namespace neg {
CGameObject *New_C3DSailBoat(void) { return new C3DSailBoat; }			// '3SAI' (0x9a4 bytes)
}

void C3DSailBoat::Process(float dt)							// 0x43ef40
{
	C3DAI::Process(dt);										// 0x408000
	if (m_state != 2 && m_state != 3)
	{
		m_sailed = 0;										// (sic: the points come again next time)
		m_hold_when_touched = 0;
		m_held = 0;
		m_hover = 0;
		return;
	}
	m_time += dt;
	m_hover = 1;												// floating: no gravity
	float r[4];
	v201(r);												// +0x328
	v203((float)(sin(m_time * 4.0f) * 5.0), r[1], (float)(sin(m_time * 3.5) * 5.0));	// +0x330; 0x49d5e8, 0x4b31c0, 0x4a6848
	if (!m_sailed)
	{
		m_sailed = 1;
		// 0x42adc0: 50 points (no difficulty scaling; the pop-up shows the award)
		g_score += 50;
		g_score_popup = 50;
		g_score_popup_time = 1.2f;
	}
	m_hold_when_touched = 1;
}

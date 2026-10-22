/*  C3DRocket - see C3DRocket.h. Reconstructed from Neutron.exe 0x43d090..0x43d520. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DRocket.h"
#include "C3DNewSmokePuff.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"

using namespace neg;

C3DRocket::C3DRocket()										// 0x43d090
{
	SetName((char *)"C3DYOKIANSHIP");						// 0x4702d0 (sic)
	Log("C3DYOKIANSHIP()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3RCK');							// 0x462220
	m_state = 3;												// state
	m_ai_state = 3;												// AIState
	C3DAI::ev72(m_anim_wag, (char *)"none");
	C3DAI::ev72(m_anim_run, (char *)"none");
	C3DAI::ev72(m_anim_fly, (char *)"none");
	C3DAI::ev72(m_anim_sit, (char *)"none");
	C3DAI::ev72(m_anim_none, (char *)"none");
	C3DAI::ev72(m_anim_walk, (char *)"none");
	v67(100.0f);											// radius
	v152(100.0f);
	v52(1);
	m_turn_rates.y = 0.3f;											// turn rate y (0x8d4)
	v42(1);
	v40(1);
	m_db = GetDatabase("objects.omt");						// 0x46a910
	f_648 = 0;												// animations off
	set_shape(Load3DShape(m_db, 15));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	for (int i = 0; i < 10; i++)
	{
		m_puffs[i] = new C3DNewSmokePuff;					// 0x433070 (0x630 bytes)
		// ("C3DNewSmokePuff": debug log of the allocation)
		m_puffs[i]->hide(true);								// element +0x58
	}
	m_next_puff = 0;
	m_puff_time = 0;
	v34(1);
}

C3DRocket::~C3DRocket()										// 0x43d330
{
}

namespace neg {
CGameObject *New_C3DRocket(void) { return new C3DRocket; }				// '3RCK' (0x9cc bytes)
}

void C3DRocket::Process(float dt)							// 0x43d380
{
	C3DAI::Process(dt);										// 0x408000
	m_puff_time = dt + m_puff_time;
	if (m_state != 3) return;
	if (m_puff_time > 0.1)									// 0x49cfb8 (double)
	{
		m_puff_time = 0;
		const OMedia3DVector p = v224(0, -10.0f, 0);		// +0x384
		m_puffs[m_next_puff]->Start(p.x, p.y, p.z, p.w);	// its element +0xd4
		if (++m_next_puff > 9) m_next_puff = 0;
	}
	float r[4];
	v91(r);													// +0x170: the initial rotation
	v202(r[0], r[1], r[2], r[3]);							// +0x32c
}

void C3DRocket::v260(void)									// 0x43d490
{
	C3DAI::v260();											// 0x409480
	v67(1.0f);												// +0x110: radius
}

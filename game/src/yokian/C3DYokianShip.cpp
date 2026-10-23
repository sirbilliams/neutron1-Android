/*  C3DYokianShip - see C3DYokianShip.h. Reconstructed from Neutron.exe 0x44b7d0..0x44bee0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokianShip.h"
#include "C3DAbductee.h"
#include "C3DYokCargo.h"
#include "../level4/C3DTractorBeam.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Physics.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

static int LevelID(void)	{ return static_cast<CGameObject *>(g_gametype)->m_object_id; }	// g_gametype +0x490

C3DYokianShip::C3DYokianShip()								// 0x44b7d0
{
	SetName((char *)"C3DYOKIANSHIP");
	Log("C3DYOKIANSHIP()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3YSH');							// 0x462220
	m_speed = 700.0f;											// speed
	m_state = 3;												// AI state: patrol only
	m_ai_state = 3;
	strcpy(m_anim_wag, "none");									// 0x407fa0: no animations
	strcpy(m_anim_run, "none");
	strcpy(m_anim_fly, "none");
	strcpy(m_anim_sit, "none");
	strcpy(m_anim_none, "none");
	strcpy(m_anim_walk, "none");
	v67(100.0f);											// (BG slot 68) radius
	v152(100.0f);											// (BG slot 153)
	v52(0);
	m_turn_rates.y = 0.3f;											// turn rate y
	v42(0);
	v40(0);
	m_db = GetDatabase("yokianship.omt");					// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 0));						// 0x462230 / 0x477ba0
	MakeShapeUnLit(get_shape());							// 0x477550
	v34(1);
	m_beam = NULL;
	m_abductee = NULL;
	if (LevelID() != 'LV1D' && LevelID() != 'LV1F')
	{
		CGameObject *beam = new C3DTractorBeam;				// 0x4462c0 (0x6d0 bytes)
		m_beam = beam;
		OMediaElement *be = dynamic_cast<OMediaElement *>(beam);
		if (be) be->force_show();							// its constructor hid it
		beam->v107(0);										// +0x1b0
		m_abductee = new C3DAbductee;						// 0x4076e0 (0x6c4 bytes)
	}
	ev51();													// 0x4626f0
	m_beam_on = 0;
	m_beam_time = 0;
	// (then a call to the empty 0x472970)
}

C3DYokianShip::~C3DYokianShip()								// 0x44bb10
{
}

namespace neg {
CGameObject *New_C3DYokianShip(void) { return new C3DYokianShip; }		// '3YSH' (0x9ac bytes)
}

void C3DYokianShip::Process(float dt)						// 0x44bbc0
{
	C3DAI::Process(dt);										// 0x408000
	const OMedia3DVector ahead = v224(0, 0, 1200.0f);		// +0x384: the beam's place
	float a[4], b[4];
	v155(a);												// +0x270 constant acceleration (BG slot 156)
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	if (LevelID() == 'LV1D' || LevelID() == 'LV1F') return;
	if (f_64c != -1 && f_64c > GetTaskState("SCENE")) return;	// RequiredLevel (0x45fea0)
	if (!m_beam) return;
	m_beam->v196(ahead.x, ahead.y, ahead.z, ahead.w);		// +0x314
	float r[4];
	v201(r);												// +0x328 rotation
	m_beam->v202(r[0], r[1], r[2], r[3]);					// +0x32c
	m_beam_time -= dt;
	if (m_beam_on)
	{
		const OMedia3DVector q = v224(0, 0, m_beam_time * 0.2f * 2100.0f + 1200.0f);	// 0x4c19ac, 0x4c19a8, 0x4c19a4
		m_abductee->v197(q.x, (5.0f - m_beam_time) * q.y * 0.2f, q.z);	// +0x318 (0x48d950)
	}
	if (!(m_beam_time < 0)) return;
	m_beam_time = 5.0f;
	m_beam_on = m_beam_on == 0;
	CGameObject *beam = m_beam, *abductee = m_abductee;
	OMediaElement *be = dynamic_cast<OMediaElement *>(beam);
	OMediaElement *ae = dynamic_cast<OMediaElement *>(abductee);
	if (m_beam_on)
	{
		if (ae) ae->force_show();							// (2002 +0x70 hide count: 1, then hide(false))
		if (be) be->force_show();
		return;
	}
	if (be) be->hide(true);									// element +0x58
	if (ae) ae->hide(true);
}

void C3DYokianShip::RotateToDest(float dt)					// 0x449a00 (shared)
{
	YokRotateToDest(this, dt);
}

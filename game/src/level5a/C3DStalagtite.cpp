/*  C3DStalagtite - see C3DStalagtite.h. Reconstructed from Neutron.exe 0x441bb0..0x4422c0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DStalagtite.h"
#include "C3DRock.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "OMedia3DShape.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DStalagtite::C3DStalagtite()								// 0x441bb0
{
	SetName((char *)"C3DSTALAGTITE");
	Log("C3DSTALAGTITE()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3STA');							// 0x462220
	v67(100.0f);											// radius
	v152(100.0f);
	v52(0);
	v94(2);
	v46(0);
	v42(0);
	v40(1);
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 4));						// 0x477ba0
	MakeShapeUnLit(get_shape());							// 0x477550
	v34(1);
	for (int i = 0; i < 3; i++)
	{
		m_rock[i] = new C3DRock;							// 0x43cc40 (0x6d4 bytes)
		if (m_rock[i]) m_rock[i]->hide(true);				// element +0x58
	}
	m_broken = 0;
}

C3DStalagtite::~C3DStalagtite()								// 0x441e70
{
}

namespace neg {
CGameObject *New_C3DStalagtite(void) { return new C3DStalagtite; }		// '3STA' (0x6d4 bytes)
}

void C3DStalagtite::AddTouching(CGameObject *other)			// 0x441ec0
{
	CGameObject::AddTouching(other);						// 0x470a90
	v132(1);												// +0x214
}

void C3DStalagtite::ev72(void)								// 0x441ee0
{
	v136(1);												// +0x224
	v132(1);												// +0x214
	PlayGameSoundIndex(-1, 0xc9, false);					// 0x458980
	for (int i = 0; i < 3; i++)
		if (m_rock[i]) m_rock[i]->v136(1);
}

void C3DStalagtite::Process(float dt)						// 0x441f70
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (v135()) AddRotation(dt * 20.0f, 0, 0);				// +0x220, +0x334 (0x495324)
}

void C3DStalagtite::ResetObject(void)						// 0x441fb0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	m_broken = 0;
	force_show();											// inline: hide_count = 1, hide(false)
}

void C3DStalagtite::v222(void)								// 0x441fe0
{
	OMediaElement::hide(true);								// element +0x58
	if (m_broken) return;
	v136(0);												// +0x224
	PlayGameSoundIndex(-1, 0xca, false);					// 0x458980
	m_broken = 1;
	float p[4];
	if (m_rock[0])
	{
		v195(p);											// +0x310: our place
		m_rock[0]->v196(p[0], p[1], p[2], p[3]);			// rock +0x314
		m_rock[0]->v162(200.0f, 400.0f, 150.0f);			// +0x28c
		m_rock[0]->force_show();
		m_rock[0]->v132(1);									// +0x214
		m_rock[0]->v136(1);									// +0x224
	}
	if (m_rock[1])
	{
		v195(p);
		m_rock[1]->v196(p[0], p[1], p[2], p[3]);
		m_rock[1]->v132(1);
		m_rock[1]->force_show();
		m_rock[1]->v162(-100.0f, 500.0f, -250.0f);
		m_rock[1]->v136(1);
	}
	if (m_rock[2])
	{
		v195(p);
		m_rock[2]->v196(p[0], p[1], p[2], p[3]);
		m_rock[2]->force_show();
		m_rock[2]->v132(1);
		m_rock[2]->v162(0, 600.0f, 350.0f);
		m_rock[2]->v136(1);
	}
}

/*  C3DRock - see C3DRock.h. Reconstructed from Neutron.exe 0x43cc40..0x43d080. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DRock.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "OMedia3DShape.h"
#pragma GCC diagnostic pop

using namespace neg;

// The game's random number generator as a fraction: 0..1 in steps of 0.001 (0x48e5d8).
static float RandomFraction(void)
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	return (float)((double)r * 0.001f);
}

C3DRock::C3DRock()											// 0x43cc40
{
	SetName((char *)"C3DROCK");
	Log("C3DROCK()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3ROK');							// 0x462220
	v67(100.0f);											// radius
	v152(100.0f);
	v107(0);
	v52(0);
	v94(2);
	v46(0);
	v42(1);
	v40(1);
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 10));						// 0x477ba0
	MakeShapeUnLit(get_shape());							// 0x477550
	m_rot_x = RandomFraction();
	m_rot_y = RandomFraction();
	m_rot_z = RandomFraction();
	v107(0);
	m_countdown = 0;
}

C3DRock::~C3DRock()											// 0x43cec0
{
}

namespace neg {
CGameObject *New_C3DRock(void) { return new C3DRock; }					// '3ROK' (0x6d4 bytes)
}

void C3DRock::ev106(void)									// 0x43cf20
{
	v132(1);												// +0x214
}

void C3DRock::AddTouching(CGameObject *other)				// 0x43cf10
{
	CGameObject::AddTouching(other);
}

void C3DRock::v222(void)									// 0x43cf40
{
	m_countdown = 10.0f;
}

void C3DRock::Process(float dt)								// 0x43cf50
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (m_countdown <= 0)
	{
		if (v131())											// +0x210: physics on
			AddRotation(dt * m_rot_x * 100.0f, dt * m_rot_y * 50.0f, dt * m_rot_z * 70.0f);	// 0x48d96c / 0x48d400 / 0x491404
		return;
	}
	const float t = m_countdown;
	m_countdown = t - dt;
	if (m_countdown <= 0) v136(0);							// +0x224
	if (m_countdown <= -15.0f)								// 0x49d5d8
	{
		v132(0);											// +0x214
		OMediaElement::hide(true);							// element +0x58
	}
}

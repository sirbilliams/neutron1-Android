/*  C3DPendulum - see C3DPendulum.h. Reconstructed from Neutron.exe 0x416eb0..0x440980. */
#include "C3DPendulum.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#include <math.h>

using namespace neg;

// The running level's ID (g_gametype +0x490: its ObjectID, e.g. 'L1_A').
static int LevelID(void)
{
	return static_cast<CGameObject *>(g_gametype)->m_object_id;
}

static bool IsLevel1Run(int id)
{
	return id == 'L1_A' || id == 'L1_B' || id == 'L1_C';
}

C3DPendulum::C3DPendulum()									// 0x416eb0
{
	SetName((char *)"C3DPENDULUM");
	Log("C3DPENDULUM");
	C3DPendulum::InitObject();								// 0x435160 (called directly)
	C3DObject::SetObjectID('3PEN');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	f_924 = (float)(((*seed >> 16) * 501) >> 16) * 0.005f;	// N2:0x4ef574: phase 0..2.5
	C3DObject::InitPhysics();								// 0x4623d0
	f_928 = 60.0f;											// SwingHeight
	f_930 = 0;
	v94(2);
	C3DObject::InitPhysics();
	// (then a call to the empty 0x472970)
}

C3DPendulum::~C3DPendulum() {}								// 0x435110

namespace neg {
CGameObject *New_C3DPendulum(void) { return new C3DPendulum; }	// '3PEN' (0x940 bytes)
}

void C3DPendulum::InitObject(void)							// 0x435160
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// N2:0x41b0a0
	RegisterProperty("SwingHeight", &f_928, PROP_FLOAT, 0);
	InitAnim();												// element +0x110
	const int id = LevelID();
	if (IsLevel1Run(id))
	{
		CreateAnim((char *)"HIDEFAULT", (char *)"swinghood.ase");	// +0xe0
		CreateTexture((char *)"swinghood.png", 0);			// +0xfc
		v34(1);												// +0x88
	}
	else
	{
		CreateAnim((char *)"HIDEFAULT", (char *)"pendulum.ase");
		CreateTexture((char *)"pendulum.png", 0);
		v67(100.0f);										// +0x10c: radius
	}
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"DEFAULT", 1);								// +0xe8 (sic: the animation was made as "HIDEFAULT")
}

void C3DPendulum::AddTouching(CGameObject *other)			// N2:0x45cfa0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other == g_camera) f_930 = 7.0f;
}

void C3DPendulum::RemoveTouching(CGameObject *other)		// 0x44a100
{
	CGameObject::RemoveTouching(other);						// 0x470b20
}

unsigned char C3DPendulum::v231(CGameObject *other)			// N2:0x45ce70
{
	if (g_camera == other) return 1;
	return other->m_object_id == '3JIM';
}

void C3DPendulum::Process(float dt)							// N2:0x45cfd0
{
	if (g_gametype->IsPaused()) return;						// +0x114
	C3DAnimated::Process(dt);								// N2:0x41c120
	f_924 += dt;
	if (f_930 > 0)
	{
		f_930 -= dt * 3.0f;									// 0x48e5a0
		if (f_930 <= 0) f_930 = 0;
	}
	float rot[4];
	if (!IsLevel1Run(LevelID()))
	{
		if (g_enemy_time_scale != 1.0f)						// N2:0x540594, 0x48d924: the stopwatch
			f_924 -= dt * 0.75f;							// 0x49d5ac
		const float z = (float)(sinf(f_924) * f_928);
		v201(rot);											// +0x324
		v203(rot[0], rot[1], z);							// +0x32c
		return;
	}
	if (f_930 > 0)
	{
		const float s = sinf(g_enemy_time_scale * f_924 * 5.0f);	// 0x48d950
		v201(rot);
		v203(s * f_930 + s * f_930, rot[1], rot[2]);
		return;
	}
	v201(rot);
	v203(0, rot[1], rot[2]);
}

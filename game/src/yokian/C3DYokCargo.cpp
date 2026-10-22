/*  C3DYokCargo - see C3DYokCargo.h. Reconstructed from Neutron.exe 0x449360..0x449cb0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokCargo.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Bridge.h"
#include "../Globals.h"
#include "../Physics.h"
#include "../SaveGame.h"
#include "../Todo.h"
#pragma GCC diagnostic pop
#include <math.h>
#include <string.h>

using namespace neg;

static int LevelID(void)	{ return static_cast<CGameObject *>(g_gametype)->m_object_id; }	// g_gametype +0x490

C3DYokCargo::C3DYokCargo()									// 0x449360
{
	SetName((char *)"C3DYOKCARGO");
	Log("C3DYOKCARGO()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3YCA');							// 0x462220
	m_speed = 500.0f;											// speed
	m_state = 1;												// AI state: wait
	m_ai_state = 1;
	strcpy(m_anim_wag, "none");									// 0x407fa0: no animations
	strcpy(m_anim_run, "none");
	strcpy(m_anim_fly, "none");
	strcpy(m_anim_sit, "none");
	strcpy(m_anim_none, "none");
	strcpy(m_anim_walk, "none");
	v67(100.0f);											// (BG slot 68) radius
	v152(100.0f);											// (BG slot 153)
	v52(0);
	v42(0);
	v40(0);
	v42(0);
	v40(0);
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 2));						// 0x477ba0
	MakeShapeUnLit(get_shape());							// 0x477550
	v34(1);
	m_turn_rates.x = 0.1f;											// turn rates x, y, z
	m_turn_rates.y = 0.8f;
	m_turn_rates.z = 0.5f;
	m_fly_time = 0;
	m_mine_time = 0;
	for (int i = 0; i < 30; i++)
	{
		const int level = LevelID();
		if (level == 'LEV3' || level == 'LEV5' || level == 'LV4C' || level == 'LV3D' || level == 'LV1D')
			m_mines[i] = NULL;
		else
		{
			// new C3DMine (0x638 bytes, 0x42efa0). TODO: C3DMine is not reconstructed yet.
			NEG_TODO(0x42efa0, "C3DYokCargo: C3DMine");
			m_mines[i] = NULL;
		}
	}
	m_next_mine = 0;
	// (then a call to the empty 0x472970)
}

C3DYokCargo::~C3DYokCargo()									// 0x449670
{
}

namespace neg {
CGameObject *New_C3DYokCargo(void) { return new C3DYokCargo; }			// '3YCA' (0xa20 bytes)
}

void C3DYokCargo::ResetObject(void)							// 0x4496c0
{
	C3DAI::ResetObject();									// 0x407eb0
	m_fly_time = 0;
	m_mine_time = 0;
	m_next_mine = 0;
}

void C3DYokCargo::Process(float dt)							// 0x4496e0
{
	C3DAI::Process(dt);										// 0x408000
	float a[4], b[4];
	v155(a);												// +0x270 constant acceleration (BG slot 156)
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	if (m_state != 3) return;									// patrolling only
	if (g_camera && m_speed < 1800.0f && g_camera->NameContains((char *)"C3DROCKET"))	// 0x4a311c
	{
		float c[4], p[4];
		g_camera->v195(c);									// +0x310
		v195(p);
		const float dx = p[0] - c[0], dy = p[1] - c[1], dz = p[2] - c[2];
		if (sqrtf(dx * dx + dy * dy + dz * dz) < 7000.0f) m_speed = 1400.0f;	// 0x4a3e38
	}
	m_fly_time = dt + m_fly_time;
	if (!(m_fly_time > 25.0f)) return;						// 0x48fe04
	m_mine_time = dt + m_mine_time;
	const float every = m_speed <= 1000.0f ? 5.0f : 2.0f;	// 0x48e5ac; 0x48d950 / 0x48d954
	if (!(m_mine_time > every)) return;
	m_mine_time = 0;
	CGameObject *mine = m_mines[m_next_mine];
	if (!mine) return;
	if (++m_next_mine > 29) m_next_mine = 0;
	// The mine is dropped at our position (its element slot 90, position, slot 137(1), element
	// place, shown, its +0x62c and +0x624 cleared, its colour (1, 1, 1, 1)).
	NEG_TODO(0x449890, "C3DYokCargo: drop a C3DMine");
}

void C3DYokCargo::RotateToDest(float dt)					// 0x449a00
{
	YokRotateToDest(this, dt);
}

void YokRotateToDest(C3DAI *self, float dt)					// 0x449a00
{
	if (!GameIsRunning()) return;							// 0x475ca0
	if (!self->f_005 || !self->f_00a) return;
	self->f_00a = 0;
	float rx = 0, ry = 0, rz = 0;
	if (self->f_274 != self->f_224)
	{
		rx = self->f_274 - self->f_224;
		if (rx > 180.0f) rx -= 360.0f;						// 0x48e5b0, 0x48e5c4
		else if (rx < -180.0f) rx += 360.0f;				// 0x48e5c0
		self->f_00a = 1;
	}
	if (self->f_278 != self->f_228)
	{
		ry = self->f_278 - self->f_228;
		if (ry > 180.0f) ry -= 360.0f;
		else if (ry < -180.0f) ry += 360.0f;
		self->f_00a = 1;
	}
	if (self->f_27c != self->f_22c)
	{
		rz = self->f_27c - self->f_22c;
		if (rz > 180.0f) rz -= 360.0f;
		else if (rz < -180.0f) rz += 360.0f;
		self->f_00a = 1;
	}
	self->AddRotation(rx * self->m_turn_rates.x * dt, ry * self->m_turn_rates.y * dt, rz * self->m_turn_rates.z * dt);	// +0x334
}

void C3DYokCargo::v266(void)								// 0x419aa0 (shared code)
{
	// (an empty debug log)
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990, 0x45fea0 (sic: no change)
}

void C3DYokCargo::v267(int level)							// 0x449bb0
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	C3DAnimated::v267(level);								// (BG slot 265)
	if (LevelID() != 'LEV5' || scene > 489) { ev70(1); return; }	// element +0x110 (BG ESLOT 68)
	ev70(0);
}

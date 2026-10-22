/*  C3DAISuv - see C3DAISuv.h. Reconstructed from Neutron.exe 0x40b1b0..0x40ba60. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DAISuv.h"
#include "C3DLightCone.h"
#include "../objects/C3DAITrigger.h"
#include "../C3DPlayer.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../Todo.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include <string.h>
#include <strings.h>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// Jimmy's start point name (his element +0x724).
static const char *StartPointOf(CGameObject *jim)
{
	C3DPlayer *p = dynamic_cast<C3DPlayer *>(jim);
	return p ? p->f_9c8 : NULL;
}

// The driver of the jeep (+0x69c of its element) / rocket ship (+0x740; +0x69c checked):
// Negatron's C3DJeep / C3DRocketShip layouts differ (TODO(BG)).
static CGameObject *DriverOf(CGameObject *vehicle)
{
	(void)vehicle;
	NEG_TODO(0x40b4f0, "the driver of the jeep / rocket ship an SUV hit");
	return NULL;
}

static void JimmySlot94(CGameObject *jim)	{ (void)jim; NEG_TODO(0x427ff0, "C3DJimmy element +0x178"); }

C3DAISuv::C3DAISuv()										// 0x40b1b0
{
	SetName((char *)"C3DSUV");								// 0x4702d0
	C3DAISuv::InitObject();									// 0x40b460 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3SUV');							// 0x462220
	f_968 = 700.0f;											// speed
	f_96c = 2;												// patrolling
	f_c08 = 2;												// AIState
	m_light = new C3DLightCone;								// 0x42d410 (0x6dc bytes)
	m_light->v107(0);										// +0x1b0
	m_light->m_green = 0.2f;
	m_light->m_blue = 0.2f;
	m_light->m_rot_speed = 140.0f;
	C3DAI::ev106(f_ac4, (char *)"none");
	C3DAI::ev106(f_a9c, (char *)"none");
	C3DAI::ev106(f_aec, (char *)"none");
	C3DAI::ev106(f_b14, (char *)"none");
	C3DAI::ev106(f_a74, (char *)"none");
	C3DAI::ev106(f_b3c, (char *)"none");
	m_go_time = 0;
}

C3DAISuv::~C3DAISuv()										// 0x40b410
{
}

namespace neg {
CGameObject *New_C3DAISuv(void) { return new C3DAISuv; }				// '3SUV' (0x9a4 bytes)
}

void C3DAISuv::InitObject(void)								// 0x40b460
{
	C3DAI::InitObject();									// 0x407ee0
	m_db = GetDatabase("jeep.omt");							// 0x46a910
	f_648 = 0;												// animations off
	set_shape(Load3DShape(m_db, 2));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	f_bd4 = 0;												// no hovering
	v67(500.0f);											// CGameObject +0x110: radius
	v64(0.2f);												// CGameObject +0x100
}

void C3DAISuv::AddTouching(CGameObject *other)				// 0x40b4f0
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (other->NameContains((char *)"C3DAITRIGGER") && strcasecmp(m_tag, "GOAWAY") == 0)
	{
		ev107(3);											// element +0x124
		C3DAITrigger *t = dynamic_cast<C3DAITrigger *>(other);	// (other - 0xc8)
		if (t) strcpy(f_9ac, t->m_ai_patrol);				// its +0xab8 "AIPatrol" -> "PatrolPoint" (port: checked)
		f_b8c = NULL;										// no destination
		m_go_time = 10.0f;
	}
	CGameObject *driver = NULL;
	bool hit = false;
	if (other->NameContains((char *)"C3DJEEP") && g_camera && g_camera == other)
	{
		hit = true;
		driver = DriverOf(other);
	}
	else if (other->NameContains((char *)"C3DROCKETSHIP") && g_camera && g_camera == other)
	{
		hit = true;
		driver = DriverOf(other);
	}
	else if (other->NameContains((char *)"C3DJIMMY") && g_camera)
	{
		hit = true;
		if (g_camera != other)
		{
			JimmySlot94(other);								// his element +0x178
			g_gametype->SetCamera(other);					// +0x118
		}
		driver = other;
	}
	if (!hit) return;
	const char *start = driver ? StartPointOf(driver) : NULL;
	char name[92];
	CGameObject *sp = NULL;
	if (start)
	{
		strncpy(name, start, sizeof name - 1);
		name[sizeof name - 1] = 0;
		sp = FindObjectByTag(name);							// 0x474070
	}
	if (sp)
	{
		float p[4];
		sp->v195(p);										// +0x310
		other->v196(p[0], p[1], p[2], p[3]);				// +0x314
	}
	ResetObject();											// +0x28
	PlayGameSoundIndex(-1, 180, false);						// 0x458980
}

void C3DAISuv::Process(float dt)							// 0x40b7a0
{
	v162(0, -100.0f, 0);									// +0x28c
	C3DAI::Process(dt);										// 0x408000
	const OMedia3DVector at = v224(0, 160.0f, 110.0f);		// +0x384: the headlight's place
	m_light->v196(at.x, at.y, at.z, at.w);					// +0x314
	float v[4];
	v155(v);												// +0x270: velocity
	const float vz = v[2];
	v155(v);
	v157(v[0], -1400.0f, vz);								// +0x278
	v174(v);												// +0x2bc: acceleration
	const float az = v[2];
	v174(v);
	v176(v[0], -200.0f, az);								// +0x2c4
	if (m_go_time > 0)
	{
		m_go_time -= dt;
		if (m_go_time <= 0)
		{
			ev107(2);										// back to patrolling
			m_go_time = 0;
		}
	}
}

// 0x40b8e0 / 0x40b960 (the same tail): the headlight is switched off while TaskName is below
// RequiredLevel or at RemoveLevel and beyond. It is never switched back on.
static void LightByTask(C3DAISuv *s)
{
	if (s->f_64c == -1) return;								// "RequiredLevel" (0x638)
	if (GetTaskState(s->m_task_name) < s->f_64c || GetTaskState(s->m_task_name) >= s->f_654)	// "RemoveLevel" (0x640)
	{
		if (s->m_light) s->m_light->m_off = 1;				// (sic: no light writes to 0x6d0)
	}
}

void C3DAISuv::v260(void)									// 0x40b8e0
{
	C3DAI::v260();											// 0x409480
	LightByTask(this);
}

void C3DAISuv::v267(int level)								// 0x40b960
{
	C3DAnimated::v267(level);								// 0x40e340
	LightByTask(this);
}

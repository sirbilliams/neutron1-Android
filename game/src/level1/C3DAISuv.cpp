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
#include "../JimmySlots.h"
#include "C3DJeep.h"
#include "../C3DRocketShip.h"
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
	return p ? p->f_724 : NULL;
}

// The driver of the jeep: BG +0x69c of its element, the member C3DJimmy sets when he gets on (f_a84 here).
static CGameObject *JeepDriver(CGameObject *vehicle)
{
	C3DJeep *jeep = dynamic_cast<C3DJeep *>(vehicle);
	return jeep ? jeep->f_a84 : NULL;
}

// The rocket ship: BG tests the int at +0x69c of its element, which lies in C3DAnimated's
// PickupLink name (0x655, 100 bytes; byte 71..74: zero unless a level wrote a 72+ character
// link) and takes the start point from the rider at +0x740 (f_9f4).
static CGameObject *RocketDriver(CGameObject *vehicle)
{
	C3DRocketShip *r = dynamic_cast<C3DRocketShip *>(vehicle);
	if (!r) return NULL;
	int tested;
	memcpy(&tested, r->f_669 + (0x69c - 0x655), 4);
	if (!tested) return NULL;
	return r->f_9f4;										// port: the original does not check for NULL
}

C3DAISuv::C3DAISuv()										// 0x40b1b0
{
	SetName((char *)"C3DSUV");								// 0x4702d0
	C3DAISuv::InitObject();									// 0x40b460 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3SUV');							// 0x462220
	m_speed = 700.0f;											// speed
	m_state = 2;												// patrolling
	m_ai_state = 2;												// AIState
	m_light = new C3DLightCone;								// 0x42d410 (0x6dc bytes)
	m_light->v107(0);										// +0x1b0
	m_light->m_green = 0.2f;
	m_light->m_blue = 0.2f;
	m_light->m_rot_speed = 140.0f;
	C3DAI::ev72(m_anim_run, (char *)"none");
	C3DAI::ev72(m_anim_wag, (char *)"none");
	C3DAI::ev72(m_anim_fly, (char *)"none");
	C3DAI::ev72(m_anim_sit, (char *)"none");
	C3DAI::ev72(m_anim_none, (char *)"none");
	C3DAI::ev72(m_anim_walk, (char *)"none");
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
	m_hover = 0;												// no hovering
	v67(500.0f);											// CGameObject +0x110: radius
	v64(0.2f);												// CGameObject +0x100
}

void C3DAISuv::AddTouching(CGameObject *other)				// 0x40b4f0
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (other->NameContains((char *)"C3DAITRIGGER") && strcasecmp(m_tag, "GOAWAY") == 0)
	{
		ev73(3);											// element +0x124
		C3DAITrigger *t = dynamic_cast<C3DAITrigger *>(other);	// (other - 0xc8)
		if (t) strcpy(m_patrol_point, t->m_ai_patrol);				// its +0xab8 "AIPatrol" -> "PatrolPoint" (port: checked)
		m_dest = NULL;										// no destination
		m_go_time = 10.0f;
	}
	// BG: the camera object being the jeep / rocket ship with a driver, else Jimmy himself (a jeep or
	// rocket ship without a driver falls through to the Jimmy test, which then fails: nothing happens).
	CGameObject *driver = NULL;
	if (other->NameContains((char *)"C3DJEEP") && g_camera && g_camera == other)
		driver = JeepDriver(other);
	if (!driver && other->NameContains((char *)"C3DROCKETSHIP") && g_camera && g_camera == other)
		driver = RocketDriver(other);
	if (!driver)
	{
		if (!other->NameContains((char *)"C3DJIMMY") || !g_camera) return;
		if (g_camera != other)
		{
			JimmyElementSlot94(other);						// his element +0x178 (0x427ff0)
			g_gametype->SetCamera(other);					// +0x118
		}
		driver = other;
	}
	const char *start = StartPointOf(driver);
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
			ev73(2);										// back to patrolling
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

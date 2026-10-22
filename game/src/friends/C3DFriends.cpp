/*  C3DFriends - see C3DFriends.h. Reconstructed from Neutron.exe 0x41b490..0x41e580 and the
    shared copies 0x415090, 0x418290, N2:0x417110, N2:0x4369f0, 0x419aa0, N2:0x436900, 0x4418e0, N2:0x44fa60,
    N2:0x436920 / N2:0x427a40 / N2:0x45b120. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DFriends.h"
#include "../C3DShadow.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../InterfaceElement.h"
#include "../Physics.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
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

// --------------------------------------------------------------------------- construction

C3DFriends::C3DFriends()									// 0x41b490
{
	m_hover = 0;											// no hovering
	v64(0.05f);
	m_talk_state[0] = 0;									// TalkState0..4
	m_talk_state[1] = -1;
	m_talk_state[2] = -1;
	m_talk_state[3] = -1;
	m_talk_state[4] = -1;
	for (int i = 0; i < 5; i++) strcpy(m_talk_trigger[i], "none");	// TalkTrigger0..4
	m_talks = 0;
	m_timer = 100.0f;
	f_008 = 1;												// CGameObject +0x8
	m_hold_when_touched = 1;								// held when touched
	if (g_windowed) m_shadow = NULL;						// 0x509a13
	else m_shadow = new C3DShadow;							// 0x5f4 bytes (the original keeps its CGameObject part)
	// (debug log "C3DShadow", 0x5f4)
	if (m_shadow)
	{
		m_shadow->v36(1);									// CGameObject +0x90
		m_shadow->SetSize(90, 30);							// 0x43f870
	}
}

C3DFriends::~C3DFriends()									// 0x41b740
{
	// (only the vtables; the shadow is not deleted)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DFriends::InitObject(void)							// 0x41b840
{
	C3DAI::InitObject();
	RegisterProperty("TalkState0", &m_talk_state[0], PROP_LONG, 0);
	RegisterProperty("TalkTrigger0", m_talk_trigger[0], PROP_ALPHA, 0);
	RegisterProperty("TalkState1", &m_talk_state[1], PROP_LONG, 0);
	RegisterProperty("TalkTrigger1", m_talk_trigger[1], PROP_ALPHA, 0);
	RegisterProperty("TalkState2", &m_talk_state[2], PROP_LONG, 0);
	RegisterProperty("TalkTrigger2", m_talk_trigger[2], PROP_ALPHA, 0);
	RegisterProperty("TalkState3", &m_talk_state[3], PROP_LONG, 0);
	RegisterProperty("TalkTrigger3", m_talk_trigger[3], PROP_ALPHA, 0);
	RegisterProperty("TalkState4", &m_talk_state[4], PROP_LONG, 0);
	RegisterProperty("TalkTrigger4", m_talk_trigger[4], PROP_ALPHA, 0);
}

void C3DFriends::ResetObject(void)							// 0x41b790
{
	C3DAI::ResetObject();
	m_talks = 0;
	m_timer = 100.0f;
}

void C3DFriends::Process(float dt)							// 0x41b950
{
	C3DAI::Process(dt);
	m_timer += dt;
	if (m_state == 0) ev80();								// attacking a friend means: talk (element +0x140)
	if (!f_13e)												// (CGameObject +0x13e)
	{
		if (m_shadow) m_shadow->hide(true);					// element +0x58
		return;
	}
	if (g_windowed) return;									// 0x509a13
	float p[4];
	v195(p);
	const OMedia3DVector a(p[0], p[1], p[2], p[3]);
	const OMedia3DVector b(p[0], p[1] - 500.0f, p[2], p[3]);	// 0x48e5bc
	if (!PhysicsClosestHit(a, b, false)) return;			// 0x47c6e0: the floor under us
	if (!m_shadow) return;
	const float floor_y = g_phys_ray.end.y;					// 0x5cfc64
	v195(p);
	m_shadow->v197(p[0], floor_y + 10.0f, p[2]);			// slot 198 (+0x318); 0x48e5ec
	if (m_shadow->get_collision_level() != 0)				// (2002 element +0x70)
	{
		m_shadow->set_collision_level((omt_CollisionLevel)1);
		m_shadow->hide(false);
	}
}

// --------------------------------------------------------------------------- element family

void C3DFriends::ev79(void)									// 0x41e580
{
	if (m_state == 10) ev93(1400.0f, OMedia3DVector(0, 0, 0, 0));
}

void C3DFriends::ev80(void)									// 0x41b7b0
{
	// (debug log)
	CGameObject *o = ev95();								// element +0x17c
	if (!o) return;
	ev96();													// element +0x180
	o->v23(o);												// its slot 23 (+0x5c): activated (by itself, as the original passes it)
	m_talks++;
	m_timer = 0;
	m_state = 9;
	ev82();													// element +0x148: face the way the target faces, turned round
}

void C3DFriends::ev90(void)									// 0x41bc20
{
	// (debug log)
	m_state = 3;
}

CGameObject *C3DFriends::ev95(void)							// 0x41bb20
{
	// (debug log "GetActivatedObject TaskState is %d")
	for (int i = 0; i < 5; i++)
	{
		CGameObject *o = FindObjectByTag(m_talk_trigger[i]);
		const int s = m_talk_state[i];
		if (o && s != -1 && (s == (int)f_4a0 || s == -2)) return o;	// our task state (CLocalGameObject +0x494)
	}
	return NULL;
}

void C3DFriends::ev96(void)									// 0x41b810
{
	AddScore(100);											// 0x42adc0
	v261();													// slot 260 (+0x410): stop
}

// --------------------------------------------------------------------------- shared copies

void C3DFriends::FriendInitObjectAnim(void)				// 0x415090
{
	Log("InitObject()");
	C3DFriends::InitObject();								// 0x41b840
	InitAnim();												// element +0x110
	// (then a call to the empty 0x472970)
}

void C3DFriends::FriendInitObjectAIAnim(void)				// 0x418290
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0 (sic: not C3DFriends', no Talk* properties)
	InitAnim();												// element +0x110
	// (then a call to the empty 0x472970)
}

void C3DFriends::FriendPropsLoadedShow(void)				// N2:0x417110
{
	C3DAI::v260();											// 0x409480
	ev70(1);												// element +0x118
}

void C3DFriends::FriendNoAnimIdle(void)					// N2:0x4369f0
{
	ev58((char *)"IDLE", 1);								// element +0xe8
}

void C3DFriends::FriendTaskStateRewrite(void)				// 0x419aa0
{
	// (debug log, empty)
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990 / 0x45fea0 (as shipped: writes back what it read)
}

void C3DFriends::FriendLevelScene(int level)				// N2:0x436900
{
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
	C3DAnimated::v267(level);								// 0x40e340
}

void C3DFriends::FriendLevelSceneShow(int level)			// 0x4418e0
{
	C3DAnimated::v267(level);								// 0x40e340
	GetTaskState("SCENE");									// (the result is not used)
	ev70(1);												// element +0x118
}

void C3DFriends::FriendTouchScene(void)					// N2:0x44fa60
{
	// (debug log, empty)
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
}

// v270: when a cut scene starts (new mode 3) while we STOP, play <anim> instead and remember
// STOP in C3DPlayerObject's f_938 (f_934 = 1); when it ends (old mode 3), go back to it.
// C3DAnimated::v270 is not called.
void C3DFriends::FriendChangeState(int old_mode, int new_mode, const char *anim)	// N2:0x436920
{
	// (debug log "C3DFowl::ChangeState %d  %d", or Cindy's / Numey's)
	if (new_mode == 3 && f_934 == -1 && strcasecmp("STOP", GetAnim()) == 0)	// element +0xf4
	{
		strcpy(f_938, "STOP");
		f_934 = 1;
		ev58((char *)anim, 1);								// element +0xe8
	}
	if (old_mode == 3 && f_934 != -1)
	{
		ev58(f_938, 1);
		f_934 = -1;
	}
}

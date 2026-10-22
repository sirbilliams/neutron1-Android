/*  C3DAITrigger - see C3DAITrigger.h. Reconstructed from Neutron.exe 0x40ba70..0x40d100. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DAITrigger.h"
#include "../C3DObject.h"
#include "../C3DAnimated.h"
#include "../C3DAI.h"
#include "../C3DJimmy.h"
#include "../C3DPlayer.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../Todo.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- small helpers

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// 0x4038c0: set the type of item `item` (0..28) of menu page `page` ("Activating Item %d for %d
// IsActive:%d" is a compiled-out log).
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 0x1d) return;
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;						// port: the original does not check the page
}

// 0x4061d0: a pickup collected: the top-right pop-up for picture `pic` (3 s; queued behind the
// one showing), and its count under the pickup icon.
static void PickupPopup(int pic, int index)
{
	if (g_4f815c <= 0)
	{
		g_4f8158 = -1;
		g_4f8154 = pic;
		g_4f815c = 3.0f;
		g_4f8160 = (unsigned int)index;
	}
	else if (g_4f8154 != -1)
	{
		g_4f8158 = pic;
		g_4f7db8[pic * 2]++;
		return;
	}
	g_4f7db8[pic * 2]++;
}

// 0x460e70: ask for the task file `name` to be loaded (0x4fc67c = 1, the name into 0x4fc604).
static void RequestTaskFile(const char *name)
{
	g_4fc67c = 1;
	strcpy((char *)g_4fc604, name);
}

// 0x406f90(screen): open in-game menu screen `screen` (once: 0x4f8198), with the cursor.
// TODO(core): the BG menu.
static void OpenMenuScreen(int screen)
{
	(void)screen;
	NEG_TODO(0x406f90, "C3DAITrigger: open a menu screen");
}

// The element of a game object (the original subtracts 0xc0 / 0xc8 without looking).
static OMediaElement *Elt(CGameObject *o)	{ return o ? dynamic_cast<OMediaElement *>(o) : NULL; }
// The 2002 "show" pattern: hide count (+0x70) to 1, then hide(false).
static void ForceShow(CGameObject *o)		{ if (OMediaElement *e = Elt(o)) e->force_show(); }
static void Hide(CGameObject *o)			{ if (OMediaElement *e = Elt(o)) e->hide(true); }	// element +0x58

static C3DAnimated *Anim(CGameObject *o)	{ return o ? dynamic_cast<C3DAnimated *>(o) : NULL; }
static void ObjSetAnimation(CGameObject *o, const char *name, bool loop)	// element +0xe0 (BG ESLOT 56)
{
	if (C3DAnimated *a = Anim(o)) a->ev58((char *)name, loop);
}
// C3DAI (port: an object whose name only contains "C3DAI" but is none is left alone).
static C3DAI *AI(CGameObject *o)			{ return o ? dynamic_cast<C3DAI *>(o) : NULL; }
static void AISetState(CGameObject *ai, int state)							// element +0x124 (BG ESLOT 73)
{
	if (C3DAI *a = AI(ai)) a->ev73(state);
}
static void AISetSpeed(CGameObject *ai, float speed)						// C3DAI +0x6c4
{
	if (C3DAI *a = AI(ai)) a->m_speed = speed;
}
static void AISetPatrolPoint(CGameObject *ai, const char *name)			// C3DAI +0x708 = name, +0x8c0 = 0
{
	if (C3DAI *a = AI(ai))
	{
		strcpy(a->m_patrol_point, name);
		a->m_dest = NULL;
	}
}
// Element slot 82 (+0x148) of the camera object (a C3DPlayer: Jimmy).
static void CameraSlot82(CGameObject *o)
{
	C3DPlayer *p = o ? dynamic_cast<C3DPlayer *>(o) : NULL;
	if (p) p->StepSoundOff();										// (C3DJimmy: stop the footstep loop)
}

// --------------------------------------------------------------------------- construction

C3DAITrigger::C3DAITrigger()								// 0x40ba70
{
	SetName((char *)"C3DAITRIGGER");
	Log("C3DAITrigger()");
	C3DAITrigger::InitObject();								// 0x40bf60 (called directly)
	SetObjectID('3AIT');									// 0x462d80
	strcpy(m_sprite_db, "icons.omt");
	m_sprite_index = 9;
	m_sprite_size = 50;
	strcpy(m_activate_by, "JIM1");
	strcpy(m_activate_by_anim, "none");
	m_activate_state[0] = 0;
	m_activate_state[1] = -1;
	m_activate_state[2] = -1;
	m_activate_state[3] = -1;
	m_activate_state[4] = -1;
	for (int i = 0; i < 5; i++) strcpy(m_activate_object[i], "none");
	strcpy(m_is_a, "none");
	strcpy(m_ai_target, "none");
	strcpy(m_ai_anim, "none");
	strcpy(m_ai_new_pos, "none");
	m_ai_new_rot_y = -1.0f;
	m_ai_hide_obj = -1;
	strcpy(m_next_trigger, "none");
	strcpy(m_ai_patrol, "none");
	strcpy(m_player_controlled, "none");
	v67(250.0f);											// (BG slot 68) radius
	v46(0);
	v52(1);
	v32(1);
	m_times_to_trigger = -1;
	m_times_triggered = 0;
	f_008 = 1;												// element +0xd0
	C3DSprite::InitPhysics();								// 0x464210
	m_touch_activated = 0;
	m_ai_state = 2;
	v34(1);
	m_ai_speed = -1;
	f_c00 = 0;
	// (then a call to the empty 0x472970)
}

C3DAITrigger::~C3DAITrigger() {}							// 0x40bee0

namespace neg {
CGameObject *New_C3DAITrigger(void) { return new C3DAITrigger; }		// '3AIT' (0xc0c bytes)
}

// --------------------------------------------------------------------------- CGameObject-family slots

void C3DAITrigger::InitObject(void)							// 0x40bf60
{
	C3DTriggerType::InitObject();							// 0x447c60
	RegisterProperty("Radius",           &f_034,               PROP_FLOAT, 0);
	RegisterProperty("ActivateBy",       m_activate_by,        PROP_ALPHA, 0);
	RegisterProperty("IsA",              m_is_a,               PROP_ALPHA, 0);
	RegisterProperty("ActivateByAnim",   m_activate_by_anim,   PROP_ALPHA, 0);
	RegisterProperty("ActivateState0",   &m_activate_state[0], PROP_LONG,  0);
	RegisterProperty("ActivateObject0",  m_activate_object[0], PROP_ALPHA, 0);
	RegisterProperty("ActivateState1",   &m_activate_state[1], PROP_LONG,  0);
	RegisterProperty("ActivateObject1",  m_activate_object[1], PROP_ALPHA, 0);
	RegisterProperty("ActivateState2",   &m_activate_state[2], PROP_LONG,  0);
	RegisterProperty("ActivateObject2",  m_activate_object[2], PROP_ALPHA, 0);
	RegisterProperty("ActivateState3",   &m_activate_state[3], PROP_LONG,  0);
	RegisterProperty("ActivateObject3",  m_activate_object[3], PROP_ALPHA, 0);
	RegisterProperty("ActivateState4",   &m_activate_state[4], PROP_LONG,  0);
	RegisterProperty("ActivateObject4",  m_activate_object[4], PROP_ALPHA, 0);
	RegisterProperty("AITarget",         m_ai_target,          PROP_ALPHA, 0);
	RegisterProperty("AIAnim",           m_ai_anim,            PROP_ALPHA, 0);
	RegisterProperty("AIState",          &m_ai_state,          PROP_LONG,  0);
	RegisterProperty("AISpeed",          &m_ai_speed,          PROP_LONG,  0);
	RegisterProperty("AIPatrol",         m_ai_patrol,          PROP_ALPHA, 0);
	RegisterProperty("AINewPos",         m_ai_new_pos,         PROP_ALPHA, 0);
	RegisterProperty("AINewRotY",        &m_ai_new_rot_y,      PROP_FLOAT, 0);
	RegisterProperty("AIHideObj",        &m_ai_hide_obj,       PROP_LONG,  0);
	RegisterProperty("PlayerControlled", m_player_controlled,  PROP_ALPHA, 0);
	RegisterProperty("TimesToTrigger",   &m_times_to_trigger,  PROP_LONG,  0);
	RegisterProperty("TouchActivated",   &m_touch_activated,   PROP_LONG,  0);
}

void C3DAITrigger::ResetObject(void)						// 0x40bf20
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	if (m_touch_activated) v234();							// +0x3ac (BG slot 235): collision on
	else v232();											// +0x3a4 (BG slot 233): off
	m_times_triggered = 0;
}

void C3DAITrigger::AddTouching(CGameObject *other)			// 0x40c200
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_touch_activated) Trigger(other);
}

void C3DAITrigger::v23(CGameObject *o)						// 0x40ca70
{
	(void)o;
	Trigger(NULL);
}

void C3DAITrigger::v267(int a2)								// 0x40ca80
{
	CLocalGameObject::v267(a2);								// (BG slot 265)
	GetTaskState(m_task_name);								// 0x45fea0 (the result is not used)
}

// --------------------------------------------------------------------------- the script

// 0x40c230: the first ActivateObject<n> that exists and whose ActivateState<n> (not -1) is the
// state of this trigger's task as the object last read it (CLocalGameObject +0x494).
CGameObject *C3DAITrigger::FindActiveObject(void)
{
	for (int i = 0; i < 5; i++)
	{
		CGameObject *o = FindObjectByTag(m_activate_object[i]);
		if (o && m_activate_state[i] != -1 && m_activate_state[i] == f_4a0) return o;
	}
	return NULL;
}

// 0x40c300: fire. `caller` is who fired it (NULL for a chain or a callback).
void C3DAITrigger::Trigger(CGameObject *caller)
{
	bool go = false, target_is_caller = false;
	// (debug logs "CALLING AITrigger %s" x2, m_tag)
	CGameObject *by = FindObjectByTag(m_activate_by);
	if (!by)
	{
		if (strcasecmp(m_activate_by, "none") == 0) go = true;
	}
	else
	{
		// (debug log "5 %s  %s")
		if (by == caller)
		{
			go = true;
			if (by->NameContains((char *)"C3DANIMATED") && strcasecmp(m_activate_by_anim, "none") != 0)
				ObjSetAnimation(by, m_activate_by_anim, true);	// (debug logs "Setting %s to animation %s for %s" x2)
		}
	}
	if (strcasecmp(m_is_a, "none") != 0 && caller && caller->NameContains(m_is_a))
		target_is_caller = true;
	else if (!go) return;

	if (m_times_to_trigger != -1 && ++m_times_triggered > m_times_to_trigger) return;

	if (strcasecmp(m_player_controlled, "none") != 0 && strcasecmp(m_player_controlled, "NULL") == 0)
	{
		if (g_camera)
		{
			g_camera->v261();								// +0x410 (BG slot 260)
			if (g_camera->NameContains((char *)"C3DJIMMY"))
			{
				// (debug log "Setting Jim1 animation STOP")
				ObjSetAnimation(g_camera, "STOP", true);
				CameraSlot82(g_camera);
			}
		}
		g_gametype->SetCamera(NULL);						// +0x118
	}

	if (strcasecmp(m_ai_target, "none") != 0 || target_is_caller)
	{
		CGameObject *t = target_is_caller ? caller : FindObjectByTag(m_ai_target);
		if (t)
		{
			if (t->NameContains((char *)"C3DSPRITE"))
			{
				if (m_ai_hide_obj != -1)
				{
					if (m_ai_hide_obj == 0) ForceShow(t);
					else Hide(t);
				}
				CGameObject *p = FindObjectByTag(m_ai_new_pos);
				if (p)
				{
					// (debug log "\tSetting position of %s to %s for %s")
					float v[4];
					p->v195(v);								// +0x310
					t->v196(v[0], v[1], v[2], v[3]);		// +0x314
				}
			}
			if (t->NameContains((char *)"C3DANIMATED"))
			{
				if (m_ai_hide_obj != -1)
				{
					if (m_ai_hide_obj == 0) { ForceShow(t); t->v132(1); }	// +0x214 (BG slot 133)
					else { Hide(t); t->v132(0); }
				}
				if (strcasecmp(m_ai_anim, "none") != 0)
					ObjSetAnimation(t, m_ai_anim, true);	// (debug log "\tSetting sequence of %s to %s for %s")
				CGameObject *p;
				if (strcasecmp(m_ai_new_pos, "none") != 0 && (p = FindObjectByTag(m_ai_new_pos)) != NULL)
				{
					float v[4];
					p->v195(v);
					t->v196(v[0], v[1], v[2], v[3]);
				}
			}
			if (t->NameContains((char *)"C3DAI"))
			{
				if (m_ai_hide_obj != -1)
				{
					if (m_ai_hide_obj == 0) { ForceShow(t); t->v132(1); }
					else { Hide(t); t->v132(0); }
				}
				// (debug log "setting %s to %d")
				AISetState(t, m_ai_state);
				if (m_ai_speed != -1) AISetSpeed(t, (float)m_ai_speed);	// "\tChanging speed of %s to %d for %s"
				if (strcasecmp(m_ai_patrol, "none") != 0) AISetPatrolPoint(t, m_ai_patrol);
				if (m_ai_new_rot_y != -1.0f)
				{
					// (debug log "\tSetting Rotation of %s to %f for %s")
					float a[4], b[4];
					t->v201(a);								// +0x328: current angles
					const float az = a[2], ry = m_ai_new_rot_y;
					t->v201(b);
					t->v92(b[0], ry, az);					// +0x174 (BG slot 93): the initial rotation
					t->f_274 = b[0];						// and the one it turns to
					t->f_278 = ry;
					t->f_27c = az;
					t->f_280 = 1.0f;
				}
			}
			else if (t->NameContains((char *)"C3DSTALAGTITE"))
			{
				// its element slot 72 (+0x120). TODO: C3DStalagtite is not reconstructed yet.
				NEG_TODO(0x40c981, "C3DAITrigger: C3DStalagtite slot 72");
			}
		}
	}

	CGameObject *toggle;
	if (strcasecmp(m_toggle_object, "none") != 0 && (toggle = FindObjectByTag(m_toggle_object)) != NULL)
	{
		if (CLocalGameObject *l = dynamic_cast<CLocalGameObject *>(toggle)) l->v268(m_toggle);	// +0x428 (BG slot 266) NextState(Toggle)
	}

	CGameObject *n = FindActiveObject();					// 0x40c230
	SpecialAction();										// 0x40caa0
	if (f_c00)
	{
		CallNextTrigger("default", 0);						// element +0xd4
		f_c00 = 0;
		return;
	}
	if (n) { CallNextTrigger(n->m_tag, 0); return; }		// (debug logs "16.5", "\tCalling Next Trigger %s for %s")
	CallNextTrigger(m_next_trigger, 0);
}

// 0x40caa0: the built-in actions of the special tags (most advance the SCENE task one step when
// it is at the right state).
void C3DAITrigger::SpecialAction(void)
{
	int scene = GetTaskState("SCENE");						// 0x45fea0
	const char *tag = m_tag;
	CGameObject *jim;
	#define IS(s) (strcasecmp(tag, s) == 0)					// 0x487c20
	if (IS("CARLOUT") && scene == 70) SetTaskState("SCENE", 75);	// 0x45f990
	if (IS("CALLFROMNICK") && scene == 90) SetTaskState("SCENE", 100);
	if (IS("TICKETBOOTH") && scene == 320)
	{
		SetTaskState("SCENE", 330);
		SetItemState(2, 28, 1);
		PickupPopup(28, 0);
	}
	if (IS("teleportexplanation") && (scene == 30 || scene == 31)) SetTaskState("SCENE", 35);
	if (IS("RESCUECARL") && scene == 480) SetTaskState("SCENE", 490);
	if (IS("SEECARL") && scene == 520) SetTaskState("SCENE", 530);
	if (IS("EVADEYOKES") && scene == 540) SetTaskState("SCENE", 550);
	if (IS("INVISPART"))
	{
		if (scene == 162) SetTaskState("SCENE", 168);
		else if (scene == 490) SetTaskState("SCENE", 500);
	}
	if (IS("ESCAPESHIP") && scene == 172) SetTaskState("SCENE", 178);
	if (IS("GOINGHOME") && scene == 178) SetTaskState("SCENE", 200);
	if (IS("LANDSHIP") && scene == 510) SetTaskState("SCENE", 520);
	if (IS("REMOTE")) OpenMenuScreen(3);					// 0x406f90
	if (IS("ABDUCTED") && scene == 530) SetTaskState("SCENE", 540);
	if (IS("SAVECARL") && scene == 510) SetTaskState("SCENE", 520);
	if (IS("CARLDIS") && scene == 380) SetTaskState("SCENE", 390);
	if (IS("BEAMOFF") && scene == 410)
	{
		SetItemState(2, 24, 1);
		PickupPopup(24, 0);
		SetTaskState("SCENE", 420);
	}
	if (IS("FOWLINV") && scene == 460) SetTaskState("SCENE", 470);
	if (IS("PUTGODDARD") && (scene == 120 || scene == 145) && (jim = FindObjectByTag("JIM1")) != NULL)
	{
		// Jimmy's element slots 94 (+0x178) and 93 (+0x174)(4). TODO(core): BG C3DJimmy.
		NEG_TODO(0x40cdf9, "C3DAITrigger PUTGODDARD: Jimmy slots 94 / 93");
	}
	if (IS("GOGODDARD") && GetTaskState("SCENE") == 168)
	{
		SetTaskState("SCENE", 170);
		SetItemState(0, 2, 1);
		OpenMenuScreen(6);
	}
	if (IS("JIMEND") && (jim = FindObjectByTag("JIM1")) != NULL)
	{
		// Jimmy's element slot 94 (+0x178). TODO(core): BG C3DJimmy.
		NEG_TODO(0x40ce50, "C3DAITrigger JIMEND: Jimmy slot 94");
	}
	if (IS("RECHARGE"))
	{
		if (g_health >= 25.0f) f_c00 = 1;					// 0x4f83d4, 0x48fe04: full already: "default"
		else g_health = 25.0f;
	}
	if (IS("KITEND1") && GetTaskState("KITTY1") == 0) SetTaskState("KITTY1", 10);
	if (IS("KITEND2") && GetTaskState("KITTY2") == 0) SetTaskState("KITTY2", 10);
	if (IS("KITEND3") && GetTaskState("KITTY3") == 0) SetTaskState("KITTY3", 10);
	if (IS("GETFUEL4") && GetTaskState("SCENE") == 390)
	{
		SetTaskState("SCENE", 400);
		OpenMenuScreen(5);
	}
	if (IS("BONUSSCREEN") && (jim = FindObjectByTag("JIM1")) != NULL)
	{
		// Jimmy +0xa18 (an object) gets +0x4f0 = 1. TODO(core): BG C3DJimmy.
		NEG_TODO(0x40cf2c, "C3DAITrigger BONUSSCREEN: Jimmy +0xa18");
	}
	if (IS("GIVEKEY") && GetTaskState("SCENE") == 205)
	{
		SetTaskState("SCENE", 210);
		SetItemState(2, 17, 1);
		PickupPopup(17, 0);
	}
	if (IS("GIVEAUTO") && GetTaskState("SCENE") == 330)
	{
		SetTaskState("SCENE", 340);
		SetItemState(2, 22, 1);
		PickupPopup(22, 0);
	}
	if (IS("RESTARTGAME")) RequestTaskFile("NewGame.tsk");	// 0x460e70
	#undef IS
	(void)scene;
}

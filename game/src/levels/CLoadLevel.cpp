/*  CLoadLevel - see CLoadLevel.h. Reconstructed from Neutron.exe 0x457b10..0x4586c0 (ESLOT 55
    0x458510, the load a trigger starts for an object, is not written yet: TODO(BG)). */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CLoadLevel.h"
#include "../C2DInGameMenu.h"
#include "../CEditor.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../Globals.h"
#include "../Todo.h"
#include "../C3DPlayer.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- Jimmy's side

// C3DJimmy (a C3DPlayer; its members keep Negatron's names): f_b89 (+0x955) the level file
// Jimmy is in and f_9c8 (+0x724) his start point there, f_c25 (+0x88c) / f_b24 (+0x8f0) the
// level and start point to return to, f_b88 (+0x954) and f_bd9 (+0x9a5) two flags RETURN sets.
static C3DPlayer *PlayerOf(CGameObject *jim)	{ return jim ? dynamic_cast<C3DPlayer *>(jim) : NULL; }

// --------------------------------------------------------------------------- construction

CLoadLevel::CLoadLevel()													// 0x457b10
{
	static char name[] = "C3DLOADLEVEL";		// SetName upper-cases in place
	SetName(name);
	Log("C3DLoadLevel()");
	CLoadLevel::InitObject();					// (direct call)
	C3DSprite::SetObjectID('LOAD');				// 0x462d80
	strcpy(m_sprite_db, "icons.omt");
	m_sprite_index = 2;
	m_sprite_size = 100;
	strcpy(m_level_name, "none");
	strcpy(m_start_point, "none");
	strcpy(m_required_task, "none");
	m_required_level = 0;
	C3DSprite::InitSpriteProperties(200.0f);	// 0x4645f0
	CGameObject::v67(250.0f);					// radius ("Radius", f_034)
	CGameObject::v46(0);
	CGameObject::v32(1);
	CGameObject::v34(1);
	f_008 = 1;
	C3DSprite::InitPhysics();					// 0x464210
	m_fade_type = 0;
	m_sound_index = -1;
	m_exact_level = -1;
	m_fade_time = 1.0f;
	g_view_restore_x = 0.0f;					// 0x4f8418..: the view the menu puts back after a fade
	g_view_restore_y = 0.0f;
	g_view_restore_z = 0.0f;
	g_view_restore_ax = 0.0f;
	g_view_restore_ay = 0.0f;
	g_view_restore = 0;							// 0x4f8430
	m_promotion = 0;							// port: Negatron only (see the header)
	m_load_screen = 0;
	m_go_if_active = -1;
}

CLoadLevel::~CLoadLevel()													// 0x457d60
{
}

// --------------------------------------------------------------------------- element-family slots

// 0x458400: the story step of a tagged exit.
void CLoadLevel::ev55(void)
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	if (strcasecmp(m_tag, "LABTONEIGH") == 0 && scene == 60)	// 0x487c20 _stricmp
	{
		C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(1);	// 0x4038c0(1, 1, 1): gadget item 1
		if (p) p->items[1].type = 1;
		SetTaskState("SCENE", 65);							// 0x45f990
	}
	if (strcasecmp(m_tag, "YOKIAN2") == 0 && GetTaskState("SCENE") == 108) SetTaskState("SCENE", 110);
	if (strcasecmp(m_tag, "YOKIANS2") == 0 && GetTaskState("SCENE") == 108) SetTaskState("SCENE", 110);
	if (strcasecmp(m_tag, "PLANTSAVED") == 0 && GetTaskState("SCENE") == 470)
	{
		SetTaskState("SCENE", 480);
		g_pickup_counts[2 * 25] += -1;						// 0x4061c0
		if (g_pickup_counts[2 * 25] < 1)					// 0x4061b0
		{
			C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(2);	// 0x403910(2, 25)
			if (p) p->items[25].type = 0;
		}
	}
}

// 0x46df70 (CEditor element +0x100): load `name` at once. (A's CEditor names this slot
// RequestLevelDelayed; Boy Genius's takes only the name and sets m_load_pending.)
static void EditorLoadNow(const char *name)
{
	g_editor->m_load_pending = 1;
	strcpy(g_editor->m_level_name, name);
}

void CLoadLevel::CallLoadLevel(void)										// 0x458370
{
	// ("Calling LoadLevel with startpoint %s for %s": debug log)
	ev55();													// element +0xd8
	EditorLoadNow(m_level_name);							// g_editor +0x100
}

// --------------------------------------------------------------------------- CGameObject-family slots

void CLoadLevel::InitObject(void)											// 0x457da0
{
	Log("InitObject()");
	C3DSprite::InitObject();
	RegisterProperty("LevelName",     m_level_name,      PROP_ALPHA, 0);
	RegisterProperty("StartPoint",    m_start_point,     PROP_ALPHA, 0);
	RegisterProperty("RequiredTask",  m_required_task,   PROP_ALPHA, 0);
	RegisterProperty("RequiredLevel", &m_required_level, PROP_LONG,  0);
	RegisterProperty("ExactLevel",    &m_exact_level,    PROP_LONG,  0);
	RegisterProperty("Radius",        &f_034,            PROP_FLOAT, 0);
	RegisterProperty("SoundIndex",    &m_sound_index,    PROP_LONG,  0);
	RegisterProperty("FadeType",      &m_fade_type,      PROP_LONG,  0);
	RegisterProperty("FadeTime",      &m_fade_time,      PROP_FLOAT, 0);
	SetObjectID('LOAD');									// element +0xc0
}

// 0x403c10: start the in-game menu's fade (type, seconds; at least 0.1 s). (Boy Genius's has no
// fade setting, no 6 -> 4 and no "fading" flag; the variables are the menu's.)
static void StartFadeBG(int type, float t)
{
	g_fade_state = type;									// 0x4f8190
	if (t < 0.1f) t = 0.1f;									// 0x48d920
	g_fade_duration = t;									// 0x4ec490
	g_fade_time = t;										// 0x4f818c
	g_fade_done = false;									// 0x4ec494
}

static void JimmySlot94(CGameObject *jim)	{ (void)jim; NEG_TODO(0x427ff0, "C3DJimmy element +0x178"); }

// 0x457ec0: Jimmy walked into the exit. The original's debug logs ("MENTEMP Touched a
// CLoadLevel to %s at %s", "MENTEMP %s %s %s %s %s %s ", ...) are empty in the shipped build.
void CLoadLevel::AddTouching(CGameObject *other)
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// vtable +0x18
	C3DPlayer *p = PlayerOf(other);
	if (!p) return;											// port: a C3DJIMMY that is no player (the original has no check)
	if (strcasecmp(m_required_task, "none") != 0)
	{
		const int state = GetTaskState(m_required_task);
		if (state == -1) ErrorLog("ERROR: Task %s not found in in %s", m_required_task, m_tag);	// (debug log in the original)
		else
		{
			if (state < m_required_level) return;
			if (m_exact_level != -1 && m_exact_level != state) return;
		}
	}
	if (strcasecmp(m_level_name, "RETURN") == 0)
	{
		// "MENTEMP hit a RETURN loadpoint returning"
		JimmySlot94(other);									// element +0x178
		p->f_b88 = 1;										// (+0x954)
		p->f_bd9 = 1;										// (+0x9a5)
		char level[80], start[100], ret_level[100], ret_start[100];
		strcpy(level, p->f_c25);							// (+0x88c)
		strcpy(start, p->f_b24);							// (+0x8f0)
		strcpy(ret_level, p->f_c25);
		strcpy(ret_start, p->f_b24);
		p->ev121(level, start, ret_level, ret_start, 0);	// element +0x168 (port: Negatron's loading picture argument, 0)
		if (m_sound_index != -1) SetChannelVolume((short)PlaySoundIndex(-1, m_sound_index, false), g_sfx_volume);	// 0x47d390, 0x47dc80
		other->v176(0.0f, 0.0f, 0.0f);						// vtable +0x2c4: stop
		return;
	}
	if (strcasecmp(m_level_name, "none") == 0) return;
	switch (g_4f0588)										// the gadget in hand
	{
		case 0: case 1: case 4: case 7:
			JimmySlot94(other);								// element +0x178
			break;
	}
	ev55();													// element +0xd8
	char level[80], start[80], ret_level[100], ret_start[100];	// (copies: ev121 overwrites Jimmy's strings)
	strcpy(level, m_level_name);
	strcpy(start, m_start_point);
	strcpy(ret_level, p->f_b89);							// (+0x955)
	strcpy(ret_start, p->f_9c8);							// (+0x724)
	p->ev121(level, start, ret_level, ret_start, 0);		// element +0x168
	if (m_sound_index != -1) SetChannelVolume((short)PlaySoundIndex(-1, m_sound_index, false), g_sfx_volume);
	if (m_fade_type != -1)
	{
		g_editor->m_load_pending = 0;						// the fade loads the level when it ends
		StartFadeBG(m_fade_type, m_fade_time);				// 0x403c10
		g_view_restore = 1;									// 0x4f8430
		p->ev79(1);											// element +0x11c: animation paused
	}
}

void CLoadLevel::v258(void)	{ C3DSprite::v258(); }							// 0x4583f0

void CLoadLevel::v260(void)													// 0x4583b0
{
	strcpy(m_sprite_db, "icons.omt");
	C3DSprite::v260();
}

// --------------------------------------------------------------------------- factory

namespace neg {
CGameObject *New_CLoadLevel(void)	{ return new CLoadLevel; }				// factory 'LOAD' (0x6f4 bytes)
}

/*  CLoadLevel - see CLoadLevel.h. Reconstructed from Neutron.exe 0x457b10..0x4586e0. */
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
#include "../Fade.h"
#include "../JimmySlots.h"
#include "../CGameType.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- Jimmy's side

// C3DJimmy (a C3DPlayer; its members are named at their 2002 element offsets in Negatron's
// layout, TODO(E): Neutron.exe's): +0x955 the level file Jimmy is in and +0x724 his start point
// there, +0x88c / +0x8f0 the level and start point to return to, +0x954 and +0x9a5 two flags
// RETURN sets. The original writes them straight into the touching object.
static C3DPlayer *PlayerOf(CGameObject *jim)	{ return jim ? dynamic_cast<C3DPlayer *>(jim) : NULL; }

// C3DJimmy element slot 90 (+0x168, "PlayerLoadLevel"): go to `level` at start point `start`,
// remembering `ret_level` / `ret_start` to come back to. (Through copies: the strings may be
// Jimmy's own, which PlayerLoadLevel overwrites.)
static void JimmyLoadLevel(C3DPlayer *p, const char *level, const char *start, const char *ret_level, const char *ret_start)
{
	char l[100], s[100], rl[100], rs[100];
	strcpy(l, level); strcpy(s, start); strcpy(rl, ret_level); strcpy(rs, ret_start);
	p->n2_ev121(l, s, rl, rs, 0);							// TODO(E): Neutron.exe's slot 90 (4 strings)
}

// --------------------------------------------------------------------------- small helpers

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it && strcasecmp((*it)->m_tag, tag) == 0) return *it;
	return NULL;
}

// 0x4038c0: item `item` (0..0x1c) of menu page `page` on or off. 0x403910: off.
// TODO(F): the menu pages of Neutron.exe (0x4f8164, 0x28-byte items); through C2DInGameMenu::Page.
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 0x1d || item >= C2DInGameMenu::MENU_ITEMS) return;
	if (C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page)) p->items[item].type = state;
}
static void ClearItem(short page, short item)	{ SetItemState(page, item, 0); }

// --------------------------------------------------------------------------- construction

CLoadLevel::CLoadLevel()													// 0x457b10
{
	static char name[] = "C3DLOADLEVEL";		// SetName upper-cases in place
	SetName(name);
	Log("C3DLoadLevel()");
	CLoadLevel::InitObject();					// (direct call)
	C3DSprite::SetObjectID('LOAD');				// 0x462d80
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_level_name, "none");
	m_sprite_index = 2;
	m_sprite_size = 100;
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
	g_view_restore_x = 0.0f;					// the view C2DInGameMenu puts back after a fade
	g_view_restore_y = 0.0f;
	g_view_restore_z = 0.0f;
	g_view_restore_ax = 0.0f;
	g_view_restore_ay = 0.0f;
	g_view_restore = 0;
	// (debug log, empty in the shipped build)
}

CLoadLevel::~CLoadLevel()													// 0x457d60
{
}

// --------------------------------------------------------------------------- element-family slots

void CLoadLevel::ev53(void)									// 0x458370
{
	// (debug log "Calling LoadLevel with startpoint %s for %s")
	ev54();													// element +0xd8
	g_editor->LoadLevelNow(m_level_name);					// editor slot 64 (+0x100)
}

// 0x458400: the story steps some exits move on when used (task "SCENE").
void CLoadLevel::ev54(void)
{
	if (strcasecmp(m_tag, "LABTONEIGH") == 0 && GetTaskState("SCENE") == 60)	// 0x4f2818
	{
		SetItemState(1, 1, 1);								// 0x4038c0
		SetTaskState("SCENE", 65);							// 0x45f990
	}
	if (strcasecmp(m_tag, "YOKIAN2") == 0 && GetTaskState("SCENE") == 108) SetTaskState("SCENE", 110);	// 0x4f2810
	if (strcasecmp(m_tag, "YOKIANS2") == 0 && GetTaskState("SCENE") == 108) SetTaskState("SCENE", 110);	// 0x4f2804
	if (strcasecmp(m_tag, "PLANTSAVED") == 0 && GetTaskState("SCENE") == 470)							// 0x4f27f8
	{
		SetTaskState("SCENE", 480);
		g_4f7db8[25 * 2] += -1;								// 0x4061c0: pickup count
		if (g_4f7db8[25 * 2] < 1) ClearItem(2, 25);			// 0x4061b0, 0x403910
	}
}

// 0x458510: a trigger sends `who` (the camera object, normally Jimmy) to `game` at `start`.
void CLoadLevel::ev55(CGameObject *who, char *game, char *start)
{
	C3DPlayer *p = PlayerOf(who);
	if (g_camera)
	{
		if (p) p->f_a99 = 0;								// +0x7fc
		g_editor->f_7f8 = 1;
	}
	StopAllChannels();										// 0x47d850
	if (p)
	{
		strcpy(p->f_9c8, start);							// +0x724
		strcpy(p->f_a18, start);							// +0x774
	}
	// (debug log "Calling LoadLevel with startpoint %s for %s touched by %s")
	ev54();
	g_editor->LoadLevelNow(game);							// editor slot 64
	if (g_camera && g_camera->NameContains((char *)"C3DJIMMY"))
	{
		neg::JimmyElementSlot95(g_camera);					// element +0x17c
		if (C3DPlayer *c = PlayerOf(g_camera)) c->ev82();	// element +0x148
	}
	if (m_sound_index != -1) SetChannelVolume((short)PlaySoundIndex(-1, m_sound_index, false), g_sfx_volume);	// 0x47d390, 0x47dc80
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
	// (debug log)
}

// 0x457ec0: Jimmy walked into the exit. (The original's debug logs "MENTEMP Touched a CLoadLevel
// to %s at %s", "MENTEMP %s %s %s %s %s %s", ... are empty in the shipped build.)
void CLoadLevel::AddTouching(CGameObject *other)
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// vtable +0x18
	C3DPlayer *p = PlayerOf(other);
	if (!p) return;											// port: the original takes it as a C3DJimmy unchecked

	if (strcasecmp(m_required_task, "none") != 0)
	{
		const int state = GetTaskState(m_required_task);	// 0x45fea0
		if (state == -1) ErrorLog("ERROR: Task %s not found in in %s", m_required_task, m_tag);
		else
		{
			if (state < m_required_level) return;
			if (m_exact_level != -1 && m_exact_level != state) return;
		}
	}

	if (strcasecmp(m_level_name, "RETURN") == 0)
	{
		// "MENTEMP hit a RETURN loadpoint returning"
		neg::JimmyElementSlot94(other);						// element +0x178
		p->f_b88 = 1;										// +0x954
		p->f_bd9 = 1;										// +0x9a5
		JimmyLoadLevel(p, p->f_c25, p->f_b24, p->f_c25, p->f_b24);	// element +0x168
		if (m_sound_index != -1) SetChannelVolume((short)PlaySoundIndex(-1, m_sound_index, false), g_sfx_volume);
		other->v176(0.0f, 0.0f, 0.0f);						// vtable +0x2c4: stop
		return;
	}
	if (strcasecmp(m_level_name, "none") == 0) return;

	switch (g_4f0588)										// the item in hand: put these away first
	{
		case 0: case 1: case 4: case 7:
			neg::JimmyElementSlot94(other);					// element +0x178
			break;
	}
	ev54();													// element +0xd8
	JimmyLoadLevel(p, m_level_name, m_start_point, p->f_b89, p->f_9c8);
	if (m_sound_index != -1) SetChannelVolume((short)PlaySoundIndex(-1, m_sound_index, false), g_sfx_volume);
	if (m_fade_type != -1)
	{
		g_editor->m_load_pending = 0;						// the fade loads the level when it is out
		g_editor->m_load_countdown = -1;				// port: until PlayerLoadLevel is Neutron.exe's (load now, slot 64), not RequestLevel (TODO(E))
		neg::StartFade(m_fade_type, m_fade_time);			// 0x403c10
		g_view_restore = 1;									// 0x4f8430
		p->PauseAnim(1);									// element +0x11c (slot 71)
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
CGameObject *New_CLoadLevel(void)	{ return new CLoadLevel; }				// factory 'LOAD' (0x718 bytes)
}

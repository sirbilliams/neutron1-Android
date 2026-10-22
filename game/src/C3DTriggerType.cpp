/*  C3DTriggerType - see C3DTriggerType.h. */
#include "DatabaseCache.h"
#include "C3DTriggerType.h"
#include "Fade.h"
#include "CJimmyGame.h"
#include "C3DJimmy.h"
#include "objects/C3DAITrigger.h"
#include "objects/C3DCutSceneCamera.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "objects/C3DMultiCutSceneCamera.h"
#pragma GCC diagnostic pop
#include "levels/CLoadLevel.h"
#include "C3DPlayer.h"
#include "C3DJimmy.h"
#include "CJimmyGame.h"
#include "CGameType.h"
#include "CEditor.h"
#include "C2DInGameMenu.h"
#include "InterfaceElement.h"
#include "Game.h"
#include "Todo.h"
#include "SaveGame.h"
#include <stdlib.h>
#include "gen/globals.h"
#include "OMediaViewPort.h"
#include "OMediaStringField.h"
#include "OMediaTrigo.h"
#include "level1/C3DJeep.h"
#include <math.h>
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// 0x46a910: the level's database cache (DatabaseCache.cpp).
using neg::GetDatabase;

// --------------------------------------------------------------------------- helpers for CallNextTrigger

// The next trigger's own entry points (Neutron.exe calls them through the element vtable of the
// next object, slot 54 +0xd8, or directly).
namespace neg { void MusicTriggerTrigger(CGameObject *o); }
static void MusicTriggerFire(CGameObject *next)		{ neg::MusicTriggerTrigger(next); }	// objects/C3DMusicTrigger.cpp (BG: element slot 54)
static void AITriggerFire(CGameObject *next, CGameObject *caller)
{
	static_cast<C3DAITrigger *>(next)->Trigger(caller);				// BG: the direct call 0x40c300(caller)
}
static void MultiCutSceneFire(CGameObject *next, CGameObject *caller)
{
	next->v23(caller);												// game slot 23 (+0x5c): TriggerCallback
}
static void CutSceneCameraFire(CGameObject *next)
{
	static_cast<C3DCutSceneCamera *>(next)->TriggerCamera();			// element +0xd8 (BG slot 54)
}
// CLoadLevel (a C3DSprite): element slot 54 (+0xd8), its story steps.
static void LoadLevelFire(CGameObject *next)		{ static_cast<CLoadLevel *>(next)->ev54(); }

// The player (Jimmy: a C3DPlayer) behind a JIM1 tag.
static C3DPlayer *PlayerOf(CGameObject *jim)		{ return jim ? dynamic_cast<C3DPlayer *>(jim) : NULL; }

// Game byte +0x73c = 0 (C3DPlayer f_7fc).
static void JimmyClearA99(CGameObject *jim)
{
	if (C3DPlayer *p = PlayerOf(jim)) p->f_7fc = 0;
}

// 0x447cf0 (its C3DLOADLEVEL case): the LoadLevel trigger hands its level over to Jimmy:
// PlayerLoadLevel (element slot 90) with the level, its start point and Jimmy's own return
// level and start point; with a FadeType the load waits for the fade (and Jimmy's morph
// animation is paused, element slot 71).
static void LoadLevelHandOver(CGameObject *loadlevel, CGameObject *jim)
{
	CLoadLevel *ll = static_cast<CLoadLevel *>(loadlevel);
	C3DPlayer *p = PlayerOf(jim);
	if (!p) return;										// port: a JIM1 that is no player (the original has no check)
	p->PlayerLoadLevel(ll->m_level_name, ll->m_start_point, p->f_88c, p->f_8f0);	// element +0x168 (slot 90)
	if (ll->m_fade_type != -1)
	{
		g_editor->m_load_pending = 0;					// the fade loads the level when it ends
		g_editor->m_load_countdown = -1;				// port: until PlayerLoadLevel is Neutron.exe's (load now, slot 64), not RequestLevel (TODO(E))
		g_view_restore = 1;								// 0x4f8430
		p->PauseAnim(1);								// element +0x11c
		// (debug log "MEBTEMP using LCP %d" with the SCENE task state)
		neg::StartFade(ll->m_fade_type, ll->m_fade_time);	// 0x403c10
	}
}

// 0x472980 (CGameObject slot 224, +0x380): a point relative to the object (struct return).
static float *ObjectRelativePoint(CGameObject *o, float *out, float a, float b, float c)
{
	if (!o) return NULL;								// port: the original calls through NULL
	const OMedia3DVector v = o->v224(a, b, c);
	out[0] = v.x; out[1] = v.y; out[2] = v.z; out[3] = v.w;
	return out;
}

// C3DJeep: the jeep can take the camera when it has a driver. TODO(BG): Neutron.exe reads the
// driver at the jeep's CGameObject +0x5dc (C3DJeep.c param_1[0x177]); f_a84 is C3DJeep's field
// until its port names that one.: the jeep can take the camera when it has a driver.
static int JeepField9c4(CGameObject *jeep)
{
	C3DJeep *j = jeep ? dynamic_cast<C3DJeep *>(jeep) : NULL;
	return j && j->f_a84;										// the driver (object 0xa84)
}

// --------------------------------------------------------------------------- construction

C3DTriggerType::C3DTriggerType()							// 0x4478c0
{
	SetName((char *)"C3DTRIGGERTYPE");
	m_toggle = -1;
	strcpy(m_toggle_object, "none");
	strcpy(m_next_trigger, "none");
	g_cutscene_skipped = 0;									// 0x4f83e4
	m_fade_type = -1;
	m_fade_time = 1.0f;
}

C3DTriggerType::~C3DTriggerType() {}						// 0x447a30

void C3DTriggerType::InitObject(void)						// 0x447c60
{
	C3DSprite::InitObject();
	RegisterProperty("Toggle",       &m_toggle,       PROP_LONG,  0);
	RegisterProperty("ToggleObject", m_toggle_object, PROP_ALPHA, 0);
	RegisterProperty("NextTrigger",  m_next_trigger,  PROP_ALPHA, 0);
	RegisterProperty("FadeType",     &m_fade_type,    PROP_LONG,  0);
	RegisterProperty("FadeTime",     &m_fade_time,    PROP_FLOAT, 0);
}

// --------------------------------------------------------------------------- v242

// 0x447a70: in the editor, holding Page Down while this trigger is selected puts the camera
// next to the NextTrigger object: an offset (20, 20, -100) turned by the camera's own angles
// (taken to degrees and back, as shipped).
void C3DTriggerType::v242(float t)
{
	CGameObject::v242(t);									// 0x4732a0 (empty; not C3DSpriteType's)
	if (!g_keys[0x22] || g_editor->m_sel_object != static_cast<CGameObject *>(this)) return;	// VK_NEXT (0x50985a)
	g_4f8182 = 1;
	if (strcasecmp(m_next_trigger, "none") == 0) return;
	CGameObject *next = FindObjectByTag(m_next_trigger);
	if (!next) return;

	const double to_deg = 360.0f / 16384.0f;				// 0x48e5b4 (x87: no rounding in between)
	const double to_angle = 16384.0f / 360.0f;				// 0x48e5e8
	const int ix = (int)((double)g_viewport->get_anglex() * to_deg * to_angle) & 0x3fff;
	const int iy = (int)((double)g_viewport->get_angley() * to_deg * to_angle) & 0x3fff;
	const int iz = (int)((double)g_viewport->get_anglez() * to_deg * to_angle) & 0x3fff;
	const float *sn = OMediaCosSinTable::global.get_sintab();	// OMediaCosSinTable +0
	const float *cs = OMediaCosSinTable::global.get_costab();	// OMediaCosSinTable +0x10004
	const float sx = sn[ix], cx = cs[ix], sy = sn[iy], cy = cs[iy], sz = sn[iz], cz = cs[iz];

	const float a = 20.0f * cz - -20.0f * sz;				// 0x495324 / 0x4bd7c4
	const float b = 20.0f * sz - 20.0f * cz;
	const float p = -100.0f * cx - a * sx;					// 0x49fac4
	const float ox = b * cy - p * sy;
	const float oz = p * cy + b * sy;
	const float oy = a * cx - 100.0f * sx;					// 0x48d96c

	float pos[4];
	next->v195(pos); g_viewport->setx(ox + pos[0]);			// CGameObject +0x30c, once per axis
	next->v195(pos); g_viewport->sety(oy + pos[1]);
	next->v195(pos); g_viewport->setz(oz + pos[2]);
}

// --------------------------------------------------------------------------- CallNextTrigger

// 0x447cf0 (element slot 53): fire the trigger called `name`: "none" does nothing, "DEFAULT"
// ends the scripted sequence (camera back to Jimmy or the vehicle he is in), anything else is an
// object tag whose class decides what firing means.
void C3DTriggerType::CallNextTrigger(const char *name)
{
	Log("CallNextTrigger %s", name);
	if (strcasecmp(name, "none") == 0)
	{
		Log("NextTrigger is none for %s", m_tag);
		return;
	}

	if (strcasecmp(name, "DEFAULT") != 0)
	{
		CGameObject *next = FindObjectByTag(name);
		static int trace = -1;									// port: NEG_TRIGGER_TRACE logs the chain
		if (trace < 0) trace = getenv("NEG_TRIGGER_TRACE") != NULL;
		if (trace) SDL_Log("[trigger] %s -> %s (%s) SCENE=%d", m_tag, name,
			next ? next->m_name : "?", GetTaskState("SCENE"));
		if (!next)
		{
			ErrorLog("ERROR: No NextTrigger %s found", name);
			return;
		}
		// (each case also writes a debug log "Calling <type> %s, NextTrigger=%s")
		if (next->NameContains((char *)"C3DMUSICTRIGGER"))		{ MusicTriggerFire(next); return; }
		if (next->NameContains((char *)"C3DAITRIGGER"))			{ AITriggerFire(next, this); return; }
		if (next->NameContains((char *)"C3DCUTSCENECAMERA"))		{ CutSceneCameraFire(next); return; }
		if (next->NameContains((char *)"C3DMULTICUTSCENE"))		{ MultiCutSceneFire(next, this); return; }
		if (next->NameContains((char *)"C3DLOADLEVEL"))
		{
			CGameObject *jim = FindObjectByTag("JIM1");
			if (!jim)
			{
				ErrorLog("ERROR: Unable to find JIM1 in level");
				return;
			}
			Log("Loading LoadLevel trigger %s for %s", name, m_tag);
			JimmyClearA99(jim);
			g_editor->f_7f8 = 1;
			g_gametype->SetCamera(jim);
			LoadLevelFire(next);
			LoadLevelHandOver(next, jim);
			g_active_sound_db = GetDatabase("SoundEffects.omt");
			if (g_active_sound_db) g_4f8434 = true;
			return;
		}
		ErrorLog("ERROR: Invalid type for NextTrigger %s", name);
		return;
	}

	// "DEFAULT"
	Log("Reseting to default");
	Log("\tReseting to DEFAULT (Good job ;)");
	CGameObject *jim = FindObjectByTag("JIM1");
	if (g_camera == jim)
	{
		g_active_sound_db = GetDatabase("SoundEffects.omt");
		if (g_active_sound_db) g_4f8434 = true;
		CLocalGameObject *l = static_cast<CLocalGameObject *>(jim);	// sic: NULL without a JIM1
		if (l && l->f_4ab) l->f_4ab = 0;					// port: the original has no NULL test
		g_cutscene_skipped = 0;								// 0x4f83e4
		return;
	}

	CGameObject *o = jim;
	CGameObject *jim2 = NULL;								// the original's iVar6
	if (jim)
	{
		if (g_menu->IsCurrentGadget(4))						// 0x4f0588
		{
			o = FindObjectByTag("C3DJEEP");
			if (o)
			{
				if (JeepField9c4(o)) g_gametype->SetCamera(o);
				else o = FindObjectByTag("JIM1");
			}
		}
		else if (g_menu->IsCurrentGadget(1))
		{
			o = FindObjectByTag("C3DROCKETSHIP");
			if (o) g_gametype->SetCamera(o);
		}
		g_gametype->SetCamera(o);

		float buf[4];
		const float *p = ObjectRelativePoint(o, buf, 0.0f, 500.0f, -500.0f);	// +0x380
		if (p)
		{
			const float dx = g_viewport->getx() - p[0];
			const float dy = g_viewport->gety() - p[1];
			const float dz = g_viewport->getz() - p[2];
			if (sqrtf(dx * dx + dz * dz + dy * dy) > 1000.0f)	// 0x48e5ac
				g_viewport->place(p[0], p[1], p[2]);
		}
		g_4f8182 = 1;
		jim2 = FindObjectByTag("JIM1");
		C3DJimmy *j = jim2 ? dynamic_cast<C3DJimmy *>(jim2) : NULL;
		if (j && strcasecmp(j->f_a4c, "none") != 0)			// the start point's music database
		{
			g_696004 = GetDatabase(j->f_a4c);
			if (g_696004) j->f_a38 = PlayMusic(g_696004, -1, 0, true);	// 0x47d710
		}
	}
	g_music_volume = g_695f80;								// 0x4f6b34 = 0x695f80
	g_active_sound_db = GetDatabase("SoundEffects.omt");
	if (g_active_sound_db) g_4f8434 = true;
	CLocalGameObject *l = static_cast<CLocalGameObject *>(jim2);	// sic: NULL without a JIM1
	if (l && l->f_4ab) l->f_4ab = 0;						// port: the original has no NULL test
	g_cutscene_skipped = 0;									// 0x4f83e4
	if (GetTaskState("SCENE") == 0x1e && jim2)
		if (C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim2)) j->ToggleMainMenu();	// element slot 124 (+0x1f0)
}

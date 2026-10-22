/*  C3DCutSceneCamera - see C3DCutSceneCamera.h. Reconstructed from Neutron.exe 0x415a00..0x416ab0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DCutSceneCamera.h"
#include "../CGameType.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../C3DAI.h"
#include "../C3DAnimated.h"
#include "../CJimmyGame.h"
#include "../DatabaseCache.h"
#include "../InterfaceElement.h"
#include "../Sound.h"
#include "../Globals.h"
#include "../Todo.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMediaViewPort.h"
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>
#include <string.h>
#include <strings.h>
#include <math.h>

using namespace neg;

// --------------------------------------------------------------------------- small helpers

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it && strcasecmp((*it)->m_tag, tag) == 0) return *it;
	return NULL;
}

// 0x42ae30: set (or clear) the cut-scene freeze flag of every object (BG +0x49f; the port's
// CLocalGameObject f_4ab, as C3DAI and C3DTriggerType read it).
static void FreezeAllObjects(bool freeze)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it) static_cast<CLocalGameObject *>(*it)->f_4ab = freeze;
}

// C3DAnimated element slot 56 (+0xe0) of an animated target: play animation `name`.
static void TargetSetAnimation(CGameObject *target, const char *name, bool loop)
{
	C3DAnimated *a = target ? dynamic_cast<C3DAnimated *>(target) : NULL;	// port: only a C3DAnimated has the slot
	if (a) a->ev58((char *)name, loop);
}

// C3DAI element +0x968: its own animations are off while a cut scene animates it.
static void AISetAnimationsDisabled(CGameObject *ai, int off)
{
	C3DAI *a = ai ? dynamic_cast<C3DAI *>(ai) : NULL;
	if (a) a->m_anims_disabled = off;
}

static bool Trace(void) { static int t = -1; if (t < 0) t = getenv("NEG_TRIGGER_TRACE") != NULL; return t != 0; }	// port: NEG_TRIGGER_TRACE

// --------------------------------------------------------------------------- construction

C3DCutSceneCamera::C3DCutSceneCamera()						// 0x415a00
{
	SetName((char *)"C3DCUTSCENECAMERA");
	Log("C3DCutSceneCamera()");
	C3DCutSceneCamera::InitObject();						// 0x415d70 (called directly)
	SetObjectID('3CAM');
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_camera_target, "none");
	strcpy(m_next_trigger, "none");
	strcpy(m_sound_db, "none");
	strcpy(m_face_object, "none");
	strcpy(m_target_act_anim, "none");
	strcpy(m_target_deact_anim, "none");
	f_720[0] = 0;
	m_sound_index = -1;
	m_sprite_index = 10;
	m_sprite_size = 50;
	m_channel = -1;
	m_targ_offset[0] = 0;
	m_targ_offset[1] = 100.0f;
	m_targ_offset[2] = 200.0f;
	m_targ_offset[3] = 0;
	m_view_from_camera = 1;
	m_active = 0;
	v32(0);
	m_look_voffset = 0;
	v67(150.0f);
	v52(0);
	v46(0);
	v232();
	m_target = NULL;
	f_008 = 1;												// element +0xd0
	f_9a0 = 0;
	m_camera_type = 0;
	m_zoom_speed = 15.0f;
	m_loop_act_anim = -1;
	m_max_dist = 600.0f;
	m_min_dist = 200.0f;
	m_initial_dist = 300.0f;
	m_look_voffset = 100.0f;
	m_deactivate_inv = 1;
	m_time = 0;
}

C3DCutSceneCamera::~C3DCutSceneCamera() {}					// 0x415d30

namespace neg {
CGameObject *New_C3DCutSceneCamera(void)	{ return new C3DCutSceneCamera; }	// factory '3CAM' (0x9cc bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DCutSceneCamera::InitObject(void)					// 0x415d70
{
	C3DTriggerType::InitObject();							// 0x447c60
	RegisterProperty("CameraTarget",     m_camera_target,     PROP_ALPHA, 0);
	RegisterProperty("SoundDatabase",    m_sound_db,          PROP_ALPHA, 0);
	RegisterProperty("SoundIndex",       &m_sound_index,      PROP_LONG,  0);
	RegisterProperty("FaceObject",       m_face_object,       PROP_ALPHA, 0);
	RegisterProperty("ViewFromCamera",   &m_view_from_camera, PROP_LONG,  0);
	RegisterProperty("TargOffsetX",      &m_targ_offset[0],   PROP_FLOAT, 0);
	RegisterProperty("TargOffsetY",      &m_targ_offset[1],   PROP_FLOAT, 0);
	RegisterProperty("TargOffsetZ",      &m_targ_offset[2],   PROP_FLOAT, 0);
	RegisterProperty("TargetActAnim",    m_target_act_anim,   PROP_ALPHA, 0);
	RegisterProperty("LoopActAnim",      &m_loop_act_anim,    PROP_LONG,  0);
	RegisterProperty("TargetDeactAnim",  m_target_deact_anim, PROP_ALPHA, 0);
	RegisterProperty("LookVoffset",      &m_look_voffset,     PROP_FLOAT, 0);
	RegisterProperty("CameraType",       &m_camera_type,      PROP_LONG,  0);
	RegisterProperty("ZoomSpeed",        &m_zoom_speed,       PROP_FLOAT, 0);
	RegisterProperty("MaxDist",          &m_max_dist,         PROP_FLOAT, 0);
	RegisterProperty("MinDist",          &m_min_dist,         PROP_FLOAT, 0);
	RegisterProperty("InitialDist",      &m_initial_dist,     PROP_FLOAT, 0);
	strcpy(m_player_controlled, "none");
	RegisterProperty("PlayerControlled", m_player_controlled, PROP_ALPHA, 0);
	RegisterProperty("DeactivateInv",    &m_deactivate_inv,   PROP_LONG,  0);
}

// 0x415f90: while the shot runs, move the camera; when the clip has ended, unfreeze the objects,
// put the target's animation back and fire NextTrigger; when the player skips (Escape, space or
// button 0, unless the menu took the press), stop the clip and fire it at once.
void C3DCutSceneCamera::Process(float dt)
{
	C3DSpriteType::Process(dt);
	if (!m_active) return;
	m_time += dt;
	if (!IsChannelPlaying(m_channel))						// 0x47d890
	{
		// (debug log "Sound not playing in Cutscenecamera tick")
		if (Trace()) SDL_Log("[trigger] 3CAM %s clip over after %.1f s", m_tag, m_time);
		FreezeAllObjects(false);							// 0x42ae30(0)
		StopChannel(m_channel, true);						// 0x47d7a0
		if (m_target && m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_deact_anim, "none") != 0)
		{
			// (debug log "Setting %s to animation %s for %s")
			if (m_target->NameContains((char *)"C3DAI")) AISetAnimationsDisabled(m_target, 0);
			TargetSetAnimation(m_target, m_target_deact_anim, true);
		}
		m_active = 0;
		if (strcasecmp(m_next_trigger, "none") == 0) return;
		CloseDatabase(m_sound_db);							// 0x46aef0
		CallNextTrigger(m_next_trigger, 0);					// element +0xd4
		return;
	}

	if ((!g_keys[0x1b] && !g_keys[0x20] && !g_joy_button0 && !g_cutscene_skipped) || g_menu_input_lock)	// 0x509853, 0x509858, 0x509ac4
	{
		UpdateView(m_time);
		return;
	}

	// skipped
	CGameObject *jim = FindObjectByTag("JIM1");
	if (C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL) j->f_1e00 = 1;	// the key does not reach his gadgets
	g_cutscene_skipped = 1;									// 0x4f83e4
	StopChannel(m_channel, true);
	m_active = 0;
	CloseDatabase(m_sound_db);
	CallNextTrigger(m_next_trigger, 0);						// (also for "none": CallNextTrigger ignores it)
	if (m_target && m_target->NameContains((char *)"C3DAI")) AISetAnimationsDisabled(m_target, 0);
}

// --------------------------------------------------------------------------- element family

// 0x4165a0: fire the shot (unless one runs): Jimmy puts his gadget away, the camera goes to
// PlayerControlled, ToggleObject is told, the objects freeze, the level music stops (the music
// at a third of the effects volume), SoundDatabase replaces the effects and the clip starts with
// TargetActAnim on the target (looping unless LoopActAnim is 0).
void C3DCutSceneCamera::TriggerCamera(void)
{
	if (Trace())
	{
		float a[4], b[4];
		v195(a); v88(b);
		SDL_Log("[trigger] 3CAM %s fired (active %d, target %s, sound %d) at %.0f %.0f %.0f / %.0f %.0f %.0f", m_tag, m_active,
			m_camera_target, m_sound_index, a[0], a[1], a[2], b[0], b[1], b[2]);
	}
	if (m_active) return;
	if (m_deactivate_inv)
	{
		CGameObject *jim = FindObjectByTag("JIM1");
		if (C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL) j->DeactivateGadget();	// element slot 94
	}
	if (strcasecmp(m_player_controlled, "none") != 0)
	{
		if (strcasecmp(m_player_controlled, "NULL") == 0)
		{
			if (g_camera)
			{
				// (debug log "There is a pco %s")
				static_cast<CLocalGameObject *>(g_camera)->v275();	// +0x438
				g_camera->v261();							// +0x410
				float a[4];
				g_camera->v201(a);							// +0x328: only the heading is kept
				g_camera->f_274 = 0;
				g_camera->f_278 = a[1];
				g_camera->f_27c = 0;
				g_camera->f_280 = a[3];
				g_camera->v202(0, a[1], 0, a[3]);			// +0x32c
			}
			g_gametype->SetCamera(NULL);					// +0x118
		}
		else
		{
			CGameObject *o = FindObjectByTag(m_player_controlled);
			if (o) g_gametype->SetCamera(o);
		}
	}
	if (strcasecmp(m_toggle_object, "none") != 0)
	{
		CGameObject *t = FindObjectByTag(m_toggle_object);
		if (t) static_cast<CLocalGameObject *>(t)->v268(m_toggle);	// +0x428
	}
	if (strcasecmp(m_camera_target, "none") == 0) return;
	FreezeAllObjects(true);									// 0x42ae30(1)
	m_target = FindObjectByTag(m_camera_target);
	if (!m_target) return;

	CGameObject *jim = FindObjectByTag("JIM1");
	C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL;
	if (g_696004)											// the level music
	{
		if (j)												// port: the original does not check
		{
			StopChannel(j->f_a38, false);
			if (strcasecmp(j->f_a4c, "none") != 0) CloseDatabase(j->f_a4c);
		}
		g_696004 = NULL;
	}
	g_music_volume = (int)((float)g_sfx_volume * 0.33f);	// 0x498148
	if (g_active_sound_db)
	{
		CloseDatabase("SoundEffects.omt");
		g_active_sound_db = NULL;
		g_sfx_db_loaded = 0;								// 0x4f8434
	}
	g_active_sound_db = GetDatabase(m_sound_db);
	if (!g_active_sound_db)
	{
		ErrorLog("ERROR: Invalid SoundDatabase %s for %s", m_sound_db, m_tag);
		return;
	}
	m_channel = PlaySoundIndex(-1, m_sound_index, false);	// 0x47d390
	SetChannelVolume((short)m_channel, (short)g_sfx_volume);	// 0x47dc80
	if (m_channel == -1)
	{
		ErrorLog("ERROR: Invalid SoundIndex %d for %s", m_sound_index, m_tag);
		return;
	}
	m_active = 1;
	m_time = 0;
	if (m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_act_anim, "none") != 0)
	{
		// (debug log "Setting %s to animation %s for %s", twice)
		TargetSetAnimation(m_target, m_target_act_anim, m_loop_act_anim != 0);
		if (m_target->NameContains((char *)"C3DAI"))
		{
			// (debug log "Disabling animations for %s", twice)
			AISetAnimationsDisabled(m_target, 1);
		}
	}
}

// 0x431670 (shared with C3DMultiCutSceneCamera): put the view at `pos` looking at `look`
// (no roll); also remembered as the view to restore after a fade.
void C3DCutSceneCamera::SetView(OMedia3DVector pos, OMedia3DVector look)
{
	g_viewport->setx(pos.x);
	g_viewport->sety(pos.y);
	g_viewport->setz(pos.z);
	g_view_restore_x = pos.x;
	g_view_restore_y = pos.y;
	g_view_restore_z = pos.z;
	memcpy(&g_view_restore_w, &pos.w, 4);
	OMedia3DVector dir;
	dir.x = look.x - pos.x;
	dir.y = look.y - pos.y;
	dir.z = look.z - pos.z;
	dir.w = 0;
	omt_Angle ax, ay;
	dir.angles(ax, ay);
	g_viewport->set_anglex(ax);
	g_viewport->set_angley(ay);
	g_viewport->set_anglez(0);
	g_view_restore_ax = (float)ax;
	g_view_restore_ay = (float)ay;
}

// (inline in 0x415f90) where the camera is at time t of the shot.
//   CameraType 2: at the sprite, looking at the target.
//   else, ViewFromCamera 0: at the target's local point (TargOffsetX, TargOffsetY, d), d zooming
//     from InitialDist by ZoomSpeed per unit of time, clamped to MinDist..MaxDist (and kept in
//     TargOffsetZ), looking at the target raised by LookVoffset.
//   else: looking at the target's local point TargOffset, from the sprite's side of it, at
//     distance InitialDist - t * ZoomSpeed rescaled into MinDist..MaxDist.
void C3DCutSceneCamera::UpdateView(float t)
{
	if (m_camera_type == 2)
	{
		OMedia3DVector me, target;
		v195(me.xyzw());									// +0x310
		m_target->v195(target.xyzw());
		SetView(me, target);								// element +0xdc
		return;
	}

	if (m_view_from_camera == 0)
	{
		m_targ_offset[2] = m_initial_dist - t * m_zoom_speed;
		if (!(m_targ_offset[2] > m_min_dist)) m_targ_offset[2] = m_min_dist;
		if (!(m_targ_offset[2] < m_max_dist)) m_targ_offset[2] = m_max_dist;
		OMedia3DVector p = m_target->v224(m_targ_offset[0], m_targ_offset[1], m_targ_offset[2]);	// +0x384
		OMedia3DVector eye;
		eye.x = p.x; eye.y = p.y; eye.z = p.z; eye.w = 1.0f;
		OMedia3DVector look;
		m_target->v195(look.xyzw());
		look.y += m_look_voffset;
		SetView(eye, look);
		return;
	}

	OMedia3DVector me;
	v195(me.xyzw());
	OMedia3DVector p = m_target->v224(m_targ_offset[0], m_targ_offset[1], m_targ_offset[2]);
	OMedia3DVector d;
	d.x = me.x - p.x; d.y = me.y - p.y; d.z = me.z - p.z; d.w = 0;
	// (x87: the z component of each rescale stays in extended precision until it is stored)
	float inv = 1.0f / d.quick_magnitude();
	d.x = inv * d.x; d.y = inv * d.y;
	const float s = m_initial_dist - t * m_zoom_speed;
	d.x = s * d.x; d.y = s * d.y; d.z = s * (inv * d.z);
	const float len = sqrtf(d.z * d.z + d.y * d.y + d.x * d.x);
	if (len > m_max_dist)
	{
		inv = 1.0f / d.quick_magnitude();
		d.x = inv * d.x; d.y = inv * d.y; const float z = inv * d.z;
		d.x = d.x * m_max_dist; d.y = d.y * m_max_dist; d.z = z * m_max_dist;
	}
	if (len < m_min_dist)									// (the length before the MaxDist rescale)
	{
		inv = 1.0f / d.quick_magnitude();
		d.x = inv * d.x; d.y = inv * d.y; const float z = inv * d.z;
		d.x = d.x * m_min_dist; d.y = d.y * m_min_dist; d.z = z * m_min_dist;
	}
	OMedia3DVector eye;
	eye.x = p.x + d.x; eye.y = p.y + d.y; eye.z = p.z + d.z; eye.w = me.w;
	SetView(eye, p);
}


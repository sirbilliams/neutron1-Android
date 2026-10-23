/*  C3DMultiCutSceneCamera - see C3DMultiCutSceneCamera.h. Reconstructed from Neutron.exe
    0x4308d0..0x431a10. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMultiCutSceneCamera.h"
#include "../CGameType.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../C3DPlayer.h"
#include "../C3DAI.h"
#include "../C3DAnimated.h"
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
#include "../SaveGame.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <string>

using namespace neg;

// --------------------------------------------------------------------------- small helpers

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it && strcasecmp((*it)->m_tag, tag) == 0) return *it;
	return NULL;
}


// C3DAnimated element slot 58 (+0xe8) of an animated target: play animation `name`.
static void TargetSetAnimation(CGameObject *target, const char *name, bool loop)
{
	C3DAnimated *a = target ? dynamic_cast<C3DAnimated *>(target) : NULL;	// port: only a C3DAnimated has the slot
	if (a) a->ev58((char *)name, loop);
}

// C3DAI element +0x6c8 = 9: the character is frozen (held by the cut scene).
static void AIFreeze(CGameObject *ai)
{
	C3DAI *a = ai ? dynamic_cast<C3DAI *>(ai) : NULL;		// port: a name with "C3DAI" that is not one is left alone
	if (a) a->m_state = 9;
}

// C3DAI element +0x968: its own animations are off while a cut scene animates it.
static void AISetAnimationsDisabled(CGameObject *ai, int off)
{
	C3DAI *a = ai ? dynamic_cast<C3DAI *>(ai) : NULL;
	if (a) a->m_anims_disabled = off;
}

// C3DAI element slot 90 (+0x168): the cut scene lets go of it.
static void AIRelease(CGameObject *ai)
{
	C3DAI *a = ai ? dynamic_cast<C3DAI *>(ai) : NULL;
	if (a) a->ev90();
}

// --------------------------------------------------------------------------- construction

C3DMultiCutSceneCamera::C3DMultiCutSceneCamera()			// 0x4308d0
{
	f_ea0 = 0;
	SetName((char *)"C3DMULTICUTSCENE");
	Log("C3DMultiCutSceneCamera()");
	C3DMultiCutSceneCamera::InitObject();					// (called directly, as in the original)
	SetObjectID('3MCA');
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_next_trigger, "none");
	strcpy(m_sound_db, "none");
	m_sprite_index = 7;
	m_sprite_size = 50;
	m_channel = -1;
	m_cam_offset[0] = 0;
	m_cam_offset[1] = 0;
	m_cam_offset[2] = 0;
	m_shot = 0;
	m_active = 0;
	v52(0);
	v46(0);
	v32(0);
	v232();
	m_time = 0;
	v67(150.0f);
	m_target = NULL;
	m_held_ai = NULL;
	m_lookat = 0;
	m_cur_camera_type = 0;
	f_008 = 1;												// element +0xd0
	m_deactivate_inv = 1;
}

C3DMultiCutSceneCamera::~C3DMultiCutSceneCamera() {}			// 0x430b00

// --------------------------------------------------------------------------- CGameObject-family slots

void C3DMultiCutSceneCamera::InitObject(void)				// 0x430b40
{
	C3DTriggerType::InitObject();							// 0x447c60
	strcpy(m_sound_db, "none");
	RegisterProperty("SoundDatabase", m_sound_db, PROP_ALPHA, 0);
	strcpy(m_target_deact_anim, "STOP");
	RegisterProperty("TargetDeactAnim", m_target_deact_anim, PROP_ALPHA, 0);
	char name[16];
	for (int i = 0; i < kShots; i++)
	{
		sprintf(name, "%s%d", "CameraTarget", i);
		strcpy(m_camera_target[i], "none");
		RegisterProperty(name, m_camera_target[i], PROP_ALPHA, 0);
		sprintf(name, "%s%d", "TargetAnim", i);
		strcpy(m_target_anim[i], "TALK");
		RegisterProperty(name, m_target_anim[i], PROP_ALPHA, 0);
		sprintf(name, "%s%d", "LookatVOffset", i);
		m_lookat_voffset[i] = 100.0f;
		RegisterProperty(name, &m_lookat_voffset[i], PROP_FLOAT, 0);
		sprintf(name, "%s%d", "SoundIndex", i);
		m_sound_index[i] = -1;
		RegisterProperty(name, &m_sound_index[i], PROP_LONG, 0);
		m_camera_type[i] = 0;
		sprintf(name, "%s%d", "CameraType", i);
		RegisterProperty(name, &m_camera_type[i], PROP_LONG, 0);
	}
	strcpy(m_player_controlled, "NULL");
	RegisterProperty("PlayerControlled", m_player_controlled, PROP_ALPHA, 0);
	RegisterProperty("DeactivateInv",    &m_deactivate_inv,   PROP_LONG,  0);
}

// 0x430da0: while a shot runs, move the camera. When its clip has ended the target gets
// TargetDeactAnim and the next shot starts; after the last one the scene ends. Escape, space or
// joystick button 0 (unless the menu took this press) skip: the clip stops and the scene ends.
void C3DMultiCutSceneCamera::Process(float dt)
{
	C3DSprite::Process(dt);
	if (!m_active) return;
	m_time += dt;
	if (!IsChannelPlaying(m_channel))						// 0x47d890
	{
		StopChannel(m_channel, true);						// 0x47d7a0
		if (m_target && m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_deact_anim, "none") != 0)
		{
			// (debug log "Setting %s to animation %s for %s")
			TargetSetAnimation(m_target, m_target_deact_anim, true);
		}
		m_active = 0;
		if (!NextShot())									// element +0xe0
		{
			EndCutScene();									// element +0xe4
			m_shot = 0;
			return;
		}
		m_active = 1;
		m_lookat = m_lookat_voffset[m_shot];
		m_cur_camera_type = m_camera_type[m_shot];
		m_shot++;
		return;
	}

	if ((g_keys[0x1b] || g_keys[0x20] || g_joy_button0 || g_cutscene_skipped) && !g_menu_input_lock)	// 0x509853, 0x509858, 0x509ac4
	{
		CGameObject *jim = FindObjectByTag("JIM1");
		C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL;
		if (j) j->f_1e00 = 1;								// the key does not reach his gadgets
		g_cutscene_skipped = 1;								// 0x4f83e4
		StopChannel(m_channel, true);
		if (m_target)
		{
			if (m_target->NameContains((char *)"C3DAI")) AISetAnimationsDisabled(m_target, 0);
			if (m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_deact_anim, "none") != 0)
				TargetSetAnimation(m_target, m_target_deact_anim, true);
		}
		m_active = 0;
		EndCutScene();										// element +0xe4
		m_shot = 0;
		return;
	}

	// the camera point in the target's frame: 0 fixed in front, 1..4 zooming in (no nearer than 100)
	switch (m_cur_camera_type)
	{
		default:
			m_cam_offset[0] = 0;
			m_cam_offset[1] = 40.0f;
			m_cam_offset[2] = 200.0f;
			break;
		case 1:
			m_cam_offset[0] = 0;
			m_cam_offset[1] = 140.0f;
			m_cam_offset[2] = 300.0f - m_time * 15.0f;		// 0x48d930, 0x49d5e0
			break;
		case 2:
			m_cam_offset[0] = 0;
			m_cam_offset[1] = 240.0f;
			m_cam_offset[2] = 500.0f - m_time * 35.0f;		// 0x48e5bc, 0x4a94c0
			break;
		case 3:
			m_cam_offset[0] = 200.0f;
			m_cam_offset[1] = 240.0f;
			m_cam_offset[2] = 700.0f - m_time * 55.0f;		// 0x4a94b8, 0x4a94bc
			break;
		case 4:
			m_cam_offset[0] = -200.0f;
			m_cam_offset[1] = 190.0f;
			m_cam_offset[2] = 700.0f - m_time * 55.0f;
			break;
	}
	if (m_cur_camera_type >= 1 && m_cur_camera_type <= 4 && m_cam_offset[2] < 100.0f)	// 0x48d96c
		m_cam_offset[2] = 100.0f;

	if (!m_target) return;
	OMedia3DVector p = m_target->v224(m_cam_offset[0], m_cam_offset[1], m_cam_offset[2]);	// +0x384
	OMedia3DVector eye;
	eye.x = p.x; eye.y = p.y; eye.z = p.z; eye.w = 1.0f;
	OMedia3DVector pos;
	m_target->v195(pos.xyzw());								// +0x310
	OMedia3DVector look;
	look.x = pos.x;
	look.y = (m_lookat - 60.0f) + pos.y;					// 0x49d5b4
	look.z = pos.z;
	look.w = pos.w;
	SetView(eye, look);										// element +0xdc
}

// --------------------------------------------------------------------------- element-family slots

// 0x431280: fire the scene: Jimmy puts his gadget away, shot 0's target is found and held (a
// C3DAI, or CameraTarget1's), ToggleObject is told, the camera is taken from the player, the
// sound database becomes SoundDatabase (the music at a third of the effects volume) and shot 0
// starts.
void C3DMultiCutSceneCamera::TriggerCamera(void)
{
	// (debug log "TC %s")
	if (getenv("NEG_TRIGGER_TRACE")) SDL_Log("[trigger] TC %s (SCENE=%d)", m_tag, GetTaskState("SCENE"));	// port
	m_shot = 0;
	if (m_deactivate_inv)
	{
		CGameObject *jim = FindObjectByTag("JIM1");
		C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL;
		if (j) j->DeactivateGadget();						// element slot 94
	}
	// (debug log "CALLING MultiCutSceneCamera %s")
	if (strcasecmp(m_camera_target[m_shot], "none") == 0) return;
	m_target = FindObjectByTag(m_camera_target[m_shot]);
	m_held_ai = NULL;
	if (!m_target)
		ErrorLog("ERROR: Cannot find CameraTargetObj %s in %s", m_camera_target[m_shot], m_tag);
	else if (m_target->NameContains((char *)"C3DAI"))
	{
		AIFreeze(m_target);
		m_held_ai = m_target;
		if (m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_anim[m_shot], "none") != 0)
			TargetSetAnimation(m_target, m_target_anim[m_shot], true);
	}
	// QUIRK: the AI to release at the end is looked up from the second shot's target as well.
	CGameObject *o = FindObjectByTag(m_camera_target[m_shot + 1]);
	if (o && o->NameContains((char *)"C3DAI")) m_held_ai = o;
	if (strcasecmp(m_toggle_object, "none") != 0)
	{
		CGameObject *t = FindObjectByTag(m_toggle_object);
		if (t) static_cast<CLocalGameObject *>(t)->v268(m_toggle);	// +0x428
	}
	if (!m_target) return;
	m_target->v261();										// +0x410
	if (m_target->NameContains((char *)"C3DAI"))
	{
		AIFreeze(m_target);
		m_held_ai = m_target;
	}
	if (g_camera)
	{
		g_camera->v261();									// +0x410
		float a[4];
		g_camera->v201(a);									// +0x328: current angles
		g_camera->v202(0, a[1], 0, a[3]);					// +0x32c: only the heading is kept
		g_camera->f_274 = 0;
		g_camera->f_278 = a[1];
		g_camera->f_27c = 0;
		g_camera->f_280 = a[3];
		static_cast<CLocalGameObject *>(g_camera)->v275();	// +0x438
		g_gametype->SetCamera(NULL);						// +0x118
	}
	if (g_active_sound_db)
	{
		CloseDatabase("SoundEffects.omt");
		g_active_sound_db = NULL;
		g_sfx_db_loaded = 0;								// 0x4f8434
	}
	g_music_volume = (int)((float)g_sfx_volume * 0.33f);	// 0x498148
	g_active_sound_db = GetDatabase(m_sound_db);
	if (!g_active_sound_db)
	{
		ErrorLog("ERROR: Invalid SoundDatabase %s for %s", m_sound_db, m_tag);
		return;
	}
	if (!NextShot()) return;								// element +0xe0
	// (debug log "Camera is active")
	m_active = 1;
	m_lookat = m_lookat_voffset[m_shot];
	m_cur_camera_type = m_camera_type[m_shot];
	m_shot++;
}

void C3DMultiCutSceneCamera::TriggerCallback(CGameObject *caller, unsigned char flag)
{
	(void)caller; (void)flag;
	TriggerCamera();
}

// 0x431670: put the view at `pos` looking at `look` (no roll); also remembered as the view to
// restore after a fade.
void C3DMultiCutSceneCamera::SetView(OMedia3DVector pos, OMedia3DVector look)
{
	g_viewport->setx(pos.x);								// +0x44
	g_viewport->sety(pos.y);
	g_viewport->setz(pos.z);
	g_view_restore_x = pos.x;								// 0x4f8418..
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
	g_viewport->set_anglex(ax);								// +0x50
	g_viewport->set_angley(ay);								// +0x52
	g_viewport->set_anglez(0);								// +0x54
	g_view_restore_ax = (float)ax;							// 0x4f8428
	g_view_restore_ay = (float)ay;							// 0x4f842c
}

// 0x431750 ("GetNextSoundChannel"): start shot m_shot: find its target (an AI is held), play its
// clip SoundIndex and its TargetAnim; the shot time restarts. No shot, no clip or no sound
// database ends the scene.
bool C3DMultiCutSceneCamera::NextShot(void)
{
	if (m_shot >= kShots) return false;						// "GNSC Exit 1"
	if (m_sound_index[m_shot] == -1) return false;			// "GNSC Exit 2"
	// (debug log "GNSC %s  %d  %s")
	m_target = FindObjectByTag(m_camera_target[m_shot]);
	if (!m_target) return false;							// "GNSC Exit 5"
	if (m_target->NameContains((char *)"C3DAI"))
	{
		AIFreeze(m_target);
		m_held_ai = m_target;
	}
	if (!g_active_sound_db) return false;					// "GNSC Exit 4"
	m_channel = PlaySoundIndex(-1, m_sound_index[m_shot], false);	// 0x47d390
	SetChannelVolume((short)m_channel, (short)g_sfx_volume);			// 0x47dc80
	if (m_channel == -1)
	{
		ErrorLog("ERROR: Invalid channel of %d for %s", m_sound_index[m_shot], m_tag);
		return false;
	}
	if (m_target->NameContains((char *)"C3DANIMATED") && strcasecmp(m_target_anim[m_shot], "none") != 0)
	{
		// (debug log "Setting %s to animation %s for %s")
		TargetSetAnimation(m_target, m_target_anim[m_shot], true);
	}
	m_time = 0;
	if (getenv("NEG_TRIGGER_TRACE")) SDL_Log("[trigger] 3MCA %s shot %d (target %s, sound %d)", m_tag, m_shot, m_camera_target[m_shot], m_sound_index[m_shot]);	// port
	// (debug log "GNSC Got sound channel %d")
	return true;
}

// 0x4311f0: the scene is over: the camera goes to PlayerControlled (unless "NULL"), the held AI
// is let go, SoundDatabase is closed and NextTrigger fires.
void C3DMultiCutSceneCamera::EndCutScene(void)
{
	if (strcasecmp(m_player_controlled, "NULL") != 0)
	{
		CGameObject *o = FindObjectByTag(m_player_controlled);
		if (o) g_gametype->SetCamera(o);					// +0x118
	}
	if (m_held_ai && m_held_ai->NameContains((char *)"C3DAI")) AIRelease(m_held_ai);	// element +0x168
	CloseDatabase(m_sound_db);								// 0x46aef0
	CallNextTrigger(m_next_trigger);						// element +0xd4
}

namespace neg {
CGameObject *New_C3DMultiCutSceneCamera(void)	{ return new C3DMultiCutSceneCamera; }	// factory '3MCA' (0xec4 bytes)
}

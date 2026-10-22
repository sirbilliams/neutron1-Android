/*  C3DTrigger - see C3DTrigger.h. Reconstructed from Neutron.exe 0x446e50..0x4478b0. */
#include "C3DTrigger.h"
#include "../CGameType.h"
#include "../C3DAnimated.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include <list>
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

C3DTrigger::C3DTrigger()									// 0x446e50
{
	SetName((char *)"C3DTRIGGER");
	Log("C3DTrigger()");
	C3DTrigger::InitObject();								// 0x447220 (called directly)
	SetObjectID('3TRI');									// element slot 48
	m_sprite_index = 0;
	m_sprite_size = 50;
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_activate_object[0], "none");
	m_activate_state[0] = 0;
	m_activate_state[1] = -1;
	m_activate_state[2] = -1;
	m_activate_state[3] = -1;
	m_activate_state[4] = -1;
	strcpy(m_activate_object[1], "none");
	strcpy(m_activate_object[2], "none");
	strcpy(m_activate_object[3], "none");
	strcpy(m_activate_object[4], "none");
	strcpy(m_activate_by, "none");
	strcpy(m_activate_anim, "none");
	strcpy(m_sound_database, "none");
	m_sound_index = -1;
	strcpy(m_next_trigger, "none");
	strcpy(m_player_controlled, "none");
	v67(250.0f);											// radius
	v46(0);
	v52(1);
	v32(1);
	m_times_to_trigger = -1;
	m_times_triggered = 0;
	m_channel = -1;
	f_008 = 1;												// (element +0xd0)
	C3DSprite::InitPhysics();								// 0x464210
	m_touch_activated = 0;
	v34(1);
}

C3DTrigger::~C3DTrigger()									// 0x4471e0
{
}

namespace neg {
CGameObject *New_C3DTrigger(void) { return new C3DTrigger; }			// '3TRI' (0xa00 bytes)
}

void C3DTrigger::InitObject(void)							// 0x447220
{
	C3DSprite::InitObject();								// 0x464130
	RegisterProperty("ActivateState0", &m_activate_state[0], PROP_LONG, 0);
	RegisterProperty("ActivateObject0", m_activate_object[0], PROP_ALPHA, 0);
	RegisterProperty("ActivateState1", &m_activate_state[1], PROP_LONG, 0);
	RegisterProperty("ActivateObject1", m_activate_object[1], PROP_ALPHA, 0);
	RegisterProperty("ActivateState2", &m_activate_state[2], PROP_LONG, 0);
	RegisterProperty("ActivateObject2", m_activate_object[2], PROP_ALPHA, 0);
	RegisterProperty("ActivateState3", &m_activate_state[3], PROP_LONG, 0);
	RegisterProperty("ActivateObject3", m_activate_object[3], PROP_ALPHA, 0);
	RegisterProperty("ActivateState4", &m_activate_state[4], PROP_LONG, 0);
	RegisterProperty("ActivateObject4", m_activate_object[4], PROP_ALPHA, 0);
	RegisterProperty("ActivateAnim", m_activate_anim, PROP_ALPHA, 0);
	RegisterProperty("ActivateBy", m_activate_by, PROP_ALPHA, 0);
	RegisterProperty("SoundDatabase", m_sound_database, PROP_ALPHA, 0);
	RegisterProperty("SoundIndex", &m_sound_index, PROP_LONG, 0);
	RegisterProperty("NextTrigger", m_next_trigger, PROP_ALPHA, 0);
	RegisterProperty("PlayerControlled", m_player_controlled, PROP_ALPHA, 0);
	RegisterProperty("TimesToTrigger", &m_times_to_trigger, PROP_LONG, 0);
	RegisterProperty("TouchActivated", &m_touch_activated, PROP_LONG, 0);
}

void C3DTrigger::AddTouching(CGameObject *other)			// 0x447400
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_touch_activated != 0)
	{
		if (!other->NameContains((char *)"C3DTRIGGER") && !other->NameContains((char *)"C3DCUTSCENECAMERA"))
			v23(other);										// +0x5c
	}
}

// 0x4476c0: the first ActivateObject<n> that exists and whose ActivateState<n> (not -1) is the
// state of this trigger's task as the object last read it (CLocalGameObject +0x494).
CGameObject *C3DTrigger::FindActiveObject(void)
{
	for (int i = 0; i < 5; i++)
	{
		CGameObject *o = FindObjectByTag(m_activate_object[i]);
		if (o && m_activate_state[i] != -1 && m_activate_state[i] == f_4a0) return o;
	}
	return NULL;
}

void C3DTrigger::v23(CGameObject *o)						// 0x447450
{
	if (!o) return;
	// (an empty debug log)
	if (strcasecmp(o->m_tag, m_activate_by) != 0 && !o->NameContains((char *)"C3DTRIGGER")
		&& !o->NameContains((char *)"C3DCUTSCENECAMERA")) return;
	// (an empty debug log)
	if (m_times_to_trigger != -1 && m_times_to_trigger <= m_times_triggered) return;
	m_times_triggered++;
	if (strcasecmp(m_player_controlled, "none") != 0)
	{
		if (strcasecmp(m_player_controlled, "NULL") == 0)
		{
			if (g_camera) g_camera->v261();					// +0x410
			g_gametype->SetCamera(NULL);					// +0x118
		}
		else g_gametype->SetCamera(FindObjectByTag(m_player_controlled));	// 0x474070
	}
	if (o->NameContains((char *)"C3DANIMATED") && strcasecmp(m_activate_anim, "none") != 0)
	{
		o->v261();											// +0x410
		C3DAnimated *a = dynamic_cast<C3DAnimated *>(o);	// port: the original calls the element's slot 56 of whatever is named C3DANIMATED
		if (a) a->ev58(m_activate_anim, 1);					// +0xe0
	}
	o->v23(this);											// the activator is called back
	if (strcasecmp(m_sound_database, "none") != 0)
	{
		g_active_sound_db = GetDatabase(m_sound_database);	// 0x46a910
		if (g_active_sound_db)
		{
			// ("Playing Sound Index %d": debug log)
			m_channel = PlayGameSoundIndex(-1, m_sound_index, false);	// 0x458980
			return;
		}
		// (debug logs "ERROR: opening Sound database %s")
	}
	CGameObject *next = FindActiveObject();
	if (!next) return;
	if (next->NameContains((char *)"C3DTRIGGER"))
	{
		C3DTrigger *t = dynamic_cast<C3DTrigger *>(next);	// port: the original calls 0x4476c0 on whatever is named C3DTRIGGER
		CGameObject *own = t ? t->FindActiveObject() : NULL;
		next->v23(own ? own : this);
	}
	else next->v23(this);
}

void C3DTrigger::Process(float dt)							// 0x447790
{
	C3DSprite::Process(dt);									// 0x463090
	if (m_channel == -1) return;
	if (IsChannelPlaying(m_channel)) return;				// 0x47d890
	StopGameSound(m_channel, true);							// 0x458a00
	m_channel = -1;
	CloseDatabase(m_sound_database);						// 0x46aef0
	g_active_sound_db = GetDatabase("SoundEffects.omt");	// 0x46a910
	if (g_active_sound_db) g_4f8434 = 1;
	CGameObject *next = FindObjectByTag(m_next_trigger);
	if (!next) return;
	CGameObject *arg = NULL;
	if (next->NameContains((char *)"C3DTRIGGER"))
	{
		C3DTrigger *t = dynamic_cast<C3DTrigger *>(next);
		if (t) arg = t->FindActiveObject();
	}
	if (!arg) arg = this;
	next->v23(arg);
}

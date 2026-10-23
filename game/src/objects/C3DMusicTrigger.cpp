/*  C3DMusicTrigger - see C3DMusicTrigger.h. Reconstructed from Neutron.exe 0x431a20..0x432160. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMusicTrigger.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
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

C3DMusicTrigger::C3DMusicTrigger()							// 0x431a20
{
	SetName((char *)"C3DMUSICTRIGGER");
	Log("C3DMusicTrigger()");
	C3DMusicTrigger::InitObject();							// 0x431c50 (called directly, as in the original)
	SetObjectID('3MUS');									// element slot 48
	strcpy(m_sprite_db, "icons.omt");
	strcpy(m_music_db, "none");
	m_sprite_index = 11;
	m_sprite_size = 50;
	m_music_index[0] = 0;
	m_music_index[1] = -1;
	m_music_index[2] = -1;
	m_music_index[3] = -1;
	m_music_index[4] = -1;
	v67(150.0f);
	C3DSprite::InitPhysics();								// 0x464210
	m_touch_activated = 1;
	v52(0);
	v46(0);
	v34(1);
	f_008 = 1;												// element +0xd0
	m_required_level = -1;									// port: the original leaves these three unset
	m_exact_level = -1;
	m_remove_level = -1;
}

C3DMusicTrigger::~C3DMusicTrigger() {}						// 0x431c10

namespace neg {
CGameObject *New_C3DMusicTrigger(void) { return new C3DMusicTrigger; }	// '3MUS' (0x74c bytes)

void MusicTriggerTrigger(CGameObject *o)
{
	C3DMusicTrigger *m = o ? dynamic_cast<C3DMusicTrigger *>(o) : NULL;	// (o - 0xc8)
	if (m) m->Trigger();
}
}

void C3DMusicTrigger::InitObject(void)						// 0x431c50
{
	C3DTriggerType::InitObject();							// 0x447c60
	RegisterProperty("MusicDatabase", m_music_db, PROP_ALPHA, 0);
	RegisterProperty("MusicIndex0", &m_music_index[0], PROP_LONG, 0);
	RegisterProperty("MusicIndex1", &m_music_index[1], PROP_LONG, 0);
	RegisterProperty("MusicIndex2", &m_music_index[2], PROP_LONG, 0);
	RegisterProperty("MusicIndex3", &m_music_index[3], PROP_LONG, 0);
	RegisterProperty("MusicIndex4", &m_music_index[4], PROP_LONG, 0);
	RegisterProperty("TouchActivated", &m_touch_activated, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	RegisterProperty("RequiredLevel", &m_required_level, PROP_LONG, 0);
	RegisterProperty("ExactLevel", &m_exact_level, PROP_LONG, 0);
	RegisterProperty("RemoveLevel", &m_remove_level, PROP_LONG, 0);
}

void C3DMusicTrigger::AddTouching(CGameObject *other)		// 0x431dc0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other == g_camera) Trigger();						// element +0xd8
}

void C3DMusicTrigger::v260(void)							// 0x431d80
{
	C3DSprite::v260();										// 0x464810
	if (m_touch_activated == 0) v136(0);					// +0x220
	if (m_touch_activated == 1) v136(1);
}

// Not touch activated: ExactLevel, else RequiredLevel <= level < RemoveLevel (-1: no upper bound)
// fires it.
void C3DMusicTrigger::v267(int level)						// 0x432080
{
	CLocalGameObject::v267(level);							// 0x4588d0
	if (m_touch_activated != 0) return;
	if (m_exact_level == -1)
	{
		if (m_required_level == -1) return;
		if (level < m_required_level) return;
		if (m_remove_level != -1 && m_remove_level <= level) return;
	}
	else if (level != m_exact_level) return;
	Trigger();												// element +0xd8
}

// 0x431df0: Jimmy ("JIM1") keeps the music channel (+0xa38) and the database name (+0xa4c); the
// loaded database is g_696004.
void C3DMusicTrigger::Trigger(void)
{
	if (g_4f83e4) return;									// (a cut scene skipped)
	C3DJimmy *jim = dynamic_cast<C3DJimmy *>(FindObjectByTag("JIM1"));	// 0x474070 (the original does not test it)
	if (!jim) return;										// port: (it would write through a null pointer)
	if (g_696004)
	{
		StopChannel(jim->f_a38, false);						// 0x47d7a0
		if (strcasecmp(jim->f_a4c, "none") != 0)			// 0x487c20
		{
			CloseDatabase(jim->f_a4c);						// 0x46aef0
			jim->f_a38 = -1;
		}
		g_696004 = NULL;
	}
	if (strcasecmp(m_music_db, "none") == 0) return;
	strcpy(jim->f_a4c, m_music_db);
	g_696004 = GetDatabase(m_music_db);						// 0x46a910
	if (!g_696004) return;
	if (jim->f_a38 != -1) return;
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;	// (a draw that is not used)
	for (int i = 0; i < 5; i++)
		if (m_music_index[i] != -1)
		{
			jim->f_a38 = PlayMusic(g_696004, -1, m_music_index[i], true);	// 0x47d710
			return;
		}
}

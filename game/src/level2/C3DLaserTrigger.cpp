/*  C3DLaserTrigger - see C3DLaserTrigger.h. Reconstructed from Neutron.exe 0x42c050..0x42c6c0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DLaserTrigger.h"
#include "../CJimmyGame.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
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

// 0x44db30 through the game type's slot +0x164: the terrain database's colour.
static void SetTerrainTint(float r, float g, float b, float a)
{
	CJimmyGame *game = dynamic_cast<CJimmyGame *>(g_gametype);
	if (game) game->SetTerrainDBColor(r, g, b, a);
}

C3DLaserTrigger::C3DLaserTrigger()							// 0x42c050
{
	SetName((char *)"C3DLASERTRIGGER");
	Log("C3DLaserTrigger()");
	C3DLaserTrigger::InitObject();							// 0x42c2a0 (called directly)
	C3DObject::SetObjectID('3LAS');							// 0x462220
	v94(2);													// slot 95
	v46(0);
	v34(1);
	C3DObject::InitPhysics();								// 0x4623d0
	MakeShapeUnLit(get_shape());							// 0x477550
	m_item_active = 1;
	m_on = 1;
	f_648 = 0;												// animations off
	strcpy(m_next, "none");
	m_toggle = -1;
	m_flash = 0;
}

C3DLaserTrigger::~C3DLaserTrigger() {}						// 0x42c1f0

namespace neg {
CGameObject *New_C3DLaserTrigger(void) { return new C3DLaserTrigger; }	// '3LAS' (0x738 bytes)
}

void C3DLaserTrigger::InitObject(void)						// 0x42c2a0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ItemActive", &m_item_active, PROP_LONG, 0);
	RegisterProperty("Next", m_next, PROP_ALPHA, 0);
	RegisterProperty("Toggle", &m_toggle, PROP_LONG, 0);
	m_db = GetDatabase("objectslevel5a.omt");				// 0x46a910
	f_648 = 0;												// animations off
	set_shape(Load3DShape(m_db, 1));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	ev51();													// element +0xcc
}

void C3DLaserTrigger::ResetObject(void)						// 0x42c240
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	v34(1);
	v46(0);
	m_on = m_item_active;
	if (m_item_active == 0) hide(true);
	else force_show();										// (+0x70 is the hide count: shown again only if hidden)
}

void C3DLaserTrigger::AddTouching(CGameObject *other)		// 0x42c410
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_on == 0 || other != g_camera) return;
	float pos[4], opos[4], rot[4];
	v195(pos);												// +0x310: position
	other->v195(opos);
	// (debug log "Touched by %s at %f, beam at %f")
	v201(rot);												// +0x328: rotation
	if (rot[0] == 0.0f)
	{
		v201(rot);
		if (rot[2] == 0.0f)
		{
			other->v195(opos);
			v195(pos);
			if (pos[1] < opos[1]) return;					// a flat beam: jumped over
		}
	}
	hide(true);
	if (m_flash == 0.0f)
	{
		m_flash = 10.0f;
		PlayGameSoundIndex(-1, 0x38, false);				// 0x458980
		SetTerrainTint(1.0f, 0.6f, 0.6f, 1.0f);				// game type +0x164 (a unused)
	}
	if (strcasecmp(m_next, "none") != 0)					// 0x487c20
	{
		CGameObject *next = FindObjectByTag(m_next);		// 0x474070
		if (CLocalGameObject *l = dynamic_cast<CLocalGameObject *>(next)) l->v268(m_toggle);	// +0x428: NextState
	}
}

void C3DLaserTrigger::RemoveTouching(CGameObject *other)	// 0x42c5b0
{
	CGameObject::RemoveTouching(other);						// 0x470b20
	if (m_on != 0) force_show();
}

void C3DLaserTrigger::Process(float dt)						// 0x42c370
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (m_flash > 0.0f)
	{
		m_flash = m_flash - dt;
		if (m_flash <= 0.0f)
		{
			m_flash = 0;
			SetTerrainTint(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
}

void C3DLaserTrigger::v268(int state)						// 0x42c5f0
{
	if (state == 0) m_on = 0;
	else if (state == 1) m_on = 1;
	else m_on = (m_on == 0);
	if (m_on == 0) hide(true);
	else force_show();
}

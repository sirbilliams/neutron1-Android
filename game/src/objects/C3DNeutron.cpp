/*  C3DNeutron - see C3DNeutron.h. Reconstructed from Neutron.exe 0x4329a0..0x433060. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DNeutron.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMediaCanvas.h"
#include "OMediaStreamOperators.h"
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// 0x42a920: the neutron energy (g_health) += d, kept in 0..100 (not in the VR levels 'VR01'..
// 'VR08'). Sound 80 when it runs out; sound 182 ("low neutron energy") once when it falls below
// 19 (g_4f83dc remembers it until it is above 20 again).
static void AddEnergy(float d)
{
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type >= 'VR01' && type <= 'VR08') return;
	if (g_health > 0 && g_health + d <= 0) PlayGameSoundIndex(-1, 80, false);	// 0x458980
	g_health += d;
	if (g_health > 100.0f)
	{
		g_health = 100.0f;
		g_4f83dc = 0;
		return;
	}
	if (g_health >= 0)
	{
		if (g_health >= 19.0f)								// 0x4a3e90
		{
			if (g_health > 20.0f) g_4f83dc = 0;				// 0x495324
			return;
		}
	}
	else g_health = 0;
	if (!g_4f83dc)
	{
		PlayGameSoundIndex(-1, 182, false);
		g_4f83dc = 1;
	}
}

// 0x42adc0: points (Boy Genius: no difficulty scaling; the "+N" pop-up shows the last award).
static void AddPoints(int points)
{
	g_score += points;										// 0x4f83c0
	if (points > 0)
	{
		g_score_popup = points;								// 0x4f83c4
		g_score_popup_time = 1.2f;							// 0x4f83c8
	}
	if (g_score < 0) g_score = 0;
}

// 0x4038c0: set the type of item `item` (0..28) of menu page `page`.
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 29) return;
	// ("Activating Item %d for %d IsActive:%d": debug log)
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;
}

// 0x4592f0: the element behind a game object (the original looks it up by its ObjectID).
static OMediaElement *ElementOf(CGameObject *o)	{ return o ? dynamic_cast<OMediaElement *>(o) : NULL; }

// --------------------------------------------------------------------------- construction

C3DNeutron::C3DNeutron()									// 0x4329a0
{
	SetName((char *)"C3DNEUTRON");
	Log("C3DSprite()");										// (sic)
	C3DSprite::InitObject();								// 0x464130 (it has no InitObject of its own)
	SetObjectID('3NEU');									// element slot 48
	OMediaDataBase *db = GetDatabase("sprites.omt");		// 0x46a910
	for (int i = 0; i < 13; i++)
	{
		m_frames[i] = LoadCanvasID(db, i);					// 0x477890
		if (m_frames[i]) m_frames[i]->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60, +0x64 = 1, +0x24 vtable +0x10
	}
	set_diffuse(OMediaFARGBColor(0.5f, 1.0f, 1.0f, 1.0f));	// +0x90.. (r, g, b, a = 1, 1, 1, 0.5)
	m_frame = 0;
	m_collected = 0;
	m_time = 0;
	InitSpriteProperties(200.0f);							// element slot 52
	v67(250.0f);
	m_gone = 0;
	v42(0);
	v40(1);
	v46(0);
	v52(1);
	v34(1);
	C3DSprite::InitPhysics();								// 0x464210
	// (an empty debug log)
}

C3DNeutron::~C3DNeutron() {}								// 0x432cf0

namespace neg {
CGameObject *New_C3DNeutron(void) { return new C3DNeutron; }	// '3NEU' (0x6c0 bytes)
}

// --------------------------------------------------------------------------- slots

void C3DNeutron::AddTouching(CGameObject *other)			// 0x432ef0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_collected || g_camera != other) return;
	AddEnergy(5.0f);										// 0x42a920
	AddPoints(5);											// 0x42adc0
	if (g_health < 100.0f) SetItemState(2, 0, 1);			// 0x48d96c, 0x4038c0
	m_channel = PlayGameSoundIndex(-1, 4, false);			// 0x458980
	m_collected = 1;
}

// 0x432d30: every 0.05 s (scaled) the next frame: spinning 0..7; collected, 8..12, then at 13
// hidden, no collisions, gone. Gone, it comes back after 240 s.
void C3DNeutron::Process(float dt)
{
	C3DSprite::Process(dt);									// 0x463090 (no pause check)
	m_time = dt + m_time;
	if (m_gone)
	{
		if (!(m_time > 240.0f)) return;						// 0x48d934
		OMediaElement *e = ElementOf(this);					// 0x4592f0
		if (e) e->force_show();								// (2002 +0x70 hide count: 1, then hide(false))
		m_collected = 0;
		m_time = 0;
		m_gone = 0;
		v136(1);											// +0x220
		return;
	}
	if (!(m_time > 0.05f)) return;							// 0x495334 (no time scale)
	m_time = 0;
	if (m_collected)
	{
		m_frame++;
		if (m_frame > 12)
		{
			if (m_frame >= 14) return;
			OMediaElement *e = ElementOf(this);
			if (e) e->hide(true);							// +0x58
			m_gone = 1;
			v136(0);
			return;
		}
	}
	else
	{
		m_frame++;
		if (m_frame > 7) m_frame = 0;
	}
	set_canvas(m_frames[m_frame]);							// element +0xac
}

void C3DNeutron::v258(void)									// 0x432ec0
{
	C3DSprite::v258();										// 0x464780
	set_canvas(m_frames[0]);
}

// Reload: four times in ten the neutron is there, else it starts collected and gone.
void C3DNeutron::v260(void)									// 0x432f70
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	if ((float)r * 0.001f < 0.4)							// 0x48e5d8, 0x4ab220 (double)
	{
		set_canvas(m_frames[0]);
		return;
	}
	OMediaElement::hide(true);								// +0x58
	m_collected = 1;
	m_gone = 1;
	v136(0);
}

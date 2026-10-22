/*  C3DRedNeutron - see C3DRedNeutron.h. Reconstructed from Neutron.exe 0x43c370..0x43cc30. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DRedNeutron.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Todo.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMediaCanvas.h"
#include "OMediaStreamOperators.h"
#include <math.h>
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

// 0x42adc0: points (Boy Genius: no difficulty scaling; the "+N" pop-up shows the last award;
// as in C3DNeutron.cpp).
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

// 0x4061d0: queue the top-right pickup pop-up (3 s; or behind the one showing) and count it.
static void AnnouncePickupBG(int pic, unsigned int index)
{
	if (g_4f815c <= 0)
	{
		g_4f8158 = -1;
		g_4f8154 = pic;
		g_4f815c = 3.0f;
		g_4f8160 = index;
	}
	else if (g_4f8154 != -1) g_4f8158 = pic;
	g_pickup_counts[2 * pic]++;								// 0x4f7db8
}

// 0x42ab30
static void Fn42ab30(void)
{
	if (g_4eefd0 == -1.0f) g_4f83d0 = 3.0f;					// 0x49b1f8
}

// 0x4592f0: the element behind a game object (the original looks it up by its ObjectID).
static OMediaElement *ElementOf(CGameObject *o)	{ return o ? dynamic_cast<OMediaElement *>(o) : NULL; }

// --------------------------------------------------------------------------- construction

C3DRedNeutron::C3DRedNeutron()								// 0x43c370
{
	SetName((char *)"C3DREDNEUTRON");
	Log("C3DREDNEUTRON()");
	C3DRedNeutron::InitObject();							// 0x43c730 (inlined)
	SetObjectID('3RED');									// 0x462d80
	OMediaDataBase *db = GetDatabase("sprites.omt");		// 0x46a910
	for (int i = 0; i < 13; i++)
	{
		m_frames[i] = LoadCanvasID(db, i);					// 0x477890
		if (m_frames[i]) m_frames[i]->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60, +0x64 = 1, +0x24 vtable +0x10 (port: checked)
	}
	set_diffuse(OMediaFARGBColor(0.5f, 1.0f, 0.2f, 0.2f));	// +0x90.. (r, g, b, a = 1, 0.2, 0.2, 0.5)
	set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// +0xb4 = 6, +0xb8 = 7 / 1
	m_frame = 0;
	m_collected = 0;
	m_time = 0;
	InitSpriteProperties(300.0f);							// element slot 52
	m_pulse = 0;
	m_gone = 0;
	v67(250.0f);
	v42(0);
	v40(1);
	v46(0);
	v52(1);
	v34(1);
	C3DSprite::InitPhysics();								// 0x464210
	// (an empty debug log)
}

C3DRedNeutron::~C3DRedNeutron() {}

namespace neg {
CGameObject *New_C3DRedNeutron(void) { return new C3DRedNeutron; }		// '3RED' (0x80c bytes)
}

// --------------------------------------------------------------------------- slots

void C3DRedNeutron::InitObject(void)						// 0x43c730
{
	CPickupType::InitObject();								// 0x45f170
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
}

void C3DRedNeutron::AddTouching(CGameObject *other)			// 0x43c9b0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (m_collected || other != g_camera) return;
	if (strcasecmp("REDNEUTRON1", m_tag) == 0)				// 0x487c20: the first one sets the scene
		SetTaskState("SCENE", 90);							// 0x45f990
	Collect();												// element +0xd8
	AnnouncePickupBG(1, 0);									// 0x4061d0
	if (strcasecmp(m_next_trigger, "none") != 0)
		CallNextTrigger(m_next_trigger, 0);					// element +0xd4
}

// 0x43c760: the pulse (size and blue channel), then every 0.05 s (scaled) the next frame:
// spinning 0..7; collected, 8..12, then at 13 hidden for good.
void C3DRedNeutron::Process(float dt)
{
	C3DSprite::Process(dt);									// 0x463090 (no pause check)
	m_time = dt + m_time;
	const float t = dt + m_pulse;
	m_pulse = t;
	const float k = (float)(m_sprite_size / 100);			// (integer division)
	const float h = (float)((sin(t * 8.0f) * 150.0 + 330.0) * k);	// 0x4b1428, 0x4b1420, 0x4b1418
	const float w = (float)((sin((t + 0.3) * 10.0) * 70.0 + 330.0) * k);	// 0x4a3110, 0x4b1410, 0x4b1408
	set_size(w, h);											// +0xac, +0xb0
	double b = sin(m_pulse * 3.0f) * (double)0.7f;			// 0x48e5a0, 0x490e90
	if (b > 0.7) b = 0.7f;									// 0x4afd18 (double), 0x48d974
	else if (b < 0) b = 0;
	set_diffuse(OMediaFARGBColor(0.8f, 1.0f, 0.2f, (float)b));	// +0x90.. (r, g, b, a = 1, 0.2, b, 0.8)
	set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// +0xb4 = 6, +0xb8 = 7 / 1
	if (!(m_time > 0.05f)) return;							// 0x495334 (no time scale)
	m_time = 0;
	if (m_collected)
	{
		m_frame++;
		if (m_frame > 12)
		{
			if (m_frame >= 14) return;
			OMediaElement *e = ElementOf(this);				// 0x4592f0
			if (e) e->hide(true);							// +0x58
			m_gone = 1;
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

void C3DRedNeutron::v258(void)								// 0x43c980
{
	C3DSprite::v258();										// 0x464780
	set_canvas(m_frames[0]);
}

void C3DRedNeutron::v260(void)								// 0x43cb10
{
	// ("PostLoad 1 %d" to the empty 0x472970)
	CPickupType::v260();									// 0x45f1f0
	// ("PostLoad 2 %d  %d")
	set_canvas(m_frames[0]);								// element +0xac
	if (g_object_states[m_pickup_index] > 0)				// taken already
	{
		// ("PostLoad 3 %d  %d")
		OMediaElement *e = ElementOf(this);					// 0x4592f0
		if (e) e->hide(true);								// +0x58
		m_gone = 1;
	}
}

// --------------------------------------------------------------------------- element-family slots

void C3DRedNeutron::Collect(void)							// 0x43ca90
{
	g_4f83cc++;												// the lives
	Fn42ab30();												// 0x42ab30
	AddPoints(200);											// 0x42adc0
	AddEnergy(25.0f);										// 0x42a920
	SetItemState(2, 1, 1);									// 0x4038c0
	v124(0);												// +0x1f4
	m_channel = PlayGameSoundIndex(-1, 14, false);			// 0x458980
	g_object_states[m_pickup_index]++;						// 0x4f8438
	m_collected = 1;
}

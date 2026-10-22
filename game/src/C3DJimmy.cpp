/*  C3DJimmy - see C3DJimmy.h. This file: construction, destruction, the factory entry and the
    hooks C3DPlayer.h declares weak. The slots are in jimmy/C3DJimmy_a.cpp and _b.cpp.       */
#include "C3DJimmy.h"
#include "C3DPermanentSprite.h"
// TODO(BG): #include "C3DRadar.h" (Negatron-only class, deleted)
#include "C3DShadow.h"
#include "C3DJetpackFire.h"
#include "C3DTargetCursor.h"
#include "C3DPointCursor.h"
// TODO(BG): #include "C3DASEObj.h" (Negatron-only class, deleted)
#include "C2DInGameMenu.h"
#include "CTaskList.h"
// TODO(BG): #include "C3DFish.h" (Negatron-only class, deleted)
#include "C3DRocketShip.h"
#include "CJimmyGame.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "Sound.h"
#include "gen/globals.h"
#include "OMedia3DShape.h"
#include <string.h>

using namespace neg;

// Factory entries of the helper classes (gen/factory.cpp: weak TODO stubs until they exist).
namespace neg {
CGameObject *New_C3DRocketShip(void);		// N2:0x46a290
}

// Helper classes without a factory ID: not reconstructed yet.

template <class T> static T *As(CGameObject *o) { return o ? dynamic_cast<T *>(o) : NULL; }

// Menu items (menu helpers, copied as in SaveGame.cpp): N2:0x404dd0 set, 0x403910 clear the type
// of item `item` of menu page `page`.
static void SetItemState(short page, short item, short state)	// N2:0x404dd0
{
	// The pickups come in pairs (10/21 and 31/38): giving one also gives (and counts) the other.
	if (item < 0 || item >= C2DInGameMenu::MENU_ITEMS) return;
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;
	if (page != C2DInGameMenu::MENU_PICKUPS) return;
	C2DInGameMenu::MenuPage *pk = C2DInGameMenu::Page(C2DInGameMenu::MENU_PICKUPS);
	if (item == 10)
	{
		if (!pk || !pk->items[21].type) { SetItemState(2, 21, 1); g_pickup_counts[2 * 21]++; }
	}
	else if (item == 21)
	{
		if (!pk || !pk->items[10].type) { SetItemState(2, 10, 1); g_pickup_counts[2 * 10]++; }
	}
	else if (item == 38)
	{
		if (!pk || !pk->items[31].type) { SetItemState(2, 31, 1); g_pickup_counts[2 * 31]++; }
	}
	else if (item == 31)
	{
		if (!pk || !pk->items[38].type) { SetItemState(2, 38, 1); g_pickup_counts[2 * 38]++; }
	}
}

static void ClearItemState(short page, short item)				// 0x403910
{
	C2DInGameMenu::MenuPage *p;
	if (item >= 0 && item < C2DInGameMenu::MENU_ITEMS && (p = C2DInGameMenu::Page(page)) != NULL)
		p->items[item].type = 0;
}

// The game's random number generator (as C3DPlayer's): 0..1000.
static int Random1001(void)
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	return ((int)((*seed >> 16) * 1001) >> 16) & 0xffff;
}

// A sparkle colour set for a new C3DMagicParticle: (r * 0.001) < 0.33 / < 0.66 / else.
static int ParticleKind(int a, int b, int c)
{
	const double f = (double)Random1001() * (double)0.001f;	// 0x48e5d8, compared as doubles N2:0x4e4e68 / N2:0x4e4e60
	if (f < 0.33) return a;
	if (f < 0.66) return b;
	return c;
}

static void SetParticleKind(C3DObject *p, int kind)	// C3DMagicParticle +0x594
{
	(void)p; (void)kind;									// TODO(BG): C3DMagicParticle is Negatron only
}

// The setup the constructor gives the rocket ship and the jet pack: hidden, and (CGameObject
// slots 52, 132, 120, 107, 36) v52(0), v132(0), v120(1), v107(0), v36(0).
static void ParkVehicle(C3DAnimated *v)
{
	if (!v) return;
	v->HideAgain();											// element +0xdc
	v->v52(0);												// +0xd0
	v->v132(0);												// +0x210
	v->v120(1);												// +0x1e0
	v->v107(0);												// +0x1ac
	v->v36(0);												// +0x90
}

// --------------------------------------------------------------------------- construction

C3DJimmy::C3DJimmy()										// N2:0x443830
{
	// (f_d70[], f_2cb0[], f_4bf0, f_4cbc, f_4d80, f_4dbc[] are OMedia3DPoints: w = 1;
	// f_4ca8, f_4d6c OMedia3DVectors: w = 0.)
	SetName((char *)"C3DJIMMY");
	Log("C3DJimmy()");
	m_step_channel = -1;									// BG 0x422160: +0xa44 = -1
	strcpy(m_tag, "JIM1");
	// (Negatron set f_6d0 = 1 here, Granny animation; Boy Genius's Jimmy is a morph object)
	C3DJimmy::InitObject();									// N2:0x444780, called directly
	SetObjectID('3JIM');									// element slot 49
	f_cac = 0;
	f_4c10 = -1;

	f_4c90 = new CTaskList;									// 0x4c8 bytes
	f_cbc = NULL;
	f_cc0 = NULL;
	g_menu = new C2DInGameMenu;								// 0x5b4 bytes
	if (g_menu)
	{
		SetItemState(0, 9, 1);								// N2:0x404dd0
		SetItemState(0, 10, 1);
		SetItemState(1, 0, 1);
		ClearItemState(1, 2);								// 0x403910
		ClearItemState(1, 3);
		ClearItemState(1, 4);
		ClearItemState(1, 5);
		ClearItemState(1, 6);
		ClearItemState(1, 7);
		ClearItemState(1, 8);
	}

	f_cc8 = As<C3DAnimated>(New_C3DRocketShip());		// 0x1284 bytes
	ParkVehicle(f_cc8);
	f_ccc = NULL;											// TODO(BG): N2 C3DJetPack (Negatron only)

	f_4cfe = 0;
	f_cc4 = new C3DShadow;									// 0x60c bytes
	f_4c8c = new C3DTargetCursor;							// 0x60c bytes
	if (f_4c8c) f_4c8c->HideAgain();						// element +0xd8
	f_d4c = new C3DPointCursor;								// 0x92c bytes
	if (f_d4c) f_d4c->HideAgain();							// element +0xdc
	for (int i = 0; i < 20; i++)
	{
		f_cd0[i] = NULL;									// TODO(BG): N2 C3DWaterBalloon (Negatron only)
	}
	f_d2c = NULL;
	f_d28 = NULL;
	f_d30 = NULL;
	f_d38 = NULL;
	f_d34 = NULL;
	f_d3c = 0;
	f_4d9c = 5000.0f;
	f_d50 = 0;
	f_d5c = NULL;
	f_4ee8 = 0;
	f_d58 = 0;
	v36(0);
	v107(0);
	f_d64 = 0;
	f_d68 = -1;
	f_d6a = 4;
	f_d60 = 50.0f;
	g_sfx_db_loaded = false;
	f_4c14 = -1;
	f_4c18 = 0;
	f_4c1c = -1;
	f_4c10 = -1;
	f_4c20 = -1;
	f_4c24 = -1;
	strcpy(f_4c28, "none");
	f_d54 = 0;
	f_009 = 1;
	f_4db8 = 0;
	f_4e78 = 0;
	f_4dbc[0].x = 0;
	f_4dbc[f_4e78].y = 0;
	f_4dbc[f_4e78].z = 0;
	f_4e60 = 0;
	f_4e5c = 0;
	f_4e64 = 0;
	f_4e68 = 0;
	f_4e6c = 0;
	f_4ccc = 0;
	f_4cd0 = 0;
	f_4c08 = -1;
	f_4c0c = -1;
	f_4d98 = -1;
	f_4cfd = 0;
	f_4d94 = 0;
	f_4ca0 = 0;
	f_4dac = 0;
	f_4db0 = -900.0f;
	f_4db4 = 0;
	f_4ee4 = 0;
	f_4cfc = 0;
	f_c9c = 0;
	f_4ef0 = NULL;

	f_d20 = new C3DGraplingHook;							// 0x92c bytes
	if (f_d20) f_d20->f_590->scale(1.0f, 1.0f, 7.0f);		// its base shape, vtable +0x34

	for (int i = 0; i < 20; i++)
	{
		f_4d00[i] = NULL;									// TODO(BG): N2 C3DMagicParticle (Negatron only)
		SetParticleKind(f_4d00[i], ParticleKind(0xe, 0xc, 0x11));
	}
	f_4d50[0] = NULL;
	SetParticleKind(f_4d50[0], ParticleKind(0x1b, 0x1c, 0x1d));

	f_4d7c = 0;
	f_4d80.x = 0;
	f_4d80.y = 0;
	f_4d80.z = 0;
	f_4c94 = 0;
	f_4c98 = 0;
	f_4c9c = 0;
	f_4ca4 = 0;
	f_4ca5 = 0;
	f_4ca6 = 0;
	f_4ca7 = 0;
	f_4cb8 = 0;
	f_4cd4 = 0;
	f_4cd8 = 0;
	f_ae8 = 10.0f;
	g_N2_537638 = 0xbf800000;									// -1.0f
	g_N2_537634 = -1.0f;
	g_4eefd0 = -1.0f;
	f_4cdc = 5.0f;
	f_4ce0 = 0;
	f_4cfe = 1;
	f_4ce8 = 0;
	f_4d90 = 0;
	f_4d54 = 0;
	f_4d60 = 0;
	strcpy(f_4e80, g_empty_string);
	g_save_slot = 6;
	f_d6c = 0;
	f_d6d = 0;
	f_42c = 1;
	f_4ce4 = 0;
	f_4cec = 1;
	f_4cf0 = 0;
	f_4cf4 = 0;
	g_4f83dc = 0;
	f_4cf8 = 1;
	f_4c04 = 0;
	f_4d58 = 0;
	f_4d5c = 0;
	f_4d68 = 0;
	f_4d64 = 0;

	f_d5c = NULL;											// port: Negatron made Goddard here; Neutron.exe does not (he is in the .gam)
	f_d40 = NULL;											// TODO(BG): N2 C3DRadar (Negatron only)
	f_d48 = 0;
	f_d44 = NULL;											// TODO(BG): N2 C3DASEObj bubble (BG: C3DBubble)
	f_4da4 = 0;
	f_4da0 = 0;
	f_4da8 = 0.35f;
	g_jimmy = static_cast<CGameObject *>(this);
	f_8a4 = 1;												// never culled
	f_4e7c = 0;
	if (g_menu) g_menu->LoadOptions("dat\\options.opt");	// +0x514
}

C3DJimmy::~C3DJimmy()										// 0x4229e0
{
	Log("~C3DJimmy()");
	// (debug log "MEBTEMP deleting jimmy!!!")
	UnInitObject();											// vtable +0x20
	// The helpers (menu, task list, vehicles ...) are not deleted: they are game objects of
	// their own (DeleteAllObjects).
}

namespace neg {
CGameObject *New_C3DJimmy(void)	{ return new C3DJimmy; }				// factory '3JIM' (0x4efc bytes)
}

C3DJimmy *neg::Jimmy(void)
{
	return g_jimmy ? dynamic_cast<C3DJimmy *>(g_jimmy) : NULL;
}

// --------------------------------------------------------------------------- hooks (C3DPlayer.h)

// N2:0x443370 (a C3DJetPack method, `this` = the jet pack): the flame of g_jimmy's jet pack and
// its own (+0x9f0 -> +0x928) to 0, stop its sound (+0x9f4).
void neg::JimmyJetpackOff(CGameObject *jimmy)
{
	(void)jimmy;	// TODO(BG): N2 C3DJetPack
}

// N2:0x4433f0 (C3DJetPack, `a` unused): loop "jetpack" on its channel +0x9f4 unless it plays.
void neg::JimmyJetpackSound(CGameObject *jimmy, int a)
{
	(void)a;
	(void)jimmy;	// TODO(BG): N2 C3DJetPack
}

// C3DPlayer N2:0x4613a0 inline: jet pack (+0xccc) -> fire (+0x9f0, if any) -> size (+0x928).
void neg::JimmyJetpackFlame(CGameObject *jimmy, float size)
{
	(void)jimmy; (void)size;	// TODO(BG): N2 C3DJetPack
}

// Element slot 138 (+0x228) of g_jimmy.
void neg::JimmyActivateMenuItem(CGameObject *jimmy, CGameObject *o, CGameObject *who, int item, float value)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->n2_ev138(o, who, item, value);
}

// C3DFish.h: Jimmy's element slot 134 (+0x218), hurt.
void neg::JimmyHurt(CGameObject *jimmy, float damage, int points, float time, unsigned char a)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->n2_ev134(damage, points, time, a);
}

// C3DRocketShip.h: Jimmy's +0x4cb8, the hit flag (the mini sub and the rocket ship clear it before a hit).
unsigned char neg::JimmyHurtFlag(CGameObject *jimmy)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	return j ? j->f_4cb8 : 0;
}
void neg::JimmySetHurtFlag(CGameObject *jimmy, unsigned char v)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->f_4cb8 = v;
}

// --------------------------------------------------------------------------- hooks (C3DRocketShip.h)

// Jimmy's element slot 130 (+0x208): "SPLASH" + "pain agh ahh".
void neg::JimmyPain(CGameObject *jimmy)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->n2_ev130();
}

// Jimmy's element slot 146 (+0x248): his bubble follows the vehicle (pos) while its time lasts.
void neg::JimmyRocketAim(CGameObject *jimmy, float dt, OMedia3DVector pos)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->n2_ev146(dt, pos);
}

// Jimmy's +0x4d94, the rocket ship's shots left.
int neg::JimmyAmmo(CGameObject *jimmy)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	return j ? j->f_4d94 : 0;
}
void neg::JimmySetAmmo(CGameObject *jimmy, int n)
{
	C3DJimmy *j = jimmy ? dynamic_cast<C3DJimmy *>(jimmy) : NULL;
	if (j) j->f_4d94 = n;
}

// Jimmy's element slot 148 (+0x250): the next water balloon (the rocket ship's shots).
void neg::JimmyRocketShoot(CGameObject *jimmy, float x, float y, float z, float side, float speed,
						   CGameObject *target, int a7, float a8)
{
	dynamic_cast<C3DJimmy *>(jimmy)->n2_ev148(x, y, z, side, speed, target, (float)a7, a8);
}

// C3DRocketShip N2:0x46bf95 inline: the next water balloon of g_jimmy aims 1300 above and 550 in
// front of its target (+0x63c..+0x644), if there is one.
void neg::JimmyGadgetShot(CGameObject *jimmy)
{
	(void)jimmy;	// TODO(BG): N2 C3DWaterBalloon
}

// --------------------------------------------------------------------------- Neutron.exe slots

void C3DJimmy::ev81(void)									// 0x4299d0
{
	if (m_step_channel == -1)
		m_step_channel = PlayGameSoundIndex(-1, static_cast<CJimmyGame *>(g_gametype)->m_surface_sound, true);	// 0x458980; game +0x54c
}

void C3DJimmy::ev82(void)									// 0x429a00
{
	if (m_step_channel != -1)
	{
		StopGameSound(m_step_channel, false);				// 0x458a00
		m_step_channel = -1;
	}
}

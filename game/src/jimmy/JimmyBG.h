/*  Helpers shared by C3DJimmy.cpp and jimmy/C3DJimmy_a.cpp / _b.cpp (Neutron.exe's free
    functions next to C3DJimmy, and short names for the vtable calls the original makes on Jimmy's
    helper objects). Internal to C3DJimmy: `static` functions, included by its .cpp files only. */
#pragma once
#include "../C3DJimmy.h"
#include "../C2DInGameMenu.h"
#include "../C3DShadow.h"
#include "../C3DTargetCursor.h"
#include "../C3DPointCursor.h"
#include "../C3DGraplingHook.h"
#include "../gadgets/C3DBubble.h"
#include "../gadgets/C3DBaseball.h"
#include "../C3DRocketShip.h"
#include "../C3DJetpackFire.h"
#include "../level1/C3DJeep.h"
#include "../CTaskList.h"
#include "../CGameType.h"
#include "../CJimmyGame.h"
#include "../CEditor.h"
#include "../Physics.h"
#include "../Sound.h"
#include "../SaveGame.h"
#include "../DatabaseCache.h"
#include "../Mouse.h"
#include "../Game.h"
#include "../Bridge.h"
#include "../Todo.h"
#include "JimmyMenu.h"
#include "../gen/globals.h"
#include "OMediaViewPort.h"
#include "OMediaTrigo.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <string>

namespace jimmybg {

using namespace neg;

static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
static const float kAngleToDeg = 0.02197265625f;			// 0x48e5b4: 360 / 16384

static inline OMedia3DVector Pos(CGameObject *o)		{ float p[4]; o->v195(p); return OMedia3DVector(p[0], p[1], p[2], p[3]); }	// +0x310
static inline OMedia3DVector Rot(CGameObject *o)		{ float p[4]; o->v201(p); return OMedia3DVector(p[0], p[1], p[2], p[3]); }	// +0x328
static inline OMedia3DVector Vel(CGameObject *o)		{ float p[4]; o->v174(p); return OMedia3DVector(p[0], p[1], p[2], p[3]); }	// +0x2bc
static inline float Dist(const OMedia3DVector &a, const OMedia3DVector &b)
{
	const float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
	return sqrtf(dy * dy + dz * dz + dx * dx);
}

// The game-object part of an element pointer, and back (the original adds / subtracts 0xc0, or
// 0xc8 for the sprites).
template <class T> static inline CGameObject *GO(T *e)	{ return e ? static_cast<CGameObject *>(e) : NULL; }
static inline C3DObject *Elem(CGameObject *o)		{ return o ? dynamic_cast<C3DObject *>(o) : NULL; }

// Show: `if (e->hide_count) { e->hide_count = 1; e->hide(false); }` (element +0x70 / +0x58);
// Hide: element +0x58 hide(true).
template <class T> static inline void Show(T *e)	{ if (e) e->force_show(); }
template <class T> static inline void Hide(T *e)	{ if (e) e->hide(true); }

// 0x42a920: energy (g_4f83d4, 0..100) += d. Not in the VR levels. "Energy out" sound 0x50 when it
// runs out, the warning 0xb6 (once, g_4f83dc) below 20.
static void AddEnergy(float d)
{
	const int id = g_gametype->m_object_id;					// +0x490
	if (id >= 'VR01' && id <= 'VR08') return;
	if (g_4f83d4 > 0 && g_4f83d4 + d <= 0) PlayGameSoundIndex(-1, 0x50, false);
	g_4f83d4 += d;
	if (g_4f83d4 > 100.0f)
	{
		g_4f83d4 = 100.0f;
		g_4f83dc = 0;
		return;
	}
	if (g_4f83d4 >= 0)
	{
		if (g_4f83d4 >= 0.8f)								// 0x4a3e90
		{
			if (g_4f83d4 <= 20.0f) return;
			g_4f83dc = 0;
			return;
		}
	}
	else g_4f83d4 = 0;
	if (!g_4f83dc)
	{
		PlayGameSoundIndex(-1, 0xb6, false);
		g_4f83dc = 1;
	}
}

// 0x42adc0: the score (g_score) += n; a gain shows the "+N" pop-up for 1.2 s.
static void ScoreAdd(int n)
{
	g_score += n;
	if (n > 0)
	{
		g_score_popup = n;									// 0x4f83c4
		g_score_popup_time = 1.2f;							// 0x4f83c8
	}
	if (g_score < 0) g_score = 0;
}

// The PDA menu's items (menu page <page> (0 gadgets, 1 ..., 2 pickups), item 0..28).
static short ItemType(short page, short item)				// 0x403950
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p && item >= 0 && item < 29) return p->items[item].type;
	return 0;
}
static void SetItemType(short page, short item, short type)	// 0x4038c0 ("Activating Item %d for %d IsActive:%d")
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p && item >= 0 && item < 29) p->items[item].type = type;
}
static void ClearItem(short page, short item)				// 0x403910 ("Deactivating Item ...")
{
	SetItemType(page, item, 0);
}
// 0x403870: <off> 1: a usable gadget (type 1) becomes unusable (4); 0: back to usable.
static void ItemOff(short page, short item, short off)
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (!p || item < 0 || item >= 29) return;
	short &t = p->items[item].type;
	if (!off) { if (t == 4) t = 1; }
	else if (t == 1) t = 4;
}

// 0x42a730: the jump keys while no gadget, or gadget 0 / 2 / 3 / 6 / 7 is out (not with Num 2
// or joystick button 3 held).
static bool KeyJumpGadget(void)
{
	if (g_keys[0x49] || g_joy_button3) return false;		// 0x509881 (Num 9? VK 0x49 'I'), 0x509ac7
	const short g = g_4f0588;
	if (!(g == -1 || g == 0 || g == 3 || g == 2 || g == 6 || g == 7)) return false;
	return g_50993a || g_keys[0x2d] || g_keys[0x10] || g_keys[0x60] || g_joy_button1;
}

// 0x46a3e0: key <k> was pressed (clears it).
static bool KeyPressed(int k)
{
	if (!g_keys[k]) return false;
	g_keys[k] = 0;
	return true;
}

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// 0x458e40 (CLocalGameObject family): the nearest object with ObjectID <id> that is active (v123)
// and f_140, by quick_magnitude of (o - it).
static CGameObject *FindNearest(CGameObject *o, int id)
{
	CGameObject *best = NULL;
	float best_d = 0;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
	{
		CGameObject *it = *i;
		if (!it || it->m_object_id != id) continue;
		if (!it->v123() || !it->f_140) continue;
		OMedia3DVector a = Pos(o), b = Pos(it);
		OMedia3DVector d(a.x - b.x, a.y - b.y, a.z - b.z, 0);
		const float m = d.quick_magnitude();
		if (!best || m < best_d) { best = it; best_d = m; }
	}
	return best;
}

// 0x4037f0 (a menu method, `this` unused): the gadget power bar sprite g_4f8028 is 20 x
// (1.04 * v) (in 640 x 480 units; v 0..100, negative counts as 0).
static void SetEnergyBar(float v)
{
	if (v < 0) v = 0;
	if (g_4f8028)
		g_4f8028->set_size((float)((double)(short)g_screen_w * (double)0.0015625f * 20.0),
						   (float)((double)(short)g_screen_h * (double)0.0020833334f * v * 1.04));
}

// 0x44e450 (CJimmyGame slot 90, +0x168): the game frozen (0x5099ab) or not.
static void FreezeGame(bool on)
{
	g_5099ab = on ? 1 : 0;
	g_4f641c = 0;
}

// The game's random number generator (0x509a40).
static unsigned int Random(void)
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	return *seed;
}

// ---- the PDA menu (C2DInGameMenu, Jimmy's f_a18). The menu class is agent F's; the slots it
// does not have yet are TODO bridges here (BG slot numbers and addresses).
static void MenuSlot(C2DInGameMenu *m, int off, unsigned int addr, const char *what)
{
	(void)m; (void)off;
	NEG_TODO(addr, what);
}
static inline void MenuMark(C2DInGameMenu *m)					{ MenuSlot(m, 0x470, 0x4039e0, "C2DInGameMenu +0x470 (f_4be = 1)"); }
static inline void MenuUnmark(C2DInGameMenu *m)					{ MenuSlot(m, 0x474, 0x4039f0, "C2DInGameMenu +0x474 (f_4be = 0)"); }
static inline bool MenuMarked(C2DInGameMenu *m)					{ (void)m; return false; }	// TODO(BG): C2DInGameMenu +0x4be
static inline void MenuGadgetsOff(C2DInGameMenu *m)				{ MenuSlot(m, 0x480, 0x403bf0, "C2DInGameMenu +0x480 (f_4bc = f_4c8 = 3)"); }
static inline void MenuGadgetBarShow(C2DInGameMenu *m)			{ if (m) m->ShowGadgetBar(); }		// +0x484 0x4036c0
static inline void MenuGadgetBarHide(C2DInGameMenu *m)			{ if (m) m->HideGadgetBar(); }		// +0x488 0x403700
static inline void MenuGadgetBarEmpty(C2DInGameMenu *m)			{ if (m) m->GadgetBarEmpty(); }		// +0x48c 0x403730
static inline void MenuGadgetBarFull(C2DInGameMenu *m)			{ if (m) m->GadgetBarFull(); }		// +0x490 0x4037a0
static inline short MenuPageShown(C2DInGameMenu *m)				{ (void)m; NEG_TODO(0x403a10, "C2DInGameMenu +0x4a8 (f_4d4)"); return 0; }
static inline void MenuSelectGadget(C2DInGameMenu *m, short g)	{ (void)g; MenuSlot(m, 0x4b0, 0x405fb0, "C2DInGameMenu +0x4b0 select gadget"); }
static inline void MenuShowHud(C2DInGameMenu *m, bool show)		{ if (m) m->v316(show); }			// +0x4b4 0x406560
static inline void MenuHudPicture(C2DInGameMenu *m, int id, bool show, int x, int y)	{ if (m) m->ShowHudPicture(id, show, x, y); }	// +0x4b8 0x4063e0
static inline void MenuGoddardMode(C2DInGameMenu *m, bool on)	{ (void)on; MenuSlot(m, 0x4bc, 0x403420, "C2DInGameMenu +0x4bc Goddard sprite"); }
static inline void MenuGadgetTimerOn(C2DInGameMenu *m)			{ MenuSlot(m, 0x4c0, 0x406650, "C2DInGameMenu +0x4c0 gadget timer sprite"); }
static inline void MenuGadgetTimerOff(C2DInGameMenu *m)			{ MenuSlot(m, 0x4c4, 0x406590, "C2DInGameMenu +0x4c4 gadget timer sprite off"); }
static inline void MenuMusic(C2DInGameMenu *m, int id)			{ (void)id; MenuSlot(m, 0x4e8, 0x407410, "C2DInGameMenu +0x4e8 menu music"); }
static inline void MenuClearVR(C2DInGameMenu *m)				{ MenuSlot(m, 0x4ec, 0x407490, "C2DInGameMenu +0x4ec (items 0..43 of the VR page)"); }
static inline void MenuShowSelected(C2DInGameMenu *m)			{ MenuSlot(m, 0x45c, 0x402f30, "C2DInGameMenu +0x45c"); }
static inline void MenuHideSelected(C2DInGameMenu *m)			{ MenuSlot(m, 0x460, 0x4033f0, "C2DInGameMenu +0x460"); }
// C2DInGameMenu +0x4e4: the "shot" HUD sprite (an element; hidden / shown).
static inline C3DObject *MenuShotSprite(C2DInGameMenu *m)		{ (void)m; return NULL; }	// TODO(BG): C2DInGameMenu +0x4e4

}	// namespace jimmybg

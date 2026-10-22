/*  C3DPickupItem - see C3DPickupItem.h. Reconstructed from Neutron.exe 0x4358b0..0x436970. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "../C3DJimmy.h"
#include "C3DPickupItem.h"
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

// 0x403910: item `item` of menu page `page` disabled.
static void ClearItemState(short page, short item)
{
	C2DInGameMenu::MenuPage *p;
	if (item >= 0 && item < 29 && (p = C2DInGameMenu::Page(page)) != NULL)
		p->items[item].type = 0;
}

// 0x403950: the type of item `item` of menu page `page` (0 when there is no such item).
static short GetItemState(short page, short item)
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p && item >= 0 && item < 29) return p->items[item].type;
	return 0;
}

static int  PickupCount(int pic)			{ return g_pickup_counts[2 * pic]; }	// 0x4061b0
static void AddPickupCount(int pic, int n)	{ g_pickup_counts[2 * pic] += n; }		// 0x4061c0

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

// 0x406f90: open the in-game menu on page `page` (cursor shown, page canvas shown, the game
// paused through game type +0x168, g_4f8048 = page). Part of the menu (agent A's C2DInGameMenu).
static void OpenInGameMenu(int page)
{
	NEG_TODO(0x406f90, "open the in-game menu on a page");
	(void)page;
}

// Jimmy's element slots used by the tagged pickups (C3DJimmy is reconstructed elsewhere).
static void JimmySlot122(CGameObject *jim)	{ if (C3DJimmy *j = jim ? dynamic_cast<C3DJimmy *>(jim) : NULL) j->SaveCheckpoint(); }	// element +0x1e8
static void JimmySelect(CGameObject *jim, short gadget)	{ (void)jim; (void)gadget; NEG_TODO(0x428d50, "C3DJimmy element +0x174: select a gadget"); }
static void JimmySlot94(CGameObject *jim)	{ (void)jim; NEG_TODO(0x427ff0, "C3DJimmy element +0x178"); }

// 0x4365d0: the story steps of the tagged pickups.
static void TaggedPickup(C3DPickupItem *p)
{
	const char *tag = p->m_tag;
	if (strcasecmp(tag, "SHRINKRAY") == 0 && GetTaskState("SCENE") == 35)
	{
		if (g_camera->NameContains((char *)"C3DJIMMY")) JimmySlot122(g_camera);
		SetTaskState("SCENE", 50);
		SetItemState(0, 3, 1);
		OpenInGameMenu(0);
		if (g_camera->NameContains((char *)"C3DJIMMY")) JimmySelect(g_camera, 3);
	}
	if (strcasecmp(tag, "SCOOTERPART") == 0 && GetTaskState("SCENE") == 100)
	{
		SetTaskState("SCENE", 108);
		SetItemState(0, 4, 1);
		SetItemState(1, 2, 1);
		OpenInGameMenu(1);
	}
	if (strcasecmp(tag, "BUBBLEPICKUP") == 0)
	{
		SetItemState(0, 0, 1);
		OpenInGameMenu(10);
	}
	if (strcasecmp(tag, "TELEPORT2SPACE") == 0 && GetTaskState("SCENE") == 170) SetTaskState("SCENE", 172);
	if (strcasecmp(tag, "CAVE2") == 0 && GetTaskState("SCENE") == 500) SetTaskState("SCENE", 510);
	if (strcasecmp(tag, "PASSCARD") == 0 && GetTaskState("SCENE") == 430)
	{
		SetTaskState("SCENE", 460);
		SetItemState(1, 5, 1);
	}
	if (strcasecmp(tag, "SEWERPART") == 0 && GetTaskState("SCENE") == 290)
	{
		SetTaskState("SCENE", 300);
		SetItemState(2, 15, 1);
		AnnouncePickupBG(15, 0);
		CGameObject *jim = FindObjectByTag("JIM1");
		if (jim)
		{
			JimmySlot94(jim);
			g_gametype->SetCamera(jim);						// +0x118
		}
	}
}

// --------------------------------------------------------------------------- construction

C3DPickupItem::C3DPickupItem()								// 0x4358b0
{
	SetName((char *)"C3DPICKUPITEM");
	Log("C3DPICKUPITEM()");
	m_initially_active = 1;
	C3DPickupItem::InitObject();							// 0x435b80 (called directly)
	SetObjectID('3PIC');									// 0x462d80
	m_sprite_index = 8;
	m_picked_up_index = -1;
	m_required_pic_num = -1;
	m_req_pic_num_amount = 1;
	m_required_level = -1;
	m_sprite_size = 50;
	strcpy(m_sprite_db, "sprites.omt");
	m_sound_index = -1;
	m_need_more_sound = -1;
	v67(100.0f);											// radius
	v46(0);
	v52(1);
	m_times_to_trigger = -1;
	m_times_triggered = 0;
	v34(1);
	m_is_ambient = 0;
	m_ambient_channel = -1;
	m_ambient_started = 0;
	m_point_value = 0;
	C3DSprite::InitPhysics();								// 0x464210
	strcpy(m_activate_object, "none");
	m_toggle = -1;
	f_760 = 0;
	m_pass_thru = 1;
	m_show_arrow = 1;
	f_008 = 1;
}

C3DPickupItem::~C3DPickupItem() {}							// 0x435ae0

namespace neg {
CGameObject *New_C3DPickupItem(void) { return new C3DPickupItem; }	// '3PIC' (0x774 bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DPickupItem::InitObject(void)						// 0x435b80
{
	CPickupType::InitObject();								// 0x45f170
	RegisterProperty("SoundIndex", &m_sound_index, PROP_LONG, 0);
	RegisterProperty("NeedMoreSound", &m_need_more_sound, PROP_LONG, 0);
	RegisterProperty("TimesToTrigger", &m_times_to_trigger, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	RegisterProperty("IsAmbient", &m_is_ambient, PROP_LONG, 0);
	RegisterProperty("PointValue", &m_point_value, PROP_LONG, 0);
	RegisterProperty("PickedUpIndex", &m_picked_up_index, PROP_LONG, 0);
	RegisterProperty("RequiredPicNum", &m_required_pic_num, PROP_LONG, 0);
	RegisterProperty("ReqPicNumAmount", &m_req_pic_num_amount, PROP_LONG, 0);
	RegisterProperty("InitallyActive", &m_initially_active, PROP_LONG, 0);
	RegisterProperty("ActivateObject", m_activate_object, PROP_ALPHA, 0);
	RegisterProperty("PassThru", &m_pass_thru, PROP_LONG, 0);
	RegisterProperty("ShowArrow", &m_show_arrow, PROP_LONG, 0);
}

void C3DPickupItem::ResetObject(void)						// 0x435b20
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	v107(1);												// +0x1b0
	const int i = m_pickup_index;
	if (i >= 0 && i < 4200 && g_object_states[i] == 0 && m_sprite_index != 0x6a)
		force_show();										// inline: hide_count = 1, hide(false)
}

// Toggle the objects named by ActivateObject and ToggleObject (their NextState, +0x428).
static void ToggleNamed(const char *name, int toggle)
{
	if (strcasecmp(name, "none") == 0) return;
	CLocalGameObject *o = dynamic_cast<CLocalGameObject *>(FindObjectByTag(name));
	if (o) o->v268(toggle);									// (every level object is one)
}

void C3DPickupItem::AddTouching(CGameObject *other)			// 0x435ce0
{
	CGameObject::AddTouching(other);						// 0x470a90
	// ("Picked up a %s": debug log)
	if (other != g_camera) return;
	if (other->NameContains((char *)"C3DGODDARD") && strcasecmp(m_tag, "SEWERPART") != 0 &&
		strcasecmp(m_next_trigger, "none") != 0) return;
	const int idx = m_pickup_index;
	if (idx >= 4200)
		ErrorLog("ERROR: OVER max pickup items in C3DPickupItem");	// (debug log in the original)
	else if (idx < 1)
	{
		if (idx == 0)
		{
			if (m_pic_number >= 0) SetItemState(2, (short)m_pic_number, 1);	// 0x4038c0
			ToggleNamed(m_activate_object, m_toggle);
			ToggleNamed(m_toggle_object, m_toggle);
			OMediaElement::hide(true);						// +0x58
			v124(0);										// +0x1f4
			v136(0);										// +0x224
		}
	}
	else
	{
		if (g_object_states[idx] != 0) return;
		if (!CanPickUp()) return;							// element +0xd8
		if (m_picked_up_index == -1)
		{
			g_object_states[idx] = 1;
			OMediaElement::hide(true);
			v136(0);
		}
		else
		{
			g_object_states[idx] = 2;
			OMediaDataBase *db = GetDatabase(m_sprite_db);	// 0x46a910
			if (db)
			{
				InitSpriteID(db, m_picked_up_index, (float)m_sprite_size);	// element +200
				if (m_sprite_index != 0x6a) force_show();
			}
		}
		ToggleNamed(m_activate_object, m_toggle);
		ToggleNamed(m_toggle_object, m_toggle);
		if (m_pic_number >= 0 && m_pic_number < 29)
		{
			SetItemState(2, (short)m_pic_number, 1);
			AnnouncePickupBG(m_pic_number, (unsigned int)m_pickup_index);	// 0x4061d0
		}
		// (> 2: "ERROR: Pickup item %d has been used %d times", debug log)
	}
	TaggedPickup(this);										// 0x4365d0
	if (m_point_value > 0 && (m_times_to_trigger == -1 || m_times_triggered < m_times_to_trigger))
	{
		AddPoints(m_point_value);							// 0x42adc0
		v136(0);
	}
	if (strcasecmp(m_next_trigger, "none") != 0) CallNextTrigger(m_next_trigger, 0);	// element +0xd4
	if (!m_is_ambient)
	{
		if (m_times_to_trigger != -1 && m_times_triggered >= m_times_to_trigger) return;
		m_times_triggered++;
		if (m_sound_index == -1) return;
		if (!IsChannelPlaying(g_4eefd8))					// 0x47d890: the pickup sound channel
			g_4eefd8 = PlayGameSoundIndex(-1, m_sound_index, false);	// 0x458980
	}
	else if (m_sound_index != -1 && m_ambient_channel != -1)
		StopGameSound(m_ambient_channel, false);			// 0x458a00
}

void C3DPickupItem::Process(float dt)						// 0x436550
{
	C3DSprite::Process(dt);									// 0x463090
	if (!m_is_ambient || m_ambient_started) return;
	m_ambient_started = 1;
	if (m_sound_index == -1) return;
	if (m_ambient_channel != -1) StopGameSound(m_ambient_channel, false);	// 0x458a00
	m_ambient_channel = PlayGameSound3DIndex(this, -1, m_sound_index, true);	// 0x4589c0
}

void C3DPickupItem::v260(void)								// 0x436200
{
	CPickupType::v260();									// 0x45f1f0
	// ("%s   %f %f  %f": tag and position, debug log)
	const int idx = m_pickup_index;
	if (idx >= 0 && idx < 4200)
	{
		const int st = idx == 0 ? 0 : g_object_states[idx];
		if (st == 0)
		{
			if (m_sprite_index != 0x6a) force_show();
			v136(1);
		}
		else if (m_picked_up_index != -1 && st == 2)
		{
			OMediaDataBase *db = GetDatabase(m_sprite_db);	// 0x46a910
			if (db)
			{
				InitSpriteID(db, m_picked_up_index, (float)m_sprite_size);	// element +200
				if (m_sprite_index != 0x6a) force_show();
			}
		}
	}
	if (m_initially_active == 0)
	{
		v232();												// +0x3a4
		OMediaElement::hide(true);
		v136(0);
	}
	switch (m_sprite_index)									// the menu item of the known sprites
	{
		case 0x46:							m_pic_number = 5;	return;
		case 0x47: case 0x48: case 0x57:	m_pic_number = 8;	return;
		case 0x49:							m_pic_number = 7;	return;
		case 0x4a:							m_pic_number = 23;	return;
		case 0x68: case 0x6b:				m_pic_number = 4;	return;
		case 0x6a:							OMediaElement::hide(true);	return;
		case 0x74:							m_pic_number = 27;	return;
		case 0x75:							m_pic_number = 18;	// (sic: falls through to 3)
		case 0x88:							m_pic_number = 3;	return;
		case 0x8c:							m_pic_number = 21;	return;
		case 0x8e:							m_pic_number = 19;	return;
		case 0x98:							m_pic_number = 9;	return;
		case 0x99: case 0x9f:				m_pic_number = 6;	return;
		case 0x9a:							m_pic_number = 11;	return;
	}
}

void C3DPickupItem::v268(int state)							// 0x4360b0 NextState
{
	// ("Calling NextState for %s": debug log)
	if (state == 0)
	{
		OMediaElement::hide(true);
		v232();												// +0x3a4
		v136(0);
		return;
	}
	if (state != 1) return;
	const int idx = m_pickup_index;
	if (idx > 4199) return;
	if (idx != 0)
	{
		if (idx < 0) return;								// ("ERROR: Incalid PickupIndex for %s", debug log)
		if (g_object_states[idx] != 0) g_object_states[idx] = 0;
	}
	if (m_sprite_index != 0x6a) force_show();
	v234();													// +0x3ac
}

// --------------------------------------------------------------------------- element-family slots

unsigned char C3DPickupItem::CanPickUp(void)				// 0x436830
{
	if (m_required_pic_num == -1) return 1;
	if (GetItemState(2, (short)m_required_pic_num) != 0 &&
		m_req_pic_num_amount <= PickupCount(m_required_pic_num))
	{
		AddPickupCount(m_required_pic_num, -m_req_pic_num_amount);
		if (PickupCount(m_required_pic_num) < 1) ClearItemState(2, (short)m_required_pic_num);
		return 1;
	}
	if (m_need_more_sound != -1 && !IsChannelPlaying(g_4eefd8))	// 0x47d890
		g_4eefd8 = PlayGameSoundIndex(-1, m_need_more_sound, false);
	return 0;
}

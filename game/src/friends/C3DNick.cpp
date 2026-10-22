/*  C3DNick - see C3DNick.h. Reconstructed from Neutron.exe N2:0x45a350..N2:0x45ab70. */
#include "C3DNick.h"
#include "C3DSkateBoard.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// N2:0x404dd0: set the type of item `item` of menu page `page`. The pickups come in pairs (10/21
// and 31/38): giving one also gives (and counts) the other. (A copy, as in CLoadLevel.cpp.)
static void SetItemState(short page, short item, short state)
{
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

// --------------------------------------------------------------------------- construction

C3DNick::C3DNick()											// N2:0x45a350
{
	SetName((char *)"C3DNICK");
	Log("C3DNick()");
	f_649 = 0;												// ev69 loads the animations later
	C3DNick::InitObject();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3NIC');							// 0x462220
	m_visible_range = 1000.0f;										// VisibleRange
	m_speed = 160.0f;											// speed
	m_state = 1;												// waiting
	m_ai_state = 1;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"WALK");					// running is walking
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	f_1d74 = new C3DSkateBoard;								// 0x92c bytes
	// (debug log of the new object: its CGameObject part, 0x92c, "C3DSkateBoard")
	v40(1);													// CGameObject +0xa0
	v42(1);
	f_1d78 = 0;
	f_1d7c = 0;
	// (then a call to the empty 0x472970)
}

C3DNick::~C3DNick()											// 0x4336a0
{
	// (debug log "Deleting C3DNick"; the skateboard is not deleted)
}

namespace neg {
CGameObject *New_C3DNick(void) { return new C3DNick; }					// '3NIC' (0x1d88 bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DNick::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DNick::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DNick::Process(float dt)								// 0x4339f0
{
	C3DFriends::Process(dt);								// N2:0x436eb0
	if (m_state != 3) return;									// patrolling only
	m_held = 0;												// not held
	if (f_1d74)												// the board under us
	{
		float p[4];
		v195(p);											// +0x30c
		f_1d74->v196(p[0], p[1], p[2], p[3]);				// +0x310
		v201(p);											// +0x324
		f_1d74->v202(p[0], p[1], p[2], p[3]);				// +0x328
	}
	f_1d78 += dt;
	if (!(f_1d78 > 5.0f)) return;							// 0x48d950
	const char *next;
	if (strcasecmp(m_anim_run, "SKATE") == 0)
	{
		m_speed = (float)((6 - f_1d7c) * 100);
		if (m_speed < 200.0f) m_speed = 200.0f;
		next = "GLIDE";
	}
	else
	{
		m_speed = (float)((9 - f_1d7c) * 100);
		if (m_speed < 400.0f) m_speed = 400.0f;
		next = "SKATE";
	}
	strcpy(m_anim_run, next);
	f_1d78 = 0;
}

void C3DNick::v267(int level)								// 0x4338a0
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	C3DAnimated::v267(level);								// 0x40e340
	switch (g_gametype->m_object_id)						// +0x490
	{
		case 'LV2A':
			ev70(scene >= 115 && scene <= 140);				// element +0x118
			return;
		case 'LEV1':
			ev70(scene >= 90 && scene <= 110);
			return;
		case 'LEV2':
		case 'LV2B':
			if (scene >= 140 && scene < 180)
			{
				if (scene == 162) return;					// left as it is
				ev70(1);
			}
			else ev70(0);
			return;
		case 'LV4A':
			ev70(scene >= 500);
			return;
		default:
			ev70(1);
			return;
	}
}

// --------------------------------------------------------------------------- element family

void C3DNick::ev69(void)									// 0x433730
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"WALK", (char *)"nickwalk.ase");		// +0xe0
	CreateAnim((char *)"STOP", (char *)"nickcoin.ase");
	CreateAnim((char *)"WAIT", (char *)"nickstop.ase");
	CreateTexture((char *)"nick.png", 0);					// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"STOP", 1);								// +0xe8
	v67(150.0f);											// CGameObject +0x10c: radius
	v64(0.5f);												// CGameObject +0x100
}

void C3DNick::ev90(void)									// N2:0x45aac0
{
	C3DFriends::ev90();									// 0x41ee20 (a jump to the empty 0x472970)
	ev72(m_anim_run, (char *)"SKATE");							// element +0x1a8
	strcpy(m_patrol_point, "NICPAT1");								// PatrolPoint
	m_dest = NULL;											// no destination yet
}

void C3DNick::ev145(void)									// 0x433b80
{
	// (debug log, empty)
	switch (GetTaskState("SCENE"))							// 0x45fea0
	{
		case 115:
			SetTaskState("SCENE", 120);						// 0x45f990
			return;
		case 125:											// another lap done
			SetTaskState("SCENE", 120);
			f_1d7c++;
			return;
		case 130:
			SetItemState(0, 6, 1);							// N2:0x404dd0
			SetTaskState("SCENE", 140);
			return;
		case 140:
		case 141:
			if (g_gametype->m_object_id == 'LV2A') return;	// +0x490
			SetTaskState("SCENE", 145);
			return;
		case 150:
			SetTaskState("SCENE", 145);
			return;
		case 160:
			SetItemState(1, 6, 1);
			SetTaskState("SCENE", 162);
			return;
		default:
			return;
	}
}

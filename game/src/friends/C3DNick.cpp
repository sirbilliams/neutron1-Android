/*  C3DNick - see C3DNick.h. Reconstructed from Neutron.exe 0x433490..0x433e70. */
#include "C3DNick.h"
#include "C3DSkateBoard.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../objects/C3DCheckPoint.h"
#include "../gen/globals.h"
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

// --------------------------------------------------------------------------- construction

C3DNick::C3DNick()											// 0x433490
{
	SetName((char *)"C3DNICK");
	Log("C3DNick()");
	f_649 = 0;												// ev69 loads the animations later
	C3DNick::InitObject();									// 0x415090 (called directly)
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
	m_board = new C3DSkateBoard;								// 0x92c bytes
	// (debug log of the new object: its CGameObject part, 0x92c, "C3DSkateBoard")
	v40(1);													// CGameObject +0xa0
	v42(1);
	m_board_timer = 0;
	m_laps = 0;
	// (then a call to the empty 0x472970)
}

C3DNick::~C3DNick()											// 0x4336a0
{
	// (debug log "Deleting C3DNick"; the skateboard is not deleted)
}

namespace neg {
CGameObject *New_C3DNick(void) { return new C3DNick; }					// '3NIC' (0xbbc bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DNick::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DNick::v266(void)									// 0x433820
{
	// (debug log, empty)
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990 / 0x45fea0 (writes back what it read)
	const int scene = GetTaskState(m_task_name);
	if (g_gametype->m_object_id == 'LV2A')
	{
		if (scene > 139) ev70(0);							// element +0x110
	}
	else if (g_gametype->m_object_id == 'LV2B' && scene > 161)
		ev70(0);
}

void C3DNick::Process(float dt)								// 0x4339f0
{
	C3DFriends::Process(dt);								// 0x41b950
	if (m_state != 3) return;									// patrolling only
	m_held = 0;												// not held
	if (m_board)												// the board under us
	{
		float p[4];
		v195(p);											// +0x310
		m_board->v196(p[0], p[1], p[2], p[3]);				// +0x314
		v201(p);											// +0x328
		m_board->v202(p[0], p[1], p[2], p[3]);				// +0x32c
	}
	m_board_timer += dt;
	if (!(m_board_timer > 5.0f)) return;							// 0x48d950
	const char *next;
	if (strcasecmp(m_anim_run, "SKATE") == 0)
	{
		m_speed = (float)((6 - m_laps) * 100);
		if (m_speed < 200.0f) m_speed = 200.0f;
		next = "GLIDE";
	}
	else
	{
		m_speed = (float)((9 - m_laps) * 100);
		if (m_speed < 400.0f) m_speed = 400.0f;
		next = "SKATE";
	}
	strcpy(m_anim_run, next);
	m_board_timer = 0;
}

void C3DNick::v267(int level)								// 0x4338a0
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	C3DAnimated::v267(level);								// 0x40e340
	switch (g_gametype->m_object_id)						// +0x490
	{
		case 'LV2A':
			ev70(scene >= 115 && scene <= 140);				// element +0x110
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
	CreateAnim((char *)"HITALK", (char *)"nicktalkboard.ase");	// +0xd8
	CreateAnim((char *)"HIWALK", (char *)"nickwalk.ase");
	CreateAnim((char *)"HISKATE", (char *)"nickskate1.ase");
	CreateAnim((char *)"HIGLIDE", (char *)"nickskate2.ase");
	CreateAnim((char *)"HISTOP", (char *)"nickcoin.ase");
	CreateAnim((char *)"HIWAIT", (char *)"nickstop.ase");
	CreateTexture((char *)"nick.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

void C3DNick::ev90(void)									// 0x433cb0
{
	C3DFriends::ev90();										// 0x41bc20
	ev72(m_anim_run, (char *)"SKATE");						// element +0x120
	strcpy(m_patrol_point, "NICPAT1");								// PatrolPoint
	m_timer += 100.0f;										// 0x48d96c (Friends' timer: no waiting)
	m_dest = NULL;											// no destination yet
	CGameObject *finish = FindObjectByTag("FINISHLINE");
	CGameObject *start = FindObjectByTag("STARTLINE");
	if (finish && finish->NameContains((char *)"C3DCHECKPOINT"))
	{
		C3DCheckPoint *c = static_cast<C3DCheckPoint *>(finish);
		c->m_count_a = 0;
		c->m_count_b = 0;
	}
	if (start && start->NameContains((char *)"C3DCHECKPOINT"))
	{
		if (GetTaskState("SCENE") != 140 && GetTaskState("SCENE") != 162)
			static_cast<C3DCheckPoint *>(start)->Enable();	// 0x414ae0
	}
	if (GetTaskState("SCENE") == 140)
	{
		ev70(0);											// element +0x110
		if (m_board) m_board->hide(true);					// (the board's own element)
		OpenConfirm(2);										// 0x406f90: the yes/no box
	}
}

void C3DNick::ev96(void)									// 0x433b80
{
	C3DFriends::ev96();										// 0x41b810
	// (debug log, empty)
	switch (GetTaskState("SCENE"))							// 0x45fea0
	{
		case 115:
			SetTaskState("SCENE", 120);						// 0x45f990
			return;
		case 125:											// another lap done
			SetTaskState("SCENE", 120);
			m_laps++;
			return;
		case 130:
			MenuSetItem(0, 6, 1);							// 0x4038c0
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
			MenuSetItem(1, 6, 1);
			MenuSetItem(2, 0x14, 1);
			AnnouncePickup(0x14, 0);						// 0x4061d0
			SetTaskState("SCENE", 162);
			return;
		default:
			return;
	}
}

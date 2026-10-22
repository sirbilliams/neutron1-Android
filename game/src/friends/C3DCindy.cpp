/*  C3DCindy - see C3DCindy.h. Reconstructed from Neutron.exe 0x414db0..0x4152a0. */
#include "C3DCindy.h"
#include "../Game.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../C2DInGameMenu.h"
#include "../InterfaceElement.h"

using namespace neg;

C3DCindy::C3DCindy()										// 0x414db0
{
	SetName((char *)"C3DCINDY");
	Log("C3DCindy()");
	f_649 = 0;												// ev69 loads the animations later (morph: f_6d0 stays 0)
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3CIN');							// 0x462220
	m_visible_range = 500.0f;								// VisibleRange
	m_speed = 160.0f;										// speed
	m_state = 2;											// patrolling
	m_ai_state = 2;											// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"WALK");				// running is walking
	C3DAI::ev72(m_anim_wag, (char *)"STOP");				// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	// (then a call to the empty 0x472970)
}

C3DCindy::~C3DCindy()										// 0x414f50
{
}

namespace neg {
CGameObject *New_C3DCindy(void) { return new C3DCindy; }				// '3CIN'
}

void C3DCindy::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DCindy::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DCindy::ev69(void)									// 0x414fa0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWAVE", (char *)"cindwave.ase");	// +0xd8
	CreateAnim((char *)"HIWALK", (char *)"cindwalk.ase");
	CreateAnim((char *)"HITALK", (char *)"cindtalk.ase");
	CreateAnim((char *)"HICHEER", (char *)"cindycheer.ase");
	CreateAnim((char *)"HITELE", (char *)"cindteleport.ase");
	CreateAnim((char *)"HISTOP", (char *)"cindstop.ase");
	CreateTexture((char *)"cindy.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

void C3DCindy::ev96(void)									// 0x4150d0
{
	C3DFriends::ev96();										// 0x41b810
	m_visible_range = 500.0f;								// (+0x704)
	if (GetTaskState("SCENE") == 260)
	{
		MenuSetItem(0, 7, 1);								// 0x4038c0
		SetTaskState("SCENE", 270);							// 0x45f990
	}
}

void C3DCindy::ev90(void)									// 0x415120
{
	C3DFriends::ev90();										// 0x41bc20
	if (GetTaskState("SCENE") == 270)
	{
		const int n = MenuPickupCount(7);					// 0x4061b0
		if (n > 0)
		{
			AddScore(n * 100);								// 0x42adc0
			MenuAddPickupCount(7, -n);						// 0x4061c0
			MenuClearItem(2, 7);							// 0x403910
		}
		OpenConfirm(4);										// 0x406f90
	}
}

void C3DCindy::v267(int level)								// 0x415180
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type == 'LEV4')
	{
		if (scene > 259 && scene < 280) { ev70(1); return; }
	}
	else if (type == 'LV3D')
	{
		if (scene > 399 && scene < 420) { ev70(1); return; }
	}
	else
	{
		if (type != 'LV4A') { ev70(1); return; }
		if (scene >= 500) { ev70(1); return; }
	}
	ev70(0);
}

/*  C3DFowl - see C3DFowl.h. Reconstructed from Neutron.exe 0x41b0a0..0x41b490, 0x4206c0. */
#include "C3DFowl.h"
#include "../Game.h"
#include "../CGameType.h"
#include "../SaveGame.h"

using namespace neg;

C3DFowl::C3DFowl()											// 0x41b0a0
{
	SetName((char *)"C3DFOWL");
	Log("C3DFowl()");
	f_649 = 0;												// ev69 loads the animations later (morph: f_6d0 stays 0)
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3FOW');							// 0x462220
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
	C3DAI::ev72(m_anim_walk, (char *)"WALK");
	// (then a call to the empty 0x472970)
}

C3DFowl::~C3DFowl()											// 0x41b250
{
}

namespace neg {
CGameObject *New_C3DFowl(void) { return new C3DFowl; }					// '3FOW'
}

void C3DFowl::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DFowl::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0
void C3DFowl::ev96(void)			{ C3DFriends::ev96(); }				// 0x4206c0 (AddScore(100), then slot 260: same as 0x41b810)

void C3DFowl::v267(int level)								// 0x41b380
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	C3DAnimated::v267(level);								// 0x40e340
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type == 'LEV4')
	{
		if (scene > 289 && scene < 310) { ev70(1); return; }
	}
	else if (type == 'LV4A')
	{
		if (scene >= 500) { ev70(1); return; }
	}
	else
	{
		if (type != 'LV4C') return;
		if (scene > 459 && scene < 470) { ev70(1); return; }
	}
	ev70(0);
}

void C3DFowl::ev69(void)									// 0x41b2a0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"fowlwalk.ase");	// +0xd8
	CreateAnim((char *)"HITALK", (char *)"fowltalk.ase");
	CreateAnim((char *)"HITELE", (char *)"fowlteleport.ase");
	CreateAnim((char *)"HICHEER", (char *)"fowlcheer.ase");
	CreateAnim((char *)"HISTOP", (char *)"fowlstop.ase");
	CreateTexture((char *)"fowl.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

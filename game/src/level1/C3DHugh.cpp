/*  C3DHugh - see C3DHugh.h. Reconstructed from Neutron.exe 0x420390..0x420700. */
#include "C3DHugh.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../SaveGame.h"

using namespace neg;

C3DHugh::C3DHugh()											// 0x420390
{
	SetName((char *)"C3DHUGH");
	Log("C3DHugh()");
	C3DHugh::InitObject();									// 0x420570 called directly
	C3DObject::SetObjectID('3HUG');							// 0x462220
	m_visible_range = 500.0f;											// VisibleRange
	m_speed = 160.0f;											// speed
	m_state = 2;												// patrolling
	m_ai_state = 2;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"WALK");					// running is walking
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	// (then a call to the empty 0x472970)
}

C3DHugh::~C3DHugh()											// 0x420520
{
}

namespace neg {
CGameObject *New_C3DHugh(void) { return new C3DHugh; }					// '3HUG' (0xbb0 bytes)
}

void C3DHugh::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DHugh::InitObject(void)								// 0x420570
{
	Log("InitObject()");
	C3DFriends::InitObject();								// 0x41b840
	InitAnim();												// element +0x110
	CreateAnim((char *)"HITALK", (char *)"hughtalk.ase");		// +0xe0
	CreateAnim((char *)"HIWALK", (char *)"hughwalk.ase");
	CreateAnim((char *)"HICOUNT", (char *)"hughcount.ase");
	CreateAnim((char *)"HISTOP", (char *)"hughstop.ase");
	CreateTexture((char *)"hugh.png", 0);					// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"STOP", 1);								// +0xe8
	v67(150.0f);											// +0x10c: radius
	v64(0.5f);												// +0x100
	// (then a call to the empty 0x472970)
}

void C3DHugh::v267(int level)								// 0x420650
{
	C3DAnimated::v267(level);								// 0x40e340
	if (g_gametype->m_object_id != 'LV4A') { ev70(1); return; }	// (+0x490 of the game type) shown outside level4a
	if (GetTaskState("SCENE") > 204) { ev70(1); return; }	// 0x45fea0: in level4a once the key is given
	ev70(0);
}

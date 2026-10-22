/*  C3DSheen - see C3DSheen.h. Reconstructed from Neutron.exe 0x43f920..0x43fe10. */
#include "C3DSheen.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../Game.h"
#include <string>

using namespace neg;

C3DSheen::C3DSheen()										// 0x43f920
{
	SetName((char *)"C3DSHEEN");
	Log("C3DSheen()");
	f_649 = 0;												// ev69 loads the animations later
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3SHE');							// 0x462220
	m_visible_range = 500.0f;											// VisibleRange
	m_speed = 160.0f;											// speed
	m_state = 2;												// patrolling
	m_ai_state = 2;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DSheen::InitPhysics();								// 0x42ab50 (called directly)
	C3DAI::ev72(m_anim_run, (char *)"WALK");					// he walks where the others run
	C3DAI::ev72(m_anim_wag, (char *)"STOP");
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
}

C3DSheen::~C3DSheen()										// 0x43fac0
{
}

namespace neg {
CGameObject *New_C3DSheen(void) { return new C3DSheen; }				// '3SHE' (0xbb0 bytes)
}

void C3DSheen::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DSheen::InitPhysics(void)	{ if (!f_4aa) C3DObject::InitPhysics(); }	// 0x42ab50
void C3DSheen::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DSheen::v267(int level)								// 0x43fbf0
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	bool show = true;
	if (type == 'LEV1') show = scene >= 280 && scene <= 299;
	else if (type == 'LEV2') show = scene >= 310 && scene <= 329;
	else if (type == 'LEV4') show = scene >= 360 && scene <= 389;
	ev70(show);												// element +0x110
}

void C3DSheen::ev96(void)									// 0x43fca0
{
	C3DFriends::ev96();									// 0x41b810
	const int scene = GetTaskState("SCENE");
	if (scene == 280) SetTaskState("SCENE", 290);
	else if (scene == 310)
	{
		C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(1);	// 0x4038c0(1, 4, 1): gadget item 4
		if (p) p->items[4].type = 1;
		SetTaskState("SCENE", 320);
	}
	else if (scene == 360) SetTaskState("SCENE", 380);
}

void C3DSheen::ResetObject(void)							// 0x43fd10
{
	float p[4];
	v88(p);													// +0x164: the start position
	v196(p[0], p[1], p[2], p[3]);							// +0x314
	float q[4];
	v195(q); const float qz = q[2];							// +0x310, three times (as shipped)
	v195(q); const float qy = q[1];
	v195(q);
	x = q[0];												// the element's position (2002 +0x44..)
	y = qy;
	z = qz;
	InitPhysics();											// +0x2c (slot 11)
	C3DFriends::ResetObject();								// 0x41b790
	m_dest = NULL;											// no destination
}

void C3DSheen::ev69(void)									// 0x43fb10
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIPICK", (char *)"shenpick.ase");	// +0xd8
	CreateAnim((char *)"HIWAVE", (char *)"shenwave.ase");
	CreateAnim((char *)"HIWALK", (char *)"shenwalk.ase");
	CreateAnim((char *)"HITALK", (char *)"shentalk.ase");
	CreateAnim((char *)"HISTOP", (char *)"shenstop.ase");
	CreateTexture((char *)"sheen.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

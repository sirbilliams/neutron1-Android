/*  C3DUltraLord - see C3DUltraLord.h. Reconstructed from Neutron.exe 0x448310..0x448760,
    0x415090 / 0x419aa0 (shared). */
#include "C3DUltraLord.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../SaveGame.h"

using namespace neg;

C3DUltraLord::C3DUltraLord()								// 0x448310
{
	SetName((char *)"C3DULTRALORD");
	Log("C3DUltraLord()");
	f_649 = 0;												// ev69 loads the animations later
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3ULT');							// 0x462220
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

C3DUltraLord::~C3DUltraLord()								// 0x4484b0
{
}

namespace neg {
CGameObject *New_C3DUltraLord(void) { return new C3DUltraLord; }		// '3ULT' (0x1d7c bytes)
}

void C3DUltraLord::InitObject(void)		{ FriendInitObjectAnim(); }		// 0x415090
void C3DUltraLord::v266(void)			{ FriendTaskStateRewrite(); }	// 0x419aa0

void C3DUltraLord::v267(int level)							// 0x448670
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	if (g_gametype->m_object_id == 'LEV3')					// (+0x490 of the game type)
	{
		if (scene > 319 && scene < 410) { ev70(1); return; }
	}
	else
	{
		if (g_gametype->m_object_id != 'LV4A') { ev70(1); return; }
		if (scene >= 500) { ev70(1); return; }
	}
	ev70(0);
}

void C3DUltraLord::ev69(void)								// 0x448500
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"ultrawalk.ase");	// +0xe0
	CreateAnim((char *)"HIFLEX1", (char *)"ultraflex.ase");
	CreateAnim((char *)"HIFLEX2", (char *)"ultraflex2.ase");
	CreateAnim((char *)"HIFLEX3", (char *)"ultraflex3.ase");
	CreateAnim((char *)"HITALK", (char *)"ultratalk.ase");
	CreateAnim((char *)"HIGIVE", (char *)"ultragive.ase");
	CreateAnim((char *)"HIWHISPER", (char *)"ultrawhisper.ase");
	CreateAnim((char *)"HISTOP", (char *)"ultrastop.ase");
	CreateTexture((char *)"ultralord.png", 0);				// +0xfc
	ev58((char *)"STOP", 1);								// +0xe8
	ApplyTexture(f_590, 0);									// +0x100
	v67(150.0f);											// CGameObject +0x10c: radius
	v64(0.5f);												// CGameObject +0x100
}

void C3DUltraLord::ev96(void)								// 0x448620
{
	C3DFriends::ev96();										// 0x41b810
	if (GetTaskState("SCENE") == 390)
	{
		MenuSetItem(0, 1, 1);								// 0x4038c0
		MenuToggleItem(0, 1, 1);							// 0x403870
	}
}

void C3DUltraLord::ev90(void)								// 0x448660
{
	m_state = 3;											// (after the empty 0x472970 call)
}

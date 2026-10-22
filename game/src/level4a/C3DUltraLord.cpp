/*  C3DUltraLord - see C3DUltraLord.h. Reconstructed from Neutron.exe 0x448310..N2:0x47a6d0,
    0x415090 / 0x419aa0 / N2:0x44fa60 (shared). */
#include "C3DUltraLord.h"
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

void C3DUltraLord::v267(int level)							// N2:0x47a660
{
	C3DAnimated::v267(level);								// 0x40e340
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
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

void C3DUltraLord::ev145(void)								// N2:0x44fa60
{
	FriendTouchScene();										// GetTaskState("SCENE") (unused)
}

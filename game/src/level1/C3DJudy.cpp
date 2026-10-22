/*  C3DJudy - see C3DJudy.h. Reconstructed from Neutron.exe 0x432730..0x42bc00, N2:0x47a660. */
#include "C3DJudy.h"
#include "../Game.h"
#include "../SaveGame.h"

using namespace neg;

C3DJudy::C3DJudy()											// 0x432730
{
	SetName((char *)"C3DJUDY");
	Log("C3DJUDY()");
	f_649 = 0;												// ev69 loads the animations later
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3MOM');							// 0x462220
	f_9a8 = 500.0f;											// VisibleRange
	f_968 = 160.0f;											// speed
	f_96c = 2;												// patrolling
	f_c08 = 2;												// AIState
	f_978.x = 0;
	f_978.y = 0;
	f_978.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev106(f_ac4, (char *)"WALK");					// running is walking
	C3DAI::ev106(f_a9c, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev106(f_b14, (char *)"STOP");
	// (then a call to the empty 0x472970)
}

C3DJudy::~C3DJudy()											// 0x42b470
{
}

namespace neg {
CGameObject *New_C3DJudy(void) { return new C3DJudy; }					// '3MOM' (0x1d7c bytes)
}

void C3DJudy::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DJudy::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DJudy::v267(int level)								// N2:0x47a660
{
	C3DAnimated::v267(level);								// 0x40e340
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
}

void C3DJudy::ev69(void)									// 0x42b4c0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"TALK", (char *)"judytalk.ase");		// +0xe0
	CreateAnim((char *)"WALK", (char *)"judywalk.ase");
	CreateAnim((char *)"STOP", (char *)"judystop.ase");
	CreateTexture((char *)"judy.png", 0);					// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"STOP", 1);								// +0xe8
	v67(100.0f);											// CGameObject +0x10c: radius
	v64(0.75f);												// CGameObject +0x100
}

void C3DJudy::ev145(void)									// N2:0x44ec00
{
	// The original tests the byte at element +0xd6, then calls the empty 0x472970 and
	// GetTaskState("SCENE") (0x45fea0), whose result is not used: no effect.
}

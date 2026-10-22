/*  C3DBenny - see C3DBenny.h. Reconstructed from Neutron.exe 0x410340..0x420060. */
#include "C3DBenny.h"
#include "../Game.h"

using namespace neg;

C3DBenny::C3DBenny()										// 0x410340
{
	SetName((char *)"C3DBENNY");
	Log("C3DBENNY()");
	f_649 = 0;												// ev69 loads the animations later (morph: f_6d0 stays 0)
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3BEN');							// 0x462220
	f_9a8 = 500.0f;											// VisibleRange
	f_968 = 160.0f;											// speed
	f_96c = 2;												// patrolling
	f_c08 = 2;												// AIState
	f_978.x = 0;
	f_978.y = 0;
	f_978.z = 450.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev106(f_ac4, (char *)"WALK");					// running is walking
	C3DAI::ev106(f_a9c, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev106(f_b14, (char *)"STOP");
	C3DAI::ev106(f_b3c, (char *)"WALK");
	f_1d60.y = -5.0f;										// the shadow
	// (then a call to the empty 0x472970)
}

C3DBenny::~C3DBenny()										// 0x4104f0
{
}

namespace neg {
CGameObject *New_C3DBenny(void) { return new C3DBenny; }				// '3BEN' (0x1d7c bytes)
}

void C3DBenny::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DBenny::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0
void C3DBenny::v267(int level)		{ FriendLevelSceneShow(level); }	// 0x4418e0
void C3DBenny::ev145(void)			{ FriendTouchScene(); }				// N2:0x44fa60

void C3DBenny::UnInitObject(void)							// 0x410580
{
	Log("C3DBenny::UnInitObject()");
	C3DAnimated::UnInitObject();							// N2:0x41ab10
	// (then a call to the empty 0x472970)
}

void C3DBenny::ev69(void)									// 0x4105b0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"WIPE", (char *)"bennywipe.ase");	// +0xe0
	CreateAnim((char *)"STOP", (char *)"bennystop.ase");
	CreateTexture((char *)"benny.png", 0);					// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"WIPE", 1);								// +0xe8
	v67(150.0f);											// CGameObject +0x10c: radius
	v64(0.1f);												// CGameObject +0x100
}

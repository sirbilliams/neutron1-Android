/*  C3DBenny - see C3DBenny.h. Reconstructed from Neutron.exe 0x410340..0x410810. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBenny.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DBenny::C3DBenny()										// 0x410340
{
	SetName((char *)"C3DBENNY");
	Log("C3DBENNY()");
	f_649 = 0;												// ev69 loads the animations later
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
	C3DAI::ev106(f_ac4, (char *)"WALK");
	C3DAI::ev106(f_a9c, (char *)"STOP");
	C3DAI::ev106(f_b14, (char *)"STOP");
	C3DAI::ev106(f_b3c, (char *)"WALK");
}

C3DBenny::~C3DBenny()										// 0x4104f0
{
}

namespace neg {
CGameObject *New_C3DBenny(void) { return new C3DBenny; }				// '3BEN' (0xbb0 bytes)
}

void C3DBenny::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DBenny::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DBenny::UnInitObject(void)							// 0x410580
{
	Log("C3DBenny::UnInitObject()");
	C3DAnimated::UnInitObject();
}

void C3DBenny::v267(int level)								// 0x4106a0
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type == 'LEV1') ev70(scene > 89 && scene < 110);	// element +0x110
	else if (type == 'LEV2') ev70((scene > 339 && scene < 351) || (scene > 89 && scene < 169));
	else if (type == 'LV4A') ev70(scene > 499);
	else ev70(1);
}

void C3DBenny::ev145(void)									// 0x410760
{
	C3DFriends::ev145();									// 0x41b810
	const int scene = GetTaskState("SCENE");
	if (scene == 110) SetTaskState("SCENE", 115);
	else if (scene == 140) SetTaskState("SCENE", 141);
	else if (scene == 340) SetTaskState("SCENE", 350);
}

void C3DBenny::ev69(void)									// 0x4105b0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"bennywalk.ase");	// +0xd8
	CreateAnim((char *)"HITALK", (char *)"bennytalk.ase");
	CreateAnim((char *)"HIPHONE", (char *)"bennyphone.ase");
	CreateAnim((char *)"HIWIPE", (char *)"bennywipe.ase");
	CreateAnim((char *)"HIWPHONE", (char *)"bennywipephone.ase");
	CreateAnim((char *)"HISTOP", (char *)"bennystop.ase");
	CreateTexture((char *)"benny.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

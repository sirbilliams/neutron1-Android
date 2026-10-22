/*  C3DJudy - see C3DJudy.h. Reconstructed from Neutron.exe 0x42b2d0..0x42b7f0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DJudy.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DJudy::C3DJudy()											// 0x42b2d0
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
	C3DAI::ev106(f_ac4, (char *)"WALK");
	C3DAI::ev106(f_a9c, (char *)"STOP");
	C3DAI::ev106(f_b14, (char *)"STOP");
}

C3DJudy::~C3DJudy()											// 0x42b470
{
}

namespace neg {
CGameObject *New_C3DJudy(void) { return new C3DJudy; }					// '3MOM' (0xbb0 bytes)
}

void C3DJudy::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DJudy::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0

void C3DJudy::ResetObject(void)								// 0x42b630
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
	C3DFriends::ResetObject();								// 0x41b790
	// ("ResetObj for %s is %d": debug log)
	f_b8c = NULL;											// no destination
}

void C3DJudy::v267(int level)								// 0x42b6f0
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type == 'LEV1') ev70(scene > 199 && scene < 260);	// element +0x110
	else if (type == 'LV4A') ev70(scene > 499);
	else ev70(1);
}

void C3DJudy::ev145(void)									// 0x42b590
{
	if (!f_016) return;										// (element +0xd6: she collides)
	C3DFriends::ev145();									// 0x41b810
	const int scene = GetTaskState("SCENE");
	if (scene == 200)
	{
		SetTaskState("SCENE", 205);
		C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(1);	// 0x4038c0(1, 3, 1): gadget item 3
		if (p) p->items[3].type = 1;
	}
	else if (scene == 210) SetTaskState("SCENE", 220);
	else if (scene == 230)
	{
		SetTaskState("SCENE", 250);
		g_pickup_counts[2 * 18] += -1;						// 0x4061c0
		if (g_pickup_counts[2 * 18] < 1)					// 0x4061b0
		{
			C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(2);	// 0x403910(2, 18)
			if (p) p->items[18].type = 0;
		}
	}
}

void C3DJudy::ev69(void)									// 0x42b4c0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIFIX", (char *)"judyfix.ase");		// +0xd8
	CreateAnim((char *)"HITALK", (char *)"judytalk.ase");
	CreateAnim((char *)"HIWALK", (char *)"judywalk.ase");
	CreateAnim((char *)"HISTOP", (char *)"judystop.ase");
	CreateTexture((char *)"judy.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(100.0f);											// CGameObject +0x110: radius
	v64(0.75f);												// CGameObject +0x100
}

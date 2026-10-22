/*  C3DLibby - see C3DLibby.h. Reconstructed from Neutron.exe 0x42cd40..0x42d410. */
#include "C3DLibby.h"
#include "../Game.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../gen/globals.h"

using namespace neg;

C3DLibby::C3DLibby()										// 0x42cd40
{
	SetName((char *)"C3DLIBBY");
	Log("C3DLibby()");
	f_649 = 0;												// ev69 loads the animations later (morph: f_6d0 stays 0)
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3LIB');							// 0x462220
	f_9a8 = 500.0f;											// VisibleRange
	f_968 = 200.0f;											// speed
	f_96c = 3;												// patrolling only
	f_c08 = 3;												// AIState
	f_978.x = 0;
	f_978.y = 0;
	f_978.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev106(f_ac4, (char *)"RUN");
	C3DAI::ev106(f_a9c, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev106(f_b14, (char *)"STOP");
	// (then a call to the empty 0x472970)
}

C3DLibby::~C3DLibby()										// 0x42cee0
{
}

namespace neg {
CGameObject *New_C3DLibby(void) { return new C3DLibby; }				// '3LIB' (0xbb0 bytes)
}

void C3DLibby::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DLibby::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0
void C3DLibby::v260(void)			{ C3DAI::v260(); }					// 0x42d010 (jmp 0x409480)

// 0x4038c0 / 0x4061d0 (as in C3DPickupItem.cpp): menu item, and the pickup pop-up.
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 29) return;
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;
}
static void AnnouncePickupBG(int pic, unsigned int index)
{
	if (g_4f815c <= 0)
	{
		g_4f8158 = -1;
		g_4f8154 = pic;
		g_4f815c = 3.0f;
		g_4f8160 = index;
	}
	else if (g_4f8154 != -1) g_4f8158 = pic;
	g_pickup_counts[2 * pic]++;								// 0x4f7db8
}

void C3DLibby::ev126(void)									// 0x42d020
{
	C3DFriends::ev126();									// 0x41bc20
	if (GetTaskState("SCENE") == 260)
	{
		SetItemState(2, 7, 1);
		AnnouncePickupBG(7, 0);
	}
}

void C3DLibby::ev145(void)									// 0x42d060
{
	C3DFriends::ev145();									// 0x41b810
	switch (GetTaskState("SCENE"))
	{
		case 255: SetTaskState("SCENE", 260); return;
		case 300: SetTaskState("SCENE", 310); return;
		case 350: SetTaskState("SCENE", 360); return;
		case 400: SetTaskState("SCENE", 410); return;
	}
}

void C3DLibby::v267(int level)								// 0x42d230
{
	C3DAnimated::v267(level);								// 0x40e340
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type > 'LV4A')
	{
		if (type == 'LV4D') { ev106(f_ac4, (char *)"WALK"); ev70(1); return; }	// element +0x120, +0x110
		ev70(1);
		return;
	}
	if (type == 'LV4A')
	{
		if (scene >= 500) { ev70(1); return; }
	}
	else if (type == 'LEV1')
	{
		if (scene >= 250 && scene < 270)
		{
			if (scene == 250) ev107(3);						// element +0x124
			ev70(1);
			return;
		}
		if (scene >= 300 && scene < 370)
		{
			ev106(f_ac4, (char *)"WALK");
			ev107(0);
			ev70(1);
			return;
		}
	}
	else
	{
		if (type != 'LV3C') { ev70(1); return; }
		if (scene >= 400 && scene < 420) { ev106(f_ac4, (char *)"WALK"); ev70(1); return; }
	}
	ev70(0);
}

void C3DLibby::ResetObject(void)							// 0x42d190 (as C3DSheen's, without InitPhysics)
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
	f_b8c = NULL;											// no destination
}

void C3DLibby::ev69(void)									// 0x42cf30
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HITALK", (char *)"libytalk.ase");	// +0xd8
	CreateAnim((char *)"HIWALK", (char *)"libywalk.ase");
	CreateAnim((char *)"HIRUN", (char *)"libyrun.ase");
	CreateAnim((char *)"HIPHONE", (char *)"libbyphone.ase");
	CreateAnim((char *)"HISTOP", (char *)"libystop.ase");
	CreateTexture((char *)"libby.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(150.0f);											// CGameObject +0x110: radius
	v64(0.5f);												// CGameObject +0x100
}

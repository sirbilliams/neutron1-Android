/*  C3DCarl - see C3DCarl.h. Reconstructed from Neutron.exe 0x413af0..0x414180. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DCarl.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// Move `o` to where `to` is (+0x310 position, +0x314 set).
static void MoveTo(CGameObject *o, CGameObject *to)
{
	float p[4];
	to->v195(p);
	o->v196(p[0], p[1], p[2], p[3]);
}

C3DCarl::C3DCarl()											// 0x413af0
{
	SetName((char *)"C3DCARL");
	Log("C3DCarl()");
	f_649 = 0;												// ev69 loads the animations later
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3CAR');							// 0x462220
	f_9a8 = 500.0f;											// VisibleRange
	f_968 = 200.0f;											// speed
	f_96c = 1;												// waiting
	f_c08 = 1;												// AIState
	f_978.x = 0;
	f_978.y = 0;
	f_978.z = 400.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev106(f_ac4, (char *)"WALK");					// running is walking
	C3DAI::ev106(f_b3c, (char *)"WALK");
	C3DAI::ev106(f_a9c, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev106(f_b14, (char *)"STOP");
}

C3DCarl::~C3DCarl()											// 0x413ca0
{
	// ("Delete C3DCarl": debug log)
}

namespace neg {
CGameObject *New_C3DCarl(void) { return new C3DCarl; }					// '3CAR' (0xbb0 bytes)
}

void C3DCarl::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090

void C3DCarl::UnInitObject(void)							// 0x413d30
{
	Log("UnInitObject()");
	C3DAnimated::UnInitObject();
}

void C3DCarl::v266(void)									// 0x413eb0
{
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990 / 0x45fea0 (writes back what it read)
	if (GetTaskState("SCENE") == 70)
	{
		CGameObject *c8 = FindObjectByTag("CARL8");
		if (c8) MoveTo(this, c8);
	}
}

void C3DCarl::v267(int level)								// 0x413f30
{
	const int scene = GetTaskState("SCENE");				// 0x45fea0 (before the base call)
	C3DAnimated::v267(level);								// 0x40e340
	const int type = g_gametype->m_object_id;				// (+0x490 of the game type)
	if (type < 'LEV4')
	{
		bool show;
		if (type == 'LEV3') show = scene > 379 && scene < 390;
		else if (type == 'LEV1') show = scene > 59 && scene < 90;
		else if (type == 'LEV2') show = scene > 269 && scene < 290;
		else show = true;
		ev70(show);											// element +0x110
		return;
	}
	if (type == 'LV1B')
	{
		if (scene >= 0 && scene < 61)
		{
			// ("Showing Carl": debug log)
			CGameObject *esc;
			if (scene > 30 && (esc = FindObjectByTag("CARLESC3")) != NULL)
			{
				ev107(6);									// element +0x124
				MoveTo(this, esc);
			}
			if (scene > 59) return;							// (sic: left as it is)
			ev70(1);
			return;
		}
		ev70(0);
		return;
	}
	if (type == 'LV4A') { ev70(scene > 499); return; }
	ev70(1);
}

void C3DCarl::ev145(void)									// 0x413e50
{
	C3DFriends::ev145();									// 0x41b810
	const int scene = GetTaskState("SCENE");
	if (scene == 50) SetTaskState("SCENE", 60);
	else if (scene == 65) SetTaskState("SCENE", 70);
	else if (scene == 270) SetTaskState("SCENE", 280);
}

void C3DCarl::ev69(void)									// 0x413d60
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"carlwalk.ase");	// +0xd8
	CreateAnim((char *)"HITALK", (char *)"carltalk.ase");
	CreateAnim((char *)"HITELE", (char *)"carlteleport.ase");
	CreateAnim((char *)"HICHEER", (char *)"carlcheer.ase");
	CreateAnim((char *)"HIINHALE", (char *)"carlinhale.ase");
	CreateAnim((char *)"HISTOP", (char *)"carlstop.ase");
	CreateTexture((char *)"carl.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"STOP", 1);								// +0xe0
	v67(100.0f);											// CGameObject +0x110: radius
	v64(0.75f);												// CGameObject +0x100
}

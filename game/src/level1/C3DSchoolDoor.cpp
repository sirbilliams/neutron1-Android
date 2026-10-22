/*  C3DSchoolDoor - see C3DSchoolDoor.h. Reconstructed from Neutron.exe 0x43f070..0x43f640. */
#include "C3DSchoolDoor.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../C2DInGameMenu.h"
#include "../gen/globals.h"
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

C3DSchoolDoor::C3DSchoolDoor()								// 0x43f070
{
	SetName((char *)"C3DSCHOOLDOOR");
	Log("C3DShoolDoor()");									// (sic)
	v52(1);
	v42(0);
	v40(0);
	v94(2);
	v46(0);
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3SCD');							// 0x462220
	strcpy(f_6c5, "doorfowl.ase");
	strcpy(f_729, "doorfowl.png");
	C3DSchoolDoor::InitObject();							// 0x43f280 (called directly)
	f_6bc = 0;
	f_6c4 = 0;
	f_6c0 = 0;
	v52(1);
	// (then a call to the empty 0x472970)
}

C3DSchoolDoor::~C3DSchoolDoor()								// 0x43f230
{
}

namespace neg {
CGameObject *New_C3DSchoolDoor(void) { return new C3DSchoolDoor; }		// '3SCD' (0x798 bytes)
}

void C3DSchoolDoor::InitObject(void)						// 0x43f280
{
	Log("C3DShrinkRay::InitObject()");						// (sic)
	C3DAnimated::InitObject();
	RegisterProperty("ASEFile", f_6c5, PROP_ALPHA, 0);
	RegisterProperty("PNGFile", f_729, PROP_ALPHA, 0);
	// (then a call to the empty 0x472970)
}

// The door opens: it squeaks and turns for 1.8 s (the first half of 0x43f2e0's repeated tail).
static void SchoolDoorOpen(C3DSchoolDoor *d)
{
	d->f_6bc = 1;
	d->f_6c0 = 1.8f;										// 0x3fe66666
	PlayGameSound(-1, std::string("squeak"), false);		// 0x40acc0 (the name), 0x458b40
}

void C3DSchoolDoor::AddTouching(CGameObject *other)			// 0x43f2e0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY") || f_6bc) return;
	// (no early out after the first case: the SCENE rules below can open it again)
	if (g_gametype->m_object_id == 'LV1C') SchoolDoorOpen(this);	// game type +0x490
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	if (scene >= 0x1a4)
	{
		SchoolDoorOpen(this);
		if (scene == 0x1a4 && strcasecmp("FOWLROOM", m_tag) == 0)	// 0x4f0a24
			SetTaskState("SCENE", 0x1ae);					// 0x45f990
		return;
	}
	if (strcasecmp(m_tag, "SHOWMEDOOR") == 0)			// 0x4f0a18
	{
		if (MenuPickupCount(0x1b) < 2) return;				// 0x4061b0
		SchoolDoorOpen(this);
		return;
	}
	if (scene < 0x14a) return;
	if (g_gametype->m_object_id != 'LEV3') return;
	SchoolDoorOpen(this);
}

void C3DSchoolDoor::Process(float dt)						// 0x43f480
{
	if (!f_6bc) return;										// (no pause check)
	f_6c0 -= dt;
	if (!f_6c4)
	{
		AddRotation(0, dt * 50.0f, 0);						// +0x334; 0x48d400
		if (f_6c0 <= 0)
		{
			f_6bc = 0;
			f_6c4 = 1;
			f_6c0 = 0;
		}
	}
	else
	{
		AddRotation(0, dt * -50.0f, 0);						// 0x4a3e78
		if (f_6c0 <= 0)
		{
			f_6bc = 0;
			f_6c4 = 0;
			f_6c0 = 0;
		}
	}
}

void C3DSchoolDoor::v260(void)								// 0x43f540
{
	C3DAnimated::v260();									// 0x40e7b0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIRAY", f_6c5);						// +0xd8 (sic: played as "RAY")
	CreateTexture(f_729, 0);								// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(40.0f);												// CGameObject +0x110: radius
	ev58((char *)"RAY", 1);									// +0xe0
}

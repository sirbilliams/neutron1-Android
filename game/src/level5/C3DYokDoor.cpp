/*  C3DYokDoor - see C3DYokDoor.h. Reconstructed from Neutron.exe 0x449cb0..0x44a440. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokDoor.h"
#include "../objects/C3DAITrigger.h"
#include "../objects/C3DCutSceneCamera.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include "OMedia3DShape.h"
#include "OMediaStreamOperators.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// --------------------------------------------------------------------------- construction

C3DYokDoor::C3DYokDoor()									// 0x449cb0
{
	SetName((char *)"C3DYOKDOOR");
	Log("C3DYokDoor()");
	C3DYokDoor::InitObject();								// 0x449f70 (called directly)
	C3DObject::SetObjectID('3DOR');							// 0x462220
	v52(1);
	v42(0);
	v40(0);
	v94(2);													// (BG slot 95)
	v46(0);
	MakeShapeUnLit(get_shape());							// 0x477550
	f_648 = 0;												// animations off
	f_924 = 1;												// ItemClosed
	f_928 = 1;
	strcpy(f_92c, "none");									// Next
	f_9f8 = 0;
	f_9fc = 5.0f;											// DoorSpeed
	f_a00 = 3.0f;											// OpenTime
	f_a04 = 0;
	f_a08 = 0;
	f_a0c = 500.0f;											// OpenAmount
	strcpy(f_990, "objects.omt");							// OmtDatabase
	f_9f4 = 0;												// OmtIndex
	f_a10 = -1;
	v34(0);
	// (then a call to the empty 0x472970)
}

C3DYokDoor::~C3DYokDoor() {}								// 0x449ec0

namespace neg {
CGameObject *New_C3DYokDoor(void) { return new C3DYokDoor; }		// '3DOR' (0x7b4 bytes)
}

// --------------------------------------------------------------------------- slots

void C3DYokDoor::InitObject(void)							// 0x449f70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ItemClosed",  &f_924, PROP_LONG,  0);
	RegisterProperty("Next",        f_92c,  PROP_ALPHA, 0);
	RegisterProperty("DoorSpeed",   &f_9fc, PROP_FLOAT, 0);
	RegisterProperty("OpenTime",    &f_a00, PROP_FLOAT, 0);
	RegisterProperty("OpenAmount",  &f_a0c, PROP_FLOAT, 0);
	RegisterProperty("OmtDatabase", f_990,  PROP_ALPHA, 0);
	RegisterProperty("OmtIndex",    &f_9f4, PROP_LONG,  0);
	f_648 = 0;												// animations off
	InitPhysics();											// +0x2c
	// (then a call to the empty 0x472970)
}

void C3DYokDoor::ResetObject(void)							// 0x449f10
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	f_928 = f_924;
	if (f_924 == 0)
	{
		float p[4];
		v88(p);												// the initial position
		v197(p[0], p[1] + f_a0c, p[2]);						// +0x314: open
	}
}

void C3DYokDoor::AddTouching(CGameObject *other)			// 0x44a0d0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DYOKIAN")) v268(0);	// +0x430
}

void C3DYokDoor::RemoveTouching(CGameObject *other)		// 0x44a100 (shared code)
{
	CGameObject::RemoveTouching(other);						// 0x470b20
}

// Moving: y = start + OpenAmount * (the fraction of the way open). Stopped open: close after
// OpenTime.
void C3DYokDoor::Process(float dt)							// 0x44a110
{
	C3DAnimated::Process(dt);								// 0x40e050 (no pause check, unlike Negatron's)
	if (f_9f8 > 0)											// 0x48d914
	{
		f_9f8 = f_9f8 - dt;
		if (f_9f8 <= 0)
		{
			f_9f8 = 0;
			StopGameSound(f_a10, false);					// 0x458a00
			f_a10 = -1;
			PlayGameSound3DIndex(this, -1, 0x3a, false);		// 0x4589c0
		}
		const float t = f_9f8 / f_9fc;
		float p[4];
		if (f_928)
		{
			v88(p);
			v197(p[0], p[1] + t * f_a0c, p[2]);				// +0x314
			return;
		}
		f_a04 = 0;
		v88(p);
		v197(p[0], p[1] + (1.0f - t) * f_a0c, p[2]);		// 0x48d924
		return;
	}
	if (f_928) return;
	f_a04 = dt + f_a04;
	if (f_a04 > f_a00)
	{
		f_a04 = 0;
		v268(1);
	}
}

void C3DYokDoor::v260(void)									// 0x44a060
{
	C3DAnimated::v260();									// (BG slot 259)
	m_db = GetDatabase(f_990);								// 0x46a910
	set_shape(Load3DShape(m_db, f_9f4));					// element +0xac (0x477ba0)
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	ev51();													// element +0xcc
}

void C3DYokDoor::v268(int state)							// 0x44a2a0 (BG slot 266)
{
	if (f_9f8 > 0)
	{
		if (f_928 == state) return;
		f_9f8 = f_9fc - f_9f8;								// turn back from where it is
	}
	else
	{
		if (f_928 == state) return;
		f_9f8 = f_9fc;
		if (f_a10 == -1) f_a10 = PlayGameSound3DIndex(this, -1, 0x3b, true);	// 0x4589c0
	}
	if (strcasecmp(f_92c, "none") != 0)						// 0x487c20 _stricmp
	{
		CGameObject *next = FindObjectByTag(f_92c);
		if (next)
		{
			if (next->NameContains((char *)"C3DAITRIGGER"))
				static_cast<C3DAITrigger *>(next)->Trigger(this, 0);				// BG: the direct call 0x40c300(this)
			else if (next->NameContains((char *)"C3DCUTSCENECAMERA"))
			{
				static_cast<C3DCutSceneCamera *>(next)->TriggerCamera(0);			// element +0xd8 (BG slot 54)
			}
		}
	}
	if (state == 0) f_928 = 0;
	else if (state == 1) f_928 = 1;
	else f_928 = f_928 == 0;
}

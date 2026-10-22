/*  C3DDoorUpDown - see C3DDoorUpDown.h. Reconstructed from Neutron.exe 0x4174f0..0x417d30. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DDoorUpDown.h"
#include "../objects/C3DAITrigger.h"
#include "../objects/C3DCutSceneCamera.h"
#include "../objects/C3DMultiCutSceneCamera.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
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

C3DDoorUpDown::C3DDoorUpDown()								// 0x4174f0
{
	SetName((char *)"C3DDOORUPDOWN");
	Log("C3DDoorUpDown()");
	v52(1);
	v42(0);
	v40(0);
	v94(2);													// (BG slot 95)
	v46(0);
	f_924 = 1;												// ItemClosed
	f_928 = 1;
	strcpy(f_92c, "none");									// Next
	f_990 = 0;
	f_994 = 5.0f;											// DoorSpeed
	f_998 = 3.0f;											// OpenTime
	f_99c = 0;
	f_a74 = 1;												// TouchActivated
	f_9a0 = 0;
	f_9a8 = 500.0f;											// OpenAmount
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3DUD');							// 0x462220
	strcpy(f_9ac, "doorfowl.ase");
	strcpy(f_a10, "doorfowl.png");
	C3DDoorUpDown::InitObject();							// 0x4177c0 (called directly)
	f_a78 = -1;
	v34(0);
	// (then a call to the empty 0x472970)
}

C3DDoorUpDown::~C3DDoorUpDown() {}							// 0x417710

namespace neg {
CGameObject *New_C3DDoorUpDown(void) { return new C3DDoorUpDown; }	// '3DUD' (0x81c bytes)
}

// --------------------------------------------------------------------------- slots

void C3DDoorUpDown::InitObject(void)						// 0x4177c0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ItemClosed",     &f_924, PROP_LONG,  0);
	RegisterProperty("Next",           f_92c,  PROP_ALPHA, 0);
	RegisterProperty("DoorSpeed",      &f_994, PROP_FLOAT, 0);
	RegisterProperty("OpenTime",       &f_998, PROP_FLOAT, 0);
	RegisterProperty("OpenAmount",     &f_9a8, PROP_FLOAT, 0);
	RegisterProperty("ASEFile",        f_9ac,  PROP_ALPHA, 0);
	RegisterProperty("PNGFile",        f_a10,  PROP_ALPHA, 0);
	RegisterProperty("TouchActivated", &f_a74, PROP_LONG,  0);
	// (then a call to the empty 0x472970)
}

void C3DDoorUpDown::ResetObject(void)						// 0x417760
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	f_928 = f_924;
	if (f_924 == 0)
	{
		float p[4];
		v88(p);												// the initial position
		v197(p[0], p[1] + f_9a8, p[2]);						// +0x314: open
	}
}

void C3DDoorUpDown::AddTouching(CGameObject *other)			// 0x4178c0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DYOKIAN")) v268(0);	// +0x430
	if (other->NameContains((char *)"C3DJIMMY") && f_a74 != 0 && f_990 == 0) v268(0);
}

// Moving: y = start + OpenAmount * (the fraction of the way open). Stopped open: close after
// OpenTime.
void C3DDoorUpDown::Process(float dt)						// 0x417930
{
	C3DAnimated::Process(dt);								// 0x40e050 (no pause check, unlike Negatron's)
	if (f_990 > 0)											// 0x48d914
	{
		f_990 = f_990 - dt;
		if (f_990 <= 0)
		{
			f_990 = 0;
			StopGameSound(f_a78, false);					// 0x458a00
			f_a78 = -1;
			PlayGameSound3DIndex(this, -1, 0x3a, false);		// 0x4589c0
		}
		const float t = f_990 / f_994;
		float p[4];
		if (f_928)
		{
			v88(p);
			v197(p[0], p[1] + t * f_9a8, p[2]);				// +0x314
			return;
		}
		f_99c = 0;
		v88(p);
		v197(p[0], p[1] + (1.0f - t) * f_9a8, p[2]);		// 0x48d924
		return;
	}
	if (f_928) return;
	f_99c = dt + f_99c;
	if (f_99c > f_998)
	{
		f_99c = 0;
		v268(1);
	}
}

void C3DDoorUpDown::v260(void)								// 0x417c50
{
	C3DAnimated::v260();									// (BG slot 259)
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIRAY", f_9ac);						// +0xd8
	CreateTexture(f_a10, 0);								// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(40.0f);												// CGameObject +0x110: radius
	ev58((char *)"RAY", 1);									// +0xe0
}

void C3DDoorUpDown::v268(int state)							// 0x417ac0 (BG slot 266)
{
	if (f_990 > 0)
	{
		if (f_928 == state) return;
		f_990 = f_994 - f_990;								// turn back from where it is
	}
	else
	{
		if (f_928 == state) return;
		f_990 = f_994;
		if (f_a78 == -1) f_a78 = PlayGameSound3DIndex(this, -1, 0x3b, true);	// 0x4589c0
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
			else if (next->NameContains((char *)"C3DMULTICUTSCENE"))
			{
				// ("Calling MultiCutScene %s, NextTrigger=%s": compiled-out log)
				next->v23(this);								// BG: +0x5c, slot 23 TriggerCallback(this)
			}
		}
	}
	if (state == 0) f_928 = 0;
	else if (state == 1) f_928 = 1;
	else f_928 = f_928 == 0;
}

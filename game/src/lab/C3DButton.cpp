/*  C3DButton - see C3DButton.h. Reconstructed from Neutron.exe 0x4115c0..0x411fa0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DButton.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include "OMedia3DMaterial.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// The running level's ID (g_gametype +0x490: its ObjectID, e.g. 'LV4B').
static int LevelID(void)
{
	return static_cast<CGameObject *>(g_gametype)->m_object_id;
}

// The button's material colour (2002 OMedia3DMaterial +0x4c..+0x58: diffuse r, g, b, a).
// port: the original does not check that the material exists.
static void SetColour(OMedia3DMaterial *m, float r, float g, float b)
{
	if (m) m->set_diffuse(OMediaFARGBColor(1.0f, r, g, b));
}

// --------------------------------------------------------------------------- construction

C3DButton::C3DButton()										// 0x4115c0
{
	SetName((char *)"C3DBUTTON");
	Log("C3DBUTTON()");
	if (LevelID() == 'LEV6' || LevelID() == 'LV6A')			// the rocket ship
	{
		strcpy(f_73c, "buttondownship.ase");
		strcpy(f_7a0, "buttonupship.ase");
	}
	else
	{
		strcpy(f_73c, "buttondown.ase");
		strcpy(f_7a0, "buttonup.ase");
	}
	strcpy(f_804, "switch.png");
	C3DButton::InitObject();								// 0x4119d0 (called directly)
	C3DObject::SetObjectID('3BUT');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	C3DObject::InitPhysics();								// 0x4623d0
	f_6c0 = 0.1f;
	f_6c4 = 0.1f;
	f_6bc = 1.0f;
	f_6c8 = 0;
	f_6cc = 0;												// ButtonAvailable
	f_6d0 = -1;
	f_6d4 = -1;
	strcpy(f_6d8, "none");									// ActivateButton
	f_868 = 0;
	f_86c = -1;												// Toggle
	f_870 = 0;
	v34(1);
	// (then a call to the empty 0x472970)
}

C3DButton::~C3DButton()										// 0x411860
{
}

namespace neg {
CGameObject *New_C3DButton(void) { return new C3DButton; }				// '3BUT' (0x87c bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DButton::InitObject(void)							// 0x4119d0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("Red", &f_6bc, PROP_FLOAT, 0);
	RegisterProperty("Green", &f_6c0, PROP_FLOAT, 0);
	RegisterProperty("Blue", &f_6c4, PROP_FLOAT, 0);
	RegisterProperty("ButtonAvailable", &f_6cc, PROP_LONG, 0);
	RegisterProperty("NASound", &f_6d0, PROP_LONG, 0);
	RegisterProperty("AvailSound", &f_6d4, PROP_LONG, 0);
	RegisterProperty("ActivateButton", f_6d8, PROP_ALPHA, 0);
	RegisterProperty("NewTaskState", &f_868, PROP_LONG, 0);
	RegisterProperty("Toggle", &f_86c, PROP_LONG, 0);
	RegisterProperty("Down.ase", f_73c, PROP_ALPHA, 0);
	RegisterProperty("Up.ase", f_7a0, PROP_ALPHA, 0);
	RegisterProperty("UpDown.Png", f_804, PROP_ALPHA, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDOWN", f_73c);					// +0xd8
	CreateAnim((char *)"HIUP", f_7a0);
	CreateTexture(f_804, 0);								// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4 (BG +0x57c: the base shape)
	v67(300.0f);											// +0x110: radius
	ev58((char *)"UP", 1);									// +0xe0
	// (then a call to the empty 0x472970)
}

void C3DButton::ResetObject(void)							// 0x4118b0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	f_6c8 = f_6cc;											// ButtonAvailable
	if (f_6cc == 0)
	{
		SetColour(f_5d4[0], f_6bc * 0.5f, f_6c0 * 0.5f, f_6c4 * 0.5f);	// dimmed (BG +0x5c0)
		ev58((char *)"DOWN", 1);							// element +0xe0
	}
	else
	{
		ev58((char *)"UP", 1);
		SetColour(f_5d4[0], f_6bc, f_6c0, f_6c4);
	}
	v67(150.0f);											// +0x110: radius
	v152(150.0f);											// +0x264
}

void C3DButton::AddTouching(CGameObject *other)				// 0x411bb0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!g_camera) return;
	if (other != g_camera && !other->NameContains((char *)"C3DBASEBALL")) return;
	if (!f_6c8 || f_870 > 0) return;						// not available, or just pressed
	// The current animation's name (element +0xe8, BG slot 58) is compared with "HIUP", the
	// name it was created with (Process compares the other name slot with "UP").
	if (strcasecmp(GetAnim(), "HIUP") == 0)					// 0x487c20 _stricmp
	{
		f_870 = 4.0f;										// reset in 4 s
		if (LevelID() == 'LV4B' && strcasecmp(m_tag, "REACTOR") == 0 && GetTaskState("REACTOR") == 0)
		{
			AddScore(1000);									// BG 0x42adc0
			SetTaskState("REACTOR", 10);					// 0x45f990
		}
		if (f_6d4 != -1) PlayGameSoundIndex(-1, f_6d4, false);	// 0x458980
		ev58((char *)"DOWN", 1);							// element +0xe0
		SetColour(f_5d4[0], 0.2f, 0.2f, 0.2f);				// dark
		CGameObject *t = FindObjectByTag(f_6d8);			// ActivateButton
		if (!t) return;
		if (CLocalGameObject *l = dynamic_cast<CLocalGameObject *>(t)) l->v268(f_86c);	// +0x428 NextState(Toggle)
		if (!t->NameContains((char *)"C3DBUTTON")) return;	// +0x18
		C3DButton *b = dynamic_cast<C3DButton *>(t);		// port: the original writes +0x6c8 of whatever is named C3DBUTTON
		if (b) b->f_6c8 = 1;								// the other button becomes available
		f_6c8 = 0;
		return;
	}
	ev58((char *)"UP", 1);
	SetColour(f_5d4[0], f_6bc, f_6c0, f_6c4);
	CGameObject *t = FindObjectByTag(f_6d8);
	if (CLocalGameObject *l = dynamic_cast<CLocalGameObject *>(t)) l->v268(f_86c);	// +0x428
}

void C3DButton::Process(float dt)							// 0x411de0
{
	C3DAnimated::Process(dt);								// 0x40e050
	float p[4];
	v88(p);													// +0x164: the start position
	v196(p[0], p[1], p[2], p[3]);							// +0x314: held there
	if (f_870 > 0)
	{
		f_870 -= dt;
		if (f_870 <= 0)
		{
			ResetObject();									// +0x28
			f_870 = 0;
		}
	}
	if (strcasecmp(ev66(), "UP") != 0) return;				// element +0x100: name of the animation playing
	const float k = (float)((double)(g_4f81a0 * 0.5f) + 0.5);	// 0x4073a0: the pulse; 0x48e5d0 (double)
	SetColour(f_5d4[0], k, 0.2f, 0.3f);
}

void C3DButton::v258(void)									// 0x411ef0
{
	CLocalGameObject::v258();								// (BG slot 257)
	v67(300.0f);
	v152(300.0f);
}

/*  C3DOctapuke - see C3DOctapuke.h. Reconstructed from Neutron.exe 0x433e70..0x4344e0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DOctapuke.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../JimmySlots.h"
#include "../gen/globals.h"
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

// 0x42a7a0 (C3DPlayer's KeyFire): the fire keys.
static bool KeyFire(void)
{
	if (g_4f83e0) return false;
	return g_509834 || g_keys[0x0d] || g_keys[0x11] || g_joy_button0;		// left mouse, Enter, Ctrl, button 0
}

// 0x42a730: the jump keys while no gadget, or gadget 0 / 2 / 3 / 6 / 7 is out (not with 'I' or button 3 held).
static bool KeyJumpGadget(void)
{
	if (g_keys['I'] || g_joy_button3) return false;			// 0x509881, 0x509ac7
	const short g = g_4f0588;
	if (!(g == -1 || g == 0 || g == 3 || g == 2 || g == 6 || g == 7)) return false;
	return g_mouse_rdown || g_keys[0x2d] || g_keys[0x10] || g_keys[0x60] || g_joy_button1;
}

C3DOctapuke::C3DOctapuke()									// 0x433e70
{
	SetName((char *)"C3DOCTAPUKE");
	Log("C3DOCTAPUKE()");
	C3DOctapuke::InitObject();								// 0x434050 (called directly)
	C3DObject::SetObjectID('3OCT');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	v34(1);
	C3DObject::InitPhysics();								// 0x4623d0
	m_riding = 0;
	strcpy(m_start_point, "none");
	m_jimmy = NULL;
	m_timer = 0;
}

C3DOctapuke::~C3DOctapuke()									// 0x434000
{
}

namespace neg {
CGameObject *New_C3DOctapuke(void) { return new C3DOctapuke; }			// '3OCT' (0x734 bytes)
}

void C3DOctapuke::InitObject(void)							// 0x434050
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("StartPoint", m_start_point, PROP_ALPHA, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"octo.ase");	// +0xd8
	CreateAnim((char *)"HISTOP", (char *)"octostop.ase");
	CreateTexture((char *)"octapuke1.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(100.0f);											// +0x110: the radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DOctapuke::AddTouching(CGameObject *other)			// 0x4343a0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	if (MenuGetItem(2, 0x1b) == 0) return;					// 0x403950: the ticket
	if (MenuPickupCount(0x1b) <= 0) return;					// 0x4061b0
	JimmyElementSlot94(other);								// his element +0x178 (0x427ff0)
	g_gametype->SetCamera(static_cast<CGameObject *>(this));	// +0x118: the ride is the camera
	m_riding = 1;
	m_jimmy = other;
	other->v261();											// +0x410
	v136(0);												// +0x224
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(other);			// (other - 0xc0)
	if (j)
	{
		j->f_7c4 = 3;										// +0x7c4 (short): control state 3, sitting
		j->f_880 = 0;										// +0x880
	}
	AddScore(100);											// 0x42adc0
	MenuAddPickupCount(0x1b, -1);							// 0x4061c0
	if (j) j->f_1e59 = 1;									// +0x1e59 (game object +0x1d99): gadgets locked
}

void C3DOctapuke::Process(float dt)							// 0x434110
{
	float p[4];
	v88(p);													// +0x164: where it was placed
	v196(p[0], p[1], p[2], p[3]);							// +0x314
	C3DAnimated::Process(dt);								// 0x40e050
	if (!m_riding)
	{
		if (m_timer > 0.0f)
		{
			m_timer = m_timer - dt;
			if (!(m_timer > 0.0f)) m_timer = 0;
		}
		else
			AddRotation(0, dt * 20.0f, 0);					// +0x334; 0x495324
		return;
	}
	AddRotation(0, dt * 60.0f, 0);							// 0x49d5b4
	CGameObject *jim = m_jimmy;
	const OMedia3DVector at = v224(0, 700.0f, 1000.0f);		// +0x384
	jim->v196(at.x, at.y, at.z, at.w);						// +0x314
	C3DAnimated *ja = dynamic_cast<C3DAnimated *>(jim);		// (jim - 0xc0)
	if (ja) ja->ev58((char *)"SIT", 1);						// element +0xe0
	// (+0x328 read of the rotation into a local: not used)
	if (ja) ja->hide(true);									// element +0x58: he rides inside the model
	if (!KeyFire() && !KeyJumpGadget()) return;				// 0x42a7a0, 0x42a730
	if (ja) ja->force_show();								// element +0x70 (hide count) = 1, +0x58 (0)
	m_riding = 0;
	v136(1);												// +0x224
	CGameObject *sp = FindObjectByTag(m_start_point);		// 0x474070
	if (sp)
	{
		float q[4];
		sp->v195(q);										// +0x310
		jim->v196(q[0], q[1], q[2], q[3]);					// +0x314
	}
	g_gametype->SetCamera(jim);								// +0x118: Jimmy has the camera again
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);
	if (j)
	{
		j->f_880 = 1;										// +0x880
		j->f_7c4 = 0;										// +0x7c4
		j->f_1e59 = 0;										// +0x1e59
	}
	m_jimmy = NULL;
	m_timer = 10.0f;
}

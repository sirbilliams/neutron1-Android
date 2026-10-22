/*  C3DPirate - see C3DPirate.h. Reconstructed from Neutron.exe 0x436c40..0x437260. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DPirate.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../JimmySlots.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <math.h>
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

C3DPirate::C3DPirate()										// 0x436c40
{
	SetName((char *)"C3DPIRATE");
	Log("C3DPIRATE()");
	C3DPirate::InitObject();								// 0x436e20 (called directly)
	C3DObject::SetObjectID('3PIR');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	m_time = 0;
	m_riding = 0;
	m_jimmy = NULL;
	strcpy(m_start_point, "none");
	m_timer = 0;
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DPirate::~C3DPirate()										// 0x436dd0
{
}

namespace neg {
CGameObject *New_C3DPirate(void) { return new C3DPirate; }				// '3PIR' (0x738 bytes)
}

void C3DPirate::InitObject(void)							// 0x436e20
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("StartPoint", m_start_point, PROP_ALPHA, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"viking.ase");	// +0xd8
	CreateTexture((char *)"viking.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DPirate::v260(void)									// 0x437200
{
	C3DAnimated::v260();									// 0x40e7b0
	v67(800.0f);											// +0x110: the radius
}

void C3DPirate::AddTouching(CGameObject *other)				// 0x437110
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	if (MenuGetItem(2, 0x1b) == 0) return;					// 0x403950: the ticket
	if (MenuPickupCount(0x1b) <= 0) return;					// 0x4061b0
	JimmyElementSlot94(other);								// his element +0x178 (0x427ff0)
	m_riding = 1;
	m_jimmy = other;
	v136(0);												// +0x224
	other->v261();											// +0x410
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

void C3DPirate::Process(float dt)							// 0x436ec0
{
	C3DAnimated::Process(dt);								// 0x40e050
	m_time = dt + m_time;
	const float wobble = (float)((double)sinf(m_time) * 60.0);	// fsin; 0x4af6a8 (double 60)
	float r[4], r2[4];
	v201(r);												// +0x328
	v201(r2);
	v203(r2[0], r[1], wobble);								// +0x330: rotation z = the rocking
	if (!m_riding)
	{
		if (m_timer > 0.0f)
		{
			m_timer = m_timer - dt;
			if (!(m_timer > 0.0f)) m_timer = 0;
		}
		return;
	}
	CGameObject *jim = m_jimmy;
	const OMedia3DVector at = v224(-300.0f, -648.0f, 0);	// +0x384
	jim->v196(at.x, at.y, at.z, at.w);						// +0x314
	C3DAnimated *ja = dynamic_cast<C3DAnimated *>(jim);		// (jim - 0xc0)
	if (ja) ja->ev58((char *)"DRIVE", 1);					// element +0xe0
	float q[4];
	v201(q);												// +0x328: he turns with it
	jim->v202(q[0], q[1], q[2], q[3]);						// +0x32c
	if (!KeyFire() && !KeyJumpGadget()) return;				// 0x42a7a0, 0x42a730
	m_riding = 0;
	v136(1);												// +0x224
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);
	if (j)
	{
		j->f_880 = 1;										// +0x880
		j->f_7c4 = 0;										// +0x7c4
	}
	CGameObject *sp = FindObjectByTag(m_start_point);		// 0x474070
	if (sp)
	{
		float s[4];
		sp->v195(s);										// +0x310
		jim->v196(s[0], s[1], s[2], s[3]);					// +0x314
	}
	if (j) j->f_1e59 = 0;									// +0x1e59
	m_jimmy = NULL;
	m_timer = 10.0f;
}

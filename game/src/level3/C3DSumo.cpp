/*  C3DSumo - see C3DSumo.h. Reconstructed from Neutron.exe 0x443dc0..0x4444e0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSumo.h"
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

C3DSumo::C3DSumo()											// 0x443dc0
{
	SetName((char *)"C3DSUMO");
	Log("C3DSUMO()");
	C3DSumo::InitObject();									// 0x443fa0 (called directly)
	C3DObject::SetObjectID('3SUM');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	v34(1);
	m_riding = 0;
	m_jimmy = NULL;
	C3DObject::InitPhysics();								// 0x4623d0
	strcpy(m_start_point, "none");
	m_timer = 0;
}

C3DSumo::~C3DSumo()											// 0x443f50
{
}

namespace neg {
CGameObject *New_C3DSumo(void) { return new C3DSumo; }					// '3SUM' (0x734 bytes)
}

void C3DSumo::InitObject(void)								// 0x443fa0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("StartPoint", m_start_point, PROP_ALPHA, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"sumo.ase");	// +0xd8
	CreateTexture((char *)"sumo.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DSumo::v260(void)									// 0x4443d0
{
	C3DAnimated::v260();									// 0x40e7b0
	v67(680.0f);											// +0x110: the radius
}

void C3DSumo::AddTouching(CGameObject *other)				// 0x4442f0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	if (MenuGetItem(2, 0x1b) == 0) return;					// 0x403950: the ticket
	if (MenuPickupCount(0x1b) <= 0) return;					// 0x4061b0
	JimmyElementSlot94(other);								// his element +0x178 (0x427ff0)
	m_riding = 1;
	m_jimmy = other;
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

void C3DSumo::Process(float dt)								// 0x444040
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
	}
	else
	{
		AddRotation(0, dt * 60.0f, 0);						// 0x49d5b4
		CGameObject *jim = m_jimmy;
		const OMedia3DVector at = v224(260.0f, 185.0f, 550.0f);	// +0x384
		jim->v196(at.x, at.y, at.z, at.w);					// +0x314
		jim->v261();										// +0x410
		C3DAnimated *ja = dynamic_cast<C3DAnimated *>(jim);	// (jim - 0xc0)
		if (ja) ja->ev58((char *)"SIT", 1);					// element +0xe0
		float r[4];
		v201(r);											// +0x328
		jim->v202(r[0], r[1] + 90.0f, r[2], r[3]);			// +0x32c, 0x4a7388
		if (KeyFire())										// 0x42a7a0 (the fire keys only)
		{
			m_riding = 0;
			v136(1);										// +0x224
			CGameObject *sp = FindObjectByTag(m_start_point);	// 0x474070
			if (sp)
			{
				float q[4];
				sp->v195(q);								// +0x310
				jim->v196(q[0], q[1], q[2], q[3]);			// +0x314
			}
			C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);
			if (j)
			{
				j->f_880 = 1;								// +0x880
				j->f_7c4 = 0;								// +0x7c4
				j->f_1e59 = 0;								// +0x1e59
			}
			m_jimmy = NULL;
			m_timer = 10.0f;
		}
	}
	v88(p);
	v196(p[0], p[1], p[2], p[3]);
}

/*  C3DMerryGo - see C3DMerryGo.h. Reconstructed from Neutron.exe 0x42e220..0x42e8d0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMerryGo.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Globals.h"
#include "../Game.h"
#include "../Sound.h"
#include "../Todo.h"
#include "../C3DJimmy.h"
#include "../JimmySlots.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
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

// 0x42a730: an action key or button is down (mouse right, Insert, Shift, numpad 0, button 1),
// unless 'I' or button 3 is down or a gadget other than 0, 2, 3, 6, 7 (or none) is selected.
static bool ActionPressed(void)
{
	if (g_keys['I'] || g_joy_button3) return false;			// 0x509881, 0x509ac7
	const short gadget = g_4f0588;
	if (gadget != -1 && gadget != 0 && gadget != 3 && gadget != 2 && gadget != 6 && gadget != 7)
		return false;
	return g_mouse_rdown || g_keys[0x2d] || g_keys[0x10] || g_keys[0x60] || g_joy_button1;
}

// Jimmy's state the merry-go-round sets (C3DJimmy fields, original offsets from his element).
static void JimmyRideState(CGameObject *jim, bool on)
{
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);
	if (!j) return;
	if (on)
	{
		j->f_7c4 = 3;										// +0x7c4 (short): control state 3
		j->f_880 = 0;										// +0x880
		if (j->f_a3c != -1)									// +0xa3c: the jet pack's sound channel
		{
			StopChannel(j->f_a3c, false);					// 0x47d7a0
			j->f_a3c = -1;
		}
		j->f_1e59 = 1;										// +0x1e59 (game object +0x1d99): gadgets locked
	}
	else
	{
		j->f_1e25 = 0;										// +0x1e25
		j->f_880 = 1;										// +0x880
		j->f_7c4 = 1;										// +0x7c4
		j->f_1e59 = 0;										// +0x1e59
	}
}

// --------------------------------------------------------------------------- construction

C3DMerryGo::C3DMerryGo()									// 0x42e220
{
	SetName((char *)"C3DMERRYGO");							// 0x4702d0
	Log("C3DMERRYGO()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	C3DObject::SetObjectID('3MER');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	v34(1);
	f_6c0 = 0;
	f_6bc = NULL;
	C3DObject::InitPhysics();								// 0x4623d0
	f_6c4 = 0;
	m_db = GetDatabase("objects.omt");						// 0x46a910
	f_648 = 0;												// animations off
	set_shape(Load3DShape(m_db, 16));						// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// 0x477550
	strcpy(f_6cc, "none");
	RegisterProperty("StartPoint", f_6cc, PROP_ALPHA, 0);
	f_6c8 = 0;
}

C3DMerryGo::~C3DMerryGo()									// 0x42e410
{
}

namespace neg {
CGameObject *New_C3DMerryGo(void) { return new C3DMerryGo; }			// '3MER' (0x738 bytes)
}

// --------------------------------------------------------------------------- slots

void C3DMerryGo::AddTouching(CGameObject *other)			// 0x42e760
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	JimmyElementSlot94(other);								// his element +0x178 (0x427ff0)
	f_6c0 = 1;
	f_6bc = other;
	v136(0);												// +0x224
	JimmyRideState(other, true);
	if (f_6c8 < 40.0f) f_6c8 = 40.0f;						// 0x49fad4
	g_4f83e0 = 1;
}

void C3DMerryGo::Process(float dt)							// 0x42e460
{
	float p[4];
	v88(p);													// +0x164: where it was placed
	v196(p[0], p[1], p[2], p[3]);							// +0x314
	C3DAnimated::Process(dt);								// 0x40e050
	if (!f_6c0)
	{
		if (f_6c8 > 0)
		{
			f_6c8 = f_6c8 - dt * 10.0f;						// 0x48e5ec
			if (f_6c8 < 0) f_6c8 = 0;
			AddRotation(0, dt * f_6c8, 0);					// +0x334
		}
	}
	else
	{
		AddRotation(0, dt * f_6c8, 0);
		CGameObject *jim = f_6bc;
		jim->v261();										// +0x410
		C3DAnimated *ja = dynamic_cast<C3DAnimated *>(jim);	// (jim - 0xc0)
		if (ja) ja->ev58((char *)"STOP", 1);				// element +0xe0
		const OMedia3DVector at = v224(0, 110.0f, 340.0f);	// +0x384
		jim->v196(at.x, at.y, at.z, at.w);					// +0x314
		float r[4];
		v201(r);											// +0x328
		jim->v202(r[0], r[1] + 90.0f, r[2], r[3]);			// +0x32c, 0x4a7388
		if (g_mouse_down || ActionPressed() || g_keys[0x09] || g_keys[0x20] || g_keys[0x11] ||
			g_joy_button0 || g_joy_button2)					// 0x509834, 0x509841, 0x509858, 0x509849, 0x509ac4, 0x509ac6
		{
			f_6c0 = 0;
			v136(1);										// +0x224
			JimmyRideState(jim, false);
			g_4f83e0 = 0;
			CGameObject *sp = FindObjectByTag(f_6cc);		// 0x474070
			if (sp)
			{
				float q[4];
				sp->v195(q);								// +0x310
				jim->v196(q[0], q[1], q[2], q[3]);			// +0x314
			}
			f_6bc = NULL;
		}
	}
	v88(p);
	v196(p[0], p[1], p[2], p[3]);
}

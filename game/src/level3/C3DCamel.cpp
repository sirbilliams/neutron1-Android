/*  C3DCamel - see C3DCamel.h. Reconstructed from Neutron.exe 0x412150..0x412820. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DCamel.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../JimmySlots.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include "OMediaViewPort.h"
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

C3DCamel::C3DCamel()										// 0x412150
{
	SetName((char *)"C3DCAMEL");
	Log("C3DCamel()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	C3DObject::SetObjectID('3CML');							// 0x462220
	m_state = 3;											// patrol only
	m_ai_state = 3;											// AIState
	m_visible_range = 500.0f;								// VisibleRange
	m_speed = 200.0f;
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	m_turn_rates.x = 2.0f;
	m_turn_rates.y = 2.0f;
	m_turn_rates.z = 2.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	CGameObject::v42(0);									// 0x470fd0 (direct calls)
	CGameObject::v40(0);									// 0x470fa0
	ev72(m_anim_run, (char *)"WALK");						// 0x407fa0 (strcpy)
	ev72(m_anim_wag, (char *)"STOP");
	ev72(m_anim_sit, (char *)"STOP");
	m_db = GetDatabase("camel.omt");						// 0x46a910
	CGameObject::v34(1);									// 0x470f10
	f_648 = 0;												// animations off
	set_shape(Load3DShape(m_db, 0));						// 0x477ba0, element +0xac
	m_move_forward = 1;
	m_sound = -1;
	f_008 = 0;
	// (then a call to the empty 0x472970)
}

C3DCamel::~C3DCamel()										// 0x412360
{
}

namespace neg {
CGameObject *New_C3DCamel(void) { return new C3DCamel; }				// '3CML' (0x9a0 bytes)
}

void C3DCamel::v260(void)									// 0x4123b0
{
	C3DAI::v260();											// 0x409480
	v152(250.0f);											// +0x264
}

void C3DCamel::Process(float dt)							// 0x4123d0
{
	C3DAI::Process(dt);										// 0x408000
	float a[4], b[4];
	v155(a);												// +0x270: the acceleration
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	float vel[4];
	v174(vel);												// +0x2bc
	float vy = vel[1];
	if (vy <= 0.0f)											// going down (0x48d914): faster
	{
		if (vy > -340.0f) vy = -340.0f;						// 0x494cc8
		m_speed = m_speed - vy * dt;
		if (m_speed > 3300.0f) m_speed = 3300.0f;			// 0x494cc4
	}
	else													// going up: slower
	{
		if (vy < 240.0f) vy = 240.0f;						// 0x48d934
		m_speed = m_speed - vy * dt;
		if (m_speed < 630.0f) m_speed = 630.0f;				// 0x494ccc
	}
	const float rate = (float)((double)(m_speed * 0.000909090909f) * 1.8);	// 0x494cc0, 0x494cb8
	m_turn_rates.x = rate;
	m_turn_rates.y = rate;
	m_turn_rates.z = rate;
}

void C3DCamel::AddTouching(CGameObject *other)				// 0x4124e0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	JimmyElementSlot94(other);								// his element +0x178 (0x427ff0)
	other->v261();											// +0x410
	g_gametype->SetCamera(static_cast<CGameObject *>(this));	// +0x118: the camel is the camera
	ev73(3);												// +0x124: patrol
	C3DAnimated *ja = dynamic_cast<C3DAnimated *>(other);	// (other - 0xc0)
	if (ja) ja->hide(true);									// element +0x58
	AddScore(25);											// 0x42adc0
	hide(true);												// element +0x58: the camel too
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(other);
	if (j) j->f_1e59 = 1;									// +0x1e59 (game object +0x1d99): gadgets locked
	if (m_sound == -1)
		m_sound = PlayGameSound3DIndex(this, -1, 0xd1, true);	// 0x4589c0
}

void C3DCamel::v243(float dt)								// 0x4125d0
{
	const float cx = g_viewport->getx(), cy = g_viewport->gety(), cz = g_viewport->getz();	// viewport +0x44
	const OMedia3DVector a = v224(0, 20.0f, 700.0f);		// +0x384: where the camera drifts to
	g_viewport->place((a.x - cx) * dt * 2.5f + g_viewport->getx(),					// 0x494cd0
					  (float)((double)((a.y - cy) * dt) * 1.8) + g_viewport->gety(),	// 0x494cb8 (double)
					  (a.z - cz) * dt * 2.5f + g_viewport->getz());
	const OMedia3DVector b = v224(0, 0, 1200.0f);			// the point it looks at
	// (the direction is taken from the position the camera had at the start of the frame)
	OMedia3DVector L(b.x - cx, b.y - cy, b.z - cz, 0);
	omt_Angle ax, ay;
	L.angles(ax, ay);
	const short bx = g_viewport->get_anglex();
	short dyaw = (short)((ay - g_viewport->get_angley()) & 0x3fff);
	if (dyaw > 0x2000) dyaw = (short)(dyaw - 0x4000);
	g_viewport->set_anglex((omt_WAngle)(g_viewport->get_anglex() + (short)(ax - bx)));
	g_viewport->set_angley((omt_WAngle)(g_viewport->get_angley() + dyaw));
	if (!KeyFire() && !KeyJumpGadget()) return;				// 0x42a7a0, 0x42a730
	CGameObject *jim = FindObjectByTag("JIM1");				// 0x474070
	if (m_sound != -1) StopGameSound(m_sound, false);		// 0x458a00
	if (!jim) return;
	float p[4];
	jim->v88(p);											// +0x164: his start position
	jim->v196(p[0], p[1], p[2], p[3]);						// +0x314
	g_gametype->SetCamera(jim);								// +0x118
	C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);			// (jim - 0xc0)
	if (j)
	{
		j->force_show();									// element +0x70 (hide count) = 1, +0x58 (0)
		j->f_1e59 = 0;										// +0x1e59 (game object +0x1d99)
	}
}

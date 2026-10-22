/*  C3DJeep - see C3DJeep.h. Reconstructed from Neutron.exe 0x4211a0..0x421d70. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DJeep.h"
#include "../Bridge.h"
#include "../C2DInGameMenu.h"
#include "../C3DAnimated.h"
#include "../C3DJimmy.h"
#include "../C3DPlayer.h"
#include "../CGameType.h"
#include "../CViewPort.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <math.h>
#include <string>
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

// The keys the original tests inline (g_keys is the keyboard, 0x509838 + the Windows key code;
// 0x509834 the left, 0x50993a the right mouse button).
static bool JumpKeyDown(void)								// right mouse, Insert, Shift, Num 0, button 1
{
	return g_50993a || g_keys[0x2d] || g_keys[0x10] || g_keys[0x60] || g_joy_button1;
}

// --------------------------------------------------------------------------- construction

C3DJeep::C3DJeep()											// 0x4211a0
{
	SetName((char *)"C3DJEEP");
	Log("C3DJeep()");
	f_9f0 = 0;												// power at ResetObject
	C3DJeep::InitObject();									// 0x4213f0 (called directly)
	f_a84 = NULL;
	C3DObject::SetObjectID('3JEE');							// 0x462220
	v67(150.0f);											// radius
	f_a38 = 1;												// camera
	v36(1);
	v107(0);
	f_00b = 0;
	v44(0);
	f_a43 = 0;												// walls do not crash it
	f_a88 = 0;
	f_6d0 = 2;												// port: a plain shape for the Negatron-shaped C3DAnimated base
}

C3DJeep::~C3DJeep()											// 0x421330
{
	Log("~C3DJeep()");
}

namespace neg {
CGameObject *New_C3DJeep(void) { return new C3DJeep; }		// '3JEE' (0x6ac bytes)
}

// --------------------------------------------------------------------------- element slots

unsigned char C3DJeep::ev123(void)							// 0x421c10: Left arrow, joystick left
{
	return g_50985d || g_joy_x < -0.5f;
}

unsigned char C3DJeep::ev124(void)							// 0x421c40: Right arrow, joystick right
{
	return g_50985f || g_joy_x > 0.5f;
}

unsigned char C3DJeep::ev125(void)							// 0x421c70: go, with the rear wheels down
{
	if (!g_509834 && !g_50985e && g_joy_y <= 0.5f && !g_joy_button0) return 0;	// left mouse, Up arrow, joystick up, button 0
	return f_9ea > 0;
}

unsigned char C3DJeep::ev126(void)							// 0x421cb0: back, with the rear wheels down
{
	if (g_joy_button0) return 0;
	const bool back = (g_50993a && !g_509834) || g_509860 || g_joy_y < -0.7f;	// right mouse, Down arrow, joystick down
	if (!back) return 0;
	return f_9ea > 0;
}

// --------------------------------------------------------------------------- CGameObject slots

void C3DJeep::InitObject(void)								// 0x4213f0
{
	Log("InitObject()");
	C3DVehicle::InitObject();								// 0x465030
	g_gametype->InitVehicleDatabase(std::string("omt\\scooter.omt"));	// +0xec
	set_shape(Load3DShape(g_vehicle_db, 0));				// 0x477ba0, element +0xac
	v66(5515.0f);											// +0x108
	MakeShapeUnLit(get_shape());							// 0x477550
	InitPhysics();											// +0x2c
}

void C3DJeep::UnInitObject(void)							// 0x4213e0
{
	Log("UnInitObject()");									// CLocalGameObject's (0x458810)
	set_shape(NULL);										// element +0xac
	f_414 = NULL;
}

void C3DJeep::InitPhysics(void)								// 0x421570
{
	if (f_4aa) return;
	Log("InitPhysics()");
	C3DObject::InitPhysics();								// 0x4623d0
}

void C3DJeep::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x421910
{
	if (!f_a44 && f_a43)
	{
		float v[4];
		v174(v);											// +0x2bc
		OMedia3DVector h(v[0], 0, v[2], v[3]);
		if (h.quick_magnitude() > 20.0f)					// 0x495324
		{
			const OMedia3DVector fwd = v226(0, 0, 1.0f);	// +0x38c
			if (!(fabsf(fwd.z * c + fwd.y * b + fwd.x * a) < 0.9f))	// 0x4a3144
				ev131();									// element +0x140: crash
		}
	}
	if (f_a44)
	{
		CLocalGameObject::v220(a, b, c, d, poly);			// 0x458890
		return;
	}
	float s = v153();										// +0x268
	if (!(s > 0)) return;
	float v[4];
	v174(v);
	OMedia3DVector n(v[0], v[1], v[2], v[3]);
	const float inv = 1.0f / n.quick_magnitude();
	n.x *= inv; n.y *= inv; n.z *= inv;
	float dot = n.z * c + n.y * b + n.x * a;
	if (dot < -0.85f) { if (!f_004) dot *= 3.5f; }			// 0x4a3140, 0x4a313c
	else if (!f_004) dot *= 1.5f;							// 0x4a3138
	if (!(dot < 0)) return;
	if (f_004)
	{
		s = s - fabsf(dot) * f_a00 * g_frame_time;			// 0x696030
		if (!(s > 100.0f)) s = 100.0f;						// 0x4a3130
	}
	else if (g_50985e || g_509834 || g_joy_y > 0.5f)		// going: Up arrow, left mouse, joystick up
	{
		s = s - fabsf(dot) * f_a00 * g_frame_time;
		if (!(s > 230.0f)) s = 230.0f;						// 0x4a3128
	}
	v154(s);												// +0x26c
}

void C3DJeep::Process(float dt)								// 0x421580
{
	if (!GameIsRunning()) return;							// 0x475ca0
	C3DCar::Process(dt);									// 0x412ed0
	if (!f_a84)												// nobody on it
	{
		f_a0c = 0;
		f_29c = 10.0f;
		f_a24 = 0;
		f_a28 = 0;
		v154(0);											// +0x26c
		f_a04 = 0;
		return;
	}
	if (C3DJimmy *j = dynamic_cast<C3DJimmy *>(f_a84)) j->MenuInput(dt);	// element +0x1ac
	if (!JumpKeyDown()) f_a80 = 0;
	else if (!f_a80 && v133())								// +0x218: a hop
	{
		v162(0, 700.0f, 0);									// +0x28c
		f_a80 = 1;
	}
	// the driver is kept on it: his position (object and element) and rotation are ours
	C3DAnimated *drv = f_a84;
	float p[4];
	v195(p);												// +0x310
	drv->v197(p[0], p[1], p[2]);							// +0x318
	drv->place(p[0], p[1], p[2]);							// element +0x44
	float r[4];
	v201(r);												// +0x328
	drv->v203(r[0], r[1], r[2]);							// +0x330
	f_a88 += dt;
	if (!(f_a88 > 0.3)) return;								// 0x4a3110
	if (v153() < 10.0f) f_a84->ev58((char *)"SCOOTSTOP", 1);	// 0x48e5ec, element +0xe0
	else f_a84->ev58((char *)"SCOOT", 1);
	f_a88 = 0;
}

void C3DJeep::v243(float dt)								// 0x421880
{
	if (ConfirmOpen()) return;								// 0x407060
	if (!GameIsRunning()) return;							// 0x475ca0
	if (g_4f8181) return;
	if (g_509834) g_4f8182 = 1;
	C3DVehicle::v243(dt);									// 0x465830
}

void C3DJeep::v260(void)									// 0x4217d0
{
	ev107(f_9f0);											// element +0xe0
}

void C3DJeep::v275(void)									// 0x4218c0
{
	if (C3DJimmy *j = dynamic_cast<C3DJimmy *>(FindObjectByTag("JIM1")))
	{
		j->v275();											// +0x438
		j->f_1e25 = 1;
	}
	ev140();												// element +0x164
	g_4f8182 = 1;
}

void C3DJeep::v277(void)									// 0x421bb0
{
	if (f_a78 != -1) StopChannel(f_a78, false);				// 0x47d7a0 (the channel is kept)
}

void C3DJeep::v278(void)									// 0x421bd0
{
	if (f_a78 != -1) f_a78 = PlayGameSound3DIndex(this, -1, 0x2e, true);	// 0x4589c0: "Scooter"
}

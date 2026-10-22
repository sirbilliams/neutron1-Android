/*  C3DRocketShip - see C3DRocketShip.h. Reconstructed from Neutron.exe 0x43d840..0x43ec80. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DRocketShip.h"
#include "C3DFireStrato.h"
#include "C3DGoddard.h"
#include "C3DJimmy.h"
#include "C2DInGameMenu.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "CGameType.h"
#include "CanvasWrapper.h"
#include "SaveGame.h"
#include "Sound.h"
#include "gen/globals.h"
#include "OMediaTrigo.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#pragma GCC diagnostic pop
#include <math.h>
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

// The game's random number generator: 0..1000.
static int Random1001(void)
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	return ((int)((*seed >> 16) * 1001) >> 16) & 0xffff;
}

// 0x458980: a sound of the sound database by its index.
static void Play(int id)
{
	PlayGameSoundIndex(-1, id, false);
}

// 0x42a7a0 (C3DPlayer's KeyFire, which this class cannot call): the fire keys.
static bool KeyFire(void)
{
	if (g_4f83e0) return false;
	return g_509834 || g_keys[0x0d] || g_keys[0x11] || g_joy_button0;		// left mouse, Enter, Ctrl, button 0
}

static OMedia3DVector Position(CGameObject *o)
{
	float p[4];
	o->v195(p);												// vtable +0x310
	return OMedia3DVector(p[0], p[1], p[2], p[3]);
}

static OMedia3DVector Rotation(CGameObject *o)
{
	float r[4];
	o->v201(r);												// vtable +0x328
	return OMedia3DVector(r[0], r[1], r[2], r[3]);
}

// CGameObject slot 226 (0x472b10): (x, y, z) turned by the object's rotation, w = 0.
static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
static OMedia3DVector ObjectDir(CGameObject *o, float x, float y, float z)
{
	float rot[4];
	o->v201(rot);
	const omt_Angle ax = (omt_Angle)((int)(rot[0] * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle ay = (omt_Angle)((int)(rot[1] * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle az = (omt_Angle)((int)(rot[2] * kDegToAngle) & omc_MaxAngleMask);
	OMedia3DVector v(x, y, z, 0.0f);
	v.rotate(ax, ay, az);
	return v;
}

// The element of a game object of the C3DObject family (the original goes back by 0xc0).
static C3DObject *Element(CGameObject *o)
{
	return o ? dynamic_cast<C3DObject *>(o) : NULL;
}

static C3DJimmy *AsJimmy(C3DAnimated *a)
{
	return a ? dynamic_cast<C3DJimmy *>(a) : NULL;
}

// The 4-float position / rotation setters (slots 196 / 202) with a vector.
static void SetPos4(CGameObject *o, const OMedia3DVector &v) { o->v196(v.x, v.y, v.z, v.w); }	// vtable +0x314
static void SetRot4(CGameObject *o, const OMedia3DVector &v) { o->v202(v.x, v.y, v.z, v.w); }	// +0x32c

// Show: force_show() (`if (hide_count) { hide_count = 1; hide(false); }`); hide: element +0x58 hide(true).
static void Show(C3DObject *e) { if (e) e->force_show(); }
static void Hide(C3DObject *e) { if (e) e->hide(true); }

// Jimmy is hurt: his element slot 104 (0x42a870).
static void Shock(C3DAnimated *rider, float energy, int points, float time)
{
	if (C3DJimmy *j = AsJimmy(rider)) j->Shock(energy, points, time);
}

// The shock flag of Jimmy (+0x1e03): cleared before a hit so that it counts again.
static void ClearShocked(C3DAnimated *rider)
{
	if (C3DJimmy *j = AsJimmy(rider)) j->f_1e03 = 0;
}

// 0x42f310 (on other - 0xc8, a C3DMine's CGameObject part): stores 2 in the object (+0x62c from
// there). The C3DMine is not reconstructed yet; the original calls it for a C3DRock too (a quirk:
// it writes 2 into that class's data, 8 bytes before its element).
static void MineSetState(CGameObject *o)
{
	(void)o;
	NEG_TODO(0x42f310, "C3DMine / C3DRock: +0x62c (from other - 0xc8) = 2");
}

// --------------------------------------------------------------------------- construction

C3DRocketShip::C3DRocketShip()								// 0x43d840
{
	SetName((char *)"C3DROCKETSHIP");
	Log("C3DRocketShip()");
	C3DRocketShip::InitObject();							// 0x43dd10 (called directly)
	v64(0.6f);
	v38(1);
	C3DObject::SetObjectID('3ROC');							// 0x462220
	f_9f0 = 0;
	f_9f4 = NULL;
	v44(0);
	v50(0);
	for (int i = 0; i < 20; i++) f_9f8[i] = NULL;			// the smoke puffs: none
	v36(0);
	f_11f0 = 0;
	strcpy(m_task_name, "LIBBY");							// element +0x4f0
	f_11f4 = NULL;
	f_11f8 = 0;
	f_1234 = 0;
	f_11fc = 0;
	f_1230 = new C3DFireStrato;								// 0x4192e0 (0x6c4 bytes)
	// (debug log of the allocation, "C3DFireStrato": compiled out)
	f_1228 = 0;
	f_122c = 0;
	f_009 = 1;
	f_123c = -1;
	f_1248 = 0;
	g_4f81ac = 0;
	f_1235 = 1;
	f_1234 = 0;
	f_1244 = 0;												// port: not set by the original (heap)
}

C3DRocketShip::~C3DRocketShip()								// 0x43da70
{
	Log("~C3DRocketShip()");
}

namespace neg {
CGameObject *New_C3DRocketShip(void) { return new C3DRocketShip; }		// '3ROC' (0xf9c bytes)
}

// --------------------------------------------------------------------------- element-family slots

void C3DRocketShip::ev108(int state)						// 0x43ea80
{
	if (state != 0)
	{
		int sound;
		if (state == 1)
		{
			if (f_1244 == 1) { f_1244 = state; return; }
			if (f_123c != -1) StopGameSound(f_123c, false);	// 0x458a00
			sound = 0xc3;									// "rocket fly"
		}
		else
		{
			if (state != 2 || f_1244 == 2) { f_1244 = state; return; }
			if (f_123c != -1) StopGameSound(f_123c, false);
			sound = 0xc4;									// "rocket blast"
		}
		f_123c = PlayGameSoundIndex(-1, sound, true);		// 0x458980
		f_1244 = state;
		return;
	}
	if (f_123c != -1)
	{
		StopGameSound(f_123c, false);
		f_123c = -1;
	}
	f_1244 = state;
}

void C3DRocketShip::ev110(void)								// 0x43e860
{
	v224(0, 20.0f, 1000.0f);								// (unused)
	ObjectDir(this, 0, 0, 1.0f);							// (unused) vtable +0x38c
	if (++f_1228 > 9) f_1228 = 0;
}

// --------------------------------------------------------------------------- CGameObject family

void C3DRocketShip::InitObject(void)						// 0x43dd10
{
	Log("InitObject()");
	C3DFlyingObject::InitObject();							// 0x419f70
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"Strato.ase");	// +0xd8
	CreateTexture((char *)"strato.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(50.0f);												// +0x110
	ev58((char *)"DEFAULT", 1);
}

void C3DRocketShip::ResetObject(void)						// 0x43ea60
{
	C3DFlyingObject::ResetObject();							// 0x41a0b0
	if (!f_9f4) f_96c = 0;
}

void C3DRocketShip::InitPhysics(void)						// 0x42ab50
{
	if (!f_4aa) C3DObject::InitPhysics();
}

void C3DRocketShip::v260(void)								// 0x43db10
{
	C3DAnimated::v260();									// 0x40e7b0
	InitPhysics();											// vtable +0x2c
	g_4f81ac++;
	f_11f8 = 0;
	f_1248 = 0;
	f_9d8 = 0;
	f_9d4 = 0;
	if (f_9f4)
	{
		// we start where the rider started
		float p[4];
		f_9f4->v88(p);										// its initial position, vtable +0x164
		unsigned int w;
		memcpy(&w, &p[3], 4);
		v89(p[0], p[1], p[2], w);							// +0x168
		float r[4];
		f_9f4->v91(r);										// its initial rotation, +0x170
		const float rz = r[2];
		f_9f4->v91(r);
		const float ry = r[1];
		f_9f4->v91(r);
		v92(r[0], ry, rz);									// +0x174
		f_96c = 500.0f;
	}
	for (int i = 0; i < 20; i++)							// the smoke puffs (none): additive
		if (OMediaCanvasElement *s = f_9f8[i] ? dynamic_cast<OMediaCanvasElement *>(f_9f8[i]) : NULL)
			s->set_blend(omblendfc_Dst_Color, omblendfc_Zero);	// +0xb4 / +0xb8 = 2 / 0
	switch (g_gametype->m_object_id)						// the ceiling by level
	{
		case 'LEV1':										f_984 = 1500.0f; break;
		case 'LEV2': case 'LV2B':							f_984 = 2300.0f; break;
		case 'LEV3': case 'LV3A': case 'LV3B': case 'LV3C': case 'LV3D': case 'LV3E':
															f_984 = 1700.0f; break;
		case 'LEV4':										f_984 = 2700.0f; break;
		case 'LV4C':										f_984 = 3700.0f; break;
		case 'LEV5': case 'LV5A': case 'LV5B': case 'VR02':
															f_984 = 3900.0f; break;
	}
}

void C3DRocketShip::v266(void)								// 0x43ea50
{
	GetTaskState("LIBBY");									// 0x45fea0 (the result is not used)
}

void C3DRocketShip::v267(int level)							// 0x43ebb0
{
	C3DAnimated::v267(level);								// 0x40e340
	if (!f_9f4) return;
	f_1235 = 1;
	f_1234 = 0;
	force_show();											// hide_count -> 1, hide(false)
	v234();													// +0x3ac
	v132(1);												// +0x214
}

void C3DRocketShip::v275(void)								// 0x43eb40
{
	// (debug log, compiled out)
	if (C3DJimmy *j = dynamic_cast<C3DJimmy *>(FindObjectByTag("JIM1")))
	{
		j->v275();											// +0x438
		j->f_1e25 = 1;
	}
}

void C3DRocketShip::v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly)	// 0x43eb70
{
	(void)nx; (void)ny; (void)nz; (void)nw; (void)poly;
	float v[4];
	v174(v);
	const float vz = v[2];
	v174(v);
	v176(v[0], -120.0f, vz);								// the ceiling stops the climb
}

void C3DRocketShip::AddTouching(CGameObject *other)			// 0x43dda0
{
	// (the original reads the camera's position into a local, before and after: no effect)
	C3DFlyingObject::AddTouching(other);					// 0x41afc0
	if (other->NameContains((char *)"C3DMINE") && f_9f4 && !f_11f8)
	{
		ClearShocked(f_9f4);
		Shock(f_9f4, -10.0f, -10, 5.0f);					// element +0x1a0
		MineSetState(other);								// 0x42f310 on other - 0xc8
		Play(0xcb);											// "explosion1"
		f_11f8 = 1;
		f_1248 = 1;
		f_1238 = 2.5f;
		f_11fc = 0;
	}
	if (other->NameContains((char *)"C3DROCK") && f_9f4 && !f_11f8)
	{
		ClearShocked(f_9f4);
		Shock(f_9f4, -10.0f, -10, 5.0f);
		MineSetState(other);								// quirk: the same call as the mine's
		Play(0xcb);
		f_11f8 = 1;											// (no smoke, no new hit length)
		f_11fc = 0;
	}
	if (other->NameContains((char *)"C3DMISSILE") && f_9f4 && !f_11f8)
	{
		ClearShocked(f_9f4);
		Shock(f_9f4, -10.0f, -10, 5.0f);
		Play(0xca);											// "explosion2"
		Hide(Element(other));								// element +0x58
		f_11f8 = 1;
		f_1248 = 1;
		f_1238 = 2.0f;
		f_11fc = 0;
	}
}

void C3DRocketShip::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x43e8d0
{
	(void)a; (void)b; (void)c; (void)d;
	if (!f_9f4 || !poly || !poly->get_material()) return;
	const int id = (int)poly->get_material()->get_chunk_ID();	// material +0xc -> +8 (0 without one)
	if (id >= 130 && id <= 139)
	{
		if (f_11f8) return;
		ClearShocked(f_9f4);
		Shock(f_9f4, -10.0f, -10, 5.0f);
		Play(0xc5);											// "rocket hit"
		f_11f8 = 1;
		f_1248 = 0;
		f_11fc = 0;
		f_1238 = 5.0f;
		return;
	}
	if (id >= 185 && id <= 189)
	{
		C3DJimmy *j = AsJimmy(f_9f4);
		if (j && j->f_1e03) return;
		Shock(f_9f4, -5.0f, -5, 5.0f);
		Play(0xc5);
		f_1238 = 5.0f;
		f_11f8 = 1;
		f_1248 = 0;
		f_11fc = 0;
		if (j) j->Splash();									// element +0x198
		return;
	}
	if (id == 170 || id == -1) return;
	if (!f_11f8)											// anything else: a small hit
	{
		ClearShocked(f_9f4);
		Shock(f_9f4, -1.0f, -1, 1.0f);
		Play(0xc5);
		f_1238 = 1.0f;
		f_11f8 = 1;
		f_1248 = 0;
		f_11fc = 0;
	}
}

// --------------------------------------------------------------------------- every frame

void C3DRocketShip::Process(float dt)						// 0x43df70
{
	C3DFlyingObject::Process(dt);							// 0x41a140
	if (!GameIsRunning())									// 0x475ca0
	{
		if (!f_9f4) return;
		// the rider and the flame go with us
		CGameObject *jim = f_9f4;
		const OMedia3DVector p = Position(this);
		jim->v197(p.x, p.y, p.z);							// +0x318
		f_9f4->place(p.x, p.y, p.z);						// its element +0x44
		const OMedia3DVector r = Rotation(this);
		jim->v203(r.x, r.y, r.z);							// +0x330
		SetPos4(f_1230, Position(this));
		SetRot4(f_1230, Rotation(this));
		return;
	}

	// the smoke trail: one puff per frame, round the ring of 20 (the puffs are never made)
	if (C3DObject *s = f_9f8[f_11f0])
	{
		if (f_96c <= 20.0f) Hide(s);
		else if (!f_1248) Hide(s);
		else
		{
			if (CGameObject *g = dynamic_cast<CGameObject *>(s)) SetPos4(g, v224(0, -20.0f, -170.0f));
			Show(s);
		}
	}
	f_11f0++;
	if (f_11f0 > 19) f_11f0 = 0;

	if (!f_9f4)
	{
		f_9cc = 0;
		return;
	}
	if (f_9cc > 10.0f && !f_1235) f_1234 = 1;				// too long in the air: the engine dies
	g_gametype->SetCamera(this);							// +0x118
	f_9f4->v120(1);											// +0x1e4
	C3DJimmy *jimmy = AsJimmy(f_9f4);
	if (jimmy && jimmy->f_a1c) jimmy->f_a1c->v120(1);		// Goddard too (port: checked)
	if (!f_9f4->IsAnim((char *)"DRIVE")) f_9f4->ev58((char *)"DRIVE", 1);	// 0x40e1d0, +0xe0

	C3DObject *fire = Element(f_1230);
	if (!f_99d)
	{
		if (fire)
		{
			if (!f_1234)
			{
				if (f_96c <= 0) Hide(fire);
				else Show(fire);							// element +0xd8
			}
			else
			{
				f_968 = 10.0f;
				if ((double)Random1001() * 0.00100000005f <= 0.3) Show(fire);	// flickers
				else Hide(fire);
			}
			SetPos4(f_1230, Position(this));
			SetRot4(f_1230, Rotation(this));
		}
	}
	else Hide(fire);										// no flame while diving

	// the riders go with us
	CGameObject *jim = f_9f4;
	{
		const OMedia3DVector p = Position(this);
		jim->v197(p.x, p.y, p.z);							// +0x318
		f_9f4->place(p.x, p.y, p.z);						// its element +0x44
		const OMedia3DVector r = Rotation(this);
		jim->v203(r.x, r.y, r.z);							// +0x330
		const omt_WAngle ax = get_anglex(), ay = get_angley(), az = get_anglez();	// element +0x50..+0x54
		f_9f4->set_angle(ax, ay, az);
	}
	if (!f_11f4)
	{
		f_11f4 = dynamic_cast<C3DGoddard *>(FindObjectByTag("C3DGODDARD"));	// 0x474070
		if (f_11f4)
		{
			if (!f_11f4->IsAnim((char *)"ROCK")) f_11f4->ev58((char *)"ROCK", 1);
			f_11f4->f_1b60 = 1;								// element +0x9a4: riding along
			f_11f4->v132(0);								// +0x214
			f_11f4->ev84();									// element +0x158
		}
	}
	if (f_11f4)
	{
		const OMedia3DVector g = v224(0, 10.0f, -25.0f);
		SetPos4(f_11f4, g);									// +0x314
		f_11f4->place(g.x, g.y, g.z);
		f_11f4->v132(0);
		const OMedia3DVector r = Rotation(this);
		f_11f4->v203(r.x, r.y, r.z);						// +0x330
		if (!f_11f4->IsAnim((char *)"ROCK")) f_11f4->ev58((char *)"ROCK", 1);
	}

	if (f_11f8)												// hit: flash red
	{
		if (f_1248) v159(0, -700.0f, 0);					// +0x280
		f_11fc = f_11fc + dt;
		float k;
		if (f_11fc <= f_1238) k = (float)((sin((double)f_11fc * 20.0) + 1.0) * 0.5);
		else
		{
			f_11f8 = 0;
			f_1248 = 0;
			f_9f4->ev58((char *)"DRIVE", 1);
			k = 1.0f;
		}
		if (f_5d4[0]) f_5d4[0]->set_diffuse(OMediaFARGBColor(1.0f, 1.0f, k, k));	// +0x4c.. (r, g, b, a) = 1, k, k, 1; port: NULL checked
	}
	if (!f_1234) return;
	v159(0, -750.0f, 0);									// the dead engine: pushed down
}

void C3DRocketShip::v243(float dt)							// 0x43e7e0
{
	if (ConfirmOpen()) return;								// 0x407060
	if (!GameIsRunning()) return;							// 0x475ca0
	C3DFlyingObject::v243(dt);								// 0x41a6c0
	if (KeyFire() && f_9f4)									// 0x42a7a0
	{
		f_122c += dt;
		if (f_122c > 0.4f) f_122c = 0;
	}
	if (C3DJimmy *j = AsJimmy(f_9f4)) j->MenuInput(dt);		// element +0x1ac
}

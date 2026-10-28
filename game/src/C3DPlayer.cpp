/*  C3DPlayer - see C3DPlayer.h.  Reconstructed from Neutron.exe: every function read against
    decomp1/raw/C3DPlayer.c (+ RelPos from C3DFlyingObject.c, ctor/dtor from _free.c). */
#include "C3DPlayer.h"
#include "Mouse.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "Physics.h"
#include "CGameType.h"
#include "CJimmyGame.h"
#include "CEditor.h"
#include "C2DInGameMenu.h"
#include "InterfaceElement.h"
#include "SaveGame.h"
#include "Sound.h"
#include "C3DJetpackFire.h"
#include "OMediaCanvasElement.h"

namespace neg { CGameObject *New_C3DJetpackFire(void); }
#include "gen/globals.h"
#include "OMediaViewPort.h"
#include "OMediaTrigo.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

using namespace neg;

static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
static const float kAngleToDeg = 0.02197265625f;			// 0x48e5b4: 360 / 16384

using neg::GetMouseDelta;									// 0x46a460 (Mouse.h)

// --------------------------------------------------------------------------- helpers

static OMedia3DVector Position(CGameObject *o)
{
	float p[4];
	o->v195(p);												// vtable +0x310
	return OMedia3DVector(p[0], p[1], p[2], p[3]);
}

static OMedia3DVector Velocity(CGameObject *o)
{
	float v[4];
	o->v174(v);												// vtable +0x2bc
	return OMedia3DVector(v[0], v[1], v[2], v[3]);
}

static OMedia3DVector Rotation(CGameObject *o)
{
	float r[4];
	o->v201(r);												// vtable +0x328
	return OMedia3DVector(r[0], r[1], r[2], r[3]);
}

// The side slip of the movement states: (k, 0, 0) turned by our heading only (OMediaCosSinTable).
static OMedia3DVector SideSlip(CGameObject *o, float k)
{
	OMedia3DVector v(k, 0.0f, 0.0f, 0.0f);
	v.rotate(0, (omt_Angle)((int)(Rotation(o).y * kDegToAngle) & omc_MaxAngleMask), 0);
	return v;
}

// The game's random number generator: 0..1000.
static int Random1001(void)
{
	unsigned int *seed = g_random_seed;						// 0x509a40
	*seed = *seed * 0x19660d + 0x7fff;
	return ((int)((*seed >> 16) * 1001) >> 16) & 0xffff;
}

// The movement of states 0, 1 and 2 (MoveWalk / MoveJump / MoveBounce): turn by f_6d4, slip
// sideways by the turn (k: 1/45 walking, 1/55 in the air) and the side step, go forward at f_6c8.
static void MoveForward(C3DPlayer *p, float dt, float k, float side, bool walk)
{
	OMedia3DVector off(0, 0, 0, 0);
	OMedia3DVector vel = Velocity(p);
	if (sqrtf(vel.x * vel.x + vel.z * vel.z + vel.y * vel.y) != 0 || side != 0)
	{
		p->f_6d8 = p->f_6d4;
		p->AddRotation(0, p->f_6d4 * dt, 0);					// +0x334
		const float s = p->f_6d8 * k * (1.0f - p->v145()) * p->f_6c8;
		off = SideSlip(p, walk ? (s + side) * dt : s);			// (in the air without dt)
	}
	OMedia3DVector ahead = p->v226(0, 0, p->f_6c8);			// +0x38c
	p->v176(off.x + ahead.x, vel.y, off.z + ahead.z);		// +0x2c4
}

// MoveJump's gravity: acceleration y = gravity * k - gravity.
static void Lift(CGameObject *o, float y)
{
	float acc[4];
	o->v155(acc);											// +0x270
	o->v157(acc[0], y, acc[2]);								// +0x278
}

// 0x403950: the type of item <item> of menu page <page> (0 when there is no such item).
static short MenuItemType(short page, short item)
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p && item >= 0 && item < 29) return p->items[item].type;
	return 0;
}

// 0x4037f0 (a menu method, `this` unused): the gadget power bar sprite g_4f8028 is 20 x
// (1.04 * v) (in 640 x 480 units; v 0..100, negative counts as 0).
static void SetEnergyBar(float v)
{
	if (v < 0) v = 0;
	if (g_4f8028)
		g_4f8028->set_size((float)((double)(short)g_screen_w * (double)0.0015625f * 20.0),
						   (float)((double)(short)g_screen_h * (double)0.0020833334f * v * 1.04));
}

// The jet pack cannot be used: in these levels, with gadget 2 or 6 or 7 out, without energy, or
// without the jet pack (menu page 0 item 3 not 1 or 4); VR04 always allows it.
static bool NoJetpack(void)
{
	const int id = g_gametype->m_object_id;					// +0x490
	const bool no = id == 'LEV6' || id == 'LV4B' || id == 'LV6A' || id == 'LEV3' || id == 'LV3A' ||
		id == 'LV3B' || id == 'LV3C' || id == 'LV3D' || g_4f0588 == 2 || g_4f83d4 <= 0 ||
		g_4f0588 == 6 || g_4f0588 == 7 || MenuItemType(0, 3) == 0 || MenuItemType(0, 3) == 4;
	return no && id != 'VR04';
}

// The chase camera of ControlWalk / ControlFly / ControlSit: the angles from the camera position
// of the frame start (cur) towards <look>; pitch is taken, heading turned the short way round.
// The old values go to 0x4f8418.. (read by the HUD radar).
static void CamAim(OMedia3DVector cur, OMedia3DVector look, bool keep)
{
	OMedia3DVector d(look.x - cur.x, look.y - cur.y, look.z - cur.z, 0.0f);
	omt_Angle ax, ay;
	d.angles(ax, ay);
	const short vx = g_viewport->get_anglex(), vy = g_viewport->get_angley();
	if (keep)
	{
		g_4f8428 = (float)vx;
		g_4f842c = (float)vy;
		g_4f8418 = g_viewport->getx();
		g_4f841c = g_viewport->gety();
		g_4f8420 = g_viewport->getz();
	}
	short turn = (short)((ay - vy) & 0x3fff);
	if (turn > 0x2000) turn -= 0x4000;
	g_viewport->set_anglex((omt_WAngle)(vx + (short)(ax - vx)));
	g_viewport->set_angley((omt_WAngle)(vy + turn));
}

static void CamMoveBy(float dx, float dy, float dz)
{
	g_viewport->setx(dx + g_viewport->getx());
	g_viewport->sety(dy + g_viewport->gety());
	g_viewport->setz(dz + g_viewport->getz());
}

// --------------------------------------------------------------------------- construction

C3DPlayer::C3DPlayer()										// 0x437280
{
	// (f_714, f_7c8, f_7d8, f_804, f_838, f_9a8 are OMedia3DPoints: w = 1; f_814, f_854 vectors: w = 0.)
	SetName((char *)"C3DPLAYER");
	Log("C3DPlayer()");
	f_6bc = 30.0f;
	f_6c0 = 35.0f;
	f_6c4 = 600.0f;
	f_6c8 = 0;
	f_6cc = 35.0f;
	f_6d0 = 0;
	f_6d4 = 0;
	f_6d8 = 0;
	f_6dc = 0;
	v62(0.8f);												// +0xf8
	v64(0.7f);												// +0x100
	v77(0, 0, 0);											// BG +0x138
	float t[4];
	v76(t);													// BG +0x134
	v171(t[0], t[1], t[2], t[3]);							// BG +0x2b0
	v38(1);
	v66(100.0f);
	v44(0);
	v50(0);
	f_6e0 = 1;
	v218(0, 0, 0);											// +0x36c
	C3DPlayer::ResetCamera();								// 0x437740
	f_7ec = -40.0f;
	f_7f0 = 10.0f;
	f_6e4 = 0;
	f_6e5 = 0;
	f_6e6 = 0;
	f_6e7 = 0;
	f_7fc = 1;
	f_710 = 0;
	f_594 = 0;												// element short +0x580: C3DAnimated's detail level (HI)
	f_7c4 = 0;
	f_7f4 = 0;
	f_800 = 0;
	f_828 = 0;
	f_804.set(0, 0, 0);
	f_814 = OMedia3DVector(0, 0, 0, 0);
	f_824 = 0;
	f_830 = 0;
	f_834 = 0;
	f_7f8 = dynamic_cast<C3DJetpackFire *>(New_C3DJetpackFire());	// 0x421d80 (0x6cc bytes)
	if (f_7f8) f_7f8->f_924 = static_cast<CGameObject *>(this);	// its +0x6bc
	f_86c = 0;
	strcpy(f_88c, "none");
	strcpy(f_8f0, "none");
	strcpy(f_724, "none");
	strcpy(f_774, "none");
	f_82a = 0;
	f_82c = 0;
	g_4f83e0 = 0;
	f_888 = 0;
	f_848 = 0;
	f_84c = 10.0f;
	f_850 = 0;
	f_851 = 0;
	f_864 = 0;
	f_868 = 0;
	g_4f83d4 = 0;
	f_870 = 0;
	f_874 = 0;
	f_880 = 0;
	f_884 = 8.0f;
	f_954 = 0;
	f_9f8 = 0;
	strcpy(f_955, "level1b.gam");
	strcpy(f_88c, "level1b.gam");
	f_9a8.set(0, 0, 0);
	f_9a5 = 0;
	f_9b8 = 0;
	strcpy(f_9bc, g_4f81a8);
	// port: not set by the original (left as the allocation had them)
	f_7e8 = 0;
	f_838.set(0, 0, 0);
	f_878 = 0;
	f_87c = 0;
	f_714.set(0, 0, 0);
}

C3DPlayer::~C3DPlayer()										// 0x437790
{
	Log("~C3DPlayer()");
}

// --------------------------------------------------------------------------- CGameObject family

void C3DPlayer::InitObject(void)							// 0x437830
{
	Log("C3DPlayer::InitObject()");
	C3DAnimated::InitObject();								// 0x40d530
	RegisterProperty("StartPoint", f_724, PROP_ALPHA, 0);
	SetObjectID('3PLA');									// element +0xc4
	InitPhysics();											// vtable +0x2c
}

void C3DPlayer::ResetObject(void)							// 0x437890
{
	C3DAnimated::ResetObject();								// CLocalGameObject's (0x4587f0)
	f_6c8 = 0;
	f_6d4 = 0;
	f_6d8 = 0;
	f_6dc = Rotation(this).y;
}

void C3DPlayer::v261(void)									// 0x43a750 (BG slot 260)
{
	// (debug log)
	f_6c8 = 0;
	v176(0, 0, 0);											// +0x2c4
	StepSoundOff();											// element +0x148
}

void C3DPlayer::v262(CGameObject *other, float dist)		// 0x43a790
{
	if (!other->NameContains((char *)"C3DGODDARD"))			// vtable +0x18
	{
		CGameObject::v262(other, dist);						// 0x4740c0
		return;
	}
	if (g_camera != (CGameObject *)this) v176(0, 0, 0);
}

void C3DPlayer::RotateToDest(float dt)						// 0x43a420
{
	if (!GameIsRunning() || !f_005 || !f_00a) return;		// 0x475ca0
	f_00a = 0;
	float dx = 0, dy = 0, dz = 0;
	if (f_274 != f_224)
	{
		dx = f_274 - f_224;
		if (dx > 180.0f) dx -= 360.0f;
		else if (dx < -180.0f) dx += 360.0f;
		f_00a = 1;
	}
	if (f_278 != f_228)
	{
		dy = f_278 - f_228;
		if (dy > 180.0f) dy -= 360.0f;
		else if (dy < -180.0f) dy += 360.0f;
		f_00a = 1;
	}
	if (f_27c != f_22c)
	{
		dz = f_27c - f_22c;
		if (dz > 180.0f) dz -= 360.0f;
		else if (dz < -180.0f) dz += 360.0f;
		f_00a = 1;
	}
	AddRotation((float)(dx * dt * 4.5), (float)(dy * dt * 3.7), (float)(dz * dt * 4.5));	// vtable +0x330
}

void C3DPlayer::Process(float dt)							// 0x437940
{
	if (!GameIsRunning()) return;							// 0x475ca0
	f_834 += dt;
	C3DAnimated::Process(dt);								// 0x40e050
	if (f_868 > 0)
	{
		f_868 -= dt;
		if (f_868 <= 0 && g_4f83e0)
		{
			// the climb is over: on top of the fence / ladder
			ev58((char *)"STOP", 0);						// element +0xe0
			g_4f83e0 = 0;
			f_888 = 0;
			v196(f_838.x, f_838.y, f_838.z, f_838.w);		// +0x314
			v42(1);											// +0xa8
			f_868 = 0;
		}
	}
	if (g_camera != static_cast<CGameObject *>(this)) return;
	if (g_4f83e0 || g_4f8181) return;
	if (f_7c4 < 0) goto done;
	if (f_7c4 > 1)
	{
		if (f_7c4 == 2) MoveBounce(dt);						// element +0x13c
		goto done;
	}
	if (KeyJump()) f_7c4 = 1;								// element +0x158
	else if (v133())										// on the floor (+0x218)
	{
		if (g_camera == static_cast<CGameObject *>(this))
		{
			if (f_6c8 <= 10.0f)
			{
				if (f_7c4 == 1) JetpackOff();				// element +0x140
				if (f_82a == 0)
				{
					if (KeyFire())
					{
						if (g_4f0588 != 4)
						{
							PlayAnim((char *)"SHOOT");
							StepSoundOff();
						}
					}
					else if (!f_9bc[0]) PlayAnim((char *)"STOP");
				}
				else if (f_82a == -1) { if (!KeyFire()) PlayAnim((char *)"LEFT"); }
				else if (!KeyFire()) PlayAnim((char *)"RIGHT");
			}
			else
			{
				if (f_7c4 == 1) JetpackOff();
				PlayAnim((char *)"RUN");
			}
		}
		f_7e8 = 0;
		f_7c4 = 0;
		f_6d4 = 0;
	}
	if (f_7c4 == 1) MoveJump(dt);							// element +0x134
	else
	{
		if (g_camera && f_6c8 < 10.0f && strcasecmp(ev66(), "RUN") == 0) PlayAnim((char *)"STOP");	// element +0x100
		MoveWalk(dt);										// element +0x130
	}
done:
	v154(f_6c8);											// +0x26c
}

void C3DPlayer::v243(float dt)								// 0x438a60
{
	if (!g_camera) return;
	if ((KeyFire() || KeyJump()) && f_9bc[0])
	{
		f_9bc[0] = 0;
		f_9b8 = 0;
	}
	if (f_888 > 0)
	{
		// climbing: the camera rises
		short mdx, mdy;
		GetMouseDelta(&mdx, &mdy);							// 0x46a460 (the movement is dropped)
		g_viewport->sety(dt * 150.0f + g_viewport->gety());
		f_888 -= dt;
		if (f_888 <= 0) f_888 = 0;
		return;
	}
	switch (f_7c4)
	{
		case 0: ControlWalk(dt); break;						// "Walking A" / "Walking B" (debug log)
		case 1: case 2: ControlFly(dt); break;				// "Flying"
		case 3: ControlSit(dt); break;						// "Sitting"
	}
}

// Wall contact: hurting walls (materials 110..129), then, walking, the start of a climb.
void C3DPlayer::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x43aa40
{
	int id = -1;
	if (f_834 <= 3.0f) return;
	if (poly && poly->get_material())
	{
		id = (int)poly->get_material()->get_chunk_ID();		// material +0xc -> +8 (0 without one)
		if (id >= 0x78 && id <= 0x81) TouchedShock(OMedia3DVector(a, b, c, d));	// element +0x160
		if (id >= 0x6e && id <= 0x77) TouchedHurt();		// element +0x164
	}
	if (f_7c4 != 0) return;
	if (!((id >= 0x3c && id <= 0x45) || id == 0xb4)) return;
	omt_Angle ax, ay;
	if (id == 0xb4)
	{
		// ladder: on a floor polygon, the ladder in front at knee height
		if (!v205()) return;								// +0x338
		OMedia3DVector p = Position(this);
		if (!PhysicsLineBlocked(OMedia3DVector(p.x, p.y + 50.0f, p.z, p.w), v224(0, 50.0f, 200.0f), false)) return;	// 0x47c210
		if (strcasecmp(ev66(), "LADDER") == 0) return;
		OMedia3DVector n(-a, -b, -c, d);
		n.angles(ax, ay);
		f_278 = (float)ay * kAngleToDeg;
		v203(0, f_278, 0);									// +0x330
		OMedia3DVector q = v224(0, 0, 25.0f);
		v196(q.x, q.y, q.z, q.w);							// +0x314
		PlayAnim((char *)"LADDER");							// element +0x150
		return;
	}
	// fence: nothing above it, both sides of us against it, on a floor polygon
	OMedia3DVector p = Position(this);
	if (PhysicsLineBlocked(OMedia3DVector(p.x, p.y + 320.0f, p.z, p.w), v224(0, 320.0f, 300.0f), false)) return;
	p = Position(this);
	if (!PhysicsLineBlocked(OMedia3DVector(p.x + 10.0f, p.y + 50.0f, p.z, p.w), v224(10.0f, 50.0f, 300.0f), false)) return;
	p = Position(this);
	if (!PhysicsLineBlocked(OMedia3DVector(p.x - 10.0f, p.y + 50.0f, p.z, p.w), v224(-10.0f, 50.0f, 300.0f), false)) return;
	if (!v205()) return;
	OMedia3DVector n(-a, -b, -c, d);
	n.angles(ax, ay);
	f_278 = (float)ay * kAngleToDeg;
	OMedia3DVector r = Rotation(this);
	v203(r.x, f_278, r.z);
	OMedia3DVector q = v224(0, 0, 25.0f);
	v196(q.x, q.y, q.z, q.w);
	PlayAnim((char *)"FENCE");
}

// --------------------------------------------------------------------------- animations

void C3DPlayer::ev67(void)									// 0x43a900 "AnimEnded"
{
	// (debug log "AnimEnded")
	if (strcasecmp(ev66(), "FENCE") == 0 || strcasecmp(ev66(), "LADDER") == 0)
	{
		ev58((char *)"STOP", 0);
		g_4f83e0 = 0;
		f_888 = 0;
		v261();												// +0x410
		f_6e4 = 0;
		f_6c8 = 0;
		v196(f_838.x, f_838.y, f_838.z, f_838.w);			// +0x314: on top
		v42(1);												// +0xa8
		return;
	}
	if (strcasecmp(ev66(), "SPLAT") == 0)
	{
		ev58((char *)"STOP", 0);
		return;
	}
	if (strcasecmp(ev66(), "HIT") == 0)
	{
		ev58((char *)"STOP", 0);
		g_4f83e0 = 0;
		f_888 = 0;
	}
}

void C3DPlayer::PlayAnim(char *name)						// 0x43aff0
{
	if (strcasecmp(name, ev66()) == 0) return;
	// these play to their end
	if (strcasecmp(ev66(), "HIT") == 0 || strcasecmp(ev66(), "SPLAT") == 0 ||
		strcasecmp(ev66(), "FENCE") == 0 || strcasecmp(ev66(), "LADDER") == 0 ||
		strcasecmp(ev66(), "SWING") == 0) return;
	if (f_9bc[0] && strcasecmp(name, f_9bc) == 0)
	{
		if (strcasecmp(ev66(), "STOP") == 0) ev58(f_9bc, 1);	// an idle animation, from STOP only
		return;
	}
	if (strcasecmp(name, "RUN") == 0)
	{
		StepSoundOn();										// element +0x144
		ev58((char *)"RUN", 1);
		v157(0, 0, 0);										// +0x278
		return;
	}
	if (strcasecmp(ev66(), "EDGE") != 0 && strcasecmp(name, "EDGE") == 0)
	{
		StepSoundOff();
		ev58((char *)"EDGE", 1);
		return;
	}
	if (strcasecmp(ev66(), "JUMP") != 0 && strcasecmp(name, "JUMP") == 0)
	{
		ev58((char *)"JUMP", 0);
		return;
	}
	if (strcasecmp(name, "SWING") == 0)
	{
		StepSoundOff();
		ev58((char *)"SWING", 1);
		return;
	}
	if (strcasecmp(name, "BACK") == 0)
	{
		if (strcasecmp("BACK", ev66()) != 0) ev58((char *)"BACK", 1);
		return;
	}
	if (f_6c8 < 0) return;									// going backwards: BACK stays
	if (strcasecmp(name, "STOP") == 0 && strcasecmp(ev66(), "SPLAT") != 0)
	{
		StepSoundOff();
		ev58((char *)"STOP", 1);
		v157(0, 0, 0);
		return;
	}
	if (strcasecmp(name, "FALL") == 0)
	{
		ev58((char *)"FALL", 1);
		StepSoundOff();
		return;
	}
	if (strcasecmp(name, "LEFT") == 0)
	{
		StepSoundOff();
		ev58((char *)"LEFT", 1);
		v157(0, 0, 0);
		return;
	}
	if (strcasecmp(name, "RIGHT") == 0)
	{
		StepSoundOff();
		ev58((char *)"RIGHT", 1);
		v157(0, 0, 0);
		return;
	}
	if (strcasecmp(name, "HIT") == 0)
	{
		ev58((char *)"HIT", 0);
		return;
	}
	if (strcasecmp(name, "FENCE") == 0)
	{
		OMedia3DVector top = v224(0, 320.0f, 150.0f);
		f_838.set(top.x, top.y, top.z, top.w);
		v176(0, 0, 0);
		v42(0);
		StepSoundOff();
		g_4f83e0 = 1;
		f_888 = 4.0f;
		f_868 = 5.0f;
		f_834 = 0;
		ev58((char *)"FENCE", 0);
		PlayGameSoundIndex(-1, 0x92, false);				// 0x458980
		return;
	}
	if (strcasecmp(name, "LADDER") == 0)
	{
		OMedia3DVector top = v224(0, 800.0f, 150.0f);
		f_838.set(top.x, top.y, top.z, top.w);
		v176(0, 0, 0);
		v42(0);
		StepSoundOff();
		g_4f83e0 = 1;
		f_888 = 6.0f;
		f_868 = 5.0f;
		f_834 = 0;
		ev58((char *)"LADDER", 0);
		PlayGameSoundIndex(-1, 0x8f, false);
		return;
	}
	ev58(name, 1);
}

// --------------------------------------------------------------------------- input

unsigned char C3DPlayer::KeyJump(void)						// 0x4378d0
{
	if (!g_50993a && !g_keys[0x2d] && !g_keys[0x10] && !g_keys[0x60] && !g_joy_button1) return 0;	// right mouse, Insert, Shift, Num 0
	if (g_4f0588 == 6 && KeyFire() && v133()) return 0;		// gadget 6 shooting on the floor
	return 1;
}

unsigned char C3DPlayer::KeyFire(void)						// 0x42a7a0
{
	if (g_4f83e0) return 0;
	if (!g_509834 && !g_keys[0x0d] && !g_keys[0x11] && !g_joy_button0) return 0;	// left mouse, Enter, Ctrl
	return 1;
}

// --------------------------------------------------------------------------- movement

void C3DPlayer::MoveWalk(float dt)							// 0x437c40
{
	OMedia3DVector vel = Velocity(this);
	if (g_4f83e0) return;
	MoveForward(this, dt, 1.0f / 45.0f, f_870, true);				// 0x49fabc
	if (!g_camera)
	{
		f_9bc[0] = 0;
		f_9b8 = 0;
	}
	if (!v133() || !v205()) return;							// on a floor polygon (+0x338)
	// port: (the original works out the floor's tilt here - v224(0, 0, 1), the polygon's normal,
	// OMedia3DVector::angles and 0x476ff0 - into values it never reads; left out)
	vel = Velocity(this);
	v176(vel.x, vel.y - 250.0f, vel.z);						// pressed onto the floor
}

void C3DPlayer::MoveJump(float dt)							// 0x437f90
{
	f_7ec = -40.0f;
	f_7f0 = 10.0f;
	MoveForward(this, dt, 1.0f / 55.0f, 0, false);					// 0x49d5b8
	// (position and velocity read here, not used)
	const float g = g_gravity.y;							// 0x5cfc04
	if (f_7e8 < 3) Lift(this, g * 1.33f - g);				// 0x4afce4: a low-gravity jump
	else
	{
		if (f_7f8 && f_7f8->f_928 <= 0)						// "MEBTEMP BurnTime > 0" (debug log)
		{
			if (f_7e8 == 4 || f_7e8 == 3)
			{
				f_7e8 = 0;
				f_7c4 = 1;
				ev58((char *)"FALL", 1);
				f_7f8->f_928 = 0;
				JetpackOff();
				return;
			}
		}
		if (f_86c < 500.0f)
		{
			Lift(this, -g - (float)(g * 0.1));				// burning: up
			if (Velocity(this).y < -1.0f) Lift(this, -g - (float)(g * 0.3));
		}
		else Lift(this, g * 0.1f - g);						// high enough: hover
	}
	if (KeyJump())
	{
		if (f_7f8)
		{
			const float pct = f_7f8->f_928 / f_884 * 100.0f;
			// (debug log "MEBTEMP percent=%f")
			SetEnergyBar(pct);								// 0x4037f0
		}
		switch (f_7e8)
		{
			case 0:
				if (v133())
				{
					if (f_7f8) f_7f8->f_928 = 0;
					PlayAnim((char *)"JUMP");
					StepSoundOff();
					v162(0, 550.0f, 0);						// +0x28c
					f_7e8 = 1;
					f_874 = 0;
					return;
				}
				PlayAnim((char *)"FALL");
				JetpackOff();
				return;
			case 1:
				f_874 += dt;
				if (f_874 <= 0.4f) return;					// 0x4afce0
				// fall through: held long enough, the jet pack
			case 2:
				if (NoJetpack())
				{
					f_7e8 = 3;
					return;
				}
				if (f_7f8)
				{
					f_7f8->f_928 = f_884;
					v152(100.0f);							// +0x264
				}
				PlayAnim((char *)"FLY");
				JetpackOn(2);								// element +0x138
				{
					OMedia3DVector v = Velocity(this);
					v176(v.x, 10.0f, v.z);
				}
				f_7e8 = 3;
				return;
			case 3:
				if (!v133()) return;
				f_7c4 = 1;
				ev58((char *)"FALL", 1);
				v152(60.0f);
				if (f_7f8) f_7f8->f_928 = 0;
				JetpackOff();
				f_7e8 = 4;
				return;
			case 5:
				if (v133()) f_7e8 = 0;
				return;
		}
		return;
	}
	switch (f_7e8)
	{
		case 1: f_7e8 = 2; return;
		case 3:
			f_7c4 = 1;
			ev58((char *)"FALL", 1);
			if (f_7f8) f_7f8->f_928 = 0;
			JetpackOff();
			f_7e8 = 4;
			return;
		case 4: f_7e8 = 0; return;
		case 5: if (v133()) f_7e8 = 0; return;
	}
}

void C3DPlayer::MoveBounce(float dt)						// 0x4387e0
{
	f_7ec = -10.0f;
	f_7f0 = 10.0f;
	MoveForward(this, dt, 1.0f / 55.0f, 0, false);
	// (position and velocity read here, not used)
	if (!v133()) return;
	Bounced();												// element +0x154
	if (KeyJump())
	{
		OMedia3DVector v = Velocity(this);
		v176(v.x, 900.0f, v.z);
		PlayGameSoundIndex(-1, 0x6a, false);
		return;
	}
	f_6c8 *= 0.5f;
	OMedia3DVector v = Velocity(this);
	v176(v.x, 300.0f, v.z);
}

OMedia3DVector C3DPlayer::RelPos(float x, float y, float z)	// 0x43a5d0 (also C3DFlyingObject's)
{
	OMedia3DVector r = Rotation(this);
	float a = r.y + f_6d4;
	if (a < 0) a = (float)(360 - abs((int)a));
	else if (a > 360.0f) a -= 360.0f;
	const omt_Angle az = (omt_Angle)((int)(r.z * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle ay = (omt_Angle)((int)(a * kDegToAngle) & omc_MaxAngleMask);
	OMedia3DVector v(x, y, z, 0.0f);
	v.rotate(0, ay, az);
	OMedia3DVector p = Position(this);
	return OMedia3DVector(p.x + v.x, p.y + v.y, p.z + v.z, 1.0f);
}

unsigned char C3DPlayer::FloorFarBelow(void)				// 0x43a7f0
{
	if (f_7c4 != 0) return 0;
	OMedia3DVector a = v224(0, 0, 70.0f);
	OMedia3DVector b(a.x, a.y - 2000.0f, a.z, a.w);
	if (!PhysicsClosestHit(a, b, false)) return 0;			// 0x47c6e0
	return abs((int)(g_phys_ray.end.y - a.y)) > 70 ? 1 : 0;	// 0x5cfc64
}

// --------------------------------------------------------------------------- control + camera

void C3DPlayer::ControlWalk(float dt)						// 0x438bc0
{
	short mdx = 0, mdy = 0;
	GetMouseDelta(&mdx, &mdy);								// 0x46a460
	if (g_4f83e0) return;
	f_848 = (float)mdx + f_848;
	// camera height: gadget 6 out, the jump key held: Down / Up / mouse y
	if (KeyFire())
	{
		if (g_4f0588 == 6)
		{
			if (g_joy_y < -0.5 || g_keys[0x28]) f_84c = f_84c - dt * 20.0f;
			else if (g_joy_y > 0.5 || g_keys[0x26]) f_84c = dt * 20.0f + f_84c;
			else f_84c = (float)mdy * dt + f_84c;
		}
	}
	else if (g_4f0588 != 6) f_84c = 10.0f;

	f_6e4 = 0;
	// (the position is read here and not used)
	if (!v133()) PlayAnim((char *)"FALL");

	// the idle animations: after 10 s still, one of SCRATCH (20%), PLAY (30%), BUTTONS (30%)
	f_9b8 += dt;
	if (f_9b8 > 10.0f)
	{
		if (!f_9bc[0])
		{
			const float r = (float)Random1001() * 0.001f;	// 0x48e5d8
			if (r >= 0.2)
			{
				if (r < 0.4)
				{
					strcpy(f_9bc, "SCRATCH");
					PlayGameSoundIndex(-1, 0x76, false);
				}
				else if (r < 0.7) strcpy(f_9bc, "PLAY");
				else
				{
					strcpy(f_9bc, "BUTTONS");
					PlayGameSoundIndex(-1, 0x64, false);
				}
			}
			if (f_9bc[0]) PlayAnim(f_9bc);
			f_9b8 = 0;
		}
	}
	else if (f_9bc[0])
	{
		if (strcasecmp("SCRATCH", f_9bc) == 0 && f_9b8 > 2.5) { f_9bc[0] = 0; f_9b8 = 0; }
		if (strcasecmp("BUTTONS", f_9bc) == 0 && f_9b8 > 3.0f) { f_9bc[0] = 0; f_9b8 = 0; }
		if (strcasecmp("PLAY", f_9bc) == 0 && f_9b8 > 4.8) { f_9bc[0] = 0; f_9b8 = 0; }
	}

	f_714.x = g_viewport->getx();
	f_714.y = g_viewport->gety();
	f_714.z = g_viewport->getz();
	// (the camera's direction, 0x476f10 + OMedia3DVector::angles, is worked out here and not used)

	bool moved = false;
	f_82a = 0;
	f_850 = 0;
	if (mdy < -30 || g_keys[0x28] || g_joy_y < -0.5f)		// Down / mouse pulled back
	{
		if (!((g_4f0588 == 3 || g_4f0588 == -1) && KeyFire()) && !f_864)
		{
			f_9b8 = 0;
			f_9bc[0] = 0;
			f_850 = 1;
			if (f_828) mdx = 0;
		}
	}
	float turn = (float)((1.05 - f_6c8 / f_6c4) * mdx);		// 0x4afd00
	if (turn > 1.0f)
	{
		moved = true;
		f_82a = -1;
		f_878 = 0.1f;
		f_9b8 = 0;
		f_9bc[0] = 0;
	}
	if (turn < -1.0f)
	{
		moved = true;
		f_82a = 1;
		f_87c = 0.1f;
		f_9b8 = 0;
		f_9bc[0] = 0;
	}
	if (turn > 25.0f)
	{
		moved = true;
		turn = 25.0f;
		f_82a = -1;
		f_878 = 0.5f;
	}
	else if (turn < -25.0f)
	{
		moved = true;
		turn = -25.0f;
		f_82a = 1;
		f_87c = 0.5f;
	}
	if (f_82a == 0)
	{
		if (f_87c > 0) f_87c -= dt;
		if (f_87c > 0) f_82a = 1;
		else f_87c = 0;
		if (f_878 > 0) f_878 -= dt;
		if (f_878 > 0) f_82a = -1;
		else f_878 = 0;
	}
	f_870 = 0;
	if (g_keys[0x25] || g_joy_x < -0.5f)						// Left: turn and step aside
	{
		turn += 20.0f;
		f_870 = -1000.0f;
		f_82a = -1;
		moved = true;
		f_6e6 = 1;
		f_9b8 = 0;
		f_9bc[0] = 0;
		if (f_6c8 < f_6c4)
		{
			f_6c8 += f_6bc;
			if (f_6c8 > f_6c4) f_6c8 = f_6c4;
		}
	}
	else if (g_keys[0x27] || g_joy_x > 0.5f)					// Right
	{
		turn -= 20.0f;
		f_870 = 1000.0f;
		f_82a = 1;
		moved = true;
		f_6e6 = 1;
		f_9b8 = 0;
		f_9bc[0] = 0;
		if (f_6c8 < f_6c4)
		{
			f_6c8 += f_6bc;
			if (f_6c8 > f_6c4) f_6c8 = f_6c4;
		}
	}
	if (g_keys[0x26] || g_joy_y > 0.5f)						// Up: forward
	{
		f_6e4 = 1;
		moved = true;
		f_9b8 = 0;
		f_9bc[0] = 0;
		if (turn != 0) turn *= 0.5f;
		if (f_6c8 < f_6c4)
		{
			f_6c8 += f_6bc;
			if (f_6c8 > f_6c4) f_6c8 = f_6c4;
		}
	}
	if (f_850)
	{
		f_6e4 = 1;
		f_9b8 = 0;
		f_9bc[0] = 0;
		if (!f_828)
		{
			f_6c8 = (float)(f_6c8 - f_6c0 * 1.5);
			if (f_6c8 < f_6c4 * -0.5f) f_6c8 = f_6c4 * -0.5f;
			if (f_830 > 1.5)
			{
				// Back held 1.5 s: turn round
				f_6c8 = f_6c4;
				f_828 = 1;
				const float ry = Rotation(this).y;
				turn = 0;
				f_710 = ry + 180.0f;
			}
		}
		else f_6c8 = f_6c4;
		moved = true;
	}
	else f_830 = 0;
	if (f_828 && !f_850) f_828 = 0;
	if (!f_6e4)
	{
		// nothing pressed: slow down
		if (f_6c8 > 0)
		{
			f_6c8 -= f_6c0;
			if (f_6c8 < 0) f_6c8 = 0;
		}
		if (f_6c8 < 0)
		{
			f_6c8 += f_6c0;
			if (f_6c8 >= 0) f_6c8 = 0;
		}
	}
	if (g_4f0588 == 6 && (g_509834 || g_joy_button0)) f_6c8 = 0;	// gadget 6 firing: stand
	if (f_6c8 < 0) PlayAnim((char *)"BACK");
	if (!f_850 && mdy >= 2)
	{
		// the mouse pushed forward: speed
		f_6c8 = (float)(mdy * 16) + f_6c8;
		if (f_6c8 > f_6c4) f_6c8 = f_6c4;
		moved = true;
	}
	if (moved)
	{
		if (f_828)
		{
			if (turn != 0)
			{
				f_710 = Rotation(this).y - turn * 0.5f;
				v218(0, f_710, 0);							// +0x36c
			}
			else v218(0, f_710, 0);
		}
		else v218(0, Rotation(this).y + turn, 0);
	}

	// the chase camera
	OMedia3DVector want;
	if (f_828)
	{
		if (f_830 > 1.7) want = v224(f_7c8.x, f_7c8.y, (float)(-f_7c8.z * 3.7));	// turning round: in front
		else want = OMedia3DVector(f_804.x, f_804.y, f_804.z, f_804.w);
	}
	else
	{
		want = v224(f_7c8.x, f_7c8.y - (f_84c + f_84c), f_7c8.z);
		f_804.set(want.x, want.y, want.z, want.w);
	}
	OMedia3DVector look = v224(f_7d8.x, (float)(f_84c * 1.9 + f_7d8.y), f_7d8.z);
	const float k = CamPull(look, &want);					// element +0x16c
	float dx = want.x - f_714.x, dy = want.y - f_714.y, dz = want.z - f_714.z;
	if (f_851)
	{
		dx = (float)(dx * dt * 0.2);
		dy = (float)(dy * dt * 0.2);
		dz = (float)(dz * dt * 0.2);
	}
	else if (f_828)
	{
		dx = dx * k * dt;
		dy = dy * k * dt;
		dz = dz * k * dt;
	}
	else
	{
		dx = dx * f_854.x * k * dt;
		dy = (float)(dy * dt * (f_854.y + 0.2) * k);
		dz = dz * f_854.z * k * dt;
	}
	CamMoveBy(dx, dy, dz);
	CamAim(OMedia3DVector(f_714.x, f_714.y, f_714.z, f_714.w), look, true);
}

void C3DPlayer::ControlFly(float dt)						// 0x439900
{
	short mdx = 0, mdy = 0;
	GetMouseDelta(&mdx, &mdy);								// 0x46a460
	float t = (float)mdx;
	if (t == 0)
	{
		if (g_keys[0x25] || g_joy_x < -0.5f)
		{
			t += dt * 100.0f;
			f_6e6 = 1;
		}
		else if (g_keys[0x27] || g_joy_x > 0.5f)
		{
			t -= dt * 100.0f;
			f_6e6 = 1;
		}
		else if (f_6e6)
		{
			// the turn eases back to 0
			if (f_6d4 < 0)
			{
				f_6d4 += dt * 100.0f;
				if (f_6d4 > 0) f_6d4 = 0;
			}
			else if (f_6d4 > 0)
			{
				f_6d4 -= dt * 100.0f;
				if (f_6d4 < 0) f_6d4 = 0;
			}
		}
	}
	else
	{
		t *= dt;
		f_6e6 = 0;
	}
	f_6d4 += t;
	const float throttle = 1.0f / dt * (float)mdy;
	f_6e4 = 0;
	f_6e5 = 0;
	if (g_keys[0x26] || g_joy_y > 0.5f || throttle > 40.0f || (f_7c4 == 2 && KeyFire()))
	{
		f_6e4 = 1;
		if (f_6c8 < f_6c4)
		{
			float inc = throttle;
			if (g_keys[0x26] || g_joy_y > 0.5f || (f_7c4 == 2 && KeyFire())) inc = f_6bc;
			f_6c8 += inc;
			if (f_6c8 > f_6c4) f_6c8 = f_6c4;
		}
	}
	if (!f_6e4 && f_6c4 * 0.90909094f < f_6c8) f_6c8 -= f_6c0 * 0.5f;	// 0x49d5dc
	if (v133())
	{
		if (f_6c8 > 0) f_6c8 -= f_6c0 * 0.5f;
		else f_6c8 = 0;
	}
	if (g_keys[0x28] || g_joy_y < -0.5f || throttle < -40.0f)
	{
		f_6e5 = 1;
		if (f_6c8 > 0)
		{
			if (v133()) f_6c8 -= f_6c0;
			else f_6c8 -= f_6c0 * 0.5f;
			if (f_6c8 < 0) f_6c8 = 0;
		}
		else if (f_6c8 < 0)
		{
			f_6c8 += f_6c0;
			if (f_6c8 > 0) f_6c8 = 0;
		}
	}
	// pitch with the throttle, roll with the turn
	float roll;
	if (f_7c4 == 2)
	{
		if (f_6d4 < -50.0f) f_6d4 = -50.0f;
		else if (f_6d4 > 50.0f) f_6d4 = 50.0f;
		roll = f_6d4 * -0.25f;
	}
	else
	{
		if (f_6d4 < -30.0f) f_6d4 = -30.0f;
		else if (f_6d4 > 30.0f) f_6d4 = 30.0f;
		roll = -f_6d4;
	}
	const float ry = Rotation(this).y;
	if (f_6e4) v218(f_7ec, ry, roll);						// +0x36c
	else if (f_6e5) v218(f_7f0, ry, roll);
	else v218(0, ry, roll);

	// the camera
	OMedia3DVector cur(g_viewport->getx(), g_viewport->gety(), g_viewport->getz(), 0);
	OMedia3DVector want = RelPos(f_7c8.x, f_7c8.y, f_7c8.z);	// element +0x120
	OMedia3DVector look = v224(f_7d8.x, f_7d8.y, f_7d8.z);
	const float k = CamPull(look, &want);
	float dx = want.x - cur.x, dy = want.y - cur.y, dz = want.z - cur.z;
	if (f_851)
	{
		dx = (float)(dx * dt * 0.1);
		dy = (float)(dy * dt * 0.1);
		dz = (float)(dz * dt * 0.1);
	}
	else if (f_7c4 == 2)
	{
		dx = (float)(dx * dt * k * 1.8);
		const float h = g_viewport->gety();
		if (abs((int)(Position(this).y - h)) <= 800 && !(Position(this).y + 20.0f > g_viewport->gety()))
		{
			// QUIRK: compares the camera's move (dy) with the camera height
			if (abs((int)(dy - g_viewport->gety())) > 0x226) dy = (float)(dy * dt * k * 0.8);
			else dy = (float)(dy * dt * k * 0.2);
		}
		else dy = (float)(dy * dt * k * 1.5);
		dz = (float)(dz * dt * k * 1.8);
	}
	else
	{
		dx = (float)(dx * dt * k * 1.2);
		dy = dy * k * dt + dy * k * dt;
		dz = (float)(dz * dt * k * 1.2);
	}
	CamMoveBy(dx, dy, dz);
	CamAim(cur, look, false);
}

void C3DPlayer::ControlSit(float dt)						// 0x43a120
{
	short mdx = 0, mdy = 0;
	GetMouseDelta(&mdx, &mdy);								// 0x46a460
	float t = (float)mdx;
	if (t == 0)
	{
		if (g_keys[0x25] || g_joy_x < -0.5f)
		{
			t += dt * 100.0f;
			f_6e6 = 1;
		}
		else if (g_keys[0x27] || g_joy_x > 0.5f)
		{
			t -= dt * 100.0f;
			f_6e6 = 1;
		}
		else if (f_6e6)
		{
			if (f_6d4 < 0)
			{
				f_6d4 += dt * 100.0f;
				if (f_6d4 > 0) f_6d4 = 0;
			}
			else if (f_6d4 > 0)
			{
				f_6d4 -= dt * 100.0f;
				if (f_6d4 < 0) f_6d4 = 0;
			}
		}
	}
	else
	{
		t *= dt;
		f_6e6 = 0;
	}
	f_6d4 += t;
	OMedia3DVector cur(g_viewport->getx(), g_viewport->gety(), g_viewport->getz(), 0);
	OMedia3DVector want = RelPos(f_7c8.x, f_7c8.y, f_7c8.z);
	OMedia3DVector look = v224(f_7d8.x, f_7d8.y, f_7d8.z);
	const float k = CamPull(look, &want);
	CamMoveBy((float)((want.x - cur.x) * dt * k * 1.2), (float)((want.y - cur.y) * dt * k * 1.2),
			  (float)((want.z - cur.z) * dt * k * 1.2));
	CamAim(cur, look, false);
}

// 0x43b820: a wall between Jimmy and the camera pulls the camera to 3/4 of the way to the hit point.
float C3DPlayer::CamPull(OMedia3DVector look, OMedia3DVector *cam)	// 0x43b820
{
	(void)look;
	float p[4];
	v195(p);												// our position (vtable +0x310)
	OMedia3DVector a(p[0], p[1], p[2]);
	a.w = p[3];
	if (!PhysicsLineBlocked(a, *cam, false)) return 1.0f;	// 0x47c210
	*cam = g_phys_ray.end;									// 0x5cfc60: the hit point
	const float dx = (p[0] - cam->x) * 0.75f;				// 0x49d5ac
	const float dy = (p[1] - cam->y) * 0.75f;
	const float dz = (p[2] - cam->z) * 0.75f;
	cam->x = p[0] - dx;
	cam->y = p[1] - dy;
	cam->z = p[2] - dz;
	return 1.5f;											// 0x4a3138
}

void C3DPlayer::ResetCamera(void)							// 0x437740
{
	f_7c8.set(0, 200.0f, -350.0f);
	f_7d8.set(0, 80.0f, 0);
	f_854.x = 1.6f;
	f_854.y = 1.0f;
	f_854.z = 1.6f;
}

// "PlayerLoadLevel": go to <game> at <start>; <ret_game> / <ret_start> are where the phone booth
// (or the level's exit) brings him back to.
void C3DPlayer::PlayerLoadLevel(char *game, char *start, char *ret_game, char *ret_start)	// 0x43b5a0
{
	// (debug logs "Calling PlayerLoadLevel  %s  %s  %s  %s", "1 %s  %s".."4 %s  %s")
	f_7fc = 0;
	g_editor->f_7f8 = 1;
	StopAllChannels();										// 0x47d850
	char g[80], s[80], rg[80], rs[80];
	strcpy(g, game);
	strcpy(s, start);
	strcpy(rg, ret_game);
	strcpy(rs, ret_start);
	strcpy(f_724, s);
	strcpy(f_774, s);
	strcpy(f_88c, rg);
	strcpy(f_8f0, rs);
	strcpy(f_955, g);
	if (!f_9a5)
	{
		OMedia3DVector p = Position(this);
		f_9a8.set(p.x, p.y, p.z, p.w);
	}
	g_editor->LoadLevelNow(g);								// +0x100 slot 64 (0x46df70): load at the next frame
	StepSoundOff();
}

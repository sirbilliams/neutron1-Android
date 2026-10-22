/*  C3DGoddard - see C3DGoddard.h. Reconstructed from Neutron.exe 0x41c810..0x41eea0.
    Constants: 0x48e5e8 = 16384 / 360, 0x48e5b4 = 360 / 16384, 0x49fabc = 1 / 45, 0x49fab8 = 52,
    0x49fadc = 14.4 (mouse to steering), 0x48e5dc = 200 (arrow distance from the viewport),
    0x49fac0 = 0.8 (chase camera).                                                             */
#include "C3DGoddard.h"
#include "C3DJimmy.h"				// DeactivateGadget, f_1e00
#include "Mouse.h"
#include "C3DShadow.h"
#include "C3DCursor.h"
#include "CGameType.h"
#include "Bridge.h"					// GameIsRunning
#include "Physics.h"
#include "Game.h"
#include "Sound.h"
#include "gen/globals.h"
#include "OMediaElement.h"
#include "OMediaViewPort.h"
#include "OMedia3DVector.h"
#include "OMediaTrigo.h"
#include <math.h>
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
static const float kAngleToDeg = 0.02197265625f;			// 0x48e5b4: 360 / 16384

// The six places around the target (static OMedia3DVectors 0x4f8200, 0x4f81d0, 0x4f81f0,
// 0x4f81c0, 0x4f81e0, 0x4f81b0; w = 0): 0 behind left, 1 left, 2 behind right, 3 right,
// 4 in front, 5 behind.
static const float kSides[6][3] =
{
	{  380.0f, -125.0f,  430.0f },							// 0x4f8200
	{  430.0f, -125.0f,  -30.0f },							// 0x4f81d0
	{ -380.0f, -125.0f,  430.0f },							// 0x4f81f0
	{ -430.0f, -125.0f,  -30.0f },							// 0x4f81c0
	{    0.0f, -125.0f, -430.0f },							// 0x4f81e0
	{    0.0f, -125.0f,  430.0f },							// 0x4f81b0
};

// --------------------------------------------------------------------------- helpers

static OMedia3DVector Position(CGameObject *o)				// vtable +0x310
{
	float p[4];
	o->v195(p);
	return OMedia3DVector(p[0], p[1], p[2], p[3]);
}

// CGameObject slot 227 (0x472b10): (x, y, z) turned by the object's rotation, w = 0 (as in
// C3DAI.cpp; the generated declaration of v226 has the wrong signature).
static OMedia3DVector ObjectDir(CGameObject *o, float x, float y, float z)
{
	float rot[4];
	o->v201(rot);											// vtable +0x328
	const omt_Angle ax = (omt_Angle)((int)(rot[0] * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle ay = (omt_Angle)((int)(rot[1] * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle az = (omt_Angle)((int)(rot[2] * kDegToAngle) & omc_MaxAngleMask);
	OMedia3DVector v(x, y, z, 0.0f);
	v.rotate(ax, ay, az);
	return v;
}

// The game's random number generator (as C3DPlayer's): 0..1000.
static int Random1001(void)
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	return ((int)((*seed >> 16) * 1001) >> 16) & 0xffff;
}

// The running level's ID (g_gametype +0x490: its ObjectID, e.g. 'LV1B').
static int LevelID(void)
{
	return static_cast<CGameObject *>(g_gametype)->m_object_id;
}

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// 0x458e40 (CLocalGameObject family): the nearest object with ObjectID <id> that is active
// (v123) and f_140, by quick_magnitude of (o - it) (as in C3DJimmy_a.cpp).
static CGameObject *FindNearest(CGameObject *o, int id)
{
	CGameObject *best = NULL;
	float best_d = 0;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
	{
		CGameObject *it = *i;
		if (!it || it->m_object_id != id) continue;
		if (!it->v123() || !it->f_140) continue;
		OMedia3DVector a = Position(o), b = Position(it);
		OMedia3DVector d(a.x - b.x, a.y - b.y, a.z - b.z, 0);
		const float m = d.quick_magnitude();
		if (!best || m < best_d)
		{
			best_d = m;
			best = it;
		}
	}
	return best;
}

// The arrow of a C3DCursor (its CGameObject part) as an element.
static C3DObject *Arrow(CGameObject *o)
{
	return o ? dynamic_cast<C3DObject *>(o) : NULL;
}

// The arrow <arrow> 200 units in front of the viewport towards <target>, turned like us (rot,
// only its z kept), spinning at <spin> degrees a second (inline in v243, once per arrow).
static void PointArrow(CGameObject *arrow, CGameObject *target, const float rot[4], float dt, float spin)
{
	const float vx = g_viewport->getx(), vy = g_viewport->gety(), vz = g_viewport->getz();	// 2002 +0x44..+0x4c
	const OMedia3DVector rp = Position(target);
	OMedia3DVector d(rp.x - vx, rp.y - vy, rp.z - vz, 0);
	const float k = 1.0f / d.quick_magnitude();
	d.x = k * d.x;
	d.y = k * d.y;
	d.z = k * d.z;
	d.x = d.x * 200.0f;										// 0x48e5dc
	d.y = d.y * 200.0f;
	d.z = d.z * 200.0f;
	float ar[4];
	arrow->v201(ar);										// +0x328
	arrow->v196(d.x + vx, d.y + vy, d.z + vz, 1.0f);		// +0x314
	arrow->v202(rot[0], rot[1], ar[2], rot[3]);				// +0x32c
	arrow->AddRotation(0, 0, dt * spin);					// +0x334
	Arrow(arrow)->force_show();								// element +0x70 / hide(false)
}

// --------------------------------------------------------------------------- construction

C3DGoddard::C3DGoddard()									// 0x41c810
{
	SetName((char *)"C3DGODDARD");
	Log("C3DGoddard()");
	C3DGoddard::InitObject();								// 0x41cd70 (called directly)
	C3DObject::SetObjectID('3GOD');							// 0x462220
	m_turn_rates.x = 0.3f;									// turn rates
	m_turn_rates.y = 3.2f;
	m_turn_rates.z = 0.3f;
	m_time = 0;
	m_speed = 1.0f;
	m_state = 4;
	m_ai_state = 4;											// "AIState"
	C3DGoddard::ev95(0);									// 0x41cc30
	f_1b5e = 0;
	v136(1);
	m_target_id = '3JIM';
	v50(0);
	f_1b50 = new C3DShadow;									// 0x5f4 bytes
	v107(0);
	v36(0);
	f_1b54 = -1;
	f_1b58 = -1;
	f_009 = 1;
	m_hover = 1;											// hovering
	f_1b60 = 0;
	f_1b64 = 0;
	f_1b68 = 0;
	f_1b6c = 0;
	f_1b70 = 700.0f;
	f_1b74 = 30.0f;
	m_speed = 0;
	f_1b78 = 1;
	f_1b94 = 1500.0f;
	f_1b7c = -1;
	ev72(m_anim_wag, (char *)"WAG");
	ev72(m_anim_run, (char *)"RUN");
	ev72(m_anim_fly, (char *)"FLY");
	ev72(m_anim_sit, (char *)"WAG");
	ev72(m_anim_none, (char *)"WAG");
	ev72(m_anim_walk, (char *)"RUN");
	C3DCursor *c = new C3DCursor;							// 0x6c4 bytes
	f_1b80 = c ? static_cast<CGameObject *>(c) : NULL;
	Arrow(f_1b80)->hide(true);								// element +0x58
	c = new C3DCursor;
	f_1b84 = c ? static_cast<CGameObject *>(c) : NULL;
	Arrow(f_1b84)->hide(true);
	f_1b88 = NULL;
	f_1b8c = NULL;
	f_1b90 = 0;
	v42(1);
	m_990 = 150.0f;
	f_1ba0 = 0;
	f_1ba4 = 0;
}

C3DGoddard::~C3DGoddard()									// 0x41cbe0
{
}

namespace neg {
CGameObject *New_C3DGoddard(void)	{ return new C3DGoddard; }				// factory '3GOD' (0x9f4 bytes)
}

// --------------------------------------------------------------------------- element slots

void C3DGoddard::ev82(void)									// 0x41e530
{
	float r[4];
	m_target->v201(r);										// +0x328
	if (!m_arrived) v218(0, r[1], 0);						// CGameObject +0x36c
}

void C3DGoddard::ev84(void)									// 0x41e6b0
{
	if (f_1b54 != -1) f_1b54 = -1;							// (the sound is not stopped)
}

void C3DGoddard::ev86(void)									// 0x41e6b0 (the same code)
{
	if (f_1b54 != -1) f_1b54 = -1;
}

void C3DGoddard::ev88(void)									// 0x41e6d0
{
	if (f_1b58 != -1)
	{
		StopGameSound(f_1b58, false);						// 0x458a00
		f_1b58 = -1;
	}
}

void C3DGoddard::ev92(float unused)							// 0x41ddd0
{
	(void)unused;
	{
		const OMedia3DVector p = Position(this);
		m_dest_vec.x = m_dest_point.x - p.x;
		m_dest_vec.y = m_dest_point.y - Position(this).y;
		m_dest_vec.z = m_dest_point.z - Position(this).z;
	}
	omt_Angle a1, a2;
	m_dest_vec.angles(a1, a2);
	float ry;
	if (m_state == 5) ry = (float)a2 * kAngleToDeg + 180.0f;	// 0x48e5b0: fleeing - turned round
	else ry = (float)a2 * kAngleToDeg;
	if (!v133()) v218(0, ry, 0);							// +0x36c: yaw only (C3DAI's also turns x)
	if (!m_arrived) v218(0, ry, 0);
}

void C3DGoddard::ev95(short side)							// 0x41cc30
{
	if (side < 0 || side > 5) return;						// (the switch has no default: nothing is stored)
	m_offset.x = kSides[side][0];
	m_offset.y = kSides[side][1];
	m_offset.z = kSides[side][2];
	f_1b5c = side;
}

unsigned char C3DGoddard::ev96(short side)					// 0x41e9e0
{
	OMedia3DVector me = Position(this);						// CGameObject +0x310
	me.y = me.y + 20.0f;									// 0x495324
	if (side < 0 || side > 5) return 0;
	float t[4];
	{
		const OMedia3DVector v = m_target->v224(kSides[side][0], 20.0f, kSides[side][2]);	// +0x384
		t[0] = v.x; t[1] = v.y; t[2] = v.z; t[3] = v.w;
	}
	if (PhysicsLineBlocked(me, OMedia3DVector(t[0], t[1], t[2], t[3]), false)) return 0;	// 0x47c210
	ev95(side);												// +0x17c
	return 1;
}

void C3DGoddard::ev97(float dt)								// 0x41cea0
{
	float side_x = 0, side_z = 0;
	float v[4];
	v174(v);												// CGameObject +0x2bc: velocity
	const float vy = v[1];
	if ((float)sqrt((double)v[0] * v[0] + (double)v[2] * v[2] + (double)vy * vy) != 0)
	{
		f_1b6c = f_1b68;
		AddRotation(0, f_1b68 * dt, 0);						// +0x334
		// leaning out of the turn: (a, 0, 0) turned by our heading (the inlined rotation of
		// angles (0, y, 0): sin 0 / cos 0 from the table)
		const float a = (1.0f - v145()) * (f_1b6c * (1.0f / 45.0f)) * m_speed * dt;	// +0x248; 0x49fabc
		float r[4];
		v201(r);											// +0x328
		const int ay = (int)(r[1] * kDegToAngle) & omc_MaxAngleMask;	// _ftol
		OMediaCosSinTable &tab = OMediaCosSinTable::global;
		const float s0 = tab.sin(0), c0 = tab.cos(0), sy = tab.sin(ay), cy = tab.cos(ay);
		const float t = -(s0 * a);
		const float u = c0 * a;
		const float w = -(s0 * t);
		side_x = u * cy - w * sy;
		side_z = w * cy + u * sy;
	}
	const OMedia3DVector f = ObjectDir(this, 0, 0, m_speed);	// +0x38c
	v176(side_x + f.x, vy, side_z + f.z);					// +0x2c4: velocity (y kept)
	if (f_1b64 > 0)
	{
		f_1b64 = f_1b64 - dt * 52.0f;						// 0x49fab8
		if (f_1b64 <= 0) f_1b64 = 0;
	}
	if (f_1b64 < 0)
	{
		f_1b64 = dt * 52.0f + f_1b64;
		if (!(f_1b64 < 0)) f_1b64 = 0;
	}
}

void C3DGoddard::ev98(unsigned char on)						// 0x41dcd0
{
	if (on)
	{
		float r[4];
		v201(r);											// +0x328
		v203(0, r[1], 0);									// +0x330
		v152(60.0f);										// +0x264
		if (f_1b7c == -1) f_1b7c = PlayGameSoundIndex(-1, 0xcc, true);	// 0x458980: the sonar
		return;
	}
	v152(bg_v67());											// +0x264 (CGameObject +0x10c)
	Arrow(f_1b80)->hide(true);								// element +0x58
	Arrow(f_1b84)->hide(true);
	m_speed = 450.0f;
	if (f_1b7c != -1)
	{
		StopGameSound(f_1b7c, true);						// 0x458a00
		f_1b7c = -1;
	}
}

void C3DGoddard::ev99(void)									// 0x41e720
{
	// try the two sides next to ours, else (while hovering) stay
	static const short next[6][2] = { { 5, 1 }, { 0, 4 }, { 3, 5 }, { 2, 4 }, { 1, 3 }, { 0, 2 } };
	const short s = f_1b5c;
	if (s < 0 || s > 5) return;
	m_time = 0;
	f_1b5e = s;
	short a = next[s][0], b = next[s][1];
	if (s == 4)
	{
		if (!((float)Random1001() * 0.001f <= 0.5))			// 0x48e5d8, 0x48e5d0 (double)
		{
			a = 3;
			b = 1;
		}
	}
	if (ev96(a)) return;									// +0x180
	if (ev96(b)) return;
	if (m_hover) ev95(s);									// +0x17c
}

// --------------------------------------------------------------------------- CGameObject slots

void C3DGoddard::InitObject(void)							// 0x41cd70
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// element +0x108
	static const char *const anims[][2] = {
		{ "HIFLY", "godfly.ASE" }, { "HIEAT", "godeat.ASE" }, { "HIRUN", "godrun.ASE" },
		{ "HISIT", "godsit.ASE" }, { "HISCOOT", "godscooter.ASE" }, { "HIWAG", "godwag.ASE" },
		{ "HIGROWL", "godgrowl.ASE" }, { "HIROCK", "godrocket.ASE" }, { "HIPOINT", "godpoint.ASE" },
	};
	for (unsigned i = 0; i < sizeof(anims) / sizeof(anims[0]); i++)
		CreateAnim((char *)anims[i][0], (char *)anims[i][1]);	// element +0xd8
	CreateTexture((char *)"goddard02.png", 0);				// element +0xf0
	ApplyTexture(f_590, 0);									// element +0xf4 (BG +0x57c: the HI base shape)
	ev58((char *)"WAG", 1);									// element +0xe0 (0x4eca68)
}

void C3DGoddard::InitPhysics(void)							// 0x42ab50
{
	if (!f_4aa) C3DObject::InitPhysics();					// 0x4623d0
}

void C3DGoddard::AddTouching(CGameObject *other)			// 0x41eda0
{
	C3DAI::AddTouching(other);								// 0x40a3c0
	if (other->NameContains((char *)"C3DJIMMY") && m_state == 4)	// following: jump over him
	{
		m_keep_point = 1;
		f_1ba0 = 0;
		m_dest_point.y = Position(m_target).y + 500.0f;		// 0x48e5bc
	}
}

void C3DGoddard::v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly)	// 0x41ee10
{
	(void)nx; (void)ny; (void)nz; (void)nw; (void)poly;
	m_keep_point = 0;
}

void C3DGoddard::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x41e700
{
	(void)a; (void)b; (void)c; (void)d; (void)poly;
	if (m_state == 4) ev99();								// element +0x18c
}

void C3DGoddard::Process(float dt)							// 0x41dee0
{
	if (m_keep_point)										// the jump lasts 5 s at most
	{
		f_1ba0 = dt + f_1ba0;
		if (f_1ba0 > 5.0f)									// 0x48d950
		{
			f_1ba0 = 0;
			m_keep_point = 0;
		}
	}
	if (!GameIsRunning()) return;							// 0x475ca0
	if (!v131())											// +0x210
	{
		f_1b50->hide(true);									// element +0x58
		return;
	}
	if (g_camera && g_camera->NameContains((char *)"C3DROCKETSHIP"))
		v132(0);											// +0x214
	if (!g_windowed)										// 0x509a13: the blob shadow on the ground under us
	{
		const OMedia3DVector q = Position(this);
		const OMedia3DVector a(q.x, q.y + 100.0f, q.z, q.w);	// 0x48d96c
		const OMedia3DVector b(q.x, a.y - 2000.0f, q.z, q.w);	// 0x48e5a4
		if (PhysicsClosestHit(a, b, false))					// 0x47c6e0
		{
			const float hit_y = g_phys_ray.end.y;			// 0x5cfc64
			if (f_1b50)
			{
				const OMedia3DVector s = Position(this);
				f_1b50->v197(s.x, hit_y + 30.0f, s.z);		// +0x318; 0x48d404
				f_1b50->force_show();						// element +0x70 / hide(false)
			}
		}
	}
	if (g_camera == static_cast<CGameObject *>(this)) return;	// steered (v243)
	if (!g_camera)
	{
		// no camera object (a cut scene): in state 4 he sits (SIT, stopped) and the animation runs
		if (m_state == 4)
		{
			if (!f_1b60)
			{
				f_1b60 = 1;
				ev58((char *)"SIT", 1);						// element +0xe0 (0x4eca5c)
				v261();										// +0x410: stop
				float acc[4];
				v155(acc);									// +0x270
				const float az = acc[2];
				v155(acc);
				v157(acc[0], g_gravity.y, az);				// +0x278; 0x5cfc04
			}
			C3DAnimated::Process(dt);						// 0x40e050
			return;
		}
		m_anims_disabled = 0;
		C3DAI::Process(dt);									// 0x408000
		return;
	}
	// Jimmy (or another object) has the camera: follow him
	m_anims_disabled = 1;
	m_state = 4;
	ev92(dt);												// element +0x170
	if (f_1b90)												// go behind the camera object
	{
		f_1b90 = 0;
		if (!g_camera)
		{
			ErrorLog("ERROR: no pco for C3DGoddard");
		}
		else if (g_camera->NameContains((char *)"C3DJIMMY"))
		{
			const OMedia3DVector c = Position(g_camera);
			const OMedia3DVector t = g_camera->v224(0, 0, -250.0f);	// +0x384
			if (!PhysicsLineBlocked(c, t, false))			// 0x47c210
				v196(t.x, t.y, t.z, t.w);					// +0x314
		}
	}
	if ((g_keys[0x47] || g_509acb) && !g_4f8198)			// 0x50987f ('G'), joystick button 7
		f_1b90 = 1;
	if (!f_1b60) C3DAI::Process(dt);						// 0x408000
	else C3DAnimated::Process(dt);							// 0x40e050
	float v[4];
	v174(v);												// +0x2bc
	if (v[1] > 0) ev94(dt, 115.0f);							// element +0x178
	m_time = dt + m_time;
	if (!m_target || m_state != 4) return;
	const OMedia3DVector p = Position(this);
	const OMedia3DVector q = Position(m_target);
	const float d = (float)sqrt((double)(p.y - q.y) * (p.y - q.y) + (double)(p.z - q.z) * (p.z - q.z) + (double)(p.x - q.x) * (p.x - q.x));
	if (d > 350.0f && f_1b60)								// 0x49fae4: far enough: stand up again
	{
		f_1b60 = 0;
		m_hover = 1;
	}
	if (d > 440.0f) m_speed = 600.0f;						// 0x49fae0
	else m_speed = 450.0f;
	if (d > 1500.0f)										// 0x495318: left behind - jump there
	{
		m_keep_point = 0;
		const OMedia3DVector t = m_target->v224(0, 0, -800.0f);	// +0x384
		v196(t.x, t.y, t.z, t.w);							// +0x314
	}
	if (f_1b5c != 2 && f_1b5c != 0 && m_time > 2.0f) ev99();	// 0x48d954; +0x18c
	if (!v133())											// +0x218: in the air
	{
		if (!IsAnim((char *)"FLY")) ev58((char *)"FLY", 1);
		return;
	}
	v174(v);												// +0x2bc
	if ((float)sqrt((double)v[2] * v[2] + (double)v[1] * v[1] + (double)v[0] * v[0]) > 20.0f)	// 0x495324
	{
		if (!IsAnim((char *)"RUN")) ev58((char *)"RUN", 1);
	}
	else if (!IsAnim((char *)"WAG")) ev58((char *)"WAG", 1);
}

void C3DGoddard::v243(float dt)								// 0x41d0c0
{
	C3DObject::v243(dt);									// 0x462650: the viewport behind us
	ev97(dt);												// element +0x184
	short mx = 0, my = 0;
	GetMouseDelta(&mx, &my);								// 0x46a460
	float v[4];
	v174(v);												// +0x2bc
	if (v[1] > 0) ev94(dt, 115.0f);							// element +0x178
	f_1b98 = (float)mx * dt * 14.4f + f_1b98;				// 0x49fadc
	f_1b9c = (float)my * dt * 14.4f + f_1b9c;

	int anim = 0;											// the animation wanted
	// Turn keys: g_keys[0x25] / [0x27] are the cursor keys (0x50985d / 0x50985f), the joystick x
	// is 0x509aa0.
	const bool left = g_keys[0x25] || f_1b98 > 3.0f || g_joy_x < -0.5f;
	const bool right = !left && (g_keys[0x27] || f_1b98 < -3.0f || g_joy_x > 0.5f);
	if (left || right)
	{
		if (left)
		{
			f_1b64 = f_1b64 + 7.0f;							// 0x48e5b8
			if (f_1b64 > 60.0f) f_1b64 = 60.0f;				// 0x49d5b4
		}
		else
		{
			f_1b64 = f_1b64 - 7.0f;
			if (f_1b64 < -60.0f) f_1b64 = -60.0f;			// 0x49d5b0
		}
		if (m_speed <= 50.0f)								// 0x48d400: turning gets him moving
		{
			if (m_speed < 40.0f) m_speed = 40.0f;			// 0x49fad4
			m_speed = f_1b74 + m_speed;
		}
	}
	else
	{
		anim = (int)(unsigned short)my;						// sic: the original leaves its animation variable
															// holding the mouse y here (a short written into a zeroed int)
	}
	// Go: cursor up (0x50985e), the mouse y, the left mouse button (0x509834), the joystick y
	// (0x509aa4) or button 0 (0x509ac4); back: cursor down (0x509860), the joystick, the mouse y.
	if (g_keys[0x26] || f_1b9c > 1.0f || g_mouse_down || g_joy_y > 0.5f || g_joy_button0)
	{
		anim = 0;
		if (m_speed < f_1b70)
		{
			if (m_speed < 20.0f) m_speed = 20.0f;			// 0x495324
			m_speed = f_1b74 + m_speed;
			if (m_speed > f_1b70) m_speed = f_1b70;
		}
	}
	else if (g_keys[0x28] || g_joy_y < -0.5f || f_1b9c < -1.0f)
	{
		anim = 0;
		if (m_speed > -200.0f) m_speed = m_speed - f_1b74;	// 0x49facc
	}
	else if (m_speed <= 0) anim = 1;
	else
	{
		m_speed = m_speed - f_1b74 * 0.25f;					// 0x49fad0
		if (m_speed < 10.0f)								// 0x48e5ec
		{
			anim = 1;
			m_speed = 0;
		}
	}
	if (m_speed > 10.0f) m_speed = m_speed - f_1b74 * (1.0f / 3.0f);	// 0x49fac8
	else if (m_speed < -10.0f) m_speed = f_1b74 * (1.0f / 3.0f) + m_speed;	// 0x49d5d0
	else
	{
		m_speed = 0;
		anim = 1;
	}
	f_1b98 = 0;
	f_1b9c = 0;
	f_1b68 = f_1b64;

	// Jump keys: the right mouse button (0x50993a), Insert, Shift, Num 0 (0x509865, 0x509848,
	// 0x509898) or joystick button 1 (0x509ac5).
	float a[4];
	if (!g_mouse_rdown && !g_keys[0x2d] && !g_keys[0x10] && !g_keys[0x60] && !g_joy_button1)
	{
		v155(a);											// +0x270
		const float az = a[2];
		v155(a);
		v157(a[0], 0, az);									// +0x278
		v174(v);
		if (v[1] < -200.0f)									// 0x49facc: falling: no faster
		{
			v174(v);
			const float vz = v[2];
			v174(v);
			v176(v[0], -200.0f, vz);						// +0x2c4
		}
	}
	else if (Position(this).y < f_1b94)						// flying up
	{
		v155(a);
		const float az = a[2];
		v155(a);
		v157(a[0], 50.0f - g_gravity.y, az);				// 0x48d400, 0x5cfc04
		anim = 3;
		v174(v);
		if (v[1] < -100.0f)									// 0x49fac4
		{
			v174(v);
			const float vz = v[2];
			v174(v);
			v176(v[0], -100.0f, vz);
		}
	}
	else													// at the ceiling
	{
		v155(a);
		const float az = a[2];
		v155(a);
		v157(a[0], 0, az);
		v174(v);
		if (v[1] < -200.0f)
		{
			v174(v);
			const float vz = v[2];
			v174(v);
			v176(v[0], -200.0f, vz);
		}
	}
	if (!v133()) anim = 3;									// +0x218 in the air: FLY
	if (f_1b78 != anim)
	{
		if (anim == 0) { f_1b78 = 0; ev58((char *)"RUN", 1); }
		else if (anim == 1) { f_1b78 = 1; ev58((char *)"WAG", 1); }
		else if (anim == 3) { f_1b78 = 3; ev58((char *)"FLY", 1); }
	}

	// the arrows: 200 units in front of the viewport towards the nearest red neutron / pickup
	float rot[4];
	{
		float r[4];
		v201(r);											// +0x328 (x, y and w are used)
		rot[0] = r[0]; rot[1] = r[1]; rot[2] = 0; rot[3] = r[3];
	}
	f_1b8c = FindNearest(this, '3RED');						// 0x458e40
	if (f_1b8c) PointArrow(f_1b80, f_1b8c, rot, dt, 100.0f);	// 0x48d96c
	else Arrow(f_1b80)->hide(true);
	f_1b88 = FindNearest(this, '3PIC');
	if (f_1b88) PointArrow(f_1b84, f_1b88, rot, dt, -100.0f);	// 0x49fac4
	else Arrow(f_1b84)->hide(true);

	// Tab (0x509841), Space (0x509858), Esc (0x509853) or joystick button 2 (0x509ac6): back to Jimmy
	if (g_keys[0x09] || g_keys[0x20] || g_keys[0x1b] || g_joy_button2)
	{
		CGameObject *jim = FindObjectByTag("JIM1");			// 0x474070 (0x4ec7f8)
		if (jim && !g_4f8182)
		{
			C3DJimmy *j = dynamic_cast<C3DJimmy *>(jim);	// port: the original does not check
			if (j)
			{
				j->DeactivateGadget();						// Jimmy's element +0x178
				g_gametype->SetCamera(jim);					// g_gametype +0x118
				j->f_1e00 = 1;								// the key does not reach his gadgets
			}
		}
	}

	// The chase camera: from our position 80% of the way to the point behind and above us
	// (110 up, 210 back), or to where a wall is in the way (15 above that).
	const OMedia3DVector p = Position(this);
	OMedia3DVector t = v224(0, 110.0f, -210.0f);			// +0x384
	if (PhysicsLineBlocked(p, t, false))					// 0x47c210
	{
		t.x = g_phys_ray.end.x;								// 0x5cfc60
		t.y = g_phys_ray.end.y + 15.0f;						// 0x49d5e0
		t.z = g_phys_ray.end.z;
		t.w = g_phys_ray.end.w;
	}
	OMedia3DVector d(t.x - p.x, t.y - p.y, t.z - p.z, 0);
	const float len = (float)sqrt((double)d.y * d.y + (double)d.x * d.x + (double)d.z * d.z);
	const float k = 1.0f / d.quick_magnitude();
	d.x = k * d.x;
	d.y = k * d.y;
	d.z = k * d.z;
	g_viewport->place(d.x * len * 0.8f + p.x,				// 0x49fac0; 2002 +0x44..+0x4c
					  d.y * len * 0.8f + p.y,
					  d.z * len * 0.8f + p.z);
	float r[4];
	v201(r);												// +0x328
	const int ax = (int)(r[0] * kDegToAngle);				// _ftol; stored as shorts at 2002 +0x50..+0x54
	const int ay = (int)(r[1] * kDegToAngle);
	g_viewport->set_angle((omt_Angle)ax, (omt_Angle)ay, 0);
}

void C3DGoddard::v260(void)									// 0x41e590
{
	C3DAI::v260();											// 0x409480
	f_1b94 = 1500.0f;
	// hidden in LV4A, LV1C, LV1D, LEV6, LEV7 and LV6A; VR06 lets him fly higher
	const int id = LevelID();
	bool hidden;
	if (id > 'LV4A')
	{
		hidden = (id == 'LV6A');
		if (!hidden && id == 'VR06') f_1b94 = 3500.0f;
	}
	else if (id == 'LV4A') hidden = true;
	else if (id > 'LV1D') hidden = false;
	else if (id >= 'LV1C') hidden = true;
	else if (id < 'LEV6') hidden = false;
	else hidden = (id <= 'LEV7');
	if (hidden)
	{
		hide(true);											// element +0x58
		v52(0);												// +0xd0
		v132(0);											// +0x214
		f_1ba4 = 1;
		return;
	}
	force_show();											// element +0x70 / hide(false)
	v52(1);
	v132(1);
	InitPhysics();											// +0x2c
	ev95(0);												// element +0x17c
	f_1b90 = 1;												// next Process: behind Jimmy
	v67(130.0f);											// +0x110
	f_1ba4 = 0;
}

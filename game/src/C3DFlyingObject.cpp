/*  C3DFlyingObject - see C3DFlyingObject.h. Reconstructed from Neutron.exe 0x419b70..0x41b090, 0x43a5d0. */
#include "C3DFlyingObject.h"
#include "Mouse.h"
#include "C3DPlayer.h"										// the input tests (PlayerKey*)
#include "Game.h"
#include "Bridge.h"
#include "Physics.h"
#include "CGameType.h"
#include "gen/globals.h"
#include "OMediaViewPort.h"
#include "OMediaTrigo.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

using namespace neg;

static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360

// --------------------------------------------------------------------------- helpers

using neg::GetMouseDelta;									// 0x46a460 (Mouse.h)

static OMedia3DVector Position(CGameObject *o)
{
	float p[4];
	o->v195(p);												// vtable +0x310
	return OMedia3DVector(p[0], p[1], p[2], p[3]);
}

// --------------------------------------------------------------------------- construction

C3DFlyingObject::C3DFlyingObject()							// 0x419b70
{
	// (f_9a0 and f_9b0 are OMedia3DPoints: w = 1.)
	SetName((char *)"C3DFLYINGOBJECT");
	Log("C3DFlyingObject()");
	v62(0.8f);
	f_960 = 400.0f;											// AccelRate
	f_968 = 1400.0f;										// MaxSpeed
	f_970 = 35.0f;
	f_964 = 1.0f;											// DecelRate
	f_984 = 2500.0f;										// MaxHeight
	f_96c = 0;
	f_974 = 0;
	f_978 = 0;
	f_97c = 0;
	f_980 = 0;
	f_9a0.x = 0;
	f_9a0.y = 200.0f;
	f_9a0.z = -500.0f;
	f_9b0.x = 0;
	f_9b0.y = 50.0f;
	f_9b0.z = 200.0f;
	f_9c4 = 20.0f;											// AccelLean
	f_9c8 = -20.0f;											// DecelLean
	f_988 = 650.0f;											// UpRate
	f_98c = -650.0f;										// DownRate
	f_990 = 650.0f;											// MaxVertVelocity
	f_994 = 0;												// NewGravity
	v77(0, 0, 0);
	f_998 = 1;
	f_99c = 0;
	f_99e = 0;
	f_99f = 0;
	f_005 = 1;
	f_00a = 1;
	v66(100.0f);
	v52(1);
	f_9c0 = 0;
	C3DFlyingObject::InitObject();							// 0x419f70 (called directly)
	v71(0, 0, 0);
	v157(0, 0, 0);
	f_9cc = 0;
	f_9d0 = 0;
	f_9d4 = 0;
	f_9d8 = 0;
	f_9dc = 0;
	f_9dd = 0;
	// port: not set by the original (left as the allocation had them)
	f_99d = 0;
}

C3DFlyingObject::~C3DFlyingObject()							// 0x419ed0
{
	Log("~C3DFlyingObject()");
}

// --------------------------------------------------------------------------- CGameObject family

void C3DFlyingObject::InitObject(void)						// 0x419f70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("MaxHeight", &f_984, PROP_FLOAT, 0);
	RegisterProperty("MaxSpeed", &f_968, PROP_FLOAT, 0);
	RegisterProperty("AccelRate", &f_960, PROP_FLOAT, 0);
	RegisterProperty("DecelRate", &f_964, PROP_FLOAT, 0);
	RegisterProperty("UpRate", &f_988, PROP_FLOAT, 0);
	RegisterProperty("DownRate", &f_98c, PROP_FLOAT, 0);
	RegisterProperty("MaxVertVelocity", &f_990, PROP_FLOAT, 0);
	RegisterProperty("NewGravity", &f_994, PROP_FLOAT, 0);
	RegisterProperty("AccelLean", &f_9c4, PROP_FLOAT, 0);
	RegisterProperty("DecelLean", &f_9c8, PROP_FLOAT, 0);
	SetObjectID('3FLY');									// element +0xc4
}

void C3DFlyingObject::ResetObject(void)						// 0x41a0b0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	f_978 = 0;
	f_97c = 0;
	f_9c0 = 0;
	f_99c = 0;
	f_99d = 0;
	v52(1);													// vtable +0xd0
	v71(0, 0, 0);											// +0x120
	v157(0, 0, 0);											// +0x278
	v34(1);													// +0x88
	v176(0, 0, 0);											// +0x2c4
	float r[4];
	v201(r);												// +0x328
	f_980 = r[1];
}

void C3DFlyingObject::v258(void)							// 0x41afb0
{
	CLocalGameObject::v258();								// (log, ResetObject)
}

void C3DFlyingObject::AddTouching(CGameObject *other)		// 0x41afc0
{
	CGameObject::AddTouching(other);
	if (other->m_object_id == '3PIC')						// a pickup
	{
		if (strcasecmp(other->m_tag, "BARRELROLL") == 0) f_9d4 = 1.2f;
		if (strcasecmp(other->m_tag, "BARRELROLL2") == 0) f_9d4 = 2.4f;
	}
}

void C3DFlyingObject::RotateToDest(float dt)				// 0x41adf0
{
	if (f_9d4 > 0) return;									// no turning towards the destination while rolling
	if (!GameIsRunning() || !f_00a) return;					// 0x475ca0
	f_00a = 0;
	const float want_x = f_274, want_y = f_278, want_z = f_27c;
	float r[4];
	v201(r);
	const float rx = r[0];
	v201(r);
	const float ry = r[1];
	v201(r);
	const float rz = r[2];
	float dx = 0, dy = 0, dz = 0;
	if (want_x != rx)
	{
		dx = want_x - rx;
		if (dx > 180.0f) dx -= 360.0f;
		else if (dx < -180.0f) dx += 360.0f;
		f_00a = 1;
	}
	if (want_y != ry)
	{
		dy = want_y - ry;
		if (dy > 180.0f) dy -= 360.0f;						// (no check for < -180 here, unlike x and z)
		f_00a = 1;
	}
	if (want_z != rz)
	{
		dz = want_z - rz;
		if (dz > 180.0f) dz -= 360.0f;
		else if (dz < -180.0f) dz += 360.0f;
		f_00a = 1;
	}
	AddRotation((float)((double)dx * dt * 2), (float)((double)dy * dt * 4.0f), (float)((double)dz * dt * 3.0f));	// vtable +0x330
}

// --------------------------------------------------------------------------- flight

void C3DFlyingObject::Process(float dt)						// 0x41a140
{
	float side_x = 0, side_z = 0;							// the lean part of the velocity (0 when not moving)
	C3DAnimated::Process(dt);								// 0x40e050
	float v[4];
	v174(v);												// velocity, vtable +0x2bc
	const float vy0 = v[1];
	f_9cc += dt;
	const float speed = (float)sqrt((double)v[0] * v[0] + (double)v[2] * v[2] + (double)vy0 * vy0);

	// turn-around (T, Enter, joystick button 8) and barrel roll (R, joystick button 6) keys: not in a menu / paused
	if (!g_4f8198 && !g_4f8182)
	{
		if (!g_50988c && !g_509845 && !g_509acc) f_9dd = 0;
		else if (speed != 0 && f_9dd == 0)
		{
			f_9dd = 1;
			if (!(f_9d8 > 0)) f_9d8 = 1.0f;
		}
		if (!g_50988a && !g_509aca) f_9dc = 0;
		else if (speed != 0 && f_9dc == 0)
		{
			f_9dc = 1;
			if (!(f_9d4 > 0)) f_9d4 = 1.0f;
		}
	}

	if (!(f_9d8 > 0))
	{
		if (speed != 0)
		{
			f_97c = f_978;
			AddRotation(0, f_978 * dt, 0);					// vtable +0x334
		}
	}
	else													// turning around
	{
		AddRotation(0, dt * 180.0f, 0);
		f_9d8 -= dt;
		if (f_9d8 < 0) f_9d8 = 0;
	}

	if (f_9d4 > 0)											// barrel roll
	{
		f_9d4 -= dt;
		if (f_9d4 <= 0) f_9d4 = 0;
		AddRotation(0, 0, dt * 270.0f);
	}

	if (speed != 0)
	{
		// the sideways part: (lean, 0, 0) turned by the heading (OMT's rotate inlined; the x
		// angle is 0, so its sine and cosine are the tables' first entries)
		const float lean = (float)((1.0 - (double)v145()) * ((double)f_97c * 0.0181818176f) * f_96c);	// 0x49d5b8
		float r[4];
		v201(r);
		const OMediaCosSinTable &t = OMediaCosSinTable::global;
		const omt_Angle a = (omt_Angle)((int)((double)r[1] * kDegToAngle) & omc_MaxAngleMask);	// 0x47f940 (_ftol)
		const float s0 = t.sin(0), c0 = t.cos(0);
		const float sa = t.sin(a), ca = t.cos(a);
		const float c0l = c0 * lean;
		const float s0s0l = -(s0 * -(s0 * lean));
		side_x = c0l * ca - s0s0l * sa;
		side_z = s0s0l * ca + c0l * sa;
	}
	const OMedia3DVector fwd = v226(0, 0, f_96c);			// vtable +0x38c
	if (!(f_9d8 > 0))
		v176(side_x + fwd.x, vy0, side_z + fwd.z);

	float p[4];
	v195(p);												// position
	const float px = p[0], py = p[1], pz = p[2];
	if (f_96c >= 100.0f)
	{
		const float top = f_984;
		if (top <= py)										// at the ceiling
		{
			v197(px, top, pz);								// +0x318
			f_9c0 = 0;
			if (f_99d) f_9c0 = f_98c;
		}
		else if (!f_99c)
		{
			if (!f_99d)
			{
				// neither: bring the vertical speed back to 0
				f_9c0 = 0;
				float w[4];
				v174(w);
				if (w[1] <= 0) f_9c0 = 160.0f;
				else f_9c0 = -160.0f;
				if (w[1] < 60.0f && w[1] > -60.0f)
				{
					v175(w[0], 0, w[2], w[3]);				// +0x2c0
					f_9c0 = 0;
				}
			}
			else f_9c0 = f_98c;
		}
		else f_9c0 = f_988;
	}
	float acc[4];
	v155(acc);
	const float az = acc[2];
	v155(acc);
	v157(acc[0], f_9c0 - g_gravity.y, az);					// gravity cancelled (0x5cfc04)
	// (debug text "VERT V %d" / "MAX V %d" of the velocity y and f_990: compiled out)
	float w[4];
	v174(w);
	if (w[1] > f_990)
	{
		v174(w);
		const float wz = w[2];
		v174(w);
		v176(w[0], f_990, wz);
	}
	v174(w);
	if (-f_990 > w[1])
	{
		v174(w);
		const float wz = w[2];
		v174(w);
		v176(w[0], -f_990, wz);
	}
}

// 0x419de0 (element slot 75): the camera position <cam> pulled in front of a wall between the
// ship and it; 2 when it was moved, else 1. <look> is not used.
float C3DFlyingObject::ev109(OMedia3DVector look, OMedia3DVector *cam)
{
	(void)look;
	float p[4];
	v195(p);												// our position
	OMedia3DVector a(p[0], p[1], p[2]);
	a.w = p[3];
	if (!PhysicsLineBlocked(a, *cam, false)) return 1.0f;
	*cam = g_phys_ray.end;									// the hit point (0x5cfc60)
	const float dx = (p[0] - cam->x) * 0.75f;
	const float dy = (p[1] - cam->y) * 0.75f;
	const float dz = (p[2] - cam->z) * 0.75f;
	cam->x = p[0] - dx;
	cam->y = p[1] - dy;
	cam->z = p[2] - dz;
	return 2.0f;											// 0x48d954
}

OMedia3DVector C3DFlyingObject::ev106(float x, float y, float z)	// 0x43a5d0 
{
	float r[4];
	v201(r);
	float a = r[1] + f_978;
	if (a < 0) a = (float)(360 - abs((int)a));
	else if (a > 360.0f) a -= 360.0f;
	const omt_Angle az = (omt_Angle)((int)(r[2] * kDegToAngle) & omc_MaxAngleMask);
	const omt_Angle ay = (omt_Angle)((int)(a * kDegToAngle) & omc_MaxAngleMask);
	OMedia3DVector v(x, y, z, 0.0f);
	v.rotate(0, ay, az);
	OMedia3DVector p = Position(this);
	return OMedia3DVector(p.x + v.x, p.y + v.y, p.z + v.z, 1.0f);
}

// --------------------------------------------------------------------------- controls + camera

// The chase camera's heading: the angles from <cur> towards <look>; pitch is taken, heading turned
// the short way round (as C3DPlayer's).
static void CamAim(OMedia3DVector cur, OMedia3DVector look)
{
	OMedia3DVector d(look.x - cur.x, look.y - cur.y, look.z - cur.z, 0.0f);
	omt_Angle ax, ay;
	d.angles(ax, ay);
	const short vx = g_viewport->get_anglex(), vy = g_viewport->get_angley();
	short turn = (short)((ay - vy) & 0x3fff);
	if (turn > 0x2000) turn -= 0x4000;
	g_viewport->set_anglex((omt_WAngle)(vx + (short)(ax - vx)));
	g_viewport->set_angley((omt_WAngle)(vy + turn));
}

void C3DFlyingObject::v243(float dt)						// 0x41a6c0
{
	short mdx = 0, mdy = 0;
	GetMouseDelta(&mdx, &mdy);								// 0x46a460
	const OMedia3DVector cur(g_viewport->getx(), g_viewport->gety(), g_viewport->getz(), 0.0f);
	const float mx = (float)mdx;
	f_9d0 = (float)((double)mdy * dt + f_9d0);

	// turning
	float turn = mx;
	if (mx == 0)
	{
		if (g_50985d || g_joy_x < -0.5f)					// Left arrow
		{
			f_99e = 1;
			turn = dt * 160.0f + turn;
		}
		else if (g_50985f || g_joy_x > 0.5f)				// Right arrow
		{
			f_99e = 1;
			turn = turn - dt * 160.0f;
		}
		else if (f_99e)										// ease back to 0
		{
			if (0 <= f_978)
			{
				if (0 < f_978)
				{
					f_978 = f_978 - dt * 130.0f;
					if (f_978 < 0) f_978 = 0;
				}
			}
			else
			{
				f_978 = dt * 130.0f + f_978;
				if (f_978 > 0) f_978 = 0;
			}
		}
	}
	else													// the mouse turns
	{
		turn = mx * dt;
		f_99e = 0;
	}
	f_978 = turn + f_978;

	// speed
	if (g_509882 && g_509881 && g_509885 && g_509891 && g_509848)	// a cheat key combination: full speed
	{
		f_968 = 2500.0f;
		f_96c = 2500.0f;
	}
	f_99c = 0;
	f_99d = 0;
	if (g_50985e || g_joy_y > 0.5f || g_509834 || f_9d0 > 15.0f)	// Up arrow, joystick up, left mouse, mouse up: climb
	{
		f_99c = 1;
		if (f_96c < f_968)
		{
			f_96c = f_960 + f_96c;
			if (f_968 < f_96c) f_96c = f_968;
		}
	}
	if (!f_99c && f_968 * 0.909090936f < f_96c)				// 0x49d5dc: above 10/11 of the top speed, slow down
		f_96c = f_96c - f_964 * 0.5f;
	if (v133())												// on the floor
	{
		f_9cc = 0;
		if (f_96c <= 0) f_96c = 0;
		else
		{
			f_96c = f_96c - dt * 500.0f;
			if (f_96c < 0) f_96c = 0;
		}
	}
	if (g_509860 || g_joy_y < -0.5f || g_50993a || f_9d0 < -15.0f)	// Down arrow, joystick down, right mouse, mouse down: dive
	{
		f_99d = 1;
		if (f_96c <= 0)
		{
			if (f_96c < 0)
			{
				f_96c = f_964 + f_96c;
				if (f_96c > 0) f_96c = 0;
			}
		}
		else if (v133())
		{
			f_96c = f_96c - dt * f_964;
			if (f_96c < 0) f_96c = 0;
		}
		else
		{
			f_96c = f_96c - f_964 * 0.1f;					// (per frame, not per second)
			if (f_96c < 0) f_96c = 0;
		}
	}
	if (f_99c) f_99d = 0;
	if (f_9d0 > 10.0f) f_9d0 = f_9d0 - dt * 20.0f;
	if (f_9d0 < -10.0f) f_9d0 = dt * 20.0f + f_9d0;
	if (f_978 < -45.0f) f_978 = -45.0f;
	else if (f_978 > 45.0f) f_978 = 45.0f;

	// pitch and roll the model (destination rotation), and tell the engine
	float r[4];
	if (f_99c)
	{
		v201(r);
		v218(f_9c4, r[1], -f_978);							// +0x36c
		ev108(2);											// element +0x128
	}
	else if (!f_99d)
	{
		if (f_96c <= 0)
		{
			v201(r);
			v218(0, r[1], 0);
			ev108(0);
		}
		else
		{
			v201(r);
			v218(0, r[1], -f_978);
			ev108(1);
		}
	}
	else
	{
		v201(r);
		v218(f_9c8, r[1], -f_978);
		ev108(0);
	}

	// the chase camera: moves towards the point behind the ship, looks at the point in front of it
	OMedia3DVector want = v224(f_9a0.x, f_9a0.y, f_9a0.z);	// +0x384
	const OMedia3DVector look = v224(f_9b0.x, f_9b0.y, f_9b0.z);
	const float k = ev109(look, &want);						// element +0x12c: in front of walls
	const double fx = (double)(want.x - cur.x) * k * dt;
	const float fy = (want.y - cur.y) * k * dt;
	const float fz = (want.z - cur.z) * k * dt;
	const float m = (g_4f81ac > 1) ? 1.2f : 0.6f;			// 0x49d5c0 / 0x49d5c4
	g_viewport->setx((float)(fx * m + g_viewport->getx()));
	g_viewport->sety((float)((double)fy * m + g_viewport->gety()));
	g_viewport->setz((float)((double)fz * m + g_viewport->getz()));
	CamAim(cur, look);
}

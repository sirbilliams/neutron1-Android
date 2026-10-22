/*  C3DAI - see C3DAI.h. Reconstructed from Neutron.exe 0x407a40..0x40a8a0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DAI.h"
#include "objects/C3DPatrolPoint.h"
#include "Game.h"
#include "Bridge.h"
#include "Physics.h"
#include "CGameType.h"
#include "Sound.h"
#include "gen/globals.h"
#include "OMediaTrigo.h"
#pragma GCC diagnostic pop
#include <math.h>
#include <string.h>
#include <strings.h>

using namespace neg;

static const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
static const float kAngleToDeg = 0.02197265625f;			// 0x48e5b4: 360 / 16384

// --------------------------------------------------------------------------- helpers

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

static OMedia3DVector Position(CGameObject *o)
{
	float p[4];
	o->v195(p);												// slot 196 (+0x310)
	return OMedia3DVector(p[0], p[1], p[2], p[3]);
}

// (x, y, z) turned by the object's rotation (slot 202, +0x328), w = 0: CGameObject slot 227
// (0x472b10), also inlined by ev81 and AddTouching.
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

// The length of a difference vector as the original computes it (x87: one rounding at the end).
static float Length(float x, float y, float z)
{
	return (float)sqrt((double)x * x + (double)y * y + (double)z * z);
}

// 0x40e1d0 (C3DAnimated): the animation playing now is <name> (_stricmp with +0x5e8).
static bool AnimIs(C3DAI *ai, const char *name)
{
	return strcasecmp(name, ai->ev66()) == 0;				// ev66: the name of the current animation
}

// Play <name> (element slot 56) unless it plays already or a cut scene animates us.
static void PlayUnlessPlaying(C3DAI *ai, char *name)
{
	if (!AnimIs(ai, name) && ai->m_anims_disabled == 0) ai->ev58(name, 1);
}

// The patrol point behind a CGameObject (the original subtracts 0xc8 once NameContains said so).
static C3DPatrolPoint *PatrolPoint(CGameObject *o)
{
	return o ? dynamic_cast<C3DPatrolPoint *>(o) : NULL;
}

// The target's "visible" flag (CLocalGameObject +0x49d in Neutron.exe; f_4a9 of the tree's
// Negatron-shaped CLocalGameObject).
static bool Visible(CGameObject *o)		{ return static_cast<CLocalGameObject *>(o)->f_4a9 != 0; }

// m_dest_vec / m_dest_dist from m_dest_point (inline in ev76, ev81 and AddTouching).
static void DestDir(C3DAI *ai)
{
	const OMedia3DVector p = Position(ai);
	ai->m_dest_vec.x = ai->m_dest_point.x - p.x;
	ai->m_dest_vec.y = ai->m_dest_point.y - Position(ai).y;
	ai->m_dest_vec.z = ai->m_dest_point.z - Position(ai).z;
	ai->m_dest_dist = Length(ai->m_dest_vec.z, ai->m_dest_vec.y, ai->m_dest_vec.x);
}

// The angles of m_dest_vec in degrees (ev78, ev92); fleeing (state 5) turns the yaw round.
static void FacingAngles(C3DAI *ai, float *rx, float *ry)
{
	omt_Angle a1, a2;
	ai->m_dest_vec.angles(a1, a2);
	*rx = (float)a1 * kAngleToDeg;
	if (ai->m_state == 5) *ry = (float)a2 * kAngleToDeg + 180.0f;	// 0x48e5b0
	else *ry = (float)a2 * kAngleToDeg;
}

// --------------------------------------------------------------------------- construction

C3DAI::C3DAI()												// 0x407a40
{
	m_offset.w = 0;											// (vectors: w = 0, points: w = 1)
	m_dest_vec.w = 0;
	m_turn_rates.w = 1.0f;
	m_dest_point.w = 1.0f;
	m_8f8.w = 1.0f;
	m_saved_offset.w = 0;
	m_hold_pos.w = 1.0f;
	SetName((char *)"C3DAI");
	Log("C3DAI()");
	v64(0.3f);
	C3DObject::SetObjectID('3DAI');							// element slot 49, called directly
	v67(150.0f);											// slot 68: radius
	v38(1);
	m_time = 0;
	m_speed = 200.0f;
	m_target = NULL;
	m_target_id = '3JIM';
	f_005 = 1;
	f_00a = 1;
	m_state = 1;
	m_ai_state = 1;
	m_prev_state = -1;
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 0;
	m_dest_vec.x = 0;
	m_dest_vec.y = 0;
	m_dest_vec.z = 0;
	m_arrived = 0;
	m_arrived_time = 0;
	m_dest_dist = 0;
	m_stuck_dist = 0;
	m_visible_range = 2500.0f;
	m_fov = 90.0f;
	v77(0, 1.0f, 1.0f);										// slot 78
	m_turn_rates.x = 0;
	m_turn_rates.z = 0;
	v38(1);
	strcpy(m_patrol_point, "none");
	m_time2 = 0;
	m_keep_point = 0;
	v44(0);
	ev72(m_anim_wag, (char *)"WAG");						// (called directly)
	ev72(m_anim_run, (char *)"RUN");
	ev72(m_anim_fly, (char *)"FLY");
	ev72(m_anim_sit, (char *)"SIT");
	ev72(m_anim_none, (char *)"none");
	ev72(m_anim_walk, (char *)"WALK");
	strcpy(m_target_name, "JIM1");
	m_dest = NULL;
	m_turn_rates.x = 1.0f;
	m_turn_rates.y = 3.0f;
	m_turn_rates.z = 2.0f;
	m_dest_point.x = 0;
	m_dest_point.y = 0;
	m_dest_point.z = 0;
	m_8f8.x = 0;
	m_8f8.y = 0;
	m_8f8.z = 0;
	m_hover = 0;
	m_909 = 0;
	m_wait_time = 0;
	m_no_turn = 0;
	m_saved_state = 1;
	m_saved_target = NULL;
	m_saved_offset.x = 0;
	m_saved_offset.y = 0;
	m_saved_offset.z = 0;
	m_attack_time = 0;
	m_stuck_timer = 0;
	m_seen_time = 0;
	m_flee_dist = 1500.0f;
	m_938 = 0;
	m_move_forward = 0;
	m_prev_point = NULL;
	m_arrive_dist = 110.0f;
	m_keep_fleeing = 0;
	m_wander_started = 0;
	m_wander_time = 0;
	m_958 = 0;
	m_state_locked = 0;
	m_wander_range = 1500.0f;
	m_see_through_walls = 0;
	// (C3DAnimated +0x644 = 1 here: TODO(BG) that member is not known in the tree's C3DAnimated yet)
	m_anims_disabled = 0;
	m_flee_time = 0;
	m_974 = 0;
	m_check_stuck = 1;
	m_held = 0;
	m_hold_pos.x = 0;
	m_hold_pos.y = 0;
	m_hold_pos.z = 0;
	m_hold_when_touched = 0;
	m_990 = 150.0f;
	f_c64 = 0;												// port: Negatron-only members (TODO(BG))
	f_1b30 = 0;
	f_1b34 = 0;
	f_1b3c = 200.0f;
	f_1b44 = 0;
	f_930 = 1.0f;											// port: Negatron's C3DPlayerObject members (TODO(BG))
	f_934 = -1;
	strcpy(f_938, g_4f81a8);
	// (debug log)
}

C3DAI::~C3DAI()												// 0x407e60
{
}

// --------------------------------------------------------------------------- CGameObject family

void C3DAI::InitObject(void)								// 0x407ee0
{
	Log("InitObject()");
	C3DAnimated::InitObject();
	RegisterProperty("PatrolPoint",  m_patrol_point,   PROP_ALPHA, 0);
	RegisterProperty("VisibleRange", &m_visible_range, PROP_FLOAT, 0);
	RegisterProperty("FOV",          &m_fov,           PROP_FLOAT, 0);
	RegisterProperty("TargetName",   m_target_name,    PROP_ALPHA, 0);
	RegisterProperty("AIState",      &m_ai_state,      PROP_LONG,  0);
	RegisterProperty("WanderRange",  &m_wander_range,  PROP_FLOAT, 0);
	// (debug log)
}

void C3DAI::ResetObject(void)								// 0x407eb0
{
	CLocalGameObject::ResetObject();
	f_00a = 1;
	m_state = m_ai_state;
	m_dest = NULL;
}

void C3DAI::v242(float t)									// 0x407fd0
{
	C3DAnimated::v242(t);
	ev69();													// element slot 67 (+0x10c, empty)
}

void C3DAI::Process(float dt)								// 0x408000
{
	if (!GameIsRunning()) return;							// 0x475ca0
	C3DAnimated::Process(dt);
	m_time += dt;
	m_time2 += dt;
	float acc[4];
	v155(acc);												// slot 156 (read twice by the original: z, then x)
	if (m_hover)											// hovering: gravity cancelled
		v157(acc[0], -g_gravity.y, acc[2]);					// slot 158
	else if (m_state == 3)
		v157(acc[0], g_gravity.y + g_gravity.y, acc[2]);
	else
		v157(acc[0], g_gravity.y * 6.0f, acc[2]);			// 0x48e590

	if (g_camera && g_camera->NameContains((char *)"C3DJIMMY") && static_cast<CLocalGameObject *>(g_camera)->f_4ab)
	{
		v175(0, 0, 0, 0);									// Jimmy is frozen (cut scene): so are we
		return;
	}
	if (m_target) ev81();									// element +0x144
	if (m_attack_time > 0)
	{
		m_attack_time -= dt;
		if (m_attack_time <= 0)								// ev87's time is over: back to what we did
		{
			ev73(m_saved_state);
			m_target = m_saved_target;
			m_offset.x = m_saved_offset.x;
			m_offset.y = m_saved_offset.y;
			m_offset.z = m_saved_offset.z;
			m_attack_time = 0;
			ev88();
		}
	}
	if (m_seen_time > 0)
	{
		m_seen_time -= dt;
		if (m_seen_time <= 0) m_seen_time = 0;
	}

	switch (m_state)
	{
		case 0:												// attack
			if (m_target)
			{
				PlayUnlessPlaying(this, m_anim_walk);
				if (!Visible(m_target))
				{
					if (m_ai_state == 6) ev73(6);
					else ev73(2);
				}
				else
				{
					ev77(m_target);
					ev74();
				}
			}
			if (m_dest_dist > m_visible_range)
			{
				if (m_ai_state == 6) ev73(6);
				else ev73(2);
			}
			break;

		case 1:												// wait
			v176(0, 0, 0);									// slot 177
			break;

		case 2:												// patrol, watching
			if (CanSeeTarget()) ev73(0);
			// fall through
		case 3:												// patrol
		{
			if (m_check_stuck) ev89(dt);
			ev75();											// element +0x12c
			PlayUnlessPlaying(this, m_anim_run);
			if (m_dest)
			{
				if (m_seen_time <= 0) ev77(m_dest);
				else
				{
					ev81();
					const OMedia3DVector t = Position(m_target);	// (sic: the target, not m_dest)
					m_dest_point.x = t.x;
					m_dest_point.y = Position(m_target).y;
					m_dest_point.z = Position(m_target).z;
				}
				ev74();
			}
			if (!m_arrived || !m_dest) break;
			C3DPatrolPoint *pp = PatrolPoint(m_dest);
			if (pp && pp->m_wait_time > 0)					// (the original reads +0x7e0 of any m_dest)
			{
				ev73(7);
				if (m_anims_disabled == 0) ev58(pp->m_wait_anim, 1);
				m_wait_time = pp->m_wait_time;
				v176(0, 0, 0);
				break;
			}
			if (!m_dest->NameContains((char *)"C3DPATROLPOINT"))
			{
				ErrorLog("ERROR: invalid NextPatrol point for %s", m_dest->m_tag);
				break;
			}
			m_prev_point = m_dest;
			char next[100];
			strcpy(next, pp ? pp->m_next_patrol_point : "none");
			m_dest = FindObjectByTag(next);
			if (m_dest) break;
			if (strcasecmp(next, "WAIT") == 0) ev73(1);
			else if (strcasecmp(next, "SCAN") == 0) ev73(6);
			break;
		}

		case 4:												// follow
			if (!m_target || !Visible(m_target)) break;
			PlayUnlessPlaying(this, m_anim_run);
			ev77(m_target);
			ev74();
			break;

		case 5:												// flee
			if (m_seen_time > 0 && !Visible(m_target))
			{
				if (m_ai_state == 6)
				{
					ev85();
					v261();									// slot 260 (+0x410): stop
				}
				else ev73(2);
				break;
			}
			if (m_target)
			{
				ev77(m_target);
				ev74();
			}
			if (m_flee_time > 0)
			{
				m_flee_time -= dt;
				if (m_flee_time <= 0)
				{
					m_flee_time = 0;
					if (!m_state_locked) ev73(m_ai_state);
				}
			}
			if (m_dest_dist <= m_flee_dist) break;
			if (m_keep_fleeing > 0) v261();
			else ev73(m_ai_state);
			break;

		case 6:												// scan
			if (CanSeeTarget())
			{
				// (debug log "going to Attacking ")
				ev73(0);
			}
			break;

		case 7:												// waiting at a patrol point
			v176(0, 0, 0);
			if (m_wait_time > 0)
			{
				m_wait_time -= dt;
				if (m_wait_time <= 0)
				{
					ev73(m_prev_state);
					m_arrived = 0;
					if (!m_dest) break;							// port: the original calls through NULL
					if (!m_dest->NameContains((char *)"C3DPATROLPOINT"))
						ErrorLog("ERROR: invalid NextPatrol point for %s", m_dest->m_tag);
					else
						m_dest = FindObjectByTag(PatrolPoint(m_dest)->m_next_patrol_point);
				}
			}
			break;

		case 8:												// hidden
			v176(0, 0, 0);
			v132(0);										// slot 133
			hide(true);										// element +0x58
			break;

		case 10:											// wander
		{
			float p[4];
			v195(p);										// (result unused)
			m_wander_time += dt;
			PlayUnlessPlaying(this, m_anim_run);
			if (m_wander_time > 11.0f) ev93(200.0f, OMedia3DVector(0, 0, 0, 0));	// 0x48e58c
			if (!m_wander_started)
			{
				m_wander_started = 1;
				ev93(1500.0f, OMedia3DVector(0, 0, 0, 0));
			}
			ev77(m_target);									// (only tested for NULL: moves along m_dest_vec)
			ev74();
			break;
		}

		default:
			break;
	}
	if (m_hold_when_touched && m_held) v196(m_hold_pos.x, m_hold_pos.y, m_hold_pos.z, m_hold_pos.w);
	ev78(dt);												// element +0x138
}

void C3DAI::v258(void)										// 0x409460
{
	m_state = m_ai_state;
	v260();
}

void C3DAI::v260(void)										// 0x409480
{
	C3DAnimated::v260();
	m_target = FindObjectByTag(m_target_name);
	if (m_target)
	{
		m_target_id = m_target->m_object_id;				// +0x404
		const OMedia3DVector t = Position(m_target);
		m_dest_point.x = t.x;
		m_dest_point.y = Position(m_target).y;
		m_dest_point.z = Position(m_target).z;
	}
	m_ai_state = m_state;
	ev73(m_state);
	float r[4];
	v91(r);													// slot 92 (+0x170): the start rotation
	v202(r[0], r[1], r[2], r[3]);							// slot 203: rotation now
	v91(r);
	f_274 = r[0];											// rotation RotateToDest turns to
	f_278 = r[1];
	f_27c = r[2];
	f_280 = r[3];
}

void C3DAI::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x409590
{
	CGameObject::v220(a, b, c, d, poly);
	if (m_state == 10) ev93(500.0f, OMedia3DVector(a, b, c, d));	// away from the wall
}

void C3DAI::v265(float a, float b, float c, float d)		// 0x4095f0
{
	CLocalGameObject::v265(a, b, c, d);
	// (debug log "Hitwall called by %s")
	if (m_hover)											// hovering: rise over it
	{
		m_keep_point = 1;
		const OMedia3DVector p = Position(this);
		m_dest_point.x = p.x;
		m_dest_point.y = Position(this).y + 500.0f;			// 0x48e5bc
		m_dest_point.z = Position(this).z;
	}
	if (m_state == 10) ev93(500.0f, OMedia3DVector(a, b, c, d));
}

void C3DAI::RotateToDest(float dt)							// 0x4096e0
{
	if (!GameIsRunning() || !f_005 || !f_00a) return;		// 0x475ca0
	f_00a = 0;
	float dx = 0, dy = 0;
	double dz = 0;											// (x87: kept on the stack, never rounded)
	if (f_274 != f_224)
	{
		dx = f_274 - f_224;
		if (dx > 180.0f) { dx -= 360.0f; f_00a = 1; }
		else
		{
			if (dx < -180.0f) dx += 360.0f;
			f_00a = 1;
		}
	}
	if (f_278 != f_228)
	{
		float t = f_278 - f_228;
		dy = t;
		if (t > 180.0f) { t -= 360.0f; dy = t; }
		else if (t < -180.0f) { t += 360.0f; dy = t; }
		m_last_yaw_step = t;
		f_00a = 1;
	}
	if (f_27c != f_22c)
	{
		dz = (double)f_27c - f_22c;
		if (dz > 180.0f) dz -= 360.0f;
		else if (dz < -180.0f) dz += 360.0f;
		f_00a = 1;
	}
	AddRotation((float)((double)dx * m_turn_rates.x * dt), (float)((double)dy * m_turn_rates.y * dt),
				(float)(dz * m_turn_rates.z * dt));			// slot 205 (+0x334)
}

void C3DAI::AddTouching(CGameObject *other)					// 0x40a3c0
{
	CGameObject::AddTouching(other);
	other->NameContains((char *)"C3DBASEBALL");				// (result unused)
	if (m_hold_when_touched && (other == g_camera || other->NameContains((char *)"C3DGODDARD")))
	{
		m_held = 1;											// held where we are
		float p[4];
		v195(p);
		m_hold_pos.x = p[0]; m_hold_pos.y = p[1]; m_hold_pos.z = p[2]; m_hold_pos.w = p[3];
		ev58(m_anim_sit, 1);								// element +0xe0
	}
	if (!m_target || other != m_target) return;
	if (m_state == 4)
	{
		m_arrived = 0;
		m_keep_point = 1;
		const OMedia3DVector v = ObjectDir(m_target, m_offset.x, m_offset.y, -m_offset.z);	// (z negated: the other side)
		const OMedia3DVector t = Position(m_target);
		m_dest_point.x = v.x + t.x;
		m_dest_point.y = v.y + Position(m_target).y;
		m_dest_point.z = v.z + Position(m_target).z;
		DestDir(this);
	}
	if (m_state == 6) ev73(0);
}

void C3DAI::RemoveTouching(CGameObject *other)				// 0x40a390
{
	CGameObject::RemoveTouching(other);
	if (m_hold_when_touched && other == g_camera) m_held = 0;
}

void C3DAI::v261(void)										// 0x40a6b0
{
	// (debug log)
	v176(0, 0, 0);
}

// --------------------------------------------------------------------------- element family

void C3DAI::ev72(char *dst, char *src)						// 0x407fa0
{
	strcpy(dst, src);
}

void C3DAI::ev73(int state)									// 0x4089b0
{
	if (m_state_locked && (state == 0 || state == 6))
	{
		// (debug log "State %d not set for %s")
		return;
	}
	m_prev_state = m_state;
	m_state = state;
	// (debug log "Setting AIState for %s to %d")
	if (m_state == m_prev_state) return;
	switch (m_state)
	{
		case 0:
			// (debug log "Setting to ATTACKing state for %s", twice)
			if (m_anims_disabled == 0 && !m_state_locked)
			{
				ev91();
				ev58(m_anim_walk, 1);
			}
			break;
		case 1:
			// (debug log "Setting to waiting state for %s", twice)
			ev85();
			if (m_anims_disabled == 0) ev58(m_anim_sit, 1);
			break;
		case 2:
		case 3:
			// (debug log "Setting to Patroling state for %s", twice)
			// fall through
		case 4:
		case 10:
			ev84();
			if (m_anims_disabled == 0) ev58(m_anim_run, 1);
			break;
		case 6:
			// (debug log "Setting to Scanning state for %s", twice)
			if (m_state_locked) break;
			ev85();
			// (debug log "Setting seq to AnimWait for %s %s", twice)
			if (m_anims_disabled == 0) ev58(m_anim_sit, 1);
			v261();											// slot 260 (+0x410)
			break;
		default:
			break;
	}
}

void C3DAI::ev74(void)										// 0x408bc0
{
	if (m_dest_dist < m_arrive_dist)
	{
		m_arrived = 1;
		ev79();												// element +0x13c
		if (!m_keep_point) return;
		m_keep_point = 0;
		v42(1);
		v40(1);
		return;
	}
	if (m_arrived)
	{
		if (m_state == 4)
		{
			if (m_dest_dist <= m_arrive_dist * 1.5) return;	// 0x48e598
			m_arrived = 0;
			if (m_anims_disabled == 0) ev58(m_anim_sit, 1);
			return;
		}
		if (m_dest_dist <= m_arrive_dist) return;
	}
	m_arrived = 0;
}

void C3DAI::ev75(void)										// 0x408c90
{
	if (!m_dest)
	{
		m_dest = FindObjectByTag(m_patrol_point);
		if (m_move_forward && m_dest && m_dest->NameContains((char *)"C3DPATROLPOINT"))
			m_arrive_dist = 440.0f;
		if (!m_dest)
		{
			ev73(1);
			float v[4];
			v174(v);										// slot 175: velocity
			v176(0, v[1], 0);								// stop on the ground, keep falling
			return;
		}
	}
	ev76();
}

void C3DAI::ev76(void)										// 0x408d40
{
	if (!m_keep_point)
	{
		const OMedia3DVector p = Position(m_dest);
		m_dest_point.x = p.x;
		m_dest_point.y = Position(m_dest).y;
		m_dest_point.z = Position(m_dest).z;
	}
	DestDir(this);
}

void C3DAI::ev77(CGameObject *dest)							// 0x408e40
{
	if (!dest) return;
	OMedia3DVector n = m_dest_vec;
	const double inv = 1.0f / n.quick_magnitude();
	double m = 1.0f;
	if (m_state == 4)										// following: hurry when far behind
	{
		m = m_dest_dist > 1000.0f ? 1.7f : 1.0f;			// 0x48e5ac, 0x48e5a8
		if (m_dest_dist > 2000.0f) m = 3.0f;				// 0x48e5a4, 0x48e5a0
	}
	m *= m_speed;
	float vx = (float)(m * (float)(inv * n.x));
	float vy, vz;
	if (m_state == 5)										// fleeing: away from it
	{
		vx = -vx;
		vy = (float)-(m * (float)(inv * n.y));
		vz = (float)-(m * inv * n.z);
	}
	else
	{
		vy = (float)(m * (float)(inv * n.y));
		vz = (float)(m * inv * n.z);
	}
	if (m_arrived)
	{
		v176(0, 0, 0);
		if (m_anims_disabled == 0) ev58(m_anim_sit, 1);
		return;
	}
	if (m_move_forward)										// along our heading instead
	{
		const OMedia3DVector f = ObjectDir(this, 0, 0, 1.0f);	// slot 227 (+0x38c)
		v176(f.x * m_speed, f.y * m_speed, f.z * m_speed);
		return;
	}
	v176(vx, vy, vz);
}

void C3DAI::ev78(float dt)									// 0x409140
{
	if (m_no_turn)
	{
		if (m_speed > 1.0f) m_no_turn = 0;
		return;
	}
	const int s = m_state;
	if (s == 9 || s == 1 || s == 6 || s == 7 || m_anims_disabled != 0) return;
	{
		const OMedia3DVector p = Position(this);
		m_dest_vec.x = m_dest_point.x - p.x;
		m_dest_vec.y = m_dest_point.y - Position(this).y;
		m_dest_vec.z = m_dest_point.z - Position(this).z;
	}
	float rx, ry;
	FacingAngles(this, &rx, &ry);
	if (!v133())											// in the air (slot 134)
	{
		v218(rx, ry, 0);									// slot 219: turn there
		if (!AnimIs(this, m_anim_fly))
		{
			ev83();
			ev58(m_anim_fly, 1);
		}
		return;
	}
	if (!m_arrived)
	{
		v218(0, ry, 0);
		return;
	}
	if (m_target && s != 3 && s != 2 && s != 6) ev82();
	m_arrived_time += dt;
	if (s != 3 && s != 2 && s != 6)
	{
		// (debug log "Calling FacingTarget")
		ev80();
	}
	if (m_arrived_time <= 7.0f)								// 0x48e5b8
	{
		if (!AnimIs(this, m_anim_wag))
		{
			ev86();
			ev58(m_anim_wag, 1);
		}
	}
	else if (!AnimIs(this, m_anim_sit))
	{
		ev85();
		ev58(m_anim_sit, 1);
	}
}

void C3DAI::ev79(void)										// 0x409890
{
	if (m_state == 10) ev93(1400.0f, OMedia3DVector(0, 0, 0, 0));
}

void C3DAI::ev81(void)										// 0x409f80
{
	const OMedia3DVector v = ObjectDir(m_target, m_offset.x, m_offset.y, m_offset.z);
	if (m_state != 10 && !m_keep_point)
	{
		const OMedia3DVector t = Position(m_target);
		m_dest_point.x = v.x + t.x;
		m_dest_point.y = v.y + Position(m_target).y;
		m_dest_point.z = v.z + Position(m_target).z;
	}
	DestDir(this);
}

void C3DAI::ev82(void)										// 0x4093e0
{
	float r[4];
	m_target->v201(r);										// its rotation (slot 202)
	float ry;
	if (r[1] <= 180.0f) { m_target->v201(r); ry = r[1] + 180.0f; }
	else { m_target->v201(r); ry = r[1] - 180.0f; }
	v218(0, ry, 0);
}

void C3DAI::ev87(CGameObject *o, float time)				// 0x40a1b0
{
	if (m_attack_time > 0) return;							// one at a time
	m_saved_state = m_state;
	m_saved_target = m_target;
	m_saved_offset.x = m_offset.x;
	m_saved_offset.y = m_offset.y;
	m_saved_offset.z = m_offset.z;
	m_saved_offset.w = m_offset.w;
	ev73(0);
	m_offset.x = 0;
	m_target = o;
	m_offset.y = 0;
	m_offset.z = 0;
	m_attack_time = time;
}

void C3DAI::ev89(float dt)									// 0x40a250
{
	m_stuck_timer += dt;
	if (!(m_stuck_timer > 5.0f)) return;					// 0x48d950
	m_stuck_timer = 0;
	if (m_stuck_dist - 10.0f <= m_dest_dist && m_dest_dist < m_stuck_dist + 10.0f)	// hardly moved in 5 s
	{
		CGameObject *o;
		if (m_dest && m_dest->NameContains((char *)"C3DPATROLPOINT"))
		{
			m_dest = FindObjectByTag(PatrolPoint(m_dest)->m_next_patrol_point);	// skip it
			m_stuck_dist = m_dest_dist;
			return;
		}
		o = FindObjectByTag(m_patrol_point);
		if (o)
		{
			float p[4];
			o->v195(p);
			v196(p[0], p[1], p[2], p[3]);					// back to the start of the patrol
		}
	}
	m_stuck_dist = m_dest_dist;
}

void C3DAI::ev91(void)										// 0x40a6a0
{
	m_arrived = 0;
	m_arrived_time = 0;
}

void C3DAI::ev92(float unused)								// 0x409030
{
	(void)unused;
	{
		const OMedia3DVector p = Position(this);			// (only m_dest_vec: no distance here)
		m_dest_vec.x = m_dest_point.x - p.x;
		m_dest_vec.y = m_dest_point.y - Position(this).y;
		m_dest_vec.z = m_dest_point.z - Position(this).z;
	}
	float rx, ry;
	FacingAngles(this, &rx, &ry);
	if (!v133() || !m_arrived) v218(rx, ry, 0);
}

void C3DAI::ev93(float range, OMedia3DVector away)			// 0x4098c0
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	const unsigned int a = *seed >> 16;
	*seed = *seed * 0x19660d + 0x7fff;
	const unsigned int b = *seed >> 16;
	OMedia3DVector r(0, 0, 0, 0);							// a random direction on the ground
	r.x = (float)((double)(((int)(a * 1001) >> 16) & 0xffff) * 0.001f - 0.5);	// 0x48e5d8, 0x48e5d0
	r.z = (float)((double)(((int)(b * 1001) >> 16) & 0xffff) * 0.001f - 0.5);
	{
		const float inv = 1.0f / r.quick_magnitude();
		r.x *= inv;
		r.y *= inv;
		r.z *= inv;
	}
	if (away.x != 0)										// away from a wall: 550 along it
	{
		m_dest_point.x = away.x * 550.0f + Position(this).x;	// 0x48e5c8
		m_dest_point.y = Position(this).y;
		m_dest_point.z = away.z * 550.0f + Position(this).z;
	}
	else
	{
		bool done = false;
		if (m_keep_fleeing > 0 && CanSeeTarget())
		{
			OMedia3DVector d;
			const OMedia3DVector p = Position(this), t = Position(m_target);
			d.x = p.x - t.x;
			d.y = Position(this).y - Position(m_target).y;
			d.z = Position(this).z - Position(m_target).z;
			d.w = 0;
			if (Length(d.x, d.y, d.z) < m_visible_range)	// away from the target
			{
				const float inv = 1.0f / d.quick_magnitude();
				r.x = inv * d.x;
				r.z = inv * d.z;
				done = true;
			}
		}
		if (!done)
		{
			if (m_dest == NULL) m_dest = FindObjectByTag(m_patrol_point);
			if (m_dest)										// (the original computes the distance to the patrol point and ignores it)
			{
				const OMedia3DVector p = Position(this), q = Position(m_dest);
				(void)Length(p.x - q.x, Position(this).y - Position(m_dest).y, Position(this).z - Position(m_dest).z);
			}
		}
		m_dest_point.x = (float)((double)r.x * range + Position(this).x + (double)away.x * 550.0f);
		m_dest_point.y = Position(this).y;
		m_dest_point.z = (float)((double)r.z * range + Position(this).z + (double)away.z * 550.0f);
	}
	if (m_state == 10) m_wander_time = 0;
}

void C3DAI::ev94(float unused, float height)				// 0x40a6d0
{
	(void)unused;
	OMedia3DVector a = Position(this);
	OMedia3DVector b = a;
	b.y += height;
	if (!PhysicsLineBlocked(a, b, false)) return;			// 0x47c210
	float v[4];
	v174(v);
	if (!(v[1] > 0)) return;
	v174(v);
	float y = (float)(-v[1] * 0.6);							// 0x48e5f8
	if (y > -85.0f) y = -85.0f;								// 0x48e5f0
	float w[4];
	v174(w);
	v174(v);
	v176(v[0], y, w[2]);
	m_keep_point = 0;
	if (g_camera == static_cast<CGameObject *>(this))
		PlayGameSoundIndex(-1, 0x2f, false);				// 0x458980: sound 47 of the level's set
}

// --------------------------------------------------------------------------- not virtual

unsigned char C3DAI::CanSeeTarget(void)						// 0x409cc0
{
	CGameObject *t = m_target;
	if (!t) return 0;
	if (!Visible(t) && !(m_seen_time > 0)) return 0;
	OMedia3DVector d;
	{
		const OMedia3DVector a = Position(t), b = Position(this);
		d.x = a.x - b.x;
		d.y = a.y - b.y;
		d.z = a.z - b.z;
		d.w = 0;
	}
	if (!(Length(d.z, d.y, d.x) < m_visible_range)) return 0;	// VisibleRange
	if (m_fov <= 358.0f)									// FOV: not all round (0x48e5e4)
	{
		const float inv = 1.0f / d.quick_magnitude();
		const OMedia3DVector f = ObjectDir(this, 0, 0, 1.0f);	// slot 227 (+0x38c): our heading
		const float n0 = inv * d.x, n1 = inv * d.y, n2 = inv * d.z;
		const double dot = (double)f.z * n2 + (double)f.y * n1 + (double)f.x * n0;
		double lim;
		if (m_fov > 180.0f) lim = -(((double)m_fov - 180.0f) * 0.0055555557f);	// 0x48e5e0: 1 / 180
		else lim = 1.0f - (double)m_fov * 0.0055555557f;
		// The original then also tests an uninitialised local byte and only gives up when it is
		// 0. port: taken as 0.
		if (dot <= lim) return 0;
	}
	if (m_see_through_walls) return 1;
	OMedia3DVector a = Position(this), b = Position(t);
	a.y += 200.0f;											// 0x48e5dc: eye height
	b.y += 200.0f;
	return !PhysicsLineBlocked(a, b, false);				// 0x47c210
}

// port: Negatron's animation starters (its slots 117..122): <name> unless it plays already or a
// cut scene animates us. TODO(BG): remove with the subclasses' Negatron code.
void C3DAI::PlayAnimN2(char *name)
{
	PlayUnlessPlaying(this, name);
}

namespace neg {
CGameObject *New_C3DAI(void)	{ return new C3DAI; }		// factory '3DAI' (0x99c bytes)
}

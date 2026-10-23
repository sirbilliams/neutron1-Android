/*  C3DAI ('3DAI') - a computer-controlled character: it patrols a chain of C3DPatrolPoints
    ("PatrolPoint", then each point's NextPatrolPoint), watches for its target ("TargetName",
    Jimmy by default) within "VisibleRange" and its field of view ("FOV"), and then attacks,
    follows, flees or scans according to its state. The base of C3DFriends (and through it of
    Jimmy's friends and Goddard), C3DAIOmtObj, C3DEnemy and the other AI characters.
    Reconstructed from Neutron.exe: constructor 0x407a40 (factory '3DAI', 0x99c bytes),
    destructor 0x407e60 (deleting thunks 0x407e30 / 0x40a850 / 0x40a860 / 0x40a870).

    Original layout (classes.json): as C3DAnimated (element vtable 95 slots: C3DAnimated's 72 +
    72..94; OMediaAnim at +0x90; listener +0x30; CLocalGameObject at +0xc0, 275 slots), own
    members 0x6bc..0x994, vtordisp 0x994, streamer 0x998. Derived classes put theirs from 0x994.
    (Negatron's C3DAI was a C3DPlayerObject with path finding over a patrol-point network,
    states 11..18, "FallbackAIState", "InitialSpeed" and "CanHover": Boy Genius has none.)

    States (m_state, set through ev73; Process runs one case per frame):
      0  attack: walk at the target (WALK); it out of reach / gone -> 6 or 2 (by AIState)
      1  wait (stand still)              2  patrol, seeing the target -> 0
      3  patrol only (RUN to each point; a point's WaitTime -> 7; NextPatrolPoint "WAIT" -> 1,
         "SCAN" -> 6)
      4  follow the target at offset m_offset (RUN; faster when far behind)
      5  flee for m_flee_time seconds, then back to AIState; beyond m_flee_dist -> AIState
      6  scan (stand, SIT), seeing the target -> 0
      7  wait at a patrol point for its WaitTime, then back to the previous state
      8  hidden                          9  frozen (no force)
      10 wander within WanderRange (RUN; a new destination every 11 s)
    Animations: WAG, RUN, FLY, SIT, WALK (names kept in members so subclasses can rename
    them), each started only when not already playing and not while a cut scene animates us
    (m_anims_disabled); the empty slots 83..86 are told before FLY / RUN / SIT / WAG start.

    Element-family slots are `ev<n>` (`// ESLOT n`, n = vtable offset / 4, Neutron.exe's
    numbering); CGameObject-family slots keep the names of the tree's CGameObject (`// SLOT n`
    gives Neutron.exe's slot number, the name may be Negatron's: v176 is slot 177).
    Members are named by what they hold; the comment gives the object offset in Neutron.exe
    (the original's CGameObject-family code sees them at offset - 0xc0).                     */
#pragma once
#include "C3DAnimated.h"
#include "OMedia3DPoint.h"
#include "OMedia3DVector.h"

class OMedia3DPolygon;

class C3DAI : public C3DAnimated
{
	public:
	C3DAI();																// 0x407a40
	virtual ~C3DAI();														// 0x407e60

	// ---- element-family slots
	virtual void ev72(char *dst, char *src);								// ESLOT 72 0x407fa0 strcpy(dst, src)
	virtual void ev73(int state);											// ESLOT 73 0x4089b0 set the state (m_prev_state = old), its animation
	virtual void ev74(void);												// ESLOT 74 0x408bc0 arrival test: m_dest_dist < m_arrive_dist -> m_arrived + ev79
	virtual void ev75(void);												// ESLOT 75 0x408c90 destination: m_dest, else "PatrolPoint" (none: wait), then ev76
	virtual void ev76(void);												// ESLOT 76 0x408d40 destination point from m_dest, vector and distance to it
	virtual void ev77(CGameObject *dest);									// ESLOT 77 0x408e40 velocity along m_dest_vec at m_speed (scaled by distance in state 4)
	virtual void ev78(float dt);											// ESLOT 78 0x409140 turn towards the destination; animations on arrival
	virtual void ev79(void);												// ESLOT 79 0x409890 arrived: state 10 wanders on (ev93 1400)
	virtual void ev80(void) {}												// ESLOT 80 0x472970 (empty) arrived, facing on
	virtual void ev81(void);												// ESLOT 81 0x409f80 destination = target + m_offset turned by its rotation
	virtual void ev82(void);												// ESLOT 82 0x4093e0 face the way the target faces, turned round
	virtual void ev83(void) {}												// ESLOT 83 0x472970 (empty) FLY starts
	virtual void ev84(void) {}												// ESLOT 84 0x472970 (empty) RUN starts (ev73)
	virtual void ev85(void) {}												// ESLOT 85 0x472970 (empty) SIT starts
	virtual void ev86(void) {}												// ESLOT 86 0x472970 (empty) WAG starts
	virtual void ev87(CGameObject *o, float time);							// ESLOT 87 0x40a1b0 attack o for <time> s, then back to the saved state
	virtual void ev88(void) {}												// ESLOT 88 0x472970 (empty) ev87's time is over
	virtual void ev89(float dt);											// ESLOT 89 0x40a250 stuck check every 5 s (skip a patrol point / back to PatrolPoint)
	virtual void ev90(void) {}												// ESLOT 90 0x472970 (empty)
	virtual void ev91(void);												// ESLOT 91 0x40a6a0 not arrived (m_arrived, m_arrived_time = 0)
	virtual void ev92(float unused);										// ESLOT 92 0x409030 turn towards m_dest_point
	virtual void ev93(float range, OMedia3DVector away);					// ESLOT 93 0x4098c0 wander: a random destination within range (+ away * 550)
	virtual void ev94(float unused, float height);							// ESLOT 94 0x40a6d0 bumped the head: bounce down (sound 47 when we are the camera)

	// ---- not virtual
	unsigned char CanSeeTarget(void);										// 0x409cc0 target in range, in the field of view and not behind a wall

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x407ee0 properties
	virtual void ResetObject(void);											// SLOT 10  0x407eb0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x40a3c0
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x40a390
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x409590 wall contact: wanderers turn away
	virtual void Process(float dt);											// SLOT 241 0x408000
	virtual void v242(float t);												// SLOT 242 0x407fd0 (C3DAnimated's, then element slot 67)
	virtual void RotateToDest(float dt);									// SLOT 246 0x4096e0 rates m_turn_rates (1, 3, 2)
	virtual void v258(void);												// SLOT 257 0x409460 back to AIState, then v260
	virtual void v260(void);												// SLOT 259 0x409480 properties loaded: target, state, facing
	virtual void v261(void);												// SLOT 260 0x40a6b0 stop
	virtual void v265(float a, float b, float c, float d);					// SLOT 263 0x4095f0 "Hitwall": hover up / wander away

	// ---- data members (Neutron.exe object offsets)
	float				m_time;					// 0x6bc  time (+= dt)
	CGameObject			*m_target;				// 0x6c0  target (TargetName; ev87 swaps it for a while)
	float				m_speed;				// 0x6c4  speed (200)
	int					m_state;				// 0x6c8  state (see above; 1)
	int					m_prev_state;			// 0x6cc  previous state (-1)
	int					m_target_id;			// 0x6d0  target's ObjectID ('3JIM')
	OMedia3DVector		m_offset;				// 0x6d4  where to be relative to the target (state 4, ev81)
	OMedia3DVector		m_dest_vec;				// 0x6e4  destination - position
	unsigned char		m_arrived;				// 0x6f4  arrived (ev74)
	float				m_arrived_time;			// 0x6f8  time since arriving (ev78: SIT after 7 s, else WAG)
	float				m_dest_dist;			// 0x6fc  distance to the destination
	float				m_stuck_dist;			// 0x700  that distance at the last stuck check (ev89)
	float				m_visible_range;		// 0x704  "VisibleRange" (2500)
	char				m_patrol_point[100];	// 0x708  "PatrolPoint" ("none"): the first point (AITrigger AIPatrol)
	char				m_target_name[100];		// 0x76c  "TargetName" ("JIM1")
	char				m_anim_none[40];		// 0x7d0  ("none") an animation name subclasses use
	char				m_anim_wag[40];			// 0x7f8  "WAG"
	char				m_anim_run[40];			// 0x820  "RUN"
	char				m_anim_fly[40];			// 0x848  "FLY"
	char				m_anim_sit[40];			// 0x870  "SIT"
	char				m_anim_walk[40];		// 0x898  "WALK"
	CGameObject			*m_dest;				// 0x8c0  destination: a patrol point or an object
	CGameObject			*m_prev_point;			// 0x8c8  the patrol point we left last
	float				m_fov;					// 0x8cc  "FOV" in degrees (90; > 358: all round)
	OMedia3DPoint		m_turn_rates;			// 0x8d0  turn rates x, y, z (1, 3, 2) (RotateToDest)
	float				m_time2;				// 0x8e0  time (+= dt)
	unsigned char		m_keep_point;			// 0x8e4  keep m_dest_point as it is (ev76 / ev81)
	OMedia3DPoint		m_dest_point;			// 0x8e8  destination point
	OMedia3DPoint		m_8f8;					// 0x8f8  (0, 0, 0)
	unsigned char		m_hover;				// 0x908  hovering: no gravity (AITrigger AIHover)
	unsigned char		m_909;					// 0x909  0 (set with the hover rise in v265)
	float				m_wait_time;			// 0x90c  wait time left at a patrol point (state 7)
	unsigned char		m_no_turn;				// 0x910  don't turn until the speed is over 1 (ev78)
	int					m_saved_state;			// 0x914  state saved by ev87 (1)
	OMedia3DVector		m_saved_offset;			// 0x918  m_offset saved by ev87
	CGameObject			*m_saved_target;		// 0x928  target saved by ev87
	float				m_attack_time;			// 0x92c  ev87 time left
	float				m_stuck_timer;			// 0x930  stuck check timer (ev89)
	float				m_seen_time;			// 0x934  > 0: the target is seen even when it is hidden (counts down)
	unsigned char		m_938;					// 0x938  0
	int					m_ai_state;				// 0x93c  "AIState" (1): the state to start in / go back to
	float				m_flee_dist;			// 0x940  flee distance (1500): state 5 ends beyond it
	float				m_arrive_dist;			// 0x944  arrival distance (110; 440 for patrol points with m_move_forward)
	unsigned char		m_move_forward;			// 0x948  move the way we face (ev77)
	float				m_keep_fleeing;			// 0x94c  > 0: state 5 stops (v261) beyond m_flee_dist; wanderers flee a seen target
	unsigned char		m_wander_started;		// 0x950  wander started
	float				m_wander_time;			// 0x954  wander time (new destination after 11 s)
	unsigned char		m_958;					// 0x958  0
	float				m_wander_range;			// 0x95c  "WanderRange" (1500)
	float				m_last_yaw_step;		// 0x960  last yaw step (RotateToDest)
	unsigned char		m_see_through_walls;	// 0x964  (CanSeeTarget)
	int					m_anims_disabled;		// 0x968  animations disabled (a cut scene animates us)
	float				m_flee_time;			// 0x96c  flee time left (state 5)
	unsigned char		m_state_locked;			// 0x970  state locked (ev73 ignores 0 and 6)
	int					m_974;					// 0x974  0
	unsigned char		m_check_stuck;			// 0x978  1: patrols run the stuck check (ev89)
	OMedia3DPoint		m_hold_pos;				// 0x97c  position held while touched (m_held)
	unsigned char		m_held;					// 0x98c  touched by the camera or Goddard: held at m_hold_pos
	unsigned char		m_hold_when_touched;	// 0x98d  is held when touched
	float				m_990;					// 0x990  150
};

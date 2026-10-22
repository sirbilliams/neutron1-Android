/*  C3DAITrigger ('3AIT') - the level scripts' work horse: when fired (by a trigger chain, by
    being touched with "TouchActivated", or by a TriggerCallback) and allowed ("ActivateBy" is
    the caller, or "none"; or the caller's name contains "IsA", which then makes the caller the
    target), it changes "AITarget": visibility ("AIHideObj"), position ("AINewPos"), animation
    ("AIAnim") and, for an AI, its state ("AIState"), speed, patrol point and heading
    ("AINewRotY"). "PlayerControlled" NULL takes the camera away from Jimmy. It toggles
    "ToggleObject" (NextState(Toggle)), runs the built-in action of a special tag
    (SpecialAction: the SCENE task's progress, pickups, menus, restarting the game...) and then
    fires NextTrigger, or the first "ActivateObject<n>" whose "ActivateState<n>" equals the state
    of its task ("TaskName"), or "default" after a RECHARGE with enough energy.
    Reconstructed from Neutron.exe (Boy Genius): constructor 0x40ba70 (factory '3AIT', 0xc0c
    bytes), destructor 0x40bee0 (only resets the vtables, then ~C3DTriggerType; deleting thunks
    0x40beb0 / 0x40d0d0 / 0x40d0e0). Vtables 0x48fd24 (element, 54 slots: C3DTriggerType's) /
    0x48fd10 (listener) / 0x48f8c0 (game object: 0, 7, 10, 16, 23, 265 overridden) /
    streamer. Boy Genius's trigger is a simpler ancestor of Negatron's: no attach / detach,
    collision, fade, hover, lighting or ShowTarget; Fire, FindActiveObject and SpecialAction
    are not virtual; nothing in the savegame.
    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DTriggerType.h"

class C3DAITrigger : public C3DTriggerType
{
	public:
	C3DAITrigger();															// 0x40ba70
	virtual ~C3DAITrigger();												// 0x40bee0

	void Trigger(CGameObject *caller);										// 0x40c300 fire (called directly)
	CGameObject *FindActiveObject(void);									// 0x40c230 the ActivateObject<n> for the task's state
	void SpecialAction(void);												// 0x40caa0 the built-in action of a special tag

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x40bf60
	virtual void ResetObject(void);											// SLOT 10  0x40bf20 collision by TouchActivated, count reset
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x40c200 touch starts: fire (TouchActivated)
	virtual void v23(CGameObject *o);										// SLOT 23  0x40ca70 TriggerCallback: fire (no caller)
	virtual void v267(int a2);												// SLOT 265 0x40ca80 (BG numbering) the task's state read (unused)

	// FIELD 0x6bc 0x548
	char		m_activate_by[100];		// 0x6bc  "ActivateBy" (= "JIM1"; "none": anyone)
	char		m_ai_target[100];		// 0x720  "AITarget" (= "none")
	int			m_activate_state[5];	// 0x784  "ActivateState0..4" (= 0, -1, -1, -1, -1)
	char		m_activate_object[5][100];	// 0x798  "ActivateObject0..4" (= "none")
	char		m_activate_by_anim[100];	// 0x98c  "ActivateByAnim" (= "none")
	char		m_ai_anim[100];			// 0x9f0  "AIAnim" (= "none")
	char		m_player_controlled[100];	// 0xa54  "PlayerControlled" (= "none"; "NULL": take the camera away)
	char		m_ai_patrol[100];		// 0xab8  "AIPatrol" (= "none")
	char		m_ai_new_pos[100];		// 0xb1c  "AINewPos" (= "none")
	int			m_times_to_trigger;		// 0xb80  "TimesToTrigger" (= -1: always)
	int			m_times_triggered;		// 0xb84
	int			m_touch_activated;		// 0xb88  "TouchActivated" (= 0)
	int			m_ai_state;				// 0xb8c  "AIState" (= 2)
	int			m_ai_speed;				// 0xb90  "AISpeed" (= -1)
	float		m_ai_new_rot_y;			// 0xb94  "AINewRotY" (= -1)
	int			m_ai_hide_obj;			// 0xb98  "AIHideObj" (= -1)
	char		m_is_a[100];			// 0xb9c  "IsA" (= "none"): a caller whose name contains it is the target
	unsigned char f_c00;				// 0xc00  (= 0) fire "default" instead of the next trigger (once; RECHARGE)
};

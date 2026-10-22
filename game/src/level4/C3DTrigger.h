/*  C3DTrigger ('3TRI') - a general trigger of level 4b (icons.omt sprite 0, size 50, radius 250,
    collision with "TouchActivated" only): when fired by an object (a touch, or a TriggerCallback
    from another trigger) that is the one named by "ActivateBy" (or a C3DTRIGGER / C3DCUTSCENECAMERA),
    up to "TimesToTrigger" times (-1: always), it hands the camera to "PlayerControlled" (NULL:
    stops the current camera and gives it up), plays "ActivateAnim" on the activator when that is
    a C3DANIMATED, calls the activator back (its slot 23 with this trigger) and then either plays
    sound "SoundIndex" of database "SoundDatabase" (the next trigger then fires when it has ended,
    see Process) or fires the first "ActivateObject<n>" whose "ActivateState<n>" is the state its
    task last read (a trigger fires with the one it has chosen itself).
    Reconstructed from Neutron.exe: constructor 0x446e50 (factory '3TRI', 0xa00 bytes),
    destructor 0x4471e0 (only resets the vtables, then ~C3DSprite; deleting thunks 0x4471b0 /
    0x447880 / 0x447890). Vtables 0x4bd18c (element, 54 slots: C3DSprite's 53 + an empty 53) /
    0x4bd178 (listener) / 0x4bcd28 (game object at +0xc8: 0, 7, 16, 23, 241 overridden) /
    streamer. Own members 0x5e8..0x9f4, vtordisp 0x9f8.
    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                         */
#pragma once
#include "../C3DSprite.h"

class C3DTrigger : public C3DSprite
{
	public:
	C3DTrigger();															// 0x446e50
	virtual ~C3DTrigger();													// 0x4471e0

	// ---- element-family slots
	virtual void ev53(void) {}												// ESLOT 53 0x4732a0 (empty)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x447220 properties
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x447400 touch: fire (TouchActivated)
	virtual void v23(CGameObject *o);										// SLOT 23  0x447450 fire
	virtual void Process(float dt);											// SLOT 241 0x447790 the sound has ended: the next trigger

	// ---- not virtual
	CGameObject *FindActiveObject(void);									// 0x4476c0 the ActivateObject<n> for the task's state

	// FIELD 0x5e8 0x40c
	int					m_activate_state[5];	// 0x5e8  "ActivateState0..4" (0, -1, -1, -1, -1)
	char				m_activate_object[5][100];	// 0x5fc  "ActivateObject0..4" ("none")
	char				m_activate_by[100];		// 0x7f0  "ActivateBy" ("none")
	char				m_activate_anim[100];	// 0x854  "ActivateAnim" ("none")
	char				m_sound_database[100];	// 0x8b8  "SoundDatabase" ("none")
	char				m_next_trigger[100];	// 0x91c  "NextTrigger" ("none")
	int					m_sound_index;			// 0x980  "SoundIndex" (-1)
	char				m_player_controlled[100];	// 0x984  "PlayerControlled" ("none")
	int					m_times_triggered;		// 0x9e8  (0)
	int					m_times_to_trigger;		// 0x9ec  "TimesToTrigger" (-1)
	int					m_channel;				// 0x9f0  sound channel (-1)
	int					m_touch_activated;		// 0x9f4  "TouchActivated" (0)
};

/*  C3DPickupItem ('3PIC') - the pickups lying in the levels (sprites.omt canvas 8 by default):
    gadget parts, keys, sodas... and ambient sound sources ("IsAmbient", a looping 3D sound).
    Touched by the camera (Jimmy) and allowed (CanPickUp: "RequiredPicNum" x "ReqPicNumAmount"
    in the pickup menu, else the NeedMoreSound) it is collected: its object state is set, it
    hides (or shows "PickedUpIndex" instead), toggles "ActivateObject" / "ToggleObject", ticks
    its menu item "PIC_NUMBER" (with the pop-up), runs the story steps of a few tagged parts
    (SHRINKRAY, SCOOTERPART, BUBBLEPICKUP, TELEPORT2SPACE, CAVE2, PASSCARD, SEWERPART), gives
    "PointValue" points, fires NextTrigger and plays "SoundIndex" (at most "TimesToTrigger"
    times). Pickups with PickupIndex 0 are not saved. Sprite index 106 (0x6a) is never shown.
    Its SpriteIndex picks its PIC_NUMBER when the level loads (v260).
    Reconstructed from Neutron.exe: constructor 0x4358b0 (factory '3PIC', 0x774 bytes),
    destructor 0x435ae0 (only resets the vtables, then ~CPickupType; deleting thunks 0x435ab0 /
    0x436940 / 0x436950). Vtables 0x4ae9ac (element, 55 slots: CPickupType's 54 + CanPickUp) /
    0x4ae998 (listener) / 0x4ae548 (game object at +0xc8: 7, 10, 16, 241, 259, 266 overridden)
    / 0x4ae538 (streamer at +0x770). Own members 0x6cc..0x76c, vtordisp 0x76c.
    Boy Genius's is much simpler than Negatron's (no powerups, corona, voices, respawning,
    sounds by name or saved state of its own).

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "CPickupType.h"

class C3DPickupItem : public CPickupType
{
	public:
	C3DPickupItem();														// 0x4358b0
	virtual ~C3DPickupItem();												// 0x435ae0

	// ---- element-family slots it adds
	virtual unsigned char CanPickUp(void);									// ESLOT 54 0x436830 (+0xd8) the required pickups (used up), else the NeedMore sound

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x435b80
	virtual void ResetObject(void);											// SLOT 10  0x435b20
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x435ce0 collected by the camera
	virtual void Process(float dt);											// SLOT 241 0x436550 the ambient sound
	virtual void v260(void);												// SLOT 259 0x436200 collected / picked-up look, PIC_NUMBER
	virtual void v268(int state);											// SLOT 266 0x4360b0 NextState: 0 hidden, 1 back (not collected)

	// port: Negatron's menu sync (SaveGame.cpp still calls it); Boy Genius has none.
	void SyncWithMenu(void) {}

	int			m_sound_index;			// 0x6cc  "SoundIndex" (= -1)
	int			m_need_more_sound;		// 0x6d0  "NeedMoreSound" (= -1)
	int			m_times_to_trigger;		// 0x6d4  "TimesToTrigger" (= -1: always)
	int			m_times_triggered;		// 0x6d8  0
	int			m_is_ambient;			// 0x6dc  "IsAmbient" (= 0)
	int			m_ambient_channel;		// 0x6e0  the looping ambient sound (-1)
	unsigned char m_ambient_started;	// 0x6e4  0
	int			m_point_value;			// 0x6e8  "PointValue" (= 0)
	int			m_picked_up_index;		// 0x6ec  "PickedUpIndex" (= -1: hidden when collected; else this canvas)
	int			m_required_pic_num;		// 0x6f0  "RequiredPicNum" (= -1: none)
	int			m_req_pic_num_amount;	// 0x6f4  "ReqPicNumAmount" (= 1)
	int			m_initially_active;		// 0x6f8  "InitallyActive" (sic; = 1)
	char		m_activate_object[100];	// 0x6fc  "ActivateObject" (= "none")
	int			f_760;					// 0x760  0: the baseball hits C3DMovingTarget has counted
	int			m_pass_thru;			// 0x764  "PassThru" (= 1; not used)
	int			m_show_arrow;			// 0x768  "ShowArrow" (= 1): Jimmy's point cursor over it
};

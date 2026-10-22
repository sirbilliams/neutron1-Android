/*  C3DLaserTrigger ('3LAS') - a laser beam across a corridor (shape 1 of "objectslevel5a.omt",
    unlit). While on ("ItemActive", reset to it) the beam is shown; when the camera (Jimmy)
    touches it - unless the beam lies flat (no X / Z rotation) and he is above it - it hides for
    as long as he touches it, the terrain database's colours flash red (1, 0.6, 0.6) for 10 s
    (the sound 0x38 plays) and the object tagged "Next" (unless "none") gets NextState("Toggle").
    NextState (0 off, 1 on, else toggle) switches the beam.
    Reconstructed from Neutron.exe: constructor 0x42c050 (factory '3LAS', 0x738 bytes),
    destructor 0x42c1f0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x42c1c0 /
    0x42c6a0 / 0x42c6b0 / 0x42c6c0). Game object vtable 0x4a5104: slots 7, 10, 16, 17, 241, 266
    overridden (259 is a jump to C3DAnimated's).

    Original layout: as C3DAnimated, own members 0x6bc..0x730, vtordisp 0x730, streamer 0x734.  */
#pragma once
#include "../C3DAnimated.h"

class C3DLaserTrigger : public C3DAnimated
{
	public:
	C3DLaserTrigger();														// 0x42c050
	virtual ~C3DLaserTrigger();												// 0x42c1f0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42c2a0 properties, the beam's shape
	virtual void ResetObject(void);											// SLOT 10  0x42c240 on = ItemActive, shown or hidden
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x42c410 the camera touches the beam
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x42c5b0 the beam comes back
	virtual void Process(float dt);											// SLOT 241 0x42c370 the 10 s flash
	virtual void v268(int state);											// SLOT 266 0x42c5f0 "NextState": 0 off, 1 on, else toggle

	// ---- data members (names; the object offsets in the comments)
	int					m_item_active;	// 0x6bc  "ItemActive" (1)
	int					m_on;			// 0x6c0  the beam is on (1)
	int					m_toggle;		// 0x6c4  "Toggle" (-1): sent to the "Next" object's NextState
	char				m_next[100];	// 0x6c8  "Next" ("none"): tag of the object to switch
	float				m_flash;		// 0x72c  seconds of red flash left (0)
};

/*  C3DOctapuke ('3OCT') - the Octapuke ride of Retroland ("octo.ase" and "octostop.ase", texture
    "octapuke1.png"). Like C3DSumo: Jimmy touching it with a ticket (menu page 2, item 27: one is
    used up, 100 points) is picked up and held 700 up and 1000 out of it, sitting ("SIT", hidden
    meanwhile), while it turns at 60 degrees / s and the camera follows the ride (the game
    type's camera is the Octapuke). A fire, jump or gadget key puts him back (at the "StartPoint"
    object if there is one), gives him the camera again and starts a 10 s wait before the ride
    turns on its own (20 degrees / s). It never leaves its start position.
    Reconstructed from Neutron.exe: constructor 0x433e70 (factory '3OCT', 0x734 bytes),
    destructor 0x434000 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x433fd0 /
    0x4344e0 / 0x4344f0 / 0x434500).

    Original layout: as C3DAnimated (element vtable 72 slots, all C3DAnimated's; CLocalGameObject
    vtable at +0xc0: 0, 7, 16, 241 overridden), own members 0x6bc..0x72c, vtordisp 0x72c,
    streamer 0x730.                                                                         */
#pragma once
#include "../C3DAnimated.h"

class C3DOctapuke : public C3DAnimated
{
	public:
	C3DOctapuke();															// 0x433e70
	virtual ~C3DOctapuke();													// 0x434000

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x434050 properties, animations, texture, radius
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x4343a0 Jimmy touches: picked up
	virtual void Process(float dt);											// SLOT 241 0x434110 turn, hold Jimmy

	CGameObject		*m_jimmy;				// 0x6bc  Jimmy while he rides (his CGameObject part)
	unsigned char	m_riding;				// 0x6c0  Jimmy is on
	float			m_timer;				// 0x6c4  wait left before turning again
	char			m_start_point[100];		// 0x6c8  "StartPoint" ("none"): where Jimmy gets off
};

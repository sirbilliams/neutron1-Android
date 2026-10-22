/*  C3DPirate ('3PIR') - the pirate ship ride of Retroland ("viking.ase", "viking.png"). It rocks
    about its z axis (60 degrees x sin of the running time). Jimmy touching it with a ticket (menu
    page 2, item 27: one is used up, 100 points) is picked up and held -300 / -648 / 0 from it,
    driving ("DRIVE") with its rotation. A fire or jump key puts him back (at the "StartPoint"
    object if there is one) and starts a 10 s wait.
    Reconstructed from Neutron.exe: constructor 0x436c40 (factory '3PIR', 0x738 bytes),
    destructor 0x436dd0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x436da0 /
    0x437250 / 0x437260 / 0x437270).

    Original layout: as C3DAnimated (element vtable 72 slots, all C3DAnimated's; CLocalGameObject
    vtable at +0xc0: 0, 7, 16, 241, 259 overridden), own members 0x6bc..0x730, vtordisp 0x730,
    streamer 0x734.                                                                         */
#pragma once
#include "../C3DAnimated.h"

class C3DPirate : public C3DAnimated
{
	public:
	C3DPirate();															// 0x436c40
	virtual ~C3DPirate();													// 0x436dd0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x436e20 properties, animation, texture
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x437110 Jimmy touches: picked up
	virtual void Process(float dt);											// SLOT 241 0x436ec0 rock, hold Jimmy
	virtual void v260(void);												// SLOT 259 0x437200 + the radius (800)

	float			m_time;					// 0x6bc  running time (+= dt)
	CGameObject		*m_jimmy;				// 0x6c0  Jimmy while he rides (his CGameObject part)
	unsigned char	m_riding;				// 0x6c4  Jimmy is on
	float			m_timer;				// 0x6c8  wait left after he got off
	char			m_start_point[100];		// 0x6cc  "StartPoint" ("none"): where Jimmy gets off
};

/*  C3DSumo ('3SUM') - the sumo wrestler of Retroland ("sumo.ase", "sumo.png"). Jimmy touching it
    with a ticket (menu page 2, item 27: one is used up, 100 points) is picked up: he is put
    +260 / +185 / +550 from it, sits ("SIT") and turns with it at 60 degrees / s, and the sumo
    keeps its start position. A fire or jump key puts him back (at the "StartPoint" object if there
    is one) and starts a 10 s wait before the sumo turns on its own again (20 degrees / s).
    Reconstructed from Neutron.exe: constructor 0x443dc0 (factory '3SUM', 0x734 bytes),
    destructor 0x443f50 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x443f20 /
    0x444420 / 0x444430 / 0x444440).

    Original layout: as C3DAnimated (element vtable 72 slots, all C3DAnimated's; CLocalGameObject
    vtable at +0xc0: 0, 7, 16, 241, 259 overridden), own members 0x6bc..0x72c, vtordisp 0x72c,
    streamer 0x730.                                                                         */
#pragma once
#include "../C3DAnimated.h"

class C3DSumo : public C3DAnimated
{
	public:
	C3DSumo();																// 0x443dc0
	virtual ~C3DSumo();														// 0x443f50

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x443fa0 properties, animation, texture
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x4442f0 Jimmy touches: picked up
	virtual void Process(float dt);											// SLOT 241 0x444040 turn, hold Jimmy
	virtual void v260(void);												// SLOT 259 0x4443d0 + the radius (680)

	CGameObject		*m_jimmy;				// 0x6bc  Jimmy while he sits on it (his CGameObject part)
	unsigned char	m_riding;				// 0x6c0  Jimmy is on
	float			m_timer;				// 0x6c4  wait left before turning again
	char			m_start_point[100];		// 0x6c8  "StartPoint" ("none"): where Jimmy gets off
};

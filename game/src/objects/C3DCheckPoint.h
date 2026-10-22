/*  C3DCheckPoint ('3CHK') - a gate of the school races (sprites.omt canvas 42, size 100; the
    tags STARTLINE, CHECKn and FINISHLINE are what the code looks for). While it is available
    ("CheckAvail"; the one named by "Next" takes over when it is passed) it is shown and the
    scooter (a "C3DJEEP") passing it scores 10; the FINISHLINE counts the laps (m_count_a: the
    scooter's gate passes, odd = in a lap) and Nick's (m_count_b) and shows both on the race
    HUD (0x4eefc8: the stopwatch is running). When Nick (state 3, patrolling) has made 6 passes
    he walks home (NICKWALK0) and Jimmy is put back at PUTNICKHERE; when the scooter has made 6
    the race is won (SCENE 120/125 -> 130, 145 -> 160), unless Nick was first.
    Reconstructed from Neutron.exe: constructor 0x414190 (factory '3CHK', 0x660 bytes),
    destructor 0x414380 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x414350 /
    0x414be0 / 0x414bf0). Vtables 0x495e0c (element, 53 slots, C3DSprite's) / 0x495df8 (listener) /
    0x4959a8 (game object at +0xc8: 7, 16, 241, 259 overridden) / 0x495998 (streamer at +0x65c).
    Own members 0x5e8..0x658 (Negatron's checkpoint, an autosave, was a different class).     */
#pragma once
#include "../C3DSpriteType.h"

class C3DCheckPoint : public C3DSpriteType
{
	public:
	C3DCheckPoint();														// 0x414190
	virtual ~C3DCheckPoint();												// 0x414380

	// ---- not virtual
	void Enable(void);														// 0x414ae0 available, shown again
	void Disable(void);														// 0x414b00 not available, hidden
	static void DisableAll(void);											// 0x414b70 every C3DCheckPoint (ID '3CHK') disabled

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x414aa0 "CheckAvail", "Next"
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x4144c0 Nick / the scooter passes
	virtual void Process(float dt);											// SLOT 241 0x414410 the race HUD (FINISHLINE)
	virtual void v260(void);												// SLOT 259 0x414b20 enabled or not as the property says

	// ---- data members (object offsets)
	char				m_next[100];	// 0x5e8  "Next" ("none"): the gate that takes over
	int					m_check_avail;	// 0x64c  "CheckAvail" (0): available
	int					m_count_a;		// 0x650  FINISHLINE: gate passes of the scooter (odd = in a lap)
	int					m_count_b;		// 0x654  FINISHLINE: gate passes of Nick
};

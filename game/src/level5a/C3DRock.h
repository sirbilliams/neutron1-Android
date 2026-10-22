/*  C3DRock ('3ROK') - a rock of the asteroid field (objectslevel5a.omt shape 10, unlit): while
    its physics are on it spins by three random rates (0..1 each, times 100 / 50 / 70 degrees
    per second). Slot 222 starts a countdown (10 s) after which it stops colliding (below 0) and,
    far below, hides. The rocket ship's hit on a rock only stores 2 into a name buffer of
    C3DAnimated (a quirk: the same call as for a mine, see C3DRocketShip).
    Reconstructed from Neutron.exe: constructor 0x43cc40 (factory '3ROK', 0x6d4 bytes),
    destructor 0x43cec0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x43ce90 /
    0x43d060 / 0x43d070 / 0x43d080). Vtables 0x4b18e0 (element, 73 slots: C3DAnimated's 72 + 72)
    / 0x4b18cc (listener) / 0x4b1890 (OMediaAnim) / 0x4b1440 (game object at +0xc0: 16, 222, 241
    overridden) / 0x4b1430 (streamer at +0x6d4). Own members 0x6bc..0x6cc, vtordisp 0x6d0.

    Element-family slot 72 is `ev106` (the Negatron numbering); members are named by their
    Neutron.exe offsets.                                                                     */
#pragma once
#include "../C3DAnimated.h"

class C3DRock : public C3DAnimated
{
	public:
	C3DRock();																// 0x43cc40
	virtual ~C3DRock();														// 0x43cec0

	// ---- element-family slots
	virtual void ev106(void);												// ESLOT 106 0x43cf20 (slot 72) physics on

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x43cf10 (= CGameObject's)
	virtual void v222(void);												// SLOT 223 0x43cf40 starts the 10 s countdown
	virtual void Process(float dt);											// SLOT 241 0x43cf50 spin / countdown

	// ---- data members (object offsets)
	float				m_rot_x;		// 0x6bc  random 0..1: spin rate about x (x 100 deg / s)
	float				m_rot_y;		// 0x6c0  random 0..1: about y (x 50)
	float				m_rot_z;		// 0x6c4  random 0..1: about z (x 70)
	float				m_countdown;	// 0x6c8  0, 10 after v222
};

/*  C3DHumphrey ('3HUM') - Humphrey, a guard animal: an ASE AI (humprun / humpwalk / humpsleep /
    humpstop as ATTACK / WALK / STOP / STOP2, texture "humphrey.png"), radius 20. It sleeps
    (STOP) until its AITriggers move it; four of C3DAI's state animations are renamed
    (STOP / WALK / STOP / ATTACK).
    Touching Jimmy it bites (-10 energy, -10 points, 5 s) and goes to AI state 5 (flee).
    Reconstructed from Neutron.exe: constructor 0x417100 (factory '3HUM', 0x1b58 bytes),
    destructor 0x4208c0 ("Deleting Humphrey" (debug log), then ~C3DAI; deleting thunks 0x420dd0
    / 0x420890 / 0x420dc0 / 0x420de0).

    Original layout: as C3DAI (element vtable N2:0x4e37cc, 144 slots, all C3DAI's; listener
    N2:0x4e37b8; OMediaAnim N2:0x4e377c; CLocalGameObject vtable N2:0x4e3300 at +0xc0), no members of
    its own. Slot 266 is C3DArrow's code (0x419aa0).                                         */
#pragma once
#include "../C3DAI.h"

class C3DHumphrey : public C3DAI
{
	public:
	C3DHumphrey();															// 0x417100
	virtual ~C3DHumphrey();													// 0x4208c0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x420950 animations, texture
	virtual void AddTouching(CGameObject *other);							// SLOT 16  N2:0x441ff0 Jimmy: bite
	virtual void v260(void);												// SLOT 260 0x420d60 C3DAI's, then ev70(1)
	virtual void v266(void);												// SLOT 266 0x419aa0 SetTaskState(TaskName, GetTaskState(TaskName))
	virtual void v267(int level);											// SLOT 267 N2:0x441fe0 C3DAnimated's
};

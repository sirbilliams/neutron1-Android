/*  C3DDarwinFish ('3FIS') - Darwin, Jimmy's robot dog: a morph creature (.ase animations WALK,
    SHRINK, STOP; texture "darwin.png") walking at its patrol points, VisibleRange 2000, FOV
    90. Once shrunk (f_c40, set by Jimmy's shrink ray) the camera touching it picks it up:
    hidden, 50 points, pickup item 12 (PIC_NUMBER) with the pop-up, its object state set.
    Reconstructed from Neutron.exe: constructor 0x416ac0 (factory '3FIS', 0x9a8 bytes),
    destructor 0x416c70 ("Deleting DarwinFish" (debug log), then ~C3DEnemy; deleting thunks
    0x416c40 / 0x416e60 / 0x416e70 / 0x416e80). Vtables 0x498600 (element, 95 slots, 67
    overridden) / 0x4985ec (listener) / 0x4985b0 (OMediaAnim) / 0x498160 (game object at +0xc0:
    7, 16 overridden) / 0x498150 (streamer at +0x9a4). No members of its own. Element slot
    67 is Negatron's ev69.                                                                   */
#pragma once
#include "C3DEnemy.h"

class C3DDarwinFish : public C3DEnemy
{
	public:
	C3DDarwinFish();														// 0x416ac0
	virtual ~C3DDarwinFish();												// 0x416c70

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x416d00 load the animations and the texture (once)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4173c0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x416d90 shrunk: picked up by the camera
};

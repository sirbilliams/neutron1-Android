/*  C3DGirlEatingPlant ('3GIR') - the girl-eating plant (in Retroville and Jimmy's lab): an
    enemy that is also a pickup (PickupIndex / PIC_NUMBER, default picture 2): plant*.ase WALK /
    SHRINK / STOP ("plant.png"), standing (STOP) at first. Once shrunk (C3DAI's state lock, set by
    the shrink ray) Jimmy touching it collects it: hidden, no collision, 50 points, the pickup
    pop-up and count for its PIC_NUMBER, the inventory item, and its object state set.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x41c2d0 (factory '3GIR', 0x9a8
    bytes), destructor 0x41c470 (only resets the vtables, then ~C3DEnemy; deleting thunks
    0x41c4a0 / ...). Element vtable: 67 (Negatron's ev69) overridden; game object vtable
    (+0xc0): 0, 7 (C3DDarwinFish's shared InitObject), 16. No members of its own (vtordisp
    0x9a0, streamer 0x9a4). Slot numbers in the comments are Boy Genius's; the code uses
    Negatron's C3DAI names (shrunk: the state lock f_c40).                                                              */
#pragma once
#include "C3DEnemy.h"

class C3DGirlEatingPlant : public C3DEnemy
{
	public:
	C3DGirlEatingPlant();													// 0x41c2d0
	virtual ~C3DGirlEatingPlant();											// 0x41c470

	virtual void ev69(void);												// ESLOT 67 0x41c4c0 load the animations (once)
	virtual void InitObject(void);											// SLOT 7   0x4173c0 (shared)
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x41c550 shrunk + Jimmy: collected
};

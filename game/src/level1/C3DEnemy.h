/*  C3DEnemy - the base of Boy Genius's enemies (C3DYokian and its soldiers, C3DDarwinFish,
    C3DGirlEatingPlant, C3DMutantFish, C3DHumphrey, C3DDino): a C3DPickupType that only names
    itself "C3DENEMY" and clears two C3DAI flags. (Negatron's C3DEnemy, the granny Nummies and
    ghosts, is a different class.)
    Reconstructed from Neutron.exe: constructor 0x417d50 (no factory entry), destructor
    0x417e50 (only resets the vtables, then ~C3DPickupType; deleting thunks 0x417e20 / 0x417ed0
    / 0x417ee0 / 0x417ef0). Nothing overridden but the destructor; no members of its own
    (vtordisp 0x9a0, streamer 0x9a4).                                                         */
#pragma once
#include "C3DPickupType.h"

class C3DEnemy : public C3DPickupType
{
	public:
	C3DEnemy();																// 0x417d50
	virtual ~C3DEnemy();													// 0x417e50
};

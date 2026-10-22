/*  C3DEnemy - the base of the creatures (C3DDarwinFish, C3DHumphrey, C3DGirlEatingPlant,
    C3DDino, C3DMutantFish, C3DYokian...): a C3DPickupType that only clears two flags. (Not
    Negatron's '3ENE' enemy.)
    Reconstructed from Neutron.exe: constructor 0x417d50 (no factory ID), destructor 0x417e50
    (only resets the vtables; deleting thunks 0x417e20 / 0x417ed0 / 0x417ee0 / 0x417ef0).
    Vtables 0x499e28 (element, 95 slots) / 0x499e14 (listener) / 0x499dd8 (OMediaAnim) /
    0x499988 (game object at +0xc0, none overridden) / 0x499978 (streamer at +0x9a4).         */
#pragma once
#include "C3DPickupType.h"

class C3DEnemy : public C3DPickupType
{
	public:
	C3DEnemy();																// 0x417d50
	virtual ~C3DEnemy();													// 0x417e50
};

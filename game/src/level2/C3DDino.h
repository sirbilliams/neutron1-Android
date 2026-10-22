/*  C3DDino ('3DIN') - the toy dinosaur of the school (Level2): a morph creature (.ase
    animations WALK, SHRINK, STOP; texture "dino.png") walking at its patrol points, speed 400,
    like Darwin. Once shrunk (m_state_locked, set by Jimmy's shrink ray) the camera touching it
    picks it up: hidden, 50 points, pickup item 14 (PIC_NUMBER) with the pop-up, its object
    state set.
    Reconstructed from Neutron.exe: constructor 0x417100 (factory '3DIN'), destructor 0x4172b0
    (deleting thunks 0x417280 / 0x4174c0 / 0x4174d0 / 0x4174e0). Element slot 67 is Negatron's
    ev69; slot 7 is the shared 0x4173c0. No members of its own.                              */
#pragma once
#include "../level1/C3DEnemy.h"

class C3DDino : public C3DEnemy
{
	public:
	C3DDino();																// 0x417100
	virtual ~C3DDino();														// 0x4172b0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x417300 load the animations and the texture (once)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4173c0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x417400 shrunk: picked up by the camera
};

/*  C3DYokianSpy ('3SPY') - the Yokian captain's sidekick ("lackie" in level1d), with a
    C3DYokHelmet: yokcap*.ase ATTACK / WALK / TALK / SHRINK / BROKE / STOP, "yokcaptn.png" /
    "yokcaptntalk.png", mouth swapped every 0.075 s while TALK plays in a cut scene, radius 60.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44c2b0 (factory '3SPY', 0x9c8 bytes),
    destructor 0x44c480 (only resets the vtables, then ~C3DYokian). Element vtable: 67 (Negatron's
    ev69) overridden; game object vtable (+0xc0): 0, 7 (C3DDarwinFish's shared InitObject:
    C3DPickupType's + InitAnim), 241. Own members 0x9b8 (which mouth texture), 0x9bc (the mouth
    clock), vtordisp 0x9c0, streamer 0x9c4. Slot numbers are Boy Genius's.                     */
#pragma once
#include "C3DYokian.h"

class C3DYokianSpy : public C3DYokian
{
	public:
	C3DYokianSpy();														// 0x44c2b0
	virtual ~C3DYokianSpy();												// 0x44c480

	virtual void ev69(void);												// ESLOT 67 0x44c4d0 load the animations (once)
	virtual void InitObject(void);											// SLOT 7   0x4173c0 (shared)
	virtual void Process(float dt);											// SLOT 241 0x44c5d0 + the talking mouth

	int					m_mouth;		// 0x9b8  texture 0 / 1 while TALK plays
	float				m_mouth_time;	// 0x9bc
};

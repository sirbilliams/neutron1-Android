/*  C3DYokianSoldier ('3SOL') - a Yokian soldier: sold*.ase WALK / SHRINK / TALK / STOP (TALK and STOP
    are the attack animation), "yoksold.png" / "yoksoldtalk.png" ("vryoksoldier.png" for both in
    the VR levels VR01..VR08), mouth swapped every 0.075 s while TALK plays in a cut scene,
    radius 90.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44bee0 (factory '3SOL', 0x9c8 bytes),
    destructor 0x44c060 (only resets the vtables, then ~C3DYokian). Element vtable: 67 (Negatron's
    ev69) overridden; game object vtable (+0xc0): 0, 7 (C3DDarwinFish's shared InitObject:
    C3DPickupType's + InitAnim), 241. Own members 0x9b8 (which mouth texture), 0x9bc (the mouth
    clock), vtordisp 0x9c0, streamer 0x9c4. Slot numbers are Boy Genius's.                     */
#pragma once
#include "C3DYokian.h"

class C3DYokianSoldier : public C3DYokian
{
	public:
	C3DYokianSoldier();														// 0x44bee0
	virtual ~C3DYokianSoldier();												// 0x44c060

	virtual void ev69(void);												// ESLOT 67 0x44c0b0 load the animations (once)
	virtual void InitObject(void);											// SLOT 7   0x4173c0 (shared)
	virtual void Process(float dt);											// SLOT 241 0x44c1c0 + the talking mouth

	int					m_mouth;		// 0x9b8  texture 0 / 1 while TALK plays
	float				m_mouth_time;	// 0x9bc
};

/*  C3DYokianGuard ('3GUA') - a Yokian guard: guardwalk.ASE WALK / guardshrink.ASE SHRINK /
    guardatak.ASE STOP (the attack animation), texture "yokguard.png", radius 90.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44b220 (factory '3GUA',
    0x9c0 bytes), destructor 0x44b3a0 (only resets the vtables, then ~C3DYokian). Element vtable:
    67 (Negatron's ev69) overridden; game object vtable (+0xc0): 0 and 7 (C3DDarwinFish's shared
    InitObject: C3DPickupType's + InitAnim). No members of its own, vtordisp 0x9b8, streamer
    0x9bc. Slot numbers are Boy Genius's.                                                  */
#pragma once
#include "C3DYokian.h"

class C3DYokianGuard : public C3DYokian
{
	public:
	C3DYokianGuard();														// 0x44b220
	virtual ~C3DYokianGuard();												// 0x44b3a0

	virtual void ev69(void);												// ESLOT 67 0x44b3f0 load the animations (once)
	virtual void InitObject(void);											// SLOT 7   0x4173c0 (shared)
};

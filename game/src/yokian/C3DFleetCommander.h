/*  C3DFleetCommander ('3FLE') - the Yokian fleet commander (Goobot / "goobar" in level1d): commander*.ase
    TALK / WALK / STOP, "comander.png" with "comandertalk.png" for the open mouth (swapped every
    0.1 s while TALK plays in a cut scene, i.e. while there is no camera object), radius 60. Not
    a soldier: no attack animation flag (f_908 0). Shown again whenever its task changes.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x419760 (factory '3FLE', 0x9c8 bytes),
    destructor 0x4198f0 (only resets the vtables, then ~C3DYokian). Element vtable: 67 (Negatron's
    ev69) overridden; game object vtable (+0xc0): 0, 7 (C3DDarwinFish's shared InitObject:
    C3DPickupType's + InitAnim), 241, 264 (C3DArrow's shared:
    the task state written back), 265. Own members 0x9b8 (which mouth texture), 0x9bc (the mouth
    clock), vtordisp 0x9c0, streamer 0x9c4. Slot numbers are Boy Genius's.                     */
#pragma once
#include "C3DYokian.h"

class C3DFleetCommander : public C3DYokian
{
	public:
	C3DFleetCommander();														// 0x419760
	virtual ~C3DFleetCommander();												// 0x4198f0

	virtual void ev69(void);												// ESLOT 67 0x4199d0 load the animations (once)
	virtual void InitObject(void);											// SLOT 7   0x4173c0 (shared)
	virtual void Process(float dt);											// SLOT 241 0x419940 + the talking mouth
	virtual void v266(void);												// SLOT 264 0x419aa0 (shared) the task state written back
	virtual void v267(int level);											// SLOT 265 0x419ac0 task changed: shown
	int					m_mouth;		// 0x9b8  texture 0 / 1 while TALK plays
	float				m_mouth_time;	// 0x9bc
};

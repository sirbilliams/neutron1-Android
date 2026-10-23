/*  C3DPendulum ('3PEN') - a swinging chain: a morph object ("chain.ase" / "chain.png", radius
    100) that swings for ever about z, "SwingHeight" (60, degrees) * sin(t), the phase starting at
    a random 0..2.5. Collides like any C3DAnimated; only the constructor, InitObject and Process
    are its own. (Negatron's pendulum / swing hood split and its stopwatch slowdown are gone.)
    Reconstructed from Neutron.exe: constructor 0x434f60 (factory '3PEN', 0x6d4 bytes),
    destructor 0x435110 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4350e0 /
    0x4352c0 / 0x4352d0 / 0x4352e0). Vtables: element 0x4ad42c, game object (+0xc0) 0x4ad43c
    (0, 7, 241 overridden), streamer 0x4ad9fc. Own members 0x6bc / 0x6c0, vtordisp 0x6cc.   */
#pragma once
#include "../C3DAnimated.h"

class C3DPendulum : public C3DAnimated
{
	public:
	C3DPendulum();															// 0x434f60
	virtual ~C3DPendulum();													// 0x435110

	virtual void InitObject(void);											// SLOT 7   0x435160 property, animation, texture
	virtual void Process(float dt);											// SLOT 241 0x435210 swinging

	// ---- data members (object offsets)
	float				f_924;			// 0x6bc  phase (s)
	float				f_928;			// 0x6c0  "SwingHeight" (60)
};

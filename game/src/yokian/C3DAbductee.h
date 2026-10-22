/*  C3DAbductee ('3ABD') - the creature the Yokian ships' tractor beams carry: a morph-animated
    C3DAnimated (animations HISTRUGGLE "abductstruggle.ase" and HISTOP "abductstop.ase", texture
    "abducted.png", radius 20, played as STRUGGLE), no collision response. C3DYokianShip creates
    one with its C3DTractorBeam and lifts it along the beam; every frame it keeps gravity off its
    acceleration and turns itself about the x axis by 300 x the acceleration's y.
    Reconstructed from Neutron.exe: constructor 0x4076e0 (factory '3ABD', 0x6c4 bytes),
    destructor 0x407830 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x407800 /
    0x4079d0 / 0x407a00 / 0x407a10). Game object vtable (+0xc0): 0, 7, 241 overridden.
    Vtordisp 0x6bc, streamer 0x6c0 (no members of its own).                                  */
#pragma once
#include "../C3DAnimated.h"

class C3DAbductee : public C3DAnimated
{
	public:
	C3DAbductee();															// 0x4076e0
	virtual ~C3DAbductee();													// 0x407830

	virtual void InitObject(void);											// SLOT 7   0x407880 animations, texture
	virtual void Process(float dt);											// SLOT 241 0x407930 gravity cancelled, spin
};

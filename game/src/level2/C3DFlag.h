/*  C3DFlag ('3FLA') - a flag waving on its pole: a morph object ("flag.ase" as "HIDEFAULT",
    texture "flag.png") that loops its "DEFAULT" animation. No behaviour of its own.
    Reconstructed from Neutron.exe: constructor 0x419550 (factory '3FLA', 0x6c4 bytes), destructor
    0x4196b0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x419680 / 0x419730 /
    0x419740 / 0x419750). Vtables 0x49c850 (element, 72 slots, all C3DAnimated's) / 0x49c83c
    (listener) / 0x49c800 (OMediaAnim at +0x90) / 0x49c3b0 (game object at +0xc0, only slot 0
    overridden) / 0x49c3a0 (streamer at +0x6c0).                                               */
#pragma once
#include "../C3DAnimated.h"

class C3DFlag : public C3DAnimated
{
	public:
	C3DFlag();																// 0x419550
	virtual ~C3DFlag();														// 0x4196b0
};

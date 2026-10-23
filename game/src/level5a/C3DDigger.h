/*  C3DDigger ('3DIG') - the drilling machine of level 5a ("drill.ase" HIDRILL, "drill.png"): a
    C3DAnimated that only plays DRILL in a loop (v94 2, v64 0.05, v66 1000, v46 0, v52 1); it has
    no behaviour of its own.
    Reconstructed from Neutron.exe: constructor 0x416eb0 (factory '3DIG', 0x6c4 bytes),
    destructor 0x417050 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x417020 /
    0x4170d0 / 0x4170e0 / 0x4170f0). No overrides, no members of its own (vtordisp 0x6bc,
    streamer 0x6c0).                                                                       */
#pragma once
#include "../C3DAnimated.h"

class C3DDigger : public C3DAnimated
{
	public:
	C3DDigger();															// 0x416eb0
	virtual ~C3DDigger();													// 0x417050
};

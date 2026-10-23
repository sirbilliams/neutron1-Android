/*  C3DSparrow ('3SPW') - the flying critter of the cave levels: a sparrow ("sparrow2.ase" HIWALK /
    "sparrow.ase" HISTOP, "sparrow.png"), in LEV5 a vulture ("vulture02.ase" / "vulture01.ase",
    "vulture.png") and in LV5A / LV5B a bat ("batsfly.ase" / "batstop.ase", "bats.png"); a C3DAI
    that hovers (no gravity), AIState 6, radius 10. Shown again by a level change (v267).
    Reconstructed from Neutron.exe: constructor 0x4415a0 (factory '3SPW', 0x99c bytes),
    destructor 0x441750 (only resets the vtables, then ~C3DAI; deleting thunks 0x441720 /
    0x441950 / 0x441960 / 0x441970). Game object vtable (+0xc0): 0, 7, 265 overridden. No
    members of its own (vtordisp 0x994, streamer 0x998).                                   */
#pragma once
#include "../C3DAI.h"

class C3DSparrow : public C3DAI
{
	public:
	C3DSparrow();															// 0x4415a0
	virtual ~C3DSparrow();													// 0x441750

	virtual void InitObject(void);											// SLOT 7   0x4417a0 animations by the level
	virtual void v267(int level);											// SLOT 265 0x4418e0 C3DAnimated's, GetTaskState("SCENE") (unused), shown
};

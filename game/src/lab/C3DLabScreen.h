/*  C3DLabScreen ('3SCR') - a monitor in Negatron's lab: a morph object ("screen.ase", DEFAULT)
    whose picture cycles through five textures on a 13 s loop: "screen1.png" up to 1 s,
    "screen0.png" to 3 s, "screen2.png" to 5 s, "screen3.png" to 7 s, "screen4.png" to 10 s,
    then "screen1.png" again (the clock restarts once it passes 10 s, at the first frame
    before 13 s).
    Reconstructed from Neutron.exe: constructor 0x42bc00 (factory '3SCR', 0x930 bytes),
    destructor 0x42bd50 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x42bd20 /
    0x42c000 / 0x42c010 / 0x42c020).

    Original layout: as C3DAnimated (element vtable 0x4a4fc4, 94 slots; OMediaAnim vtable
    0x4a4f74 at +0x90; listener 0x4a4fb0 at +0x30; CLocalGameObject vtable 0x4a4b24 at +0xc0),
    own member 0x924, vtordisp 0x928, streamer 0x92c.                                         */
#pragma once
#include "../C3DAnimated.h"

class C3DLabScreen : public C3DAnimated
{
	public:
	C3DLabScreen();															// 0x42bc00
	virtual ~C3DLabScreen();												// 0x42bd50

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42bda0 animation and textures
	virtual void v23(CGameObject *o);										// SLOT 23  0x42bfc0 (calls the empty 0x4732a0)
	virtual void Process(float dt);											// SLOT 240 0x42be70 the picture cycle

	// ---- data members (object offsets)
	float				f_924;			// 0x924  clock of the picture cycle (0)
};

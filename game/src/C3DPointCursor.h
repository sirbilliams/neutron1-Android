/*  C3DPointCursor - the spinning 3D arrow ("pointarrow.ase", "3darrow.png", additive) that
    C3DJimmy shows over the nearest pickup (ev164, his f_d4c). No factory ID (ObjectID '3PCU').
    Reconstructed from Neutron.exe: constructor 0x43bc70 (0x92c bytes), destructor 0x43bdc0
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x43bef0 / 0x43bd90 / 0x43bf00
    / 0x43bee0).

    Original layout: as C3DAnimated (element vtable 0x4b0830, 94 slots; OMediaAnim vtable
    0x4b07e0 at +0x90; listener 0x4b081c at +0x30; CLocalGameObject vtable 0x4b0390 at +0xc0),
    no members of its own (vtordisp 0x924, streamer 0x928).                                    */
#pragma once
#include "C3DAnimated.h"

class C3DPointCursor : public C3DAnimated
{
	public:
	C3DPointCursor();														// 0x43bc70
	virtual ~C3DPointCursor();												// 0x43bdc0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x43be10 the arrow animation and texture
};

/*  C3DHydrant ('3HYD') - a gushing fire hydrant: a morph object ("waterhydrant.ase", created as
    "HIDEFAULT" and played as "DEFAULT") whose water texture flips through "whydrant0000.png" ..
    "whydrant0003.png", 30% translucent. Once 0.2 s have gone by the frames change every frame
    (the timer is never reset, as shipped). Cut scenes and AITriggers turn it on and off through
    the base classes (RequiredLevel / NextState).
    Reconstructed from Neutron.exe: constructor 0x420df0 (factory '3HYD', 0x6cc bytes),
    destructor 0x420f20 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x420ef0 /
    0x421150 / 0x421160 / 0x421170). Vtables 0x4a2508 (game object at +0xc0: 7, 241
    overridden) / 0x4a24f8 (streamer at +0x6c8; read_class / write_class copies 0x421180 /
    0x421190); element vtable: C3DAnimated's 72 slots. Own members 0x6bc..0x6c4, vtordisp
    0x6c4.

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc0 lower).                          */
#pragma once
#include "../C3DAnimated.h"

class C3DHydrant : public C3DAnimated
{
	public:
	C3DHydrant();															// 0x420df0
	virtual ~C3DHydrant();													// 0x420f20

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x420f70 animation, the four textures
	virtual void Process(float dt);											// SLOT 241 0x4210a0 the water frames

	// ---- data members (object offsets)
	int					f_6bc;			// 0x6bc  current water frame (0..3)
	float				f_6c0;			// 0x6c0  time (+= dt, never reset)
};

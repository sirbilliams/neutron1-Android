/*  C3DMerryGo ('3MER') - the playground merry-go-round (objects.omt shape 16, unlit). Jimmy
    touching it climbs on: he is held 110 up and 340 out on it, facing along it (+90 degrees),
    playing STOP, while it turns at up to 40 degrees / s (slowing down by 10 / s once he is
    off). The jump / action keys or buttons get him off, at the "StartPoint" object if there
    is one. It never moves from where it was placed.
    Reconstructed from Neutron.exe: constructor 0x42e220 (factory '3MER', 0x738 bytes),
    destructor 0x42e410 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x42e3e0 /
    0x42e8b0 / 0x42e8c0 / 0x42e8d0). Vtables 0x4a7260 (element, C3DAnimated's 72 slots) /
    0x4a724c (listener) / 0x4a7210 (OMediaAnim) / 0x4a6dc0 (game object at +0xc0: 16, 241
    overridden) / 0x4a6db0 (streamer at +0x734). Own members 0x6bc..0x730, vtordisp 0x730.

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc0 lower).                          */
#pragma once
#include "../C3DAnimated.h"

class C3DMerryGo : public C3DAnimated
{
	public:
	C3DMerryGo();															// 0x42e220
	virtual ~C3DMerryGo();													// 0x42e410

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x42e760 Jimmy climbs on
	virtual void Process(float dt);											// SLOT 241 0x42e460 turning, Jimmy held on it

	// ---- data members (object offsets)
	CGameObject			*f_6bc;			// 0x6bc  Jimmy while he rides (his CGameObject part)
	unsigned char		f_6c0;			// 0x6c0  Jimmy is on
	int					f_6c4;			// 0x6c4  0 (not used)
	float				f_6c8;			// 0x6c8  turn speed (degrees / s)
	char				f_6cc[100];		// 0x6cc  "StartPoint" ("none"): where Jimmy gets off
};

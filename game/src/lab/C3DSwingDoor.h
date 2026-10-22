/*  C3DSwingDoor ('3SWN') - a door that swings open about its y axis: a morph object (one .ase
    animation RAY, "ASEFile", default "doorcloset.ase"; texture "PNGFile", "doorcloset.png").
    Touched by the camera object (Jimmy) while "TouchActivated" (1), or told so by NextState
    (v268), it squeaks and turns at "OpenSpeed" (50 degrees / s) for "TimeToOpen" (1.8 s);
    the next time it turns back. Collides only with the camera, Goddard, the bots, '3EPR',
    Jimmy and '3WBL'. Its name is "C3DDOORSWING"; its InitObject logs "C3DShrinkRay" (copied).
    Reconstructed from Neutron.exe: constructor 0x444450 (factory '3SWN', 0xa0c bytes),
    destructor 0x444650 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x444620 /
    0x444970 / 0x444980 / 0x444990).

    Original layout: as C3DAnimated (element vtable 0x4b91f8, 94 slots; OMediaAnim vtable
    0x4b91a8 at +0x90; listener 0x4b91e4 at +0x30; CLocalGameObject vtable 0x4b8d58 at +0xc0),
    own members 0x924..0xa04, vtordisp 0xa04, streamer 0xa08.                                */
#pragma once
#include "../C3DAnimated.h"

class C3DSwingDoor : public C3DAnimated
{
	public:
	C3DSwingDoor();															// 0x444450
	virtual ~C3DSwingDoor();												// 0x444650

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4446a0 properties
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x444750 the camera: open (TouchActivated)
	virtual unsigned char v231(CGameObject *other);							// SLOT 231 N2:0x4752a0 collides with the camera, '3GOD', '3BOT', '3EPR', '3JIM', '3WBL' only
	virtual void Process(float dt);											// SLOT 240 0x4447e0 turning (C3DAnimated's is not called)
	virtual void v260(void);												// SLOT 260 0x4448a0 properties loaded: animation, texture
	virtual void v268(int state);											// SLOT 268 0x444790 "NextState": start turning (state unused)

	// ---- data members (object offsets)
	unsigned char		f_924;			// 0x924  turning
	float				f_928;			// 0x928  turning time left
	unsigned char		f_92c;			// 0x92c  open (the next turn closes it)
	char				f_92d[100];		// 0x92d  "ASEFile" ("doorcloset.ase")
	char				f_991[100];		// 0x991  "PNGFile" ("doorcloset.png")
	float				f_9f8;			// 0x9f8  "OpenSpeed" (50, degrees / s)
	float				f_9fc;			// 0x9fc  "TimeToOpen" (1.8 s)
	int					f_a00;			// 0xa00  "TouchActivated" (1)
};

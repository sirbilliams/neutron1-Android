/*  C3DSwingDoor ('3SWN') - a door that swings open about its y axis: a morph object (one .ase
    animation, created as "HIRAY" and played as "RAY"; "ASEFile", default "doorfowl.ase";
    texture "PNGFile", "doorfowl.png"). Touched by Jimmy while "TouchActivated" (1), or told so
    by NextState (v268), it squeaks and turns at "OpenSpeed" (50 degrees / s) for "TimeToOpen"
    (1.8 s); the next time it turns back. Its name is "C3DDOORSWING"; its InitObject logs
    "C3DShrinkRay" (copied).
    Reconstructed from Neutron.exe: constructor 0x444450 (factory '3SWN', 0x7a4 bytes),
    destructor 0x444650 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x444620 /
    0x444970 / 0x444980 / 0x444990). Boy Genius differs from Negatron: Jimmy by name (not the
    camera object) opens it, no collision filter (v231), no pause check in Process.

    Original layout: as C3DAnimated (element vtable 0x4b91f8, 72 slots; OMediaAnim vtable
    0x4b91a8 at +0x90; listener 0x4b91e4 at +0x30; CLocalGameObject vtable 0x4b8d58 at +0xc0),
    own members 0x6bc..0x79c, vtordisp 0x79c, streamer 0x7a0 (the member comments give the
    offsets from the object start). */
#pragma once
#include "../C3DAnimated.h"

class C3DSwingDoor : public C3DAnimated
{
	public:
	C3DSwingDoor();															// 0x444450
	virtual ~C3DSwingDoor();												// 0x444650

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4446a0 properties
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x444750 Jimmy: open (TouchActivated)
	virtual void Process(float dt);											// SLOT 241 0x4447e0 turning (C3DAnimated's is not called)
	virtual void v260(void);												// SLOT 259 0x4448a0 properties loaded: animation, texture
	virtual void v268(int state);											// SLOT 266 0x444790 "NextState": start turning (state unused)

	// ---- data members (object offsets)
	unsigned char		m_turning;			// 0x6bc  turning
	float				m_time_left;			// 0x6c0  turning time left
	unsigned char		m_open;			// 0x6c4  open (the next turn closes it)
	char				m_ase_file[100];		// 0x6c5  "ASEFile" ("doorfowl.ase")
	char				m_png_file[100];		// 0x729  "PNGFile" ("doorfowl.png")
	float				m_open_speed;			// 0x790  "OpenSpeed" (50, degrees / s)
	float				m_time_to_open;			// 0x794  "TimeToOpen" (1.8 s)
	int					m_touch_activated;			// 0x798  "TouchActivated" (1)
};

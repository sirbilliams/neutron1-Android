/*  C3DSchoolDoor ('3SCD') - the door of the school (Jimmy's house and neighbourhood levels): a
    morph object like C3DSwingDoor (one .ase animation, created as "HIRAY" and played as "RAY";
    "ASEFile", default "doorfowl.ase"; texture "PNGFile", "doorfowl.png"). When Jimmy touches
    it, depending on the level and the story (SCENE) it squeaks and turns about its y axis at
    50 degrees / s for 1.8 s (the next time it turns back). Its InitObject logs "C3DShrinkRay"
    (copied).
    Reconstructed from Neutron.exe: constructor 0x43f070 (0x798 bytes), destructor 0x43f230
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x43f610 / 0x43f600 / 0x43f5f0
    / 0x43f200). Vtables: element (72 slots: C3DAnimated's) / game object at +0xc0 (0, 7, 16,
    241, 259 own) / OMediaAnim / listener / streamer 0x43f620 / 0x43f630 (copies of
    read_class / write_class).
    Own members 0x6bc..0x790, vtordisp 0x790, streamer 0x794.                                  */
#pragma once
#include "../C3DAnimated.h"

class C3DSchoolDoor : public C3DAnimated
{
	public:
	C3DSchoolDoor();														// 0x43f070
	virtual ~C3DSchoolDoor();												// 0x43f230

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x43f280 properties
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x43f2e0 Jimmy: open, by level and SCENE
	virtual void Process(float dt);											// SLOT 241 0x43f480 turning
	virtual void v260(void);												// SLOT 259 0x43f540 properties loaded: animation, texture

	// ---- data members (object offsets)
	unsigned char		f_6bc;			// 0x6bc  turning
	float				f_6c0;			// 0x6c0  turning time left
	unsigned char		f_6c4;			// 0x6c4  open (the next turn closes it)
	char				f_6c5[100];		// 0x6c5  "ASEFile" ("doorfowl.ase")
	char				f_729[100];		// 0x729  "PNGFile" ("doorfowl.png")
};

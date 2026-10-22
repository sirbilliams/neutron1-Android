/*  C3DToolChest ('3TOL') - the tool chest in Jimmy's house (level1c): a morph object with the
    animations CLOSED ("toolchestclosed.ase"), OPEN ("toolchestopen.ase") and NOTOOLS
    ("toolchestopen2.ase", the open chest with its tools taken) and the texture "toolchest.png".
    It starts closed. When Jimmy touches it while the story is at SCENE 220 it opens empty
    (NOTOOLS), SCENE becomes 230, the tools are put into the PDA (item 18 of the pickups page,
    item 5 of the gadgets page), the pick-up pop-up for picture 18 is shown and the cut scene
    camera tagged "GOTKEY" is fired.
    Reconstructed from Neutron.exe: constructor 0x445f30 (0x6c4 bytes), destructor 0x446080
    (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4462b0 / 0x4462a0 / 0x446290
    / 0x446050).
    Original layout: as C3DAnimated, no members of its own: vtordisp 0x6bc, streamer 0x6c0.   */
#pragma once
#include "../C3DAnimated.h"

class C3DToolChest : public C3DAnimated
{
	public:
	C3DToolChest();															// 0x445f30
	virtual ~C3DToolChest();												// 0x446080

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4460d0 the animations and the texture
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x446190 Jimmy: open the chest (SCENE 220)
};

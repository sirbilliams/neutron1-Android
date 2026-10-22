/*  C3DTargetCursor - the aiming cross of Jimmy's gadgets: sprite 8 of "retainedsprites.omt",
    110 x 110 world units (ObjectID '3TAR'). A copy of C3DShadow's code (it even names itself
    "C3DSHADOW"). C3DJimmy's constructor makes the one cursor (C3DJimmy f_4c8c), hidden.
    No factory ID.
    Reconstructed from Neutron.exe: constructor 0x4453b0 (0x60c bytes), destructor 0x445540
    (only resets the vtables, then ~C3DPermanentSprite; deleting thunks 0x4455e0 / 0x445510 /
    0x4455f0).

    Original layout: as C3DSprite (element vtable 0x4ba3ac, 55 slots; listener 0x4ba398 at +0x30;
    CLocalGameObject vtable 0x4b9f48 at +0xc8), own members 0x600..0x604, vtordisp 0x604,
    streamer 0x608.                                                                            */
#pragma once
#include "C3DPermanentSprite.h"

class C3DTargetCursor : public C3DPermanentSprite
{
	public:
	C3DTargetCursor();														// 0x4453b0
	virtual ~C3DTargetCursor();												// 0x445540

	// ---- CGameObject-family overrides
	virtual void v260(void);												// SLOT 260 0x445580 sprite and size

	// ---- data members (object offsets)
	short				f_600;			// 0x600  width (110)
	short				f_602;			// 0x602  height (110)
};

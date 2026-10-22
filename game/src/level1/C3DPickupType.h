/*  C3DPickupType - the base of the AI creatures that can also be picked up (C3DEnemy and its
    kind): with f_99c set (DarwinFish) it takes a savegame object-state slot ("PickupIndex",
    handed out by the level as it loads) and a menu pickup number ("PIC_NUMBER"); collected
    (state != 0) it stays hidden. Not Negatron's CPickupType (the sprite pickups).
    Reconstructed from Neutron.exe: constructor 0x436980 (no factory ID), destructor 0x436a70
    (only resets the vtables, then ~C3DAI; deleting thunks 0x436a40 / 0x436c10 / 0x436c20 /
    0x436c30). Vtables 0x4aef44 (element, C3DAI's 95 slots) / 0x4aef30 (listener) / 0x4aeef4
    (OMediaAnim) / 0x4aeaa4 (game object at +0xc0: 7, 259, 265 overridden) / 0x4aea94
    (streamer at +0x9a4). Own members 0x994..0x9a0, vtordisp 0x9a0.

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc0 lower).                          */
#pragma once
#include "../C3DAI.h"

class C3DPickupType : public C3DAI
{
	public:
	C3DPickupType();														// 0x436980
	virtual ~C3DPickupType();												// 0x436a70

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x436ac0 + PickupIndex, PIC_NUMBER
	virtual void v260(void);												// SLOT 259 0x436b10 PickupIndex from the level; shown unless collected
	virtual void v267(int level);											// SLOT 265 0x436b80 with a TaskName: hidden once collected

	// ---- data members (object offsets)
	int					f_994;			// 0x994  "PickupIndex" (0)
	int					f_998;			// 0x998  "PIC_NUMBER" (-1)
	unsigned char		f_99c;			// 0x99c  a pickup (0)
};

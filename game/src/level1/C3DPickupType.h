/*  C3DPickupType - an AI character that can also be "collected": with m_is_pickup set (by the
    subclass) it has a slot in the savegame's object states ("PickupIndex", handed out by the
    level as it loads) and a menu pickup number ("PIC_NUMBER"). Once its state is set it stays
    hidden (C3DAI ev70(false)); with a "TaskName" it is hidden again when the task's state
    changes. The base of C3DEnemy. (Boy Genius only; CPickupType is the sprite version.)
    Reconstructed from Neutron.exe: constructor 0x436980 (no factory entry), destructor
    0x436a70 (only resets the vtables, then ~C3DAI; deleting thunks 0x436a40 / 0x436c10 /
    0x436c20 / 0x436c30). Game object vtable (+0xc0): 0, 7, 259, 265 overridden. Own members
    0x994..0x9a0, vtordisp 0x9a0, streamer 0x9a4.                                             */
#pragma once
#include "../C3DAI.h"

class C3DPickupType : public C3DAI
{
	public:
	C3DPickupType();														// 0x436980
	virtual ~C3DPickupType();												// 0x436a70

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x436ac0 + PickupIndex, PIC_NUMBER
	virtual void v260(void);												// SLOT 259 0x436b10 (BG numbering) properties loaded: the pickup index, shown unless collected
	virtual void v267(int level);											// SLOT 265 0x436b80 (BG numbering) TaskName state changed: hidden if collected

	int					m_pickup_index;	// 0x994  "PickupIndex" (0)
	int					m_pic_number;	// 0x998  "PIC_NUMBER" (-1)
	unsigned char		m_is_pickup;	// 0x99c  (0) set by subclasses that are pickups
};

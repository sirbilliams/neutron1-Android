/*  C3DSmoke ('3SM1') - a smoke emitter (VR04): hidden itself, it keeps 15 C3DSmokePuffs and, every
    0.6 s + a random 0..1 s, takes one that has faded out, puts its position into it, shows it
    and restarts its age.
    Reconstructed from Neutron.exe: constructor 0x440530 (factory '3SM1', 0x648 bytes),
    destructor 0x4407b0 (then ~C3DSpriteType; deleting thunks 0x440780 / 0x440960 / 0x440970).
    Game object vtable (+0xc8): 241 overridden. Own members 0x5e8..0x640, vtordisp 0x644.
    Slot numbers are Boy Genius's.                                                          */
#pragma once
#include "../C3DSpriteType.h"

class C3DSmokePuff;

class C3DSmoke : public C3DSpriteType
{
	public:
	C3DSmoke();																// 0x440530
	virtual ~C3DSmoke();													// 0x4407b0

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x4407f0 spawn a puff

	// ---- data members (object offsets)
	C3DSmokePuff		*m_puff[15];	// 0x5e8  the puffs
	float				m_clock;		// 0x624  time since the last puff (0)
	float				m_colour[3];	// 0x628  (1, 1, 1, not used)
	float				m_interval;		// 0x634  random 0..1: extra wait (reseeded for each puff)
	unsigned char		m_active;		// 0x638  (1)
	int					m_count;		// 0x63c  number of puffs (15)
};

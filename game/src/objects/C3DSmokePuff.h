/*  C3DSmokePuff ('3SMO') - one puff of smoke of a C3DSmoke / C3DValve / C3DGeyser: a sprite
    ("sprites.omt" canvas 37, size 40) that, once shown, grows by 80 / s (back to 40 past 400),
    drifts up (acceleration y = 25 - gravity) with a random sideways jolt each frame, and fades
    out over its life (10 s), then hides. The spawn position the owner stores in 0x60c..0x618 is
    never read by the puff itself (as in the original).
    Reconstructed from Neutron.exe: constructor 0x440980 (factory '3SMO', 0x62c bytes),
    destructor 0x440b80 (then ~C3DSpriteType; deleting thunks 0x440b50 / 0x440de0 / 0x440df0
    / 0x440e00). Game object vtable (+0xc8): 241 overridden. Own members 0x5e8..0x620, vtordisp
    0x624, streamer 0x628.                                                                 */
#pragma once
#include "../C3DSpriteType.h"

class C3DSmokePuff : public C3DSpriteType
{
	public:
	C3DSmokePuff();															// 0x440980
	virtual ~C3DSmokePuff();												// 0x440b80

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x440bc0 drift, grow and fade

	// ---- data members (object offsets)
	float				m_time;			// 0x5e8  age (0)
	float				m_red;			// 0x5ec  (1)
	float				m_green;		// 0x5f0  (1)
	float				m_blue;			// 0x5f4  (1)
	float				m_life;			// 0x5f8  fade time (10)
	float				f_5fc;			// 0x5fc  (1, not used)
	float				f_600;			// 0x600  (2, not used)
	float				f_604;			// 0x604  (0, not used)
	float				f_608;			// 0x608  (0, not used)
	float				m_pos[4];		// 0x60c  spawn position written by the owner (0, 0, 0, 1; not read)
	float				m_grow;			// 0x61c  growth (80 / s)
	unsigned char		m_active;		// 0x620  (1)
};

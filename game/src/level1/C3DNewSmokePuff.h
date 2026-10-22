/*  C3DNewSmokePuff ('3NSM') - one puff of rocket smoke: a sprite ("sprites.omt" canvas 37,
    size 40, additive) that, started at a point (Start, element slot 53), grows by 100 / s and
    fades out over 1 s, then hides. C3DRocket keeps ten of them.
    Reconstructed from Neutron.exe: constructor 0x433070 (factory '3NSM', 0x630 bytes),
    destructor 0x433270 (then ~C3DSpriteType; deleting thunks 0x433240 / 0x433440 / 0x433450
    / 0x433460). Vtables 0x4ab6a0 (element, 54 slots: C3DSprite's 53 + Start) / 0x4ab68c
    (listener) / 0x4ab23c (game object at +0xc8: 241, 259 overridden) / 0x4ab22c (streamer at
    +0x62c). Own members 0x5e8..0x628, vtordisp 0x628.                                      */
#pragma once
#include "../C3DSpriteType.h"

class C3DNewSmokePuff : public C3DSpriteType
{
	public:
	C3DNewSmokePuff();														// 0x433070
	virtual ~C3DNewSmokePuff();												// 0x433270

	// ---- element-family slots it adds
	virtual void Start(float x, float y, float z, float w);					// ESLOT 53 0x4332d0 (+0xd4) a new puff at (x, y, z)

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x433340 grow and fade
	virtual void v260(void);												// SLOT 259 0x4332b0 additive

	// ---- data members (object offsets)
	float				m_time;			// 0x5e8  age (0)
	float				m_red;			// 0x5ec  (1)
	float				m_green;		// 0x5f0  (1)
	float				m_blue;			// 0x5f4  (1)
	float				m_life;			// 0x5f8  fade time (1)
	float				f_5fc;			// 0x5fc  (1, not used)
	float				f_600;			// 0x600  (2, not used)
	float				f_604;			// 0x604  (0, not used)
	float				f_608;			// 0x608  (0, not used)
	float				f_60c;			// 0x60c  (0, not used)
	float				f_610;			// 0x610  (0, not used)
	float				f_614;			// 0x614  (0, not used)
	float				f_618;			// 0x618  (1, not used)
	float				m_grow;			// 0x61c  growth (100 / s)
	unsigned char		m_active;		// 0x620  (1)
	float				m_size;			// 0x624  current size
};

/*  C3DBalloon ('3BAL') - a balloon (level4d): a sprite (sprites.omt canvas 50, size 200)
    tinted "Red" / "Green" / "Blue" (1, 0.1, 0.1; alpha 0.8). It starts tied: Jimmy (the camera) touching it
    unties it, and it then rises (vertical speed capped at 70, no sideways drift, gravity
    cancelled with an acceleration of 10). A baseball or a rocket pops it: hidden, "balloon
    pop", 10 points (200 while still tied) plus (distance from the camera / 30 - 33) when the
    shot came from farther than 1000.
    Reconstructed from Neutron.exe: constructor 0x40f710 (factory '3BAL', 0x600 bytes),
    destructor 0x40f8f0 (vtable resets only, then ~C3DSpriteType; deleting thunks 0x40f8c0 /
    0x40fc10 / 0x40fc20).

    Original layout: as C3DSpriteType (element vtable 0x491320, 53 slots: C3DSprite's;
    CLocalGameObject vtable 0x490ebc at +0xc8), own members 0x5e8..0x5f8, vtordisp 0x5f8,
    streamer 0x5fc. Members are named after their Boy Genius offset from the object start (the
    CGameObject-family methods of the original see them 0xc8 lower).                         */
#pragma once
#include "../C3DSpriteType.h"

class C3DBalloon : public C3DSpriteType
{
	public:
	C3DBalloon();															// 0x40f710
	virtual ~C3DBalloon();													// 0x40f8f0

	// ---- CGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10  0x40f930 tied again, still, tinted
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x40f9b0 untied by Jimmy, popped by a shot
	virtual void Process(float dt);											// SLOT 240 0x40fb10 rise once untied

	// FIELD 0x5e8 0x10
	float			m_red;				// 0x5e8  "Red" (1)
	float			m_green;			// 0x5ec  "Green" (0.1)
	float			m_blue;				// 0x5f0  "Blue" (0.1)
	unsigned char	m_popped;			// 0x5f4  (0)
	unsigned char	m_tied;				// 0x5f5  (1)
};

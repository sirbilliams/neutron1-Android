/*  C3DCorona ('3COR') - a glowing billboard (sprites.omt canvas 36, size 100) drawn additively
    in the colour of its three members (default red 1.0, green 0.1, blue 0.1) at full opacity.
    Made by C3DLightCone and C3DSwitch as their glow. (Negatron's corona also spun and pulsed
    and had properties; Neutron.exe's has neither.)
    Reconstructed from Neutron.exe: constructor 0x4154b0 (factory '3COR', 0x5fc bytes),
    destructor 0x415640 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x415610 /
    0x415720). Vtables 0x497530 (element, 53 slots, none overridden) / 0x49751c (listener) /
    0x4970cc (game object at +0xc8: 259 overridden) / streamer. Own members 0x5e8..0x5f0,
    vtordisp 0x5f4, the streamer at 0x5f8.

    Members are named after their Neutron.exe offsets (element view; the CGameObject-family
    methods of the original see them 0xc8 lower).                                            */
#pragma once
#include "../C3DSpriteType.h"

class C3DCorona : public C3DSpriteType
{
	public:
	C3DCorona();															// 0x4154b0
	virtual ~C3DCorona();													// 0x415640

	// ---- CGameObject-family overrides
	virtual void v260(void);												// SLOT 259 0x415680 load, additive in the colour

	// FIELD 0x5e8 0xc
	float		m_red;					// 0x5e8  (= 1.0)
	float		m_green;				// 0x5ec  (= 0.1)
	float		m_blue;					// 0x5f0  (= 0.1)
};

/*  C3DLightCone ('3LCO') - a turning beam of light (the SUVs' headlights): a morph object
    ("lightcone.ase" as DEFAULT, texture "lightcone.png", additive, 50% alpha) spinning about
    y at "RotSpeed" degrees / s, tinted "Red/Green/Blue". A C3DCorona glow sits on it while
    the beam points at the viewport (|cos| > 0.8, fading in up to 1); otherwise the beam itself
    shows the tint. Set off (f_6d0, by its SUV) both are hidden.
    Reconstructed from Neutron.exe: constructor 0x42d410 (factory '3LCO', 0x6dc bytes),
    destructor 0x42d5d0 (then ~C3DAnimated; deleting thunks 0x42d590 / 0x42db50 / 0x42db60 /
    0x42db70). Vtables 0x4a671c (element, C3DAnimated's 72 slots) / 0x4a6708 (listener) /
    0x4a66cc (OMediaAnim) / 0x4a627c (game object at +0xc0: 7, 241, 257, 259 overridden) /
    0x4a626c (streamer at +0x6d8). Own members 0x6bc..0x6d4, vtordisp 0x6d4.                  */
#pragma once
#include "../C3DAnimated.h"

class C3DCorona;

class C3DLightCone : public C3DAnimated
{
	public:
	C3DLightCone();															// 0x42d410
	virtual ~C3DLightCone();

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42d620 properties, animation, texture
	virtual void Process(float dt);											// SLOT 241 0x42d740 turning, the glow
	virtual void v258(void);												// SLOT 257 0x42da50 the tint to the glow and the beam
	virtual void v260(void);												// SLOT 259 0x42db00 C3DAnimated's, then v258

	// ---- data members (object offsets)
	float				m_rot_speed;	// 0x6bc  "RotSpeed" (80)
	C3DCorona			*m_corona;		// 0x6c0  the glow
	float				m_red;			// 0x6c4  "Red" (1)
	float				m_green;		// 0x6c8  "Green" (1)
	float				m_blue;			// 0x6cc  "Blue" (1)
	unsigned char		m_off;			// 0x6d0  off: beam and glow hidden (0)
};

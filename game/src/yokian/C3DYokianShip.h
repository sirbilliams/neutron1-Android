/*  C3DYokianShip ('3YSH') - a Yokian egg ship: shape 0 of "yokianship.omt" (unlit), speed 700,
    AI state 3 (patrol only) by default, gravity cancelled every frame, turning towards its
    destination rotation (y at 0.3 x the difference per second; the x / z rates are C3DAI's).
    Outside LV1D (the opening) and LV1F it carries a C3DTractorBeam 1200 ahead of itself and a
    C3DAbductee (once the task SCENE has reached the ship's RequiredLevel, or always without
    one): every 5 s the beam switches on (both shown; the abductee rises along the beam) or off
    (both hidden).
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44b7d0 (factory '3YSH', 0x9ac
    bytes), destructor 0x44bb10 (deleting thunks 0x44bae0 / 0x44be90 / 0x44bea0 / 0x44beb0).
    Game object vtable (+0xc0): 0, 8 / 10 (jumps to C3DAnimated's / C3DAI's, so not overridden
    here), 241, 246 (C3DYokCargo's shared RotateToDest). Own members 0x994..0x9a4, vtordisp
    0x9a4, streamer 0x9a8. Slot numbers are Boy Genius's; C3DAI members are f_<object offset>
    (TODO(core): BG C3DAI names).                                                              */
#pragma once
#include "../C3DAI.h"

class C3DYokianShip : public C3DAI
{
	public:
	C3DYokianShip();														// 0x44b7d0
	virtual ~C3DYokianShip();												// 0x44bb10

	virtual void Process(float dt);											// SLOT 241 0x44bbc0
	virtual void RotateToDest(float dt);									// SLOT 246 0x449a00 (shared)

	CGameObject			*m_beam;		// 0x994  C3DTractorBeam (its game object part; NULL in LV1D / LV1F)
	unsigned char		m_beam_on;		// 0x998
	float				m_beam_time;	// 0x99c  time to the next switch (5 s)
	CGameObject			*m_abductee;	// 0x9a0  C3DAbductee (its game object part)
};

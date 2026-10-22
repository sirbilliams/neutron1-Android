/*  C3DPermanentSprite - a C3DSprite whose picture comes from the databases kept for the whole
    game ("permanenticons.omt" by default) instead of the level's: projectiles, bubbles, shadows,
    the radar... Reconstructed from Neutron.exe: constructor 0x441980, destructor 0x440fe0
    (only resets the vtables, then ~C3DSprite). Same layout as C3DSprite (0x600 bytes + the
    streamer), no members of its own, no factory ID.                                          */
#pragma once
#include "C3DSprite.h"

class C3DPermanentSprite : public C3DSprite
{
	public:
	C3DPermanentSprite();													// 0x441980
	virtual ~C3DPermanentSprite();											// SLOT 0  0x440fe0 (thunks 0x435580 / 0x4353c0 / 0x435590)

	virtual void v258(void);												// SLOT 258 0x435430 properties changed: reset + reload sprite
	virtual void v260(void);												// SLOT 260 0x4354c0 (re)load the sprite canvas

#include "gen/C3DPermanentSprite_slots.inc"
#include "gen/C3DPermanentSprite_fields.inc"
};

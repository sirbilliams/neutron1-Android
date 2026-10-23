/*  C3DSpriteType - the C3DSprite most placed sprite objects derive from (trees, smoke, lights,
    projectiles, triggers through C3DTriggerType...). Adds canvas flags 1 on (re)load, a show on
    v242.
    Reconstructed from Neutron.exe: constructor 0x441980 (C3DSprite's, then canvas flags 3),
    destructor 0x441a60 (only resets the vtables, then ~C3DSprite). Layout: C3DSprite (0x5e8,
    no own members), vtordisp at +0x5e8, streamer at +0x5ec. No factory ID.               */
#pragma once
#include "C3DSprite.h"

class C3DSpriteType : public C3DSprite
{
	public:
	C3DSpriteType();														// 0x441980
	virtual ~C3DSpriteType();												// SLOT 0  0x441a60 (thunks 0x441b90 / 0x441a30 / 0x441ba0)

	virtual void AddTouching(CGameObject *other);										// SLOT 16 0x43cf10 (= CGameObject's)
	virtual void Process(float dt);											// SLOT 241 0x40ff40 (= C3DSprite's)
	virtual void v242(float t);												// SLOT 242 0x441aa0 shows a hidden sprite again
	virtual void v260(void);												// SLOT 259 0x441ac0 (re)load the sprite canvas, canvas flags 3

#include "gen/C3DSpriteType_slots.inc"

#include "gen/C3DSpriteType_fields.inc"
};

/*  C3DSpriteType - the C3DSprite most placed sprite objects derive from (trees, smoke, lights,
    projectiles, triggers through C3DTriggerType...). Adds canvas flags 1 on (re)load, a show on
    v242 and the projection-matrix hook v282.
    Reconstructed from Neutron.exe: constructor N2:0x473420, destructor 0x441a60 (only resets the
    vtables, then ~C3DSprite). Layout: C3DSprite (0x600), own members 0x600..0x668, size 0x668 +
    the streamer (vtordisp at +0x668, streamer at +0x66c). No factory ID.                     */
#pragma once
#include "C3DSprite.h"

class C3DSpriteType : public C3DSprite
{
	public:
	C3DSpriteType();														// N2:0x473420
	virtual ~C3DSpriteType();												// SLOT 0  0x441a60 (thunks 0x441b90 / 0x441a30 / 0x441ba0)

	virtual void AddTouching(CGameObject *other);										// SLOT 16 0x43cf10 (= CGameObject's)
	virtual void Process(float dt);											// SLOT 241 0x40ff40 (= C3DSprite's)
	virtual void v242(float t);												// SLOT 242 0x441aa0 shows a hidden sprite again
	virtual void v260(void);												// SLOT 259 0x441ac0 (re)load the sprite canvas, canvas flags 3
	virtual void v282(OMediaMatrix_4x4 &m, float *w, float *h);				// SLOT 282 N2:0x473630 layer's projection matrix

#include "gen/C3DSpriteType_slots.inc"

	// FIELD 0x600 0x68
	int				f_600;					// 0x600 = 0 (used by subclasses)
	char			f_604[100];				// 0x604 = "" (used by subclasses)
#include "gen/C3DSpriteType_fields.inc"
};

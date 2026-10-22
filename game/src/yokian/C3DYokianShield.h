/*  C3DYokianShield ("C3DBUBBLE") - the bubble that flashes round a Yokian when he is hit: a
    sprite (sprites.omt canvas 28, size 300), additive (src alpha, one), hidden until shown.
    ShowShield (element slot 53) shows it for 0.1 s; Process hides it again (element slot 54)
    when the time runs out. C3DYokian makes one in its constructor (kept at +0x9a0).
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44b510 (no factory entry:
    made in code, 0x5f4 bytes; its ObjectID is '3YSH', the same as C3DYokianShip's), destructor
    0x44b680 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x44b650 / 0x44b7b0
    / 0x44b7c0). Element vtable 55 slots (C3DSpriteType's 53 + 53, 54); game object vtable
    (+0xc8): 0, 241, 242 (C3DSpriteType's), 259 overridden. Own member 0x5e8 (the time left),
    vtordisp 0x5ec, streamer 0x5f0.                                                           */
#pragma once
#include "../C3DSpriteType.h"

class C3DYokianShield : public C3DSpriteType
{
	public:
	C3DYokianShield();														// 0x44b510
	virtual ~C3DYokianShield();												// 0x44b680

	// ---- element-family slots
	virtual void ShowShield(void);											// ESLOT 53 0x44b750 show for 0.1 s
	virtual void HideShield(void);											// ESLOT 54 0x44b770 hide

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x44b6f0 (BG numbering) time out: hide
	virtual void v260(void);												// SLOT 259 0x44b6c0 (BG numbering) additive, hidden

	float			m_time;					// 0x5e8  time left shown (0)
};

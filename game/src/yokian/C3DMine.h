/*  C3DMine ('3MIN') - a mine dropped by the Yokian cargo ships (C3DYokCargo keeps 30): a sprite
    (sprites.omt canvases 0x58..0x61, size 300) that blinks between two pictures every 0.8 s
    until a rocket ship touches it (C3DRocketShip stores state 2); it then plays the burst (canvases
    0x58.. every 0.08 s, from the second one half transparent) and hides.
    Reconstructed from Neutron.exe: constructor 0x42efa0 (factory '3MIN', 0x638 bytes),
    destructor 0x42f2d0 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x42f2a0 /
    0x42f4c0 / 0x42f4d0). Vtables 0x4a7d5c (element, 53 slots, none overridden) / 0x4a7d48
    (listener) / 0x4a78f8 (game object at +0xc8: 241 overridden) / 0x4a78e8 (streamer at +0x634).
    Own members 0x5e8..0x630, vtordisp 0x630.                                               */
#pragma once
#include "../C3DSpriteType.h"

class OMediaCanvas;

class C3DMine : public C3DSpriteType
{
	public:
	C3DMine();																// 0x42efa0
	virtual ~C3DMine();														// 0x42f2d0

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// SLOT 241 0x42f320 blink / burst

	// 0x42f310: the rocket ship touched it: start the burst (the original's one-line method, not virtual)
	void Explode(void) { m_state = 2; }

	// ---- data members (object offsets)
	OMediaCanvas	*m_canvas[10];		// 0x5e8  sprites.omt canvases 0x58..0x61 (0: blink 1, 9: blink 2, 0..8 the burst)
	int				m_index;			// 0x624  burst frame
	float			m_time;				// 0x628  time in this state (starts random, 0..1 s)
	int				m_state;			// 0x62c  0 blink 1, 1 blink 2, 2 bursting, 3 gone
};

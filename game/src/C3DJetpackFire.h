/*  C3DJetpackFire ('3JFI') - the flame of the player's jet pack (C3DPlayer f_7f8): an .ase morph
    object ("fire.ase", "fire.png") that sits on its owner (f_924) while the burn time (f_928)
    lasts, using energy; hidden otherwise. C3DPlayer's MoveJump sets the burn time.
    Reconstructed from Neutron.exe: factory '3JFI', constructor 0x421d80 (0x6cc bytes),
    destructor 0x421eb0 (deleting thunks 0x421e80 / 0x422100 / 0x422110 / 0x422120).
    Original layout: as C3DAnimated (element vtable 72 slots), own members 0x6bc..0x6c4,
    vtordisp 0x6c4, streamer 0x6c8. Names keep Negatron's offsets (f_924 = BG 0x6bc,
    f_928 = BG 0x6c0).                                                                        */
#pragma once
#include "C3DAnimated.h"

class C3DJetpackFire : public C3DAnimated
{
	public:
	C3DJetpackFire();														// 0x421d80
	virtual ~C3DJetpackFire();												// 0x421eb0

	// ---- element-family game slots

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x421f00 the animations and the texture
	virtual void Process(float dt);											// BG SLOT 241 0x421fb0 on the owner while burning

	// ---- data members (object offsets)
	CGameObject			*f_924;			// BG 0x6bc  the owner (the player; its CGameObject part)
	float				f_928;			// BG 0x6c0  burn time left
};

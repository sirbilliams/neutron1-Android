/*  C3DEye ('3EYE') - the giant eye of Retroland ("eye.ase", "eyemap.png"): a C3DAICar (patrols
    its points) that is not pulled down by gravity (every frame its acceleration y is set to
    -gravity, x and z kept); speed 300, no terrain/wall collisions (v42, v40,
    v52 = 0). The model is only loaded once the animations are set up (ev69).
    Reconstructed from Neutron.exe: constructor 0x418090 (factory '3EYE', 0x9b8 bytes; it
    inlines the base's InitObject), destructor 0x4181e0 (only resets the vtables, then
    ~C3DAICar; deleting thunks 0x4181b0 / 0x418360 / 0x418370 / 0x418380).

    Original layout: as C3DAICar (element vtable 144 slots; CLocalGameObject vtable at +0xc0: 0,
    7, 241 overridden), no members of its own (0x9b0 vtordisp, 0x9b4 streamer).            */
#pragma once
#include "../level5/C3DAICar.h"

class C3DEye : public C3DAICar
{
	public:
	C3DEye();																// 0x418090
	virtual ~C3DEye();														// 0x4181e0

	// ---- element-family overrides
	virtual void ev69(void);												// ESLOT 69 0x418230 load the model once

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x418290 C3DAI's, then InitAnim (the code C3DBus shares)
	virtual void Process(float dt);											// SLOT 241 0x4182d0 C3DAICar's, then the acceleration (x, -gravity, z)
};

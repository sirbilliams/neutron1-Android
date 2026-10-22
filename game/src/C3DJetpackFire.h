/*  C3DJetpackFire ('3JFI') - the jet pack on Jimmy's back with its flames: an .ase morph object
    ("jetpack.png") with the animations STOP (folded), OPEN (unfolding, then ANIMATE: the flames
    loop) and CLOSE (folding, then hidden). Each C3DJetPack makes one (its f_9f0) and moves it
    onto Jimmy every frame while he flies with it.
    Reconstructed from Neutron.exe: factory '3JFI', constructor 0x421d80 (0x938 bytes),
    destructor 0x421eb0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x422110 /
    0x421e80 / 0x422120 / 0x422100).

    Original layout: as C3DAnimated (element vtable 0x4a3600, 94 slots; OMediaAnim vtable
    0x4a35b0 at +0x90; listener 0x4a35ec at +0x30; CLocalGameObject vtable 0x4a3160 at +0xc0),
    own members 0x924..0x930, vtordisp 0x930, streamer 0x934.                                */
#pragma once
#include "C3DAnimated.h"

class C3DJetpackFire : public C3DAnimated
{
	public:
	C3DJetpackFire();														// 0x421d80
	virtual ~C3DJetpackFire();												// 0x421eb0

	// ---- element-family game slots
	virtual void ev67(void);												// ESLOT 67 N2:0x443760 animation ended: OPEN -> ANIMATE, CLOSE -> hidden

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x421f00 the animations and the texture

	// ---- data members (object offsets)
	CGameObject			*f_924;			// 0x924  g_jimmy when the jet pack was made (NULL for Jimmy's own: g_jimmy is set after)
	float				f_928;			// 0x928  flame size (0; C3DJimmy / C3DPlayer set it, nothing reads it)
	int					f_92c;			// 0x92c  0
};

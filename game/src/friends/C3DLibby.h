/*  C3DLibby ('3LIB') - Libby: a morph friend (.ase animations TALK, WALK, RUN, DIGITIZE
    (libbyvox), FREEZE (libyvoxstop), STOP; textures "libby.png" (0) and "libbywire.png" (1)),
    patrolling (AIState 3) at 200, VisibleRange 500. The AI triggers DIGITIZE / UNDIGITIZE
    switch her to the wireframe texture and back (element slots 147 / 148). Reset puts her back
    at her start position.
    Reconstructed from Neutron.exe: constructor 0x42cd40 (factory '3LIB', 0x1d7c bytes),
    destructor 0x42cee0 (deleting thunks 0x42ceb0 / 0x42d3e0 / 0x42d3f0 / 0x42d400).

    Original layout: as C3DFriends (element vtable 0x4a60d8, 149 slots: C3DFriends' 147 +
    147, 148; listener 0x4a60c4; OMediaAnim 0x4a6088; CLocalGameObject vtable 0x4a5c38 at
    +0xc0), no members of its own. Slot 126 is N2:0x45b110 (a jump to the empty 0x41ee20).      */
#pragma once
#include "C3DFriends.h"

class C3DLibby : public C3DFriends
{
	public:
	C3DLibby();																// 0x42cd40
	virtual ~C3DLibby();													// 0x42cee0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x42cf30 load the animations and the textures (once)
	virtual void ev145(void);												// ESLOT 145 N2:0x44fa60
	virtual void ev147(void);												// ESLOT 147 N2:0x44fb20 DIGITIZE: wireframe texture (1)
	virtual void ev148(void);												// ESLOT 148 N2:0x44fb40 UNDIGITIZE: her own texture (0)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void ResetObject(void);											// SLOT 10  0x42d190 back to the start position
	virtual void v260(void);												// SLOT 260 N2:0x44fa30 her own texture
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 0x4418e0
};

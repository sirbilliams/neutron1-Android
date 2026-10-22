/*  C3DLibby ('3LIB') - Libby: a morph friend (.ase animations TALK, WALK, RUN, PHONE, STOP;
    texture "libby.png"), patrolling (AIState 3) at 200, VisibleRange 500. Reset puts her back
    at her start position. Where she shows depends on the level and the story (v267): in
    Retroville while SCENE is 250..269 (at 250 she patrols) and 300..369 (walking, waiting),
    in LV3C at 400..419, in LV4A from 500, always in LV4D (walking) and elsewhere. Talking to
    her (ev145) moves SCENE on (255 -> 260, 300 -> 310, 350 -> 360, 400 -> 410); at 260 her
    ev126 gives pickup item 7.
    Reconstructed from Neutron.exe: constructor 0x42cd40 (factory '3LIB', 0xbb0 bytes),
    destructor 0x42cee0 (deleting thunks 0x42ceb0 / 0x42d3e0 / 0x42d3f0 / 0x42d400).

    Original layout: as C3DFriends (element vtable 0x4a60d8, 98 slots, 67, 90, 96 overridden;
    listener 0x4a60c4; OMediaAnim 0x4a6088; CLocalGameObject vtable 0x4a5c38 at +0xc0: 7, 10,
    259, 264, 265 overridden), no members of its own. Element slots are named by their
    Negatron numbers (Boy Genius 67 = ev69, 90 = ev126, 96 = ev145).                         */
#pragma once
#include "C3DFriends.h"

class C3DLibby : public C3DFriends
{
	public:
	C3DLibby();																// 0x42cd40
	virtual ~C3DLibby();													// 0x42cee0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67  0x42cf30 load the animations and the texture (once)
	virtual void ev126(void);												// ESLOT 90  0x42d020 C3DFriends', then pickup item 7 at SCENE 260
	virtual void ev145(void);												// ESLOT 96  0x42d060 talked to: the next SCENE step

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void ResetObject(void);											// SLOT 10  0x42d190 back to the start position
	virtual void v260(void);												// SLOT 259 0x42d010 (a jump to C3DAI's)
	virtual void v266(void);												// SLOT 264 0x419aa0
	virtual void v267(int level);											// SLOT 265 0x42d230 shown by level and SCENE
};

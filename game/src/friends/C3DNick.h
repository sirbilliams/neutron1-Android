/*  C3DNick ('3NIC') - Nick, the cool kid: an OMT morph friend (nickwalk / nickcoin / nickstop
    .ase, texture "nick.png") who rides a C3DSkateBoard while he patrols (state 3). Every 5 s
    of patrolling his "running" switches between SKATE and GLIDE, and his speed drops with each
    lap Jimmy has made him do (f_1d7c). He is shown only in the story scenes of his levels
    (v267), and talking to him moves the SCENE task on (ev145).
    Reconstructed from Neutron.exe: constructor N2:0x45a350 (factory '3NIC', 0x1d88 bytes),
    destructor 0x4336a0 (deleting thunks 0x433670 / 0x433e20 / 0x433e30 / 0x433e40).

    Original layout: as C3DFriends (element vtable 0x4abc34, 147 slots: 69, 126, 145 overridden;
    listener 0x4abc20; OMediaAnim 0x4abbe4; CLocalGameObject vtable 0x4ab794 at +0xc0), own
    members 0x1d74..0x1d80, vtordisp 0x1d80, streamer 0x1d84. The streamer vtable's read_class /
    write_class are C3DDino's copies (0x416e90 / 0x416ea0), the same jumps as C3DAI's.

    Element-family slots are `ev<n>` (`// ESLOT n`); members are f_<offset in the original
    object> (CGameObject-family code of the original sees them at offset - 0xc0).            */
#pragma once
#include "C3DFriends.h"

class C3DSkateBoard;

class C3DNick : public C3DFriends
{
	public:
	C3DNick();																// N2:0x45a350
	virtual ~C3DNick();														// 0x4336a0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69  0x433730 load the morph animations and the texture (once)
	virtual void ev90(void);												// ESLOT 126 N2:0x45aac0 skate the NICPAT1 patrol
	virtual void ev145(void);												// ESLOT 145 0x433b80 talked to: the SCENE task moves on

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void Process(float dt);											// SLOT 240 0x4339f0 the skateboard, SKATE / GLIDE
	virtual void v266(void);												// SLOT 266 0x419aa0
	virtual void v267(int level);											// SLOT 267 0x4338a0 shown in his story scenes only

	// ---- data members (object offsets)
	C3DSkateBoard		*f_1d74;		// 0x1d74 his skateboard (the original keeps its CGameObject part)
	float				f_1d78;			// 0x1d78 SKATE / GLIDE timer (5 s)
	int					f_1d7c;			// 0x1d7c laps done (SCENE 125 -> 120): each one slows him by 100
};

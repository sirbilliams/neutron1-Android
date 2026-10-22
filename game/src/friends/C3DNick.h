/*  C3DNick ('3NIC') - Nick, the cool kid: an OMT morph friend (nickwalk / nickcoin / nickstop /
    nicktalkboard / nickskate1 / nickskate2 .ase, texture "nick.png") who rides a C3DSkateBoard
    while he patrols (state 3: the school races). Every 5 s of patrolling his "running"
    animation switches between SKATE and GLIDE, and his speed drops with each lap the scooter
    has won (m_laps). He is shown only in the story scenes of his levels (v267), and talking to
    him moves the SCENE task on (ev96).
    Reconstructed from Neutron.exe: constructor 0x433490 (factory '3NIC', 0xbbc bytes),
    destructor 0x4336a0 (deleting thunks 0x433670 / 0x433e20 / 0x433e30 / 0x433e40).

    Original layout: as C3DFriends (element vtable 0x4abc34, 98 slots: 67, 90, 96 overridden;
    listener 0x4abc20; OMediaAnim 0x4abbe4; CLocalGameObject vtable 0x4ab794 at +0xc0: 7, 241,
    264, 265 overridden), own members 0xba8..0xbb4, vtordisp 0xbb8... (0xbbc in all).         */
#pragma once
#include "C3DFriends.h"

class C3DSkateBoard;

class C3DNick : public C3DFriends
{
	public:
	C3DNick();																// 0x433490
	virtual ~C3DNick();														// 0x4336a0

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67 0x433730 load the morph animations and the texture (once)
	virtual void ev90(void);												// ESLOT 90 0x433cb0 state 3: skate the NICPAT1 patrol, the race gates
	virtual void ev96(void);												// ESLOT 96 0x433b80 talked to: the SCENE task moves on

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415090
	virtual void Process(float dt);											// SLOT 241 0x4339f0 the skateboard, SKATE / GLIDE
	virtual void v266(void);												// SLOT 264 0x433820 rewrite the task state; hidden once the race is over
	virtual void v267(int level);											// SLOT 265 0x4338a0 shown in his story scenes only

	// ---- data members (object offsets)
	C3DSkateBoard		*m_board;		// 0xba8 his skateboard (the original keeps its CGameObject part)
	float				m_board_timer;	// 0xbac SKATE / GLIDE timer (5 s)
	int					m_laps;			// 0xbb0 laps lost (SCENE 125 -> 120): each one slows him by 100
};

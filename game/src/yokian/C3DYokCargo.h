/*  C3DYokCargo ('3YCA') - a Yokian cargo ship: shape 2 of "objectslevel5a.omt" (unlit), speed
    500, AI state 1 (wait) by default, gravity cancelled every frame, turning towards its
    destination rotation at 0.1 / 0.8 / 0.5 x the difference per second (x, y, z). Patrolling
    (state 3) it drops one of its 30 C3DMines every 5 s (2 s above speed 1000) once it has
    flown 25 s, and speeds up to 1400 when Jimmy's rocket comes within 7000 while it is slower
    than 1800. No mines in LEV3, LEV5, LV4C, LV3D and LV1D (the opening). In LEV5 it only shows
    once the task SCENE is past 489.
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x449360 (factory '3YCA', 0xa20
    bytes), destructor 0x449670 (deleting thunks 0x449640 / 0x449c60 / 0x449c70 / 0x449c80).
    Game object vtable (+0xc0): 0, 10, 241, 246, 264 (C3DArrow's shared: the task state
    written back), 265 overridden. Own members 0x994..0xa18 (two clocks, the 30 mines, the next
    mine), vtordisp 0xa18, streamer 0xa1c. Slot numbers are Boy Genius's; C3DAI members are
    f_<object offset> (TODO(core): BG C3DAI names).                                            */
#pragma once
#include "../C3DAI.h"

class C3DYokCargo : public C3DAI
{
	public:
	C3DYokCargo();															// 0x449360
	virtual ~C3DYokCargo();													// 0x449670

	virtual void ResetObject(void);											// SLOT 10  0x4496c0 + the clocks, the next mine
	virtual void Process(float dt);											// SLOT 241 0x4496e0
	virtual void RotateToDest(float dt);									// SLOT 246 0x449a00 (shared with C3DYokianShip)
	virtual void v266(void);												// SLOT 264 0x419aa0 (shared) the task state written back
	virtual void v267(int level);											// SLOT 265 0x449bb0 task changed: shown (LEV5: after SCENE 489)

	float				m_fly_time;		// 0x994  time flown (patrolling)
	float				m_mine_time;	// 0x998  time since the last mine
	CGameObject			*m_mines[30];	// 0x99c  its C3DMines (the original keeps their element part; NULL in the levels without mines)
	int					m_next_mine;	// 0xa14
};

// 0x449a00: turn towards the destination rotation (CGameObject f_274..f_27c) by the
// difference x the per-axis rates f_8d0 / f_8d4 / f_8d8 per second. Code shared by
// C3DYokCargo and C3DYokianShip.
void YokRotateToDest(C3DAI *self, float dt);

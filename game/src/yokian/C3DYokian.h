/*  C3DYokian - the base of the Yokians (Boy Genius's aliens: C3DFleetCommander, the soldiers,
    the spy, C3DYokianGuard): a C3DEnemy that patrols (AIState 2 by default, speed 200) with a
    ground shadow (a C3DShadow 140 x 50 kept on the terrain under it, unless g_windowed), a hit
    bubble (C3DYokianShield, shown between it and the camera while it flashes) and, for the
    spy, a helmet that follows its position and rotation.
    Hit by a baseball (one that has not hit yet): sound 0x2f, AI state 5 (flee 2200 for 5 s), 10 points. Touched by Jimmy: f_934 = 2 s. While "hit" (state locked: C3DAI f_c40): one
    of the sounds 0xf3..0xf6 (random) if its loop sound was playing, the shadow shrinks to
    40 x 15, the helmet hides; from AI state 0 it goes to state 5 (flee distance 1500, speed 400). The
    hit time running out restores it: its AIState, WALK, speed 200, shadow 140 x 50,
    helmet shown. Attacking (element slot 91) plays ATTACK and the loop sound 0x38 (stopped in
    states 2, 3 and 6, and while the game is paused).
    Reconstructed from Neutron.exe (Boy Genius only): constructor 0x44a730 (no factory entry),
    destructor 0x44a9b0 (deleting thunks 0x44a980 / 0x44b1d0 / 0x44b1e0 / 0x44b1f0). Element
    vtable 96 slots (C3DAI's 95 + 95, the empty 0x472970; 68 and 91 overridden); game object
    vtable (+0xc0): 0, 16, 241, 259 (a jump to C3DPickupType's), 272, 273 overridden. Own
    members 0x9a0..0x9b8, vtordisp 0x9b8, streamer 0x9bc.

    Slot numbers in the comments are Boy Genius's; the code uses the Negatron names of the
    C3DAI members and slots (BG offset + 0x2a4 / 0x2cc / 0x2d0: speed f_968, state f_96c,
    target f_964, hover f_bd4, flee distance f_c10, flee time f_c3c, state lock f_c40...).    */
#pragma once
#include "../level1/C3DEnemy.h"

class C3DYokianShield;
class C3DYokHelmet;
class C3DShadow;

class C3DYokian : public C3DEnemy
{
	public:
	C3DYokian();															// 0x44a730
	virtual ~C3DYokian();													// 0x44a9b0

	// ---- element-family slots
	virtual void ev70(unsigned char show);									// ESLOT 68 0x44afa0 show/hide with collision, shadow and helmet
	virtual void ev127(void);												// ESLOT 91 0x44b100 (C3DAI's: not arrived) attack: ATTACK, the loop sound
	virtual void ev95_bg(void) {}											// ESLOT 95 0x472970 (empty)

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x44b070 a baseball / Jimmy
	virtual void Process(float dt);											// SLOT 241 0x44aa00
	virtual void v277(void);												// SLOT 272 0x44b140 paused: the loop sound off
	virtual void v278(void);												// SLOT 273 0x44b160 unpaused: the loop sound again

	// port: BG's C3DAI keeps a float "hit time" at 0x974 (Negatron's 0xc44 is an int, unused here).
	float &HitTime(void) { return *reinterpret_cast<float *>(&f_c44); }

	// ---- data members (object offsets)
	C3DYokianShield		*m_shield;		// 0x9a0  the hit bubble (the original keeps its CGameObject part)
	C3DYokHelmet		*m_helmet;		// 0x9a4  the spy's helmet (NULL for the others)
	short				f_9a8;			// 0x9a8  (0)
	C3DShadow			*m_shadow;		// 0x9ac  ground shadow (NULL when g_windowed)
	unsigned char		f_9b0;			// 0x9b0  (0)
	int					m_loop_sound;	// 0x9b4  channel of the loop sound 0x38 (-1)
};

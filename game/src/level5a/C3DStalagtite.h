/*  C3DStalagtite ('3STA') - a stalactite of the cave levels (shape 4 of "objectslevel5a.omt",
    unlit, no animations, radius 100): touched it starts to fall (physics on) and, as a
    trigger object (slot 223, ToggleObject), it breaks: it hides and its three C3DRocks (kept
    hidden until then) appear at its place, thrown off with set velocities (sound 0xca). Falling
    (collision on) it spins about x at 20 degrees per second. Element slot 72 makes it fall at
    once (sound 0xc9, the rocks too).
    Reconstructed from Neutron.exe: constructor 0x441bb0 (factory '3STA', 0x6d4 bytes),
    destructor 0x441e70 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x441e40 /
    0x4422a0 / 0x4422b0 / 0x4422c0). Element vtable: 72 is new; game object vtable (+0xc0): 0, 10,
    16, 223, 241 overridden. Own members 0x6bc..0x6c8, vtordisp 0x6cc, streamer 0x6d0.     */
#pragma once
#include "../C3DAnimated.h"

class C3DRock;

class C3DStalagtite : public C3DAnimated
{
	public:
	C3DStalagtite();														// 0x441bb0
	virtual ~C3DStalagtite();												// 0x441e70

	// ---- element-family slots
	virtual void ev72(void);												// ESLOT 72 0x441ee0 falls at once (sound 0xc9, the rocks too)

	// ---- CGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10  0x441fb0 whole again
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x441ec0 starts to fall
	virtual void v222(void);												// SLOT 223 0x441fe0 breaks into the rocks
	virtual void Process(float dt);											// SLOT 241 0x441f70 spins while falling

	// ---- data members (object offsets)
	C3DRock				*m_rock[3];		// 0x6bc  the rocks it breaks into (hidden)
	unsigned char		m_broken;		// 0x6c8  (0)
};

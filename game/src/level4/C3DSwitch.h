/*  C3DSwitch ('3SWI') - a wall switch ("switch.ase" / "switch.png", radius 300, collision radius
    flags 0.05 / 1000) with a green glow (a C3DCorona 210 above it) whose red part pulses with the
    menu clock. Touched by the camera object (Jimmy) or a C3DBASEBALL it flips: with "Toggle" -1
    it toggles between on and off; otherwise it sends that state to the object named by
    "SwitchObject" (a fan, a door, ...) and gives 25 points when it was on. NextState (v268)
    0 / else switches the target off / on, hides / shows the glow, and plays a sound when the
    target is a C3DFAN. "MyState" is a property too.
    Reconstructed from Neutron.exe: constructor 0x4449c0 (factory '3SWI', 0x734 bytes),
    destructor 0x444c40 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x444c10 /
    0x4450c0 / 0x4450d0 / 0x4450e0). Vtables: element 0x4b97d4 (C3DAnimated's 72 + 72), game
    object (+0xc0) 0x4b9334 (0, 7, 10, 16, 241, 266 overridden; 259 is a thunk to C3DAnimated's).
    Own members 0x6bc..0x728, vtordisp 0x72c; "MyState" is the int at 0x558, behind
    CLocalGameObject's last member, which no other class uses.                              */
#pragma once
#include "../C3DAnimated.h"

class C3DCorona;

class C3DSwitch : public C3DAnimated
{
	public:
	C3DSwitch();															// 0x4449c0
	virtual ~C3DSwitch();													// 0x444c40

	virtual void InitObject(void);											// SLOT 7   0x444cb0 properties, animation, texture
	virtual void ResetObject(void);											// SLOT 10  0x444c90 radius 140
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x444e60 flipped by the camera / a baseball
	virtual void Process(float dt);											// SLOT 241 0x444d90 glow follows, red pulses
	virtual void v268(int state);											// SLOT 266 0x444f20 "NextState": the target and the glow

	// ---- data members (object offsets)
	int					m_my_state;			// 0x558  "MyState" (0)
	char				m_switch_object[100];	// 0x6bc  "SwitchObject" ("none"): ObjectTag of the object it switches
	C3DCorona			*m_corona;			// 0x720  the glow (210 above us)
	int					m_toggle;			// 0x724  "Toggle" (-1)
	float				m_time;				// 0x728  running time (unused)
};

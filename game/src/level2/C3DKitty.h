/*  C3DKitty ('3KIT') - a cat (School): a C3DAI whose animations are "catrun.ase" (ATTACK, WALK)
    and "catsit.ase" (STOP) with the texture "cat.png". The AI states use its names: STOP for
    waiting and sitting, WALK for running, ATTACK for walking at the target. It is shown while
    its task ("TaskName") is at most 9 and then meows with a looped 3D sound (0x53); once the
    task is beyond 9 it is hidden and silent. The sound stops while the game is paused.
    Reconstructed from Neutron.exe: constructor 0x42b800 (factory '3KIT', 0x9a0 bytes),
    destructor 0x42b980 (only resets the vtables, then ~C3DAI; deleting thunks 0x42b950 /
    0x42bbd0 / 0x42bbe0 / 0x42bbf0). Game object vtable 0x4a44ec: slots 7, 259, 264, 265, 272,
    273 overridden.

    Original layout: as C3DAI, own member 0x994, vtordisp 0x998, streamer 0x99c.              */
#pragma once
#include "../C3DAI.h"

class C3DKitty : public C3DAI
{
	public:
	C3DKitty();																// 0x42b800
	virtual ~C3DKitty();													// 0x42b980

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42b9d0 animations, texture, radius 10, "STOP"
	virtual void v260(void);												// SLOT 259 0x42ba90 shown + meowing while the task is <= 9
	virtual void v266(void);												// SLOT 264 0x419aa0 GetTaskState(TaskName) (result unused)
	virtual void v267(int level);											// SLOT 265 0x42bb00 + silent once the task is beyond 9
	virtual void v277(void);												// SLOT 272 0x42bb40 + stop the sound
	virtual void v278(void);												// SLOT 273 0x42bb60 + the sound again

	// ---- data members (names; the object offsets in the comments)
	int					m_sound;		// 0x994  channel of the meow (-1: none)
};

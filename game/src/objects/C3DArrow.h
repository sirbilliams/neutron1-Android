/*  C3DArrow ('3ARR') - a pulsing marker sprite ("sprites.omt" canvas 33, 200 wide): its height
    bobs between 150 and 250 and its colour from red to yellow, half transparent. It shows while
    the task RequiredTask has reached RequiredLevel (or equals ExactLevel when that is set).
    Reconstructed from Neutron.exe: constructor 0x40f2d0 (factory '3ARR', 0x6cc bytes),
    destructor 0x40f4b0 (only the vtables, then ~C3DSpriteType; deleting thunks 0x40f480 /
    0x40f6d0 / 0x40f6e0). Vtables 0x490db4 (element, 55 slots) / 0x490da0 (listener) / 0x490950
    (game object) / 0x490940 (streamer: C3DSprite's read_class / write_class copies 0x40f6f0 /
    0x40f700). C3DSpriteType's 0x668, own members 0x668..0x6c4, vtordisp 0x6c4, the streamer
    at 0x6c8.

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DSpriteType.h"

class C3DArrow : public C3DSpriteType
{
	public:
	C3DArrow();																// 0x40f2d0
	virtual ~C3DArrow();													// 0x40f4b0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x40f4f0 properties
	virtual void Process(float dt);											// SLOT 240 0x40f560 the pulse
	virtual void v266(void);												// SLOT 266 0x419aa0 SetTaskState(TaskName, GetTaskState(TaskName))
	virtual void v267(int level);											// SLOT 267 0x40f620 shown by RequiredTask's state

	// FIELD 0x668 0x5c
	float		m_time;					// 0x668  (= 0) += dt
	char		m_required_task[80];	// 0x66c  "RequiredTask" (= "none")
	int			m_required_level;		// 0x6bc  "RequiredLevel" (= 0)
	int			m_exact_level;			// 0x6c0  "ExactLevel" (= -1: use RequiredLevel)
};

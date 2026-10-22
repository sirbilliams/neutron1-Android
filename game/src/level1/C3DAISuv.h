/*  C3DAISuv ('3SUV') - a speeding SUV (jeep.omt shape 2, unlit) driving its patrol at 700
    with a C3DLightCone headlight 160 up and 110 ahead (green-blue tint 0.2, turning at 140).
    Pressed hard to the road (velocity y -1400, acceleration y -200). Touching Jimmy (or the
    jeep / rocket ship he drives) sends him back to his start point (sound 180) and the SUV
    resets. An SUV tagged "GOAWAY" driving into an AITrigger takes the trigger's AIPatrol as
    its patrol, drives off (state 3) and goes back to patrolling (state 2) 10 s later. The
    headlight is off while TaskName is below RequiredLevel or from RemoveLevel on.
    Reconstructed from Neutron.exe: constructor 0x40b1b0 (factory '3SUV', 0x9a4 bytes),
    destructor 0x40b410 (then ~C3DAI; deleting thunks 0x40b3e0 / 0x40ba20 / 0x40ba30 /
    0x40ba40). Vtables 0x48f728 (element, C3DAI's 95 slots) / 0x48f714 (listener) / 0x48f6d8
    (OMediaAnim) / 0x48f288 (game object at +0xc0: 7, 16, 241, 259, 265 overridden) /
    0x48f278 (streamer at +0x9a0). Own members 0x994..0x99c.                                  */
#pragma once
#include "../C3DAI.h"

class C3DLightCone;

class C3DAISuv : public C3DAI
{
	public:
	C3DAISuv();																// 0x40b1b0
	virtual ~C3DAISuv();													// 0x40b410

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x40b460 the shape
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x40b4f0 Jimmy back to his start; GOAWAY
	virtual void Process(float dt);											// SLOT 241 0x40b7a0 headlight, pressed down, GOAWAY timer
	virtual void v260(void);												// SLOT 259 0x40b8e0 the headlight by the task
	virtual void v267(int level);											// SLOT 265 0x40b960 the headlight by the task

	// ---- data members (object offsets)
	C3DLightCone		*m_light;		// 0x994  the headlight (the original keeps its CGameObject part)
	float				m_go_time;		// 0x998  GOAWAY: time until it patrols again
};

/*  C3DHumphrey ('3HUM') - Humphrey, a guard animal: a morph creature (.ase animations ATTACK
    (humprun), WALK, SHRINK, RUNSHRUNK, GROW, STOP (humpsleep), STOP2 (humpstop); texture
    "humphrey.png"), radius 20, hidden when the level loads; four of C3DAI's state animations
    are renamed (STOP / WALK / STOP / ATTACK). While SCENE is 90 his level change shows the
    clones CLONE1..7 and sets them going (state 1); his ev90 shows them, standing (state 0).
    Reconstructed from Neutron.exe: constructor 0x420730 (factory '3HUM', 0x9a8 bytes),
    destructor 0x4208c0 ("Deleting Humphrey" (debug log), then ~C3DEnemy; deleting thunks
    0x420890 / 0x420dc0 / 0x420dd0 / 0x420de0). Vtables 0x4a2370 (element, 95 slots, 90
    overridden) / 0x4a235c (listener) / 0x4a2320 (OMediaAnim) / 0x4a1ed0 (game object at
    +0xc0: 7, 259, 264, 265 overridden; 264 is C3DArrow's code 0x419aa0) / 0x4a1ec0
    (streamer at +0x9a4). No members of its own. Element slot 90 is ev90.                */
#pragma once
#include "../level1/C3DEnemy.h"

class C3DHumphrey : public C3DEnemy
{
	public:
	C3DHumphrey();															// 0x420730
	virtual ~C3DHumphrey();													// 0x4208c0

	// ---- element-family slots
	virtual void ev90(void);												// ESLOT 90  0x420bf0 SCENE 90: the clones shown, standing

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x420950 animations, texture
	virtual void v260(void);												// SLOT 259 0x420d60 C3DPickupType's, then hidden
	virtual void v266(void);												// SLOT 264 0x419aa0 SetTaskState(TaskName, GetTaskState(TaskName))
	virtual void v267(int level);											// SLOT 265 0x420a60 SCENE 90: the clones shown, going
};

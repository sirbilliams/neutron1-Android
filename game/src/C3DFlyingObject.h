/*  C3DFlyingObject - a C3DAnimated the player flies: the base of the vehicles that fly (the rocket
    ship C3DRocketShip '3ROC'). It has no factory ID of its own (InitObject sets ObjectID '3FLY').
    Reconstructed from Neutron.exe: constructor 0x419b70, destructor 0x419ed0 (deleting thunks
    0x419db0 / 0x41b050 / 0x41b060 / 0x41b070).

    Original layout (classes.json): as C3DAnimated (element vtable 0x49d474, 76 slots: C3DAnimated's
    72 + 72..75; OMediaAnim vtable 0x49d424 at +0x90; listener 0x49d460 at +0x30; CLocalGameObject
    vtable 0x49cfd4 at +0xc0), own members 0x6bc..0x73b, vtordisp 0x73c, streamer 0x740. Derived
    classes put their members from 0x73c on.

    Flying (the keys are tested inline: arrows, joystick, mouse buttons):
      v243     Left/Right (or the mouse x) turn (f_978, up to 45 deg/s), Up / left mouse / mouse up
               speed up and climb, Down / right mouse / mouse down slow down and dive (f_96c, top
               speed f_968), then the chase camera: the viewport follows the point f_9a0 behind the
               ship and looks at f_9b0 in front of it.
      Process  velocity = forward speed along the heading plus a sideways part from the turn
               (lean), vertical acceleration f_9c0 that cancels gravity (UpRate / DownRate, a
               ceiling at MaxHeight), vertical speed clamped to MaxVertVelocity; turn-around (key
               T / Enter / joystick button 8: 1 s of 180 deg/s) and barrel rolls (270 deg/s while
               f_9d4, from the BARRELROLL pickups and the R key / joystick button 6).
    ev108(state) is told each frame whether it is climbing (2), cruising (1) or slowing (0):
    C3DRocketShip plays its engine sounds from it.

    Element-family slots are `ev<n>` (`// ESLOT n`: the names of the Negatron numbering, so BG slots
    72..75 are ev106..ev109); members are f_<n>, n = the Neutron.exe object offset + 0x2a4 (the
    comments give the Neutron.exe offsets; CGameObject-family code sees them at offset - 0xc0). */
#pragma once
#include "C3DAnimated.h"
#include "OMedia3DPoint.h"
#include "OMedia3DVector.h"

class C3DFlyingObject : public C3DAnimated
{
	public:
	C3DFlyingObject();														// 0x419b70
	virtual ~C3DFlyingObject();												// 0x419ed0

	// ---- element-family slots
	virtual OMedia3DVector ev106(float x, float y, float z);				// ESLOT 106 0x43a5d0 (slot 72) position + (x,y,z) turned by (0, heading + f_978, roll), w = 1
	virtual void ev107(int a) { (void)a; }									// ESLOT 107 0x4732a0 (slot 73, empty)
	virtual void ev108(int state) { (void)state; }							// ESLOT 108 0x4732a0 (slot 74, empty) 2 climbing, 1 cruising, 0 slowing / stopped (v243)
	virtual float ev109(OMedia3DVector look, OMedia3DVector *cam);			// ESLOT 109 0x419de0 (slot 75) camera in front of walls: 1 free, 2 moved

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x419f70 properties, ObjectID '3FLY'
	virtual void ResetObject(void);											// SLOT 10  0x41a0b0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x41afc0 BARRELROLL(2) pickups / AI triggers
	virtual void Process(float dt);											// SLOT 240 0x41a140 flight
	virtual void v243(float dt);											// SLOT 243 0x41a6c0 controls + camera
	virtual void RotateToDest(float dt);									// SLOT 246 0x41adf0 rates 2 / 4 / 3 (none while rolling)
	virtual void v258(void);												// SLOT 258 0x41afb0 (CLocalGameObject's)

	// ---- data members (names: Negatron offsets; comments: Neutron.exe object offsets)
	float				f_960;			// 0x6bc  "AccelRate" (400): added to the speed every frame while climbing
	float				f_964;			// 0x6c0  "DecelRate" (1): braking
	float				f_968;			// 0x6c4  "MaxSpeed" (1400)
	float				f_96c;			// 0x6c8  forward speed
	float				f_970;			// 0x6cc  35
	float				f_974;			// 0x6d0  0
	float				f_978;			// 0x6d4  turn rate, degrees / s (+ left), -45..45
	float				f_97c;			// 0x6d8  turn rate the movement used (lean)
	float				f_980;			// 0x6dc  heading at ResetObject
	float				f_984;			// 0x6e0  "MaxHeight" (2500)
	float				f_988;			// 0x6e4  "UpRate" (650): vertical acceleration climbing
	float				f_98c;			// 0x6e8  "DownRate" (-650): diving
	float				f_990;			// 0x6ec  "MaxVertVelocity" (650)
	float				f_994;			// 0x6f0  "NewGravity" (0; not used here)
	int					f_998;			// 0x6f4  1
	unsigned char		f_99c;			// 0x6f8  climbing this frame (Up / left mouse / mouse up)
	unsigned char		f_99d;			// 0x6f9  diving this frame (Down / right mouse / mouse down)
	unsigned char		f_99e;			// 0x6fa  turning by keys (f_978 eases back to 0 after)
	unsigned char		f_99f;			// 0x6fb  0
	OMedia3DPoint		f_9a0;			// 0x6fc  camera offset (0, 200, -500)
	OMedia3DPoint		f_9b0;			// 0x70c  camera look offset (0, 50, 200)
	float				f_9c0;			// 0x71c  vertical acceleration wanted (+ gravity cancelled)
	float				f_9c4;			// 0x720  "AccelLean" (20): pitch when climbing
	float				f_9c8;			// 0x724  "DecelLean" (-20): pitch when diving
	float				f_9cc;			// 0x728  time in the air (0 on the floor)
	float				f_9d0;			// 0x72c  mouse y, summed (> 15 climbs, < -15 dives; eases back)
	float				f_9d4;			// 0x730  barrel roll time left
	float				f_9d8;			// 0x734  turn-around time left
	unsigned char		f_9dc;			// 0x738  the barrel roll key is held
	unsigned char		f_9dd;			// 0x739  the turn-around key is held
};

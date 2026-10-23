/*  C3DPlayer - the C3DAnimated the player drives: walking (keyboard arrows / joystick, mouse
    turns), the jump and the jet pack, climbing fences and ladders, the idle animations and the
    chase camera. The base of C3DJimmy ('3JIM'). No factory ID of its own (InitObject sets
    ObjectID '3PLA').
    Reconstructed from Neutron.exe: constructor 0x437280, destructor 0x437790 (deleting thunks
    0x437710 / 0x43b940 / 0x43b950 / 0x43b960).

    Original layout (classes.json): as C3DAnimated (element vtable 0x4afb64, 93 slots: C3DAnimated's
    72 + 72..92; OMediaAnim at +0x90, vtable 0x4afb14; listener +0x30, vtable 0x4afb50;
    CLocalGameObject at +0xc0, vtable 0x4af6c4, 275 slots), own members 0x6bc..0x9fc, vtordisp
    0x9fc, streamer 0xa00. C3DJimmy puts its members from 0x9fc on.

    Control state f_7c4 (Process moves by it, v243 steers and places the camera by it):
      state   Process (movement)          v243 (control + camera)
      0       MoveWalk (slot 76)          ControlWalk (73): arrows / joystick, mouse x turns
      1       MoveJump (77)               ControlFly (74): vehicle-like steering
      2       MoveBounce (79)             ControlFly (74)
      3       (nothing)                   ControlSit (75): turn on the spot
    Jump state f_7e8 (MoveJump): 0 on the floor, 1 jumped (holding the key), 2 jumped (key
    released), 3 jet pack burning (f_7f8's burn time), 4 falling after it, 5 landed.
    Climbing: AnimEnded / PlayAnim("FENCE" / "LADDER") hold the player (g_4f83e0) for f_868
    seconds while the camera rises for f_888 seconds; Process then puts him at the top (f_838).

    The BG slots 72..92 have names here instead of ev<n> (C3DAnimated's ev<n> names keep
    Negatron's numbering). Members are f_<offset in the original object> (CGameObject-family code
    sees them at offset - 0xc0).
    Left: the Negatron-only "ev103"/"ev104" aliases for the menu (C2DInGameMenu_b / _c).         */
#pragma once
#include "C3DAnimated.h"
#include "OMedia3DPoint.h"
#include "OMedia3DVector.h"

class OMedia3DPolygon;
class C3DJetpackFire;


class C3DPlayer : public C3DAnimated
{
	public:
	C3DPlayer();															// 0x437280
	virtual ~C3DPlayer();													// 0x437790

	// ---- element-family slots of C3DPlayer (Neutron.exe numbering)
	virtual OMedia3DVector RelPos(float x, float y, float z);				// ESLOT 72 0x43a5d0 position + (x,y,z) turned by (0, heading + f_6d4, roll), w = 1
	virtual void ControlWalk(float dt);										// ESLOT 73 0x438bc0 state 0: keys / mouse, idle animations, chase camera
	virtual void ControlFly(float dt);										// ESLOT 74 0x439900 states 1/2: steering, camera
	virtual void ControlSit(float dt);										// ESLOT 75 0x43a120 state 3: turn on the spot, camera
	virtual void MoveWalk(float dt);										// ESLOT 76 0x437c40 state 0 movement, pressed onto the floor
	virtual void MoveJump(float dt);										// ESLOT 77 0x437f90 state 1: the jump and the jet pack
	virtual void JetpackOn(int a) { (void)a; }								// ESLOT 78 0x4732a0 (empty) the jet pack starts
	virtual void MoveBounce(float dt);										// ESLOT 79 0x4387e0 state 2: bouncing
	virtual void JetpackOff(void) {}										// ESLOT 80 0x472970 (empty) jump / jet pack over
	virtual void StepSoundOn(void) {}										// ESLOT 81 0x472970 (empty) RUN started
	virtual void StepSoundOff(void) {}										// ESLOT 82 0x472970 (empty) an animation change / stop
	virtual unsigned char FloorFarBelow(void);								// ESLOT 83 0x43a7f0 state 0 and the floor more than 70 below RelPos-ish v224(0,0,70)
	virtual void PlayAnim(char *name);										// ESLOT 84 0x43aff0 play player animation <name> (locks, FENCE / LADDER start a climb)
	virtual void Bounced(void) {}											// ESLOT 85 0x472970 (empty) MoveBounce hit the floor
	virtual unsigned char KeyJump(void);									// ESLOT 86 0x4378d0 the jump keys (Insert, Shift, Num 0, right mouse, button 1)
	virtual unsigned char KeyFire(void);									// ESLOT 87 0x42a7a0 the fire keys (Enter, Ctrl, left mouse, button 0)
	virtual void TouchedShock(OMedia3DVector plane) { (void)plane; }		// ESLOT 88 0x475cc0 (empty) touched a wall of material 120..129
	virtual void TouchedHurt(void) {}										// ESLOT 89 0x472970 (empty) touched a wall of material 110..119
	virtual void PlayerLoadLevel(char *game, char *start, char *ret_game, char *ret_start);	// ESLOT 90 0x43b5a0 "PlayerLoadLevel"
	virtual float CamPull(OMedia3DVector look, OMedia3DVector *cam);		// ESLOT 91 0x43b820 camera pulled in front of walls (1.5) or not (1)
	virtual void ResetCamera(void);											// ESLOT 92 0x437740 the camera offsets

	// Negatron names still used by the not yet reconstructed menu (C2DInGameMenu_b / _c).
	unsigned char ev103(void) { return KeyJump(); }
	unsigned char ev104(void) { return KeyFire(); }

	// ---- element-family overrides
	virtual void ev67(void);												// ESLOT 65 0x43a900 "AnimEnded": FENCE / LADDER / SPLAT / HIT done

	// ---- CGameObject-family overrides (Negatron slot names; BG numbers in the comments)
	virtual void InitObject(void);											// SLOT 7   0x437830 "StartPoint", ObjectID '3PLA', InitPhysics
	virtual void ResetObject(void);											// SLOT 10  0x437890
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// BG SLOT 221 0x43aa40 wall contact: climb a fence (material 60..69) / ladder (180)
	virtual void Process(float dt);											// BG SLOT 241 0x437940
	virtual void v243(float dt);											// BG SLOT 243 0x438a60 control + camera (by f_7c4), or the climb camera
	virtual void RotateToDest(float dt);									// BG SLOT 246 0x43a420 rates 4.5 / 3.7 / 4.5
	virtual void v261(void);												// BG SLOT 260 0x43a750 stop (speed 0, velocity 0, StepSoundOff)
	virtual void v262(CGameObject *other, float dist);						// BG SLOT 261 0x43a790 C3DGODDARD stops us instead of pushing

	// ---- data members (object offsets)
	float				f_6bc;			// 0x6bc  acceleration per frame (30)
	float				f_6c0;			// 0x6c0  braking per frame (35)
	float				f_6c4;			// 0x6c4  top speed (600)
	float				f_6c8;			// 0x6c8  forward speed
	float				f_6cc;			// 0x6cc  35
	float				f_6d0;			// 0x6d0  0
	float				f_6d4;			// 0x6d4  turn rate, degrees / s (+ left)
	float				f_6d8;			// 0x6d8  turn rate the movement used (side slip)
	float				f_6dc;			// 0x6dc  heading at ResetObject
	int					f_6e0;			// 0x6e0  1
	unsigned char		f_6e4;			// 0x6e4  going forward this frame
	unsigned char		f_6e5;			// 0x6e5  braking this frame (ControlFly)
	unsigned char		f_6e6;			// 0x6e6  turning by keys this frame
	unsigned char		f_6e7;			// 0x6e7  0
	float				f_710;			// 0x710  heading to turn to (turning round: + 180)
	OMedia3DPoint		f_714;			// 0x714  camera position at the start of the frame
	char				f_724[80];		// 0x724  "StartPoint" ("none")
	char				f_774[80];		// 0x774  start point to return to ("none")
	short				f_7c4;			// 0x7c4  control state (see above)
	OMedia3DPoint		f_7c8;			// 0x7c8  camera offset (0, 200, -350) (ResetCamera)
	OMedia3DPoint		f_7d8;			// 0x7d8  camera look offset (0, 80, 0)
	short				f_7e8;			// 0x7e8  jump state (see above)
	float				f_7ec;			// 0x7ec  ControlFly: pitch when accelerating (-40 MoveJump, -10 MoveBounce)
	float				f_7f0;			// 0x7f0  ControlFly: pitch when braking (10)
	unsigned char		f_7f4;			// 0x7f4  0
	C3DJetpackFire		*f_7f8;			// 0x7f8  the jet pack on our back (+0x6bc owner, +0x6c0 burn time)
	unsigned char		f_7fc;			// 0x7fc  1; 0 once PlayerLoadLevel ran
	int					f_800;			// 0x800  0
	OMedia3DPoint		f_804;			// 0x804  camera position wanted (kept while turning round)
	OMedia3DVector		f_814;			// 0x814  0
	int					f_824;			// 0x824  0
	unsigned char		f_828;			// 0x828  turning round (Back held 1.5 s)
	short				f_82a;			// 0x82a  turn animation: -1 LEFT, 1 RIGHT, 0 none
	int					f_82c;			// 0x82c  0
	float				f_830;			// 0x830  time Back has been held (nothing in Neutron.exe counts it up)
	float				f_834;			// 0x834  time (Process adds dt; a climb needs > 3, a climb sets 0)
	OMedia3DPoint		f_838;			// 0x838  the top of the fence / ladder being climbed
	float				f_848;			// 0x848  mouse x, summed
	float				f_84c;			// 0x84c  camera height adjust (10; gadget 6: Up / Down)
	unsigned char		f_850;			// 0x850  Back pressed this frame
	unsigned char		f_851;			// 0x851  camera frozen (slow follow)
	OMedia3DVector		f_854;			// 0x854  camera follow factors (1.4, 1, 1.4)
	unsigned char		f_864;			// 0x864  Back does not reverse
	float				f_868;			// 0x868  climb time left (FENCE / LADDER: 5)
	float				f_86c;			// 0x86c  height above the shadow (C3DJimmy); jet pack: < 500 climbs
	float				f_870;			// 0x870  side slip of MoveWalk (-1000 left, 1000 right, 0)
	float				f_874;			// 0x874  time the jump key has been held
	float				f_878;			// 0x878  LEFT turn animation time left
	float				f_87c;			// 0x87c  RIGHT turn animation time left
	unsigned char		f_880;			// 0x880  0
	float				f_884;			// 0x884  jet pack burn time (8)
	float				f_888;			// 0x888  climb camera time left (4 fence, 6 ladder)
	char				f_88c[100];		// 0x88c  game file of the next level ("level1b.gam")
	char				f_8f0[100];		// 0x8f0  start point in the next level ("none")
	unsigned char		f_954;			// 0x954  0
	char				f_955[80];		// 0x955  game file to return to ("level1b.gam")
	unsigned char		f_9a5;			// 0x9a5  keep f_9a8 (PlayerLoadLevel does not store the position)
	OMedia3DPoint		f_9a8;			// 0x9a8  position when PlayerLoadLevel ran
	float				f_9b8;			// 0x9b8  idle time
	char				f_9bc[60];		// 0x9bc  idle animation playing ("" none, "SCRATCH", "PLAY", "BUTTONS")
	unsigned char		f_9f8;			// 0x9f8  0
};


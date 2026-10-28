/*  C3DVehicle ('3VEH') - the base of the wheeled vehicles (C3DCar -> C3DJeep, the Goddard
    scooter of level1_a): a C3DObject on up to six C3DWheel objects (f_960, placed at f_988[i] in
    the vehicle's frame; wheels 0 and 1 steer). While it is the camera object (v243):
      Left / Right (or the mouse x) steer (f_a24, +-45 degrees, 200 / s; the keys centre it
      again when let go); Go accelerates (f_9f8 / s up to the top speed f_a10 = 0.66 of the
      power f_a08; Boost: 0.825 of it), Back brakes (f_a00 / s) then reverses (down to -0.4 of
      the top speed, a rear-view mirror while reversing: ev139 / ev140); the joystick (ev112)
      steers and drives; the keys 1..9, C / V and B pick the camera (f_a38, ev110).
    Process: the wheels on the floor (front f_9e8, rear f_9ea) turn it (f_a28, by speed),
    friction, braking on slopes, and drive the velocity along the heading (or the way it slides).
    ev132 (from v248) puts the wheels under it: suspension (f_a18 down, f_a1c up), spin
    (rolling: wheel f_578, degrees), steering. A hit wall head-on above speed 20 (with f_a43)
    crashes it (ev131): no control for 4 s (ev111), then upright again.
    Reconstructed from Neutron.exe: constructor 0x464b90 (factory '3VEH', 0x670 bytes),
    destructor 0x464e20 (deleting thunks 0x467fe0 / 0x467fd0 / 0x464df0 / 0x467ff0).
    Vtables: element 0x4d5700 (90 slots: C3DObject's 54 + 54..89), game object (+0xc0) 0x4d5260
    (0, 7, 10, 11, 219, 221, 223, 241, 243, 246, 247, 248 overridden), streamer 0x4d5250. Own
    members 0x578..0x664, vtordisp 0x668, streamer 0x66c.
    Element-family slots are `ev<n>` (`// ESLOT n`: n = the slot + 51, the names the other
    vehicle classes use); members are f_<n>, n = the Neutron.exe object offset + 0x3e8 (the
    comments give the Neutron.exe offsets; CGameObject-family code sees them at offset - 0xc0). */
#pragma once
#include "../C3DObject.h"
#include "OMedia3DVector.h"

class C3DObject;
class OMediaViewPort;
class OMedia3DPolygon;

class C3DVehicle : public C3DObject
{
	public:
	C3DVehicle();															// 0x464b90
	virtual ~C3DVehicle();													// 0x464e20

	// ---- element-family slots
	virtual void HideAll(void);											// ESLOT 54  0x464f00 (slot 54, +0xd8) hide vehicle and wheels
	virtual void ShowAll(void);												// slot 55 0x464f30 (+0xdc) hide(false), with the wheels
	virtual void ev107(float power);										// ESLOT 107 0x464f60 (slot 56) power: accel, brake, top speed
	virtual void ev108(float d);											// ESLOT 108 0x464fd0 (slot 57) power += d
	virtual void ev109(float x, float y, float z);							// ESLOT 109 0x465170 (slot 58) move it and its wheels, the viewport too
	virtual void ev110(float dt);											// ESLOT 110 0x465e00 (slot 59) the camera (C3DCar overrides it)
	virtual void ev111(float dt);											// ESLOT 111 0x4669f0 crashed: 4 s, then upright
	virtual void ev112(void);												// ESLOT 112 0x466b30 joystick: steer, drive
	virtual void ev113(short *dx, short *dy);								// ESLOT 113 0x466be0 mouse movement (0x46a460)
	virtual unsigned char ev114(void);										// ESLOT 114 0x466c00 key 1 (camera 6)
	virtual unsigned char ev115(void);										// ESLOT 115 0x466c10 key 2 (camera 3)
	virtual unsigned char ev116(void);										// ESLOT 116 0x466c20 key 3 (camera 1)
	virtual unsigned char ev117(void);										// ESLOT 117 0x466c30 key 4 (camera 2)
	virtual unsigned char ev118(void);										// ESLOT 118 0x466c40 key 5 (camera 11)
	virtual unsigned char ev119(void);										// ESLOT 119 0x466c50 key 6 (camera 12)
	virtual unsigned char ev120(void);										// ESLOT 120 0x466c60 key 7 (camera 20)
	virtual unsigned char ev121(void);										// ESLOT 121 0x466c70 key 8 (camera 13)
	virtual unsigned char ev122(void);										// ESLOT 122 0x466c80 key 9 (camera 15)
	virtual unsigned char ev123(void);										// ESLOT 123 0x466c90 Left
	virtual unsigned char ev124(void);										// ESLOT 124 0x466ca0 Right
	virtual unsigned char ev125(void);										// ESLOT 125 0x466cb0 Go (rear wheels down)
	virtual unsigned char ev126(void);										// ESLOT 126 0x466ce0 Back (rear wheels down)
	virtual unsigned char ev127(void);										// ESLOT 127 0x466d20 Boost (rear wheels down)
	virtual unsigned char ev128(void);										// ESLOT 128 0x466d70 C / V pressed: next camera
	virtual unsigned char ev129(void);										// ESLOT 129 0x466da0 B held: look back (camera 8)
	virtual unsigned char ev130(void);										// ESLOT 130 0x466db0 Backspace pressed
	virtual void ev131(void);												// ESLOT 131 0x467000 crash: stop, 4 s without control
	virtual void ev132(void);												// ESLOT 132 0x467080 the wheels: place, spin, steer; tilt to the floor
	virtual void ev133(void) {}												// ESLOT 133 0x472970 (empty)
	virtual void ev134(void);												// ESLOT 134 0x467990 in the air: pitch 340
	virtual OMedia3DVector ev135(float x, float y, float z);				// ESLOT 135 0x4679c0 (x, y, z) turned by the heading only, w 0
	virtual OMedia3DVector ev136(float x, float y, float z);				// ESLOT 136 0x467aa0 = v224: local point -> world
	virtual OMedia3DVector ev137(float x, float y, float z);				// ESLOT 137 0x467af0 v224 with the steering added to the heading
	virtual OMedia3DVector ev138(float x, float y, float z);				// ESLOT 138 0x467c90 local point -> world by the element's angles
	virtual void ev139(void);												// ESLOT 139 0x467db0 the rear-view mirror on
	virtual void ev140(void);												// ESLOT 140 0x467f70 the rear-view mirror off

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x465030 '3VEH'
	virtual void ResetObject(void);											// SLOT 10  0x4650a0 stopped, rotation kept in f_978
	virtual void InitPhysics(void);											// SLOT 11  0x465070
	virtual void v218(float a, float b, float c);							// SLOT 219 0x466af0 + f_278 = f_228
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x466dd0 walls: crash, slow down
	virtual void v222(void) {}												// SLOT 223 0x41ee20 (empty)
	virtual void Process(float dt);											// SLOT 241 0x465220 wheels, turning, friction, driving
	virtual void v243(float dt);											// SLOT 243 0x465830 the controls, then the camera
	virtual void RotateToDest(float dt);									// SLOT 246 0x466b20 (CGameObject's)
	virtual void v247(void);												// SLOT 247 0x467050 (C3DObject's)
	virtual void v248(void);												// SLOT 248 0x467060 the wheels (ev132), then C3DObject's

	// ---- data members (names: Negatron offsets; comments: Neutron.exe object offsets)
	C3DObject			*f_960[6];		// 0x578  the wheels (C3DWheel, element pointers; C3DCar makes them)
	OMedia3DVector		f_978;			// 0x590  rotation at ResetObject (w = 1 at first)
	OMedia3DVector		f_988[6];		// 0x5a0  wheel positions in our frame (w 1)
	short				f_9e8;			// 0x600  front wheels (0, 1) on the floor
	short				f_9ea;			// 0x602  other wheels on the floor
	short				f_9ec;			// 0x604  all wheels on the floor
	float				f_9f0;			// 0x608  power at ResetObject (100)
	float				f_9f4;			// 0x60c  power (100; ev107)
	float				f_9f8;			// 0x610  acceleration / s (700)
	float				f_9fc;			// 0x614  = f_9f8
	float				f_a00;			// 0x618  braking / s (1200)
	float				f_a04;			// 0x61c  engine level (speed * 1.515, or f_a08 accelerating; the engine sound)
	float				f_a08;			// 0x620  top power (2250)
	float				f_a0c;			// 0x624  speed this frame (v243)
	float				f_a10;			// 0x628  top speed (1485 = 0.66 * f_a08; boost 0.825)
	float				f_a14;			// 0x62c  = f_a10
	float				f_a18;			// 0x630  suspension: down (15)
	float				f_a1c;			// 0x634  suspension: up (15)
	float				f_a20;			// 0x638  grip this frame (Process)
	float				f_a24;			// 0x63c  steering, degrees (+-45)
	float				f_a28;			// 0x640  turn rate (Process)
	float				f_a2c;			// 0x644  heading at ResetObject
	float				f_a30;			// 0x648  sliding time (Process)
	float				f_a34;			// 0x64c  crashed time (ev111)
	int					f_a38;			// 0x650  camera (1; ev110)
	int					f_a3c;			// 0x654  camera before B (looking back)
	unsigned char		f_a40;			// 0x658  accelerating
	unsigned char		f_a41;			// 0x659  braking (no grip)
	unsigned char		f_a42;			// 0x65a  0 (unused)
	unsigned char		f_a43;			// 0x65b  walls crash it (1)
	unsigned char		f_a44;			// 0x65c  crashed
	unsigned char		f_a45;			// 0x65d  steered by the keys (centre again)
	unsigned char		f_a46;			// 0x65e  the rear-view mirror always on
	unsigned char		f_a47;			// 0x65f  Back held
	unsigned char		f_a48;			// 0x660  boost
	unsigned char		f_a49;			// 0x661  joystick: accelerate
	unsigned char		f_a4a;			// 0x662  joystick: back
	OMediaViewPort		*f_a4c;			// 0x664  the rear-view mirror
};

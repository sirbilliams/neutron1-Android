/*  C3DCar - a C3DVehicle with its wheels and its chase camera: SetupCar4 makes four C3DWheel
    objects (ev141, linked both ways) or six (ev142), the engine plays "Scooter" (3D, looped)
    pitched by the speed. It leans into turns (roll -steering / 2, ev132).
    The camera (ev110), by f_a38 (keys 1..9, C / V, B in C3DVehicle::v243):
      1  behind (-30, 0, -200) +50 up, catching up 1x / 1.7x / 2.8x a second (beyond 200 / 400
         from it)          2  left where it is
      3, 4  behind and above, farther with speed   5  behind at the heading plus the steering
      6  in it (the vehicle hidden)   7  just above it   8  in front, looking back (B)
      9..12  beside a wheel   13, 16, 18  rising 4000 above it   15, 17, 19  towards (0, 3000, 1500)
      14, 20  500 behind and 1000 up (20: looking down)
    It looks at 100 in front of it (50 up), or with its own angles (6, 7, 9..12); crashed, it
    drifts up above it. With the camera collision on (g_5099aa) the probe (C3DCamera) keeps it
    out of the walls. A rear-view mirror (C3DVehicle ev139) is placed 30 behind, facing back.
    Reconstructed from Neutron.exe: constructor 0x412870 (no factory ID; ObjectID '3CAR'),
    destructor 0x4129e0 (deleting thunks 0x413ab0 / 0x413aa0 / 0x413ac0 / 0x413ad0).
    Vtables: element 0x495188 (93 slots: C3DVehicle's 90 + 90..92; 59, 63..71, 75..77, 81
    overridden; 63..71, 76, 77 = 0x413a60, return 0), game object (+0xc0) 0x494ce8 (0, 10, 241
    overridden), streamer 0x494cd8. Own members 0x668..0x698, vtordisp 0x69c, streamer 0x6a0.
    Element-family slots are `ev<n>` (`// ESLOT n`: n = the slot + 51, as C3DVehicle's); members
    are f_<n>, n = the Neutron.exe object offset + 0x3e8 (the comments give the Neutron.exe
    offsets; CGameObject-family code sees them at offset - 0xc0).                              */
#pragma once
#include "C3DVehicle.h"

class C3DCar : public C3DVehicle
{
	public:
	C3DCar();																// 0x412870
	virtual ~C3DCar();														// 0x4129e0

	// ---- element-family slots
	virtual void ev110(float dt);											// ESLOT 110 0x4130a0 (slot 59) the camera
	virtual unsigned char ev114(void) { return 0; }							// ESLOT 114..122 0x413a60: no camera keys
	virtual unsigned char ev115(void) { return 0; }
	virtual unsigned char ev116(void) { return 0; }
	virtual unsigned char ev117(void) { return 0; }
	virtual unsigned char ev118(void) { return 0; }
	virtual unsigned char ev119(void) { return 0; }
	virtual unsigned char ev120(void) { return 0; }
	virtual unsigned char ev121(void) { return 0; }
	virtual unsigned char ev122(void) { return 0; }
	virtual unsigned char ev126(void);										// ESLOT 126 0x413060 Back: joystick button 1 / down / Down arrow, rear wheels down
	virtual unsigned char ev127(void) { return 0; }							// ESLOT 127 0x413a60: no boost
	virtual unsigned char ev128(void) { return 0; }							// ESLOT 128 0x413a60: no camera toggle
	virtual void ev132(void);												// ESLOT 132 0x412f90 lean, then the wheels
	virtual void ev141(float zf, float zr, float xf, float xr, float y, int shape_f, int shape_r);	// ESLOT 141 0x412a90 "SetupCar4": four wheels
	virtual void ev142(float zf, float zm, float zr, float xf, float xm, float xr, float y,
					   int shape_f, int shape_m, int shape_r);				// ESLOT 142 0x412c90 "SetupCar4": six wheels (4, 5 in the middle)
	virtual void ev143(void) {}												// ESLOT 143 0x4732a0 (empty, one argument in the original)

	// ---- CGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10  0x412ff0 back to f_a50 / f_a60
	virtual void Process(float dt);											// SLOT 241 0x412ed0 engine sound

	// ---- data members (object offsets)
	OMedia3DVector		f_a50;			// 0x668  start position (ResetObject; w 1)
	OMedia3DVector		f_a60;			// 0x678  start rotation (ResetObject; w 1)
	float				f_a70;			// 0x688  time (+= dt)
	float				f_a74;			// 0x68c  time (+= dt)
	int					f_a78;			// 0x690  the "Scooter" channel (-1)
	int					f_a7c;			// 0x694  its base frequency (11025)
	unsigned char		f_a80;			// 0x698  0 (C3DJeep: Jump held)
};

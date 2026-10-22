/*  C3DRocketShip ('3ROC') - Jimmy's rocket ship (the Strato XL): the C3DFlyingObject Jimmy flies
    with Goddard on board ("C3DROCKETSHIP"). Jimmy's rocket gadget makes him its rider (f_9f4) and
    the camera; while it runs Process puts the rider, the flame and Goddard where the ship is,
    keeps the camera on it and flashes it red after a hit. The engine dies after 10 s in the air
    (top speed 10, pushed down) unless f_1235; Jimmy steers and reads the menu (his MenuInput).
    Reconstructed from Neutron.exe: constructor 0x43d840 (factory '3ROC', 0xf9c bytes),
    destructor 0x43da70 (deleting thunks 0x43da40 / 0x43ec40 / 0x43ec50 / 0x43ec60).

    Original layout (classes.json): as C3DFlyingObject (element vtable 0x4b2a4c, 77 slots:
    C3DFlyingObject's 76 + 76; CLocalGameObject vtable 0x4b25ac at +0xc0, 275 slots), own members
    0x73c..0xf93, vtordisp 0xf94, streamer 0xf98.

    What it owns: f_1230, the C3DFireStrato flame (its CGameObject; Process shows it while the ship
    moves and the engine runs, hides it while diving). The 20 smoke puffs f_9f8[] (0x744) are never
    made in this game (the constructor clears them; Process and v260 only look at the empty slots).
    The rider f_9f4 is Jimmy's element (tag "JIM1"; C3DJimmy's rocket gadget sets and clears it),
    f_11f4 is Goddard (found by his tag "C3DGODDARD" in Process, sat in ROCK on the wing).

    Hits (v220 walls by material, AddTouching mines, rocks and missiles): Jimmy is shocked (his
    Shock), f_11f8 starts f_1238 seconds of flashing red (f_11fc counts up); with f_1248 it is also
    pushed down (-700) and the engine sound sputters.

    Element-family slots are `ev<n>` (`// ESLOT n`: the names of the base classes, see
    C3DFlyingObject.h); members keep the names other classes use (f_<n>, n = the Negatron offset, mostly the
    Neutron.exe offset + 0x2b4); the comments give the Neutron.exe object offsets
    (CGameObject-family code sees them at offset - 0xc0).                                      */
#pragma once
#include "C3DFlyingObject.h"

class C3DGoddard;
class OMediaStreamOperators;
class OMedia3DPolygon;

class C3DRocketShip : public C3DFlyingObject
{
	public:
	C3DRocketShip();														// 0x43d840
	virtual ~C3DRocketShip();												// 0x43da70

	// ---- element-family slots
	virtual void ev108(int state);											// ESLOT 108 0x43ea80 (slot 74) engine sound: 0 off, 1 "rocket fly" (0xc3), 2 "rocket blast" (0xc4)
	virtual void ev110(void);												// ESLOT 110 0x43e860 (slot 76) only counts f_1228; its two vectors are unused

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x43dd10 "Strato.ase" animation, "strato.png"
	virtual void ResetObject(void);											// SLOT 10  0x43ea60 (speed kept with a rider)
	virtual void InitPhysics(void);											// SLOT 11  0x42ab50 (unless f_4aa)
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x43dda0 mines, rocks, missiles
	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// SLOT 220 0x43eb70 ceiling: vertical speed -120
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x43e8d0 wall: hits by material
	virtual void Process(float dt);											// SLOT 241 0x43df70 riders, camera, flame, hit flash
	virtual void v243(float dt);											// SLOT 243 0x43e7e0 the rider's menu input
	virtual void v260(void);												// SLOT 259 0x43db10 level start: rider's place, MaxHeight by level
	virtual void v266(void);												// SLOT 264 0x43ea50 GetTaskState("LIBBY") (result unused)
	virtual void v267(int level);											// SLOT 265 0x43ebb0 + with a rider: shown, engine can't die
	virtual void v275(void);												// SLOT 270 0x43eb40 Jimmy's v275

	// ---- data members (names: Negatron offsets; comments: Neutron.exe object offsets)
	int					f_9f0;			// 0x73c  0
	C3DAnimated			*f_9f4;			// 0x740  the rider: Jimmy's element (NULL: nobody on board)
	C3DObject			*f_9f8[20];		// 0x744  the smoke puffs (never made: always NULL)
	short				f_11f0;			// 0xf3c  next smoke puff (0..19)
	C3DGoddard			*f_11f4;		// 0xf40  Goddard (element; found in Process)
	unsigned char		f_11f8;			// 0xf44  hit: flashing for f_1238 s
	float				f_11fc;			// 0xf48  time since the hit
	int					f_1228;			// 0xf74  ev110 counter (0..9)
	float				f_122c;			// 0xf78  time (v243: counts while the fire key is held with a rider; wraps at 0.4)
	CGameObject			*f_1230;		// 0xf7c  the flame (a C3DFireStrato's CGameObject)
	unsigned char		f_1234;			// 0xf80  engine dead: top speed 10, pushed down (after 10 s in the air, unless f_1235)
	unsigned char		f_1235;			// 0xf81  the engine never dies (1)
	float				f_1238;			// 0xf84  length of the hit (s)
	int					f_123c;			// 0xf88  engine sound channel (-1)
	int					f_1244;			// 0xf8c  engine sound state (ev108's last)
	unsigned char		f_1248;			// 0xf90  damaged: pushed down while hit, the sound sputters
};

namespace neg {
// port: Negatron's hook for the hurt call, still used by C3DAIOmtObj.cpp (Neutron.exe calls the
// camera's element slot 104 directly: C3DJimmy::Shock). `flag` is unused.
void JimmyHurt(CGameObject *jimmy, float energy, int points, float time, unsigned char flag);
}

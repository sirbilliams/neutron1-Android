/*  C3DJimmy ('3JIM') - the player: Jimmy Neutron. A C3DPlayer (walking, the jump and the jet
    pack, climbing, the chase camera) plus everything only Jimmy has: his .ase animation set,
    his gadgets (the bubble f_a14, the rocket ship f_ab0, the shrink ray f_a10, the baseballs
    f_ab4, the grappling hook f_b0c, Goddard rides and follows), energy (g_4f83d4) and health
    (f_a28), the PDA menu (f_a18, a C2DInGameMenu) and the level start (PostLoad).
    The one C3DJimmy of the game is made by CreateStaticObjects (CreateObject('3JIM') after the
    first level load) and kept in g_jimmy (its CGameObject part).
    Reconstructed from Neutron.exe: factory '3JIM', constructor 0x422160 (0x1e7c bytes),
    destructor 0x4229e0 (deleting thunks 0x4229b0 / 0x42b280 / 0x42b290 / 0x42b2a0..0x42b2c0).

    Original layout (classes.json): as C3DPlayer (element vtable 0x4a3bdc, 131 slots: C3DPlayer's
    93 + 93..130; OMediaAnim at +0x90, vtable 0x4a3b8c; listener +0x30, vtable 0x4a3bc8;
    CLocalGameObject at +0xc0, vtable 0x4a373c, 275 slots), own members 0x9fc..0x1e74, vtordisp
    0x1e74, streamer 0x1e78.

    Gadgets (g_4f0588 the one in use, -1 none; menu page 0 items 0..7 say which he has: type 1
    has it, 4 has it but not usable here, 0 no). Activate (slot 93) / Deactivate (slot 94):
      0 bubble: he floats in it (C3DBubble f_a14)      1 rocket ship (C3DRocketShip f_ab0)
      2 power (f_1e1c / f_1e20 timers, the gadget bar) 3 the shrink ray (fired with no gadget)
      4 Goddard rides the jeep (C3DJeep f_a30)          5 Goddard (C2DInGameMenu +0x4bc)
      6 the baseball launcher (camera raised)           7 the grappling hook (f_b0c, the rope)
    Item 9 of page 0 at type 2 is the PDA's "VR" page: activating any gadget then loads that
    gadget's VR level (vr01.gam .. vr08.gam) from the phone booth.

    Element-family slots of Neutron.exe are named here (the base classes still use Negatron's
    slot numbers for their ev<n> names, see C3DPlayer.h); the comments give the BG slot.
    The PDA menu functions (BG slots 107, 116, 117, 124..127 and the free functions 0x42aff0 /
    0x42b0b0 / 0x42b180) are agent F's, in jimmy/C3DJimmy_menu.{inc,cpp}.
    Members are f_<offset in the original object> (CGameObject-family code sees them at
    offset - 0xc0).                                                                          */
#pragma once
#include "C3DPlayer.h"
#include "C3DGoddard.h"
#include "OMedia3DPoint.h"
#include "OMedia3DVector.h"

class CTaskList;
class C2DInGameMenu;
class C3DShadow;
class C3DTargetCursor;
class C3DPointCursor;
class C3DGraplingHook;
class C3DBubble;
class C3DBaseball;
class C3DRocketShip;
class C3DJeep;

class C3DJimmy : public C3DPlayer
{
	public:
	C3DJimmy();																// 0x422160
	virtual ~C3DJimmy();													// 0x4229e0

#include "jimmy/C3DJimmy_a.inc"
#include "jimmy/C3DJimmy_b.inc"
#include "jimmy/C3DJimmy_menu.inc"

	// ---- data members (object offsets). Names say what is known.
	int					f_9fc;			// 0x9fc  0
	C3DAnimated			*f_a10;			// 0xa10  C3DShrinkRay: the ray's effect (element; shown while firing)
	C3DBubble			*f_a14;			// 0xa14  the bubble (element)
	C2DInGameMenu		*f_a18;			// 0xa18  the PDA menu (also g_menu)
	short				f_a0c;			// 0xa0c  0
	C3DGoddard			*f_a1c;			// 0xa1c  Goddard (made by PostLoad; element)
	float				f_a20;			// 0xa20  0 (0.5 when a gadget ends)
	short				f_a24;			// 0xa24  4
	short				f_a26;			// 0xa26  -1
	float				f_a28;			// 0xa28  health 0..100 (50; slot 115 adds)
	C3DShadow			*f_a2c;			// 0xa2c  his blob shadow (element)
	C3DJeep				*f_a30;			// 0xa30  the scooter Goddard rides (gadget 4; made by PostLoad; element)
	int					f_a34;			// 0xa34  sound channel (-1): the shrink ray
	int					f_a38;			// 0xa38  music channel (-1): the start point's music
	int					f_a3c;			// 0xa3c  sound channel (-1): the jet pack
	int					f_a40;			// 0xa40  sound channel (-1): the bubble
	int					f_a44;			// 0xa44  sound channel (-1): footsteps
	int					f_a48;			// 0xa48  sound channel (-1): "not available"
	char				f_a4c[100];		// 0xa4c  "none": the music database of the start point
	C3DRocketShip		*f_ab0;			// 0xab0  the rocket ship (gadget 1; element)
	C3DBaseball			*f_ab4[20];		// 0xab4  the baseball pool (elements)
	short				f_b04;			// 0xb04  next baseball (0..19)
	int					f_b08;			// 0xb08  0
	C3DGraplingHook		*f_b0c;			// 0xb0c  the grappling hook's rope (element)
	C3DTargetCursor		*f_b14;			// 0xb14  the target cursor (element)
	CTaskList			*f_b18;			// 0xb18  the task list
	OMedia3DPoint		f_b1c[300];		// 0xb1c  trail of positions (slot 108; going back along it)
	float				f_1ddc;			// 0x1ddc trail: time since the last point
	int					f_1de0;			// 0x1de0 trail: last point (0..299)
	float				f_1de8;			// 0x1de8 hurt / shocked animation time
	unsigned char		f_1dec;			// 0x1dec hit (slot 89): flashing
	unsigned char		f_1ded;			// 0x1ded splat (slot 88): flashing, then pushed back
	OMedia3DVector		f_1df0;			// 0x1df0 splat: the push (x, y, z) and the plane's d
	unsigned char		f_1e00;			// 0x1e00 a gadget key is held (slot 97 acts once per press)
	unsigned char		f_1e01;			// 0x1e01 0
	unsigned char		f_1e02;			// 0x1e02 0
	unsigned char		f_1e03;			// 0x1e03 shocked (slot 104): flashing
	int					f_1e04;			// 0x1e04 sound channel (-1): the grappling hook
	int					f_1e08;			// 0x1e08 sound channel (-1): water
	OMedia3DPoint		f_1e0c;			// 0x1e0c where he last stood on the floor / the start point
	float				f_1e1c;			// 0x1e1c gadget 2: time left (7)
	float				f_1e20;			// 0x1e20 gadget 2: recharging time
	unsigned char		f_1e24;			// 0x1e24 0
	unsigned char		f_1e26;			// 0x1e26 0
	float				f_1e28;			// 0x1e28 0
	float				f_1e2c;			// 0x1e2c shocked: how long (5)
	float				f_1e30;			// 0x1e30 0
	OMedia3DPoint		f_1e38;			// 0x1e38 checkpoint position (slots 122 / 123)
	int					f_1e48;			// 0x1e48 checkpoint level (g_gametype +0x490)
	unsigned char		f_1e50;			// 0x1e50 0
	unsigned char		f_1e51;			// 0x1e51 touched the REACTOR button (no timeout sound)
	CGameObject			*f_1e54;		// 0x1e54 the point cursor (its CGameObject part)
	unsigned char		f_1e58;			// 0x1e58 0
	unsigned char		f_1e59;			// 0x1e59 gadgets locked
	float				f_1e5c;			// 0x1e5c time in the air (slot 105: landing sound when > 0.4)
	float				f_1e60;			// 0x1e60 time he cannot be splat again
	unsigned char		f_1e64;			// 0x1e64 0
	float				f_1e68;			// 0x1e68 0
	float				f_1e6c;			// 0x1e6c gadget key latch time (slot 97 waits for 0)
	int					f_1e70;			// 0x1e70 0
	// (f_55f, f_1e25, f_1e34, f_1e4c: in jimmy/C3DJimmy_menu.inc)
	// port: a CGameObject member of Neutron.exe that our (Negatron-shaped) CGameObject does not have
	unsigned char		f_55d;			// 0x55d  (CGameObject +0x49d) ghost: no collisions with AIs (gadget 2)
};

namespace neg {
C3DJimmy *Jimmy(void);					// g_jimmy as a C3DJimmy (NULL if there is none)
}

/*  C3DGoddard ('3GOD') - Jimmy's robot dog. A morph model (HI + "godfly" / "godeat" / "godrun" /
    "godsit" / "godscooter" / "godwag" / "godgrowl" / "godrocket" / "godpoint" .ASE animations,
    texture "goddard02.png"). As an AI he follows his target (Jimmy, '3JIM') in state 4 at one of
    six places around it (ev95: behind, beside, in front; ev99 / ev96 move to a free one when a
    wall is in the way, every 2 s), jumps over Jimmy when they touch, teleports behind him when
    left more than 1500 units behind, and flies (FLY) when he is not on the floor. While the
    player steers him (g_camera is Goddard: C3DJimmy's menu item 5) v243 turns and runs him with
    the keys / mouse / joystick, he flies up with the jump keys, two 3D arrows (C3DCursor f_1b80
    towards the nearest red neutron '3RED', f_1b84 towards the nearest pickup '3PIC') show where to
    go, a chase camera sits behind him, and Esc / Space / Tab / joystick button 2 hands the camera
    back to Jimmy. He has a blob shadow (f_1b50) that Process puts on the ground under him.
    C3DJimmy's constructor makes the one Goddard (C3DJimmy f_a1c); C3DRocketShip and C3DAIOmtObj
    carry him; the level LV1C / LV1D / LEV6 / LEV7 / LV4A / LV6A hide him (v260).
    Reconstructed from Neutron.exe: factory '3GOD', constructor 0x41c810 (0x9f4 bytes),
    destructor 0x41cbe0 (only resets the vtables, then ~C3DAI; deleting thunks 0x41ee70 /
    0x41cbb0 / 0x41ee80 / 0x41ee60). ResetObject (0x41e6a0 jumps), ev79 (0x41e580 jumps) and slot
    223 (0x41ee20) are the base class's. The six places (static initialisers 0x41c6a0..0x41c7e0,
    OMedia3DVectors with w = 0) are constants of the code here (kSides).

    Original layout: as C3DAI (element vtable 0x49f920, 100 slots: C3DAI's 95 + 95..99; OMediaAnim
    vtable 0x49f8d0 at +0x90; listener 0x49f90c at +0x30; CLocalGameObject vtable 0x49f480 at
    +0xc0), own members 0x994..0x9e8, vtordisp 0x9ec, streamer 0x9f0.

    Element-family slots are `ev<n>` (`// ESLOT n`); members keep the names other classes use
    (f_<n>, n = the Negatron offset = the Neutron.exe offset + 0x11bc); the comments give the
    Neutron.exe object offsets (CGameObject-family code sees them at offset - 0xc0).         */
#pragma once
#include "C3DAI.h"

class C3DShadow;
class OMediaElement;
class OMedia3DPolygon;

class C3DGoddard : public C3DAI
{
	public:
	C3DGoddard();															// 0x41c810
	virtual ~C3DGoddard();													// 0x41cbe0

	// ---- element-family slots
	virtual void ev82(void);												// ESLOT 82 0x41e530 face the way the target faces (until arrived)
	virtual void ev84(void);												// ESLOT 84 0x41e6b0 RUN starts: forget the channel f_1b54
	virtual void ev86(void);												// ESLOT 86 0x41e6b0 WAG starts: the same
	virtual void ev88(void);												// ESLOT 88 0x41e6d0 stop the sound f_1b58
	virtual void ev92(float unused);										// ESLOT 92 0x41ddd0 turn towards m_dest_point (yaw only)
	virtual void ev95(short side);											// ESLOT 95 0x41cc30 follow at kSides[side]
	virtual unsigned char ev96(short side);									// ESLOT 96 0x41e9e0 that side free (nothing between us)? then take it
	virtual void ev97(float dt);											// ESLOT 97 0x41cea0 steered: turn, run along the heading
	virtual void ev98(unsigned char on);									// ESLOT 98 0x41dcd0 "sonar" on / off
	virtual void ev99(void);												// ESLOT 99 0x41e720 blocked: try the sides next to ours

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x41cd70 model, animations
	virtual void InitPhysics(void);											// SLOT 11  0x42ab50 (unless f_4aa)
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x41eda0 Jimmy while following: jump over him
	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// SLOT 220 0x41ee10 ceiling: m_keep_point = 0
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x41e700 wall: state 4 -> ev99
	virtual void Process(float dt);											// SLOT 241 0x41dee0
	virtual void v243(float dt);											// SLOT 243 0x41d0c0 steered by the player (keys / mouse), arrows, camera
	virtual void v260(void);												// SLOT 259 0x41e590 properties loaded (hidden in some levels)

	// ---- data members (Neutron.exe object offsets)
	C3DShadow			*f_1b50;		// 0x994  blob shadow
	int					f_1b54;			// 0x998  sound channel (-1; ev84 / ev86 forget it)
	int					f_1b58;			// 0x99c  sound channel (-1; ev88 stops it)
	short				f_1b5c;			// 0x9a0  side we follow at (ev95, 0..5)
	short				f_1b5e;			// 0x9a2  side ev99 started from
	unsigned char		f_1b60;			// 0x9a4  sitting / riding along (C3DFish, C3DJimmy, C3DAIOmtObj); Process: a cut scene shows him in SIT, ends when far
	float				f_1b64;			// 0x9a8  steering: turn rate wanted (+-60, eases back)
	float				f_1b68;			// 0x9ac  steering: turn rate used (ev97)
	float				f_1b6c;			// 0x9b0  the same, as used last
	float				f_1b70;			// 0x9b4  steering: top speed (700)
	float				f_1b74;			// 0x9b8  steering: acceleration (30)
	int					f_1b78;			// 0x9bc  steering: animation playing (0 RUN, 1 WAG, 3 FLY)
	int					f_1b7c;			// 0x9c0  "sonar" sound channel (-1)
	CGameObject			*f_1b80;		// 0x9c4  C3DCursor towards the nearest red neutron (its CGameObject part)
	CGameObject			*f_1b84;		// 0x9c8  C3DCursor towards the nearest pickup (its CGameObject part)
	CGameObject			*f_1b88;		// 0x9cc  the nearest '3PIC'
	CGameObject			*f_1b8c;		// 0x9d0  the nearest '3RED'
	unsigned char		f_1b90;			// 0x9d4  go behind Jimmy (the camera object) next Process
	float				f_1b94;			// 0x9d8  steering: highest he flies (1500; VR06 3500)
	float				f_1b98;			// 0x9dc  steering: mouse x, summed
	float				f_1b9c;			// 0x9e0  steering: mouse y, summed
	float				f_1ba0;			// 0x9e4  time since the jump (m_keep_point, 5 s)
	unsigned char		f_1ba4;			// 0x9e8  not in this level (v260)
};

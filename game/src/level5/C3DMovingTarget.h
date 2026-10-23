/*  C3DMovingTarget ('3TAR') - a shooting-gallery target (VR07): a sprite (sprites.omt canvas 98, size
    100) that glides from "StartPos" to "DestPos" at "Speed" (100) and jumps back to the start
    within 100 of the end. A C3DBaseball that touches it (when it is not waiting to come back) hides
    it for "RespawnTime" (15 s) and scores "NumPoints" (10); on a C3DTrophy or a C3DPickupItem named
    "ActivateObject" the hit is counted in that object, and above "HitsRequired" (5) it is shown
    (NextState 1), its count reset.
    Reconstructed from Neutron.exe: constructor 0x430220 (factory '3TAR', 0x68c bytes), destructor
    0x430530 (deleting thunks 0x430500 / 0x430890 / 0x4308a0), AddTouching 0x430570, Process
    0x4306b0. Vtables 0x4a8a10 (game object at +0xc8: 0, 16, 241 overridden) / 0x4a8a00 (streamer).
    Members are named after their offset from the object start (element view).             */
#pragma once
#include "../C3DSpriteType.h"

class C3DMovingTarget : public C3DSpriteType
{
	public:
	C3DMovingTarget();														// 0x430220
	virtual ~C3DMovingTarget();												// 0x430530

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x430570 hit by a baseball
	virtual void Process(float dt);											// SLOT 241 0x4306b0 glide to DestPos, respawn

	// ---- data members (object offsets)
	int				f_5e8;				// 0x5e8  = 0 (unused)
	float			m_start[4];			// 0x5ec  "StartPosX/Y/Z" (0, w 1)
	float			m_dest[4];			// 0x5fc  "DestPosX/Y/Z" (0, w 1)
	float			m_speed;			// 0x60c  "Speed" (100)
	float			m_respawn;			// 0x610  seconds until it is shown again (0)
	char			m_activate[100];	// 0x614  "ActivateObject" ("none")
	int				m_hits_required;	// 0x678  "HitsRequired" (5)
	int				m_num_points;		// 0x67c  "NumPoints" (10)
	float			m_respawn_time;		// 0x680  "RespawnTime" (15)
};

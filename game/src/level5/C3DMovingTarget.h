/*  C3DMovingTarget ('3TAR') - a baseball target: a sprite (sprites.omt canvas 98, size 100) that
    glides from "StartPos" towards "DestPos" at "Speed" (100) and jumps back to the start within
    100 of the end. A baseball hitting it (while its respawn timer is out) hides it for
    "RespawnTime" (15) seconds and scores "NumPoints" (10); "ActivateObject" is the object
    that counts the hits (a trophy or a pickup item): after "HitsRequired" (5) hits it is
    activated (slot 266 with 1) and the sound 199 plays - 198 when there is no such object.
    Reconstructed from Neutron.exe: constructor 0x430220 (factory '3TAR', 0x68c bytes),
    destructor 0x430530 (deleting thunks 0x430500 / 0x430890 / 0x4308a0). The vtable 0x4a8a10
    (game object at +0xc8) overrides slots 16 (0x430570) and 241 (0x4306b0).
    Original layout: as C3DSpriteType (element vtable 0x4a8e74, 53 slots: C3DSprite's), own
    members 0x5e8..0x684, vtordisp 0x684, streamer 0x688. Members are named after their Boy
    Genius offset from the object start (the CGameObject-family methods of the original see
    them 0xc8 lower).                                                                       */
#pragma once
#include "../C3DSpriteType.h"

class C3DMovingTarget : public C3DSpriteType
{
	public:
	C3DMovingTarget();														// 0x430220
	virtual ~C3DMovingTarget();												// 0x430530

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x430570 hit by a baseball
	virtual void Process(float dt);											// SLOT 241 0x4306b0 respawn timer, glide to DestPos

	// FIELD 0x5e8 0x9c
	int				f_5e8;				// 0x5e8  = 0 (unused)
	float			m_start[4];			// 0x5ec  "StartPosX/Y/Z" (0), w = 1
	float			m_dest[4];			// 0x5fc  "DestPosX/Y/Z" (0), w = 1
	float			m_speed;			// 0x60c  "Speed" (100)
	float			m_respawn_left;		// 0x610  seconds until the hit target shows again (0)
	char			m_activate[100];	// 0x614  "ActivateObject" ("none")
	int				m_hits_required;	// 0x678  "HitsRequired" (5)
	int				m_num_points;		// 0x67c  "NumPoints" (10)
	float			m_respawn_time;		// 0x680  "RespawnTime" (15)
};

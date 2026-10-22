/*  C3DAIOmtObj ('3AIO') - an AI-driven scenery object (a car, a cart...) whose shape comes from
    an OMT database ("OmtDatabase", default "objects.omt"; 3D shape chunk "OmtIndex", default 4),
    unlit, radius "Radius". "TerrainColl" 0 makes it float clear of the terrain and other
    objects, 1 collide, -1 leaves C3DAI's setting.
    Reconstructed from Neutron.exe: constructor 0x40ae30 (factory '3AIO', 0xa08 bytes),
    destructor 0x40b020 (then ~C3DAI; deleting thunks 0x40b170 / 0x40b180 / 0x40b190 /
    0x40b1a0). Vtables 0x48f0f0 (element, C3DAI's 95 slots) / 0x48f0dc (listener) / 0x48f0a0
    (OMediaAnim) / 0x48ec50 (game object at +0xc0: 259 overridden) / 0x48ec40 (streamer at
    +0xa04). Own members 0x994..0xa00. Boy Genius's has none of Negatron's special cases
    (hazards, barrel rolls, trains, trampolines).                                            */
#pragma once
#include "../C3DAI.h"

class C3DAIOmtObj : public C3DAI
{
	public:
	C3DAIOmtObj();															// 0x40ae30
	virtual ~C3DAIOmtObj();													// 0x40b020

	// ---- CGameObject-family overrides
	virtual void v260(void);												// SLOT 259 0x40b070 shape from the database, terrain collisions

	// ---- data members (object offsets)
	char				m_omt_database[100];	// 0x994 "OmtDatabase" ("objects.omt")
	int					m_omt_index;	// 0x9f8  "OmtIndex" (4)
	int					m_terrain_coll;	// 0x9fc  "TerrainColl" (-1)
};

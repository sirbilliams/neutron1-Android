/*  C3DAIOmtObj ('3AIO') - a C3DAI whose shape comes from an OMT database ("OmtDatabase", default
    "objects.omt"; 3D shape chunk "OmtIndex", default 4), unlit. "Radius" sets the collision
    radius; "TerrainColl" -1 leaves the terrain collision as is, 0 turns it off (and hovers), 1
    on. That is all of it: Boy Genius has none of Negatron's special cases (rolling hazard,
    vehicle, train, trampoline).
    Reconstructed from Neutron.exe 0x40ae30..0x40b1a0: constructor 0x40ae30 (factory '3AIO',
    0xa08 bytes), destructor 0x40b020 (only resets the vtables, then ~C3DAI), slot 259 0x40b070
    (deleting thunks 0x40aff0 / 0x40b160 / 0x40b170 / 0x40b180).

    Original layout: as C3DAI (element vtable 0x48f0f0, 95 slots, none new; OMediaAnim vtable
    0x48f0a0 at +0x90; listener 0x48f0dc at +0x30; CLocalGameObject vtable 0x48ec50 at +0xc0,
    275 slots, none new but 259), own members 0x994..0x9fc, vtordisp 0xa00, streamer 0xa04.  */
#pragma once
#include "../C3DAI.h"

class C3DAIOmtObj : public C3DAI
{
	public:
	C3DAIOmtObj();															// 0x40ae30
	virtual ~C3DAIOmtObj();													// 0x40b020

	// ---- CGameObject-family overrides
	virtual void v260(void);												// SLOT 259 0x40b070 properties loaded: the shape from the database, terrain collision

	// ---- data members (object offsets)
	char				m_omt_database[100];	// 0x994 "OmtDatabase" ("objects.omt")
	int					m_omt_index;			// 0x9f8 "OmtIndex" (4): its 3D shape chunk
	int					m_terrain_coll;			// 0x9fc "TerrainColl" (-1)
};

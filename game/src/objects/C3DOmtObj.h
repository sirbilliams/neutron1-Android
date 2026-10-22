/*  C3DOmtObj ('3OMT') - a scenery object whose shape comes from an OMT database ("OmtDatabase",
    default "objects.omt"; 3D shape chunk "OmtIndex", default 4), unlit. With HasCollision 0 it
    neither collides nor moves.
    Reconstructed from Neutron.exe: constructor 0x434530 (factory '3OMT', 0x72c bytes),
    destructor 0x434700 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4346d0 /
    0x434830 / 0x434840 / 0x434850). Boy Genius's is the plain object: Negatron added the
    "SpecialCase" behaviours (platforms, spinners, falling tiles, resets...).

    Original layout: as C3DAnimated (element vtable 72 slots, none overridden; CLocalGameObject
    vtable at +0xc0, only slot 259 overridden), own members 0x6bc..0x724 (named by their
    Negatron offsets), vtordisp 0x724, streamer 0x728.                                       */
#pragma once
#include "../C3DAnimated.h"

class C3DOmtObj : public C3DAnimated
{
	public:
	C3DOmtObj();															// 0x434530
	virtual ~C3DOmtObj();													// 0x434700

	// ---- CGameObject-family overrides
	virtual void v260(void);												// SLOT 259 0x434750 shape from the database

	// ---- data members (object offsets)
	char				f_924[100];		// 0x6bc  "OmtDatabase" ("objects.omt")
	int					f_988;			// 0x720  "OmtIndex" (4): its 3D shape chunk
};

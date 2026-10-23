/*  C3DGeyser ('3GEY') - a geyser of level 5b: a C3DValve (Jimmy touching it is shocked) that shows
    shape 8 of "objectslevel5a.omt" (unlit, no animations) instead of the valve's model, radius
    115. Its constructor replaces the valve's five smoke puffs by nothing (they stay behind).
    Reconstructed from Neutron.exe: constructor 0x41bfd0 (factory '3GEY', 0x748 bytes),
    destructor 0x41c150 (only resets the vtables, then ~C3DValve; deleting thunks 0x41c120 /
    0x41c280 / 0x41c290 / 0x41c2a0). Game object vtable (+0xc0): 0, 7 overridden. No members of
    its own (vtordisp 0x740, streamer 0x744).                                               */
#pragma once
#include "C3DValve.h"

class C3DGeyser : public C3DValve
{
	public:
	C3DGeyser();															// 0x41bfd0
	virtual ~C3DGeyser();													// 0x41c150

	virtual void InitObject(void);											// SLOT 7   0x41c1a0 "SteamPeriod", the geyser shape
};

/*  C3DValve ('3VAL') - a steam valve ("valve.ase" HIDEFAULT, "valve.png", radius 80, 140 once
    loaded): a C3DAnimated with a "SteamPeriod" property that keeps five C3DSmokePuffs (shown
    at creation). While enabled (0x72c, 1), Jimmy touching it is shocked (-10 energy, -10 points,
    5 s) with sound 0xbc; he is remembered until the touch ends. C3DGeyser derives from it.
    Reconstructed from Neutron.exe: constructor 0x448780 (factory '3VAL', 0x748 bytes),
    destructor 0x448980 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x448950 /
    0x448c30 / 0x448c40 / 0x448c50). Element vtable: 67 (Negatron's ev69), 73 overridden (72 is
    new and empty); game object vtable (+0xc0): 0, 7, 10, 16, 17, 259 overridden. Own members
    0x6bc..0x73c, vtordisp 0x740, streamer 0x744. Slot numbers are Boy Genius's.           */
#pragma once
#include "../C3DAnimated.h"

class C3DSmokePuff;

class C3DValve : public C3DAnimated
{
	public:
	C3DValve();																// 0x448780
	virtual ~C3DValve();													// 0x448980

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 67 0x4489d0 load the animation (once)
	virtual void ev72(void) {}												// ESLOT 72 (empty)
	virtual void ev73(void);												// ESLOT 73 0x448bd0 stop the sound

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x448a30 "SteamPeriod"
	virtual void ResetObject(void);											// SLOT 10  0x448ba0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x448ab0 shocks Jimmy
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x448b20
	virtual void v260(void);												// SLOT 259 0x448b50 C3DAnimated's, then ev72 / ev73, radius 140

	// ---- data members (object offsets)
	int					m_sound;		// 0x6bc  sound channel (-1)
	C3DSmokePuff		*m_puff[5];		// 0x6c0  the puffs
	int					f_728;			// 0x728  (0)
	int					m_enabled;		// 0x72c  (1)
	float				m_steam_period;	// 0x730  "SteamPeriod" (5)
	int					f_734;			// 0x734  (0)
	int					m_puffs;		// 0x738  number of puffs (5)
	CGameObject			*m_jimmy;		// 0x73c  Jimmy while he touches it
};

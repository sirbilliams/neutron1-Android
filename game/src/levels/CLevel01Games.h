/*  The game types of the Retroville levels other than level1d (CLevel01DGame.h):
        CLevel01Game   'LEV1'  level1.gam    winter sky, level1.omt unlit, grey fog
        CLevel01AGame  'LV1A'  level1a.gam   no sky, level1a.omt lit, blue-grey fog
        CLevel01BGame  'LV1B'  level1b.gam   no sky, level1b.omt unlit, black fog
        CLevel01CGame  'LV1C'  level1c.gam   as level1b with level1c.omt
        CLevel01EGame  'LV1E'  level1e.gam   level3.omt (sic) unlit, no sky and no fog set
        CLevel01FGame  'LV1F'  level1f.gam   space sky, level1f.omt lit, transparent black fog
        CLevel01_AGame 'L1_A'  level1_a.gam  winter sky (still), level1_A.omt tinted, fog off
        CLevel01_BGame 'L1_B'  level1_b.gam  dusk sky, level1_B.omt tinted, brown fog
        CLevel01_CGame 'L1_C'  level1_c.gam  night sky (still), level1_C.omt tinted blue, grey fog
    Each overrides InitGame (ESLOT 45) only, except as noted. All are 0x68c bytes (CJimmyGame's
    layout) except CLevel01Game (0x690: one int of its own). See CLevel01DGame.h for what their
    constructors share. Reconstructed from Neutron.exe 0x455330..0x450940.                  */
#pragma once
#include "../CJimmyGame.h"

class CLevel01Game : public CJimmyGame										// vtables 0x4c5a80 / 0x4c5a6c / 0x4c561c / 0x4c560c
{
	public:
	CLevel01Game();															// 0x4503c0
	virtual ~CLevel01Game();												// 0x4504f0
	virtual void Process(float dt);											// SLOT 241 0x4508d0
	virtual void InitGame(void);											// ESLOT 45 0x450580 sky and tint by the time of day

	int			f_684;					// 0x5b8  0 (constructor; not used by the class)
};

class CLevel01AGame : public CJimmyGame										// vtables 0x4c36e0 / 0x4c36cc / 0x4c327c / 0x4c326c
{
	public:
	CLevel01AGame();														// 0x44ef30
	virtual ~CLevel01AGame();												// 0x44f050
	virtual void InitGame(void);											// ESLOT 45 0x44f0e0
};

class CLevel01BGame : public CJimmyGame										// vtables N2:0x508dc4 / N2:0x508db0 / N2:0x508934 / N2:0x508924
{
	public:
	CLevel01BGame();														// 0x450000
	virtual ~CLevel01BGame();												// 0x44f420
	virtual void InitGame(void);											// ESLOT 45 0x44f4b0
};

class CLevel01CGame : public CJimmyGame										// vtables 0x4c42c0 / 0x4c42ac / 0x4c3e5c / 0x4c3e4c
{
	public:
	CLevel01CGame();														// 0x44f630
	virtual ~CLevel01CGame();												// 0x44f750
	virtual void InitGame(void);											// ESLOT 45 0x44f7e0
};

class CLevel01EGame : public CJimmyGame										// vtables 0x4c4ea0 / 0x4c4e8c / 0x4c4a3c / 0x4c4a2c
{
	public:
	CLevel01EGame();														// 0x44fcd0
	virtual ~CLevel01EGame();												// 0x44fdf0
	virtual void InitGame(void);											// ESLOT 45 0x44fe30
};

class CLevel01FGame : public CJimmyGame										// vtables N2:0x50a744 / N2:0x50a730 / N2:0x50a2b4 / N2:0x50a2a4
{
	public:
	CLevel01FGame();														// 0x454f00
	virtual ~CLevel01FGame();												// 0x450120
	virtual void InitGame(void);											// ESLOT 45 0x450160
};

class CLevel01_AGame : public CJimmyGame									// vtables 0x4cc570 / 0x4cc55c / 0x4cc10c / 0x4cc0fc
{
	public:
	CLevel01_AGame();														// 0x455330
	virtual ~CLevel01_AGame();												// N2:0x4866a0
	virtual void Process(float dt);											// SLOT 240 0x4508d0
	virtual void InitGame(void);											// ESLOT 45 N2:0x486730
};

class CLevel01_BGame : public CJimmyGame									// vtables 0x4cb990 / 0x4cb97c / 0x4cb52c / 0x4cb51c
{
	public:
	CLevel01_BGame();														// 0x454b80
	virtual ~CLevel01_BGame();												// N2:0x486ab0
	virtual void Process(float dt);											// SLOT 240 0x4508d0
	virtual void InitGame(void);											// ESLOT 45 N2:0x486b40
};

class CLevel01_CGame : public CJimmyGame									// vtables 0x4c3cd0 / 0x4c3cbc / 0x4c386c / 0x4c385c
{
	public:
	CLevel01_CGame();														// 0x44f300
	virtual ~CLevel01_CGame();												// N2:0x486eb0
	virtual void Process(float dt);											// SLOT 240 0x4508d0
	virtual void InitGame(void);											// ESLOT 45 N2:0x486f40
};

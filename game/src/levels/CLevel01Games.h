/*  The game types of the Retroville levels other than level1d (CLevel01DGame.h):
        CLevel01Game   'LEV1'  level1.gam   sky by the time of day, level1.omt tinted, blue fog off
        CLevel01AGame  'LV1A'  level1a.gam  no sky, level1a.omt lit, blue-grey fog
        CLevel01BGame  'LV1B'  level1b.gam  no sky, level1b.omt tinted white, far plane only
        CLevel01CGame  'LV1C'  level1c.gam  as level1b with level1c.omt unlit
        CLevel01EGame  'LV1E'  level1e.gam  space sky, level1e.omt lit, far plane 120000 (as LV1D)
        CLevel01FGame  'LV1F'  level1f.gam  space sky, level1f.omt lit, black fog
    Each overrides InitGame (ESLOT 45); LEV1 also Process. All are 0x5c0 bytes (CJimmyGame's
    layout) except CLevel01Game (0x5c4: one int of its own). See CLevel01DGame.h for what their
    constructors share. They set the layers inline instead of calling a SetFog, as the other
    level game types do. Reconstructed from Neutron.exe 0x44ef30..0x450940.               */
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

class CLevel01BGame : public CJimmyGame										// vtables 0x4c3cd0 / 0x4c3cbc / 0x4c386c / 0x4c385c
{
	public:
	CLevel01BGame();														// 0x44f300
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

class CLevel01FGame : public CJimmyGame										// vtables 0x4c5490 / 0x4c547c / 0x4c502c / 0x4c501c
{
	public:
	CLevel01FGame();														// 0x450000
	virtual ~CLevel01FGame();												// 0x450120
	virtual void InitGame(void);											// ESLOT 45 0x450160
};

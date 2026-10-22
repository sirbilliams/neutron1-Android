/*  The game types of the level 5 maps:
        CLevel05Game   'LEV5'  level5.gam    level5.omt unlit, brown fog
        CLevel05AGame  'LV5A'  level5a.gam   level5a.omt unlit, sand fog
        CLevel05BGame  'LV5B'  level5b.gam   level5b.omt unlit, black fog
        CLevel05B1Game 'L5B1'  level5b1.gam  level5b1.omt unlit, black fog
        CLevel05CGame  'LV5C'  level5c.gam   downtown sky, level5c.omt unlit, light blue fog
        CLevel05FGame  'LV5F'  level5f.gam   the same InitGame as LV5C (one function in the exe)
    0x68c bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. Reconstructed from Neutron.exe 0x455700..0x455320.                           */
#pragma once
#include "../CJimmyGame.h"

class CLevel05Game : public CJimmyGame										// vtables 0x4cbf80 / 0x4cbf6c / 0x4cbb1c / 0x4cbb0c
{
	public:
	CLevel05Game();															// N2:0x48cb50
	virtual ~CLevel05Game();												// 0x455020
	virtual void InitGame(void);											// ESLOT 45 0x4550b0
};

class CLevel05AGame : public CJimmyGame										// vtables N2:0x510d44 / N2:0x510d30 / N2:0x5108b4 / N2:0x5108a4
{
	public:
	CLevel05AGame();														// 0x455700
	virtual ~CLevel05AGame();												// 0x4548d0
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0
	virtual void InitGame(void);											// ESLOT 45 0x454960
};

class CLevel05BGame : public CJimmyGame										// vtables 0x4cb990 / 0x4cb97c / 0x4cb52c / 0x4cb51c
{
	public:
	CLevel05BGame();														// N2:0x48c390
	virtual ~CLevel05BGame();												// 0x454ca0
	virtual void InitGame(void);											// ESLOT 45 0x454ce0
};

class CLevel05B1Game : public CJimmyGame									// vtables N2:0x5113a4 / N2:0x511390 / N2:0x510f14 / N2:0x510f04
{
	public:
	CLevel05B1Game();														// N2:0x48c0a0
	virtual ~CLevel05B1Game();												// N2:0x48c1c0
	virtual void InitGame(void);											// ESLOT 45 N2:0x48c200
};

class CLevel05CGame : public CJimmyGame										// vtables N2:0x512064 / N2:0x512050 / N2:0x511bd4 / N2:0x511bc4
{
	public:
	CLevel05CGame();														// N2:0x48c680
	virtual ~CLevel05CGame();												// N2:0x48c7a0
	virtual void InitGame(void);											// ESLOT 45 N2:0x48c7e0
};

class CLevel05FGame : public CJimmyGame										// vtables N2:0x5126c4 / N2:0x5126b0 / N2:0x512234 / N2:0x512224
{
	public:
	CLevel05FGame();														// N2:0x48c9b0
	virtual ~CLevel05FGame();												// N2:0x48cad0
	virtual void InitGame(void);											// ESLOT 45 N2:0x48c7e0 (CLevel05CGame's code)
};

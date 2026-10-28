/*  The game types of the level 5 maps (the Powerplant, Area 51 and the Rocket Maze):
        CLevel05Game   'LEV5'  level5.gam   sky "skylevel5", level5.omt unlit, orange fog 5000..10000
        CLevel05AGame  'LV5A'  level5a.gam  level5a.omt unlit, black fog 6000..16000
        CLevel05BGame  'LV5B'  level5b.gam  as LV5A with level5b.omt
    0x5c0 bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. They set the layers inline. Reconstructed from Neutron.exe 0x4547b0..0x455320.     */
#pragma once
#include "../CJimmyGame.h"

class CLevel05Game : public CJimmyGame										// vtables 0x4cbf80 / 0x4cbf6c / 0x4cbb1c / 0x4cbb0c
{
	public:
	CLevel05Game();														// 0x454f00
	virtual ~CLevel05Game();												// 0x455020
	virtual void InitGame(void);											// ESLOT 45 0x4550b0
};

class CLevel05AGame : public CJimmyGame										// vtables 0x4cb3a0 / 0x4cb38c / 0x4caf3c / 0x4caf2c
{
	public:
	CLevel05AGame();														// 0x4547b0
	virtual ~CLevel05AGame();												// 0x4548d0
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x454960
};

class CLevel05BGame : public CJimmyGame										// vtables 0x4cb990 / 0x4cb97c / 0x4cb52c / 0x4cb51c
{
	public:
	CLevel05BGame();														// 0x454b80
	virtual ~CLevel05BGame();												// 0x454ca0
	virtual void InitGame(void);											// ESLOT 45 0x454ce0
};

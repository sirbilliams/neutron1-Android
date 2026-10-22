/*  The game types of levels 6 and 7:
        CLevel06Game   'LEV6'  level6.gam   level6.omt unlit, dark blue fog from 2000 to 4000
        CLevel06AGame  'LV6A'  level6a.gam  level6a.omt unlit, the same fog
        CLevel07Game   'LEV7'  level7.gam   space sky, level7.omt unlit, black fog from 9000
    0x68c bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. They set the layers inline instead of calling SetFog, writing the world layer
    through both of its globals (0x509a30 and 0x509a3c, g_layer_world here).
    Reconstructed from Neutron.exe N2:0x48ce90..0x455ee0.                                     */
#pragma once
#include "../CJimmyGame.h"

class CLevel06Game : public CJimmyGame										// vtables 0x4ccb60 / 0x4ccb4c / 0x4cc6fc / 0x4cc6ec
{
	public:
	CLevel06Game();															// N2:0x48d250
	virtual ~CLevel06Game();												// 0x455820
	virtual void InitGame(void);											// ESLOT 45 0x4558b0
};

class CLevel06AGame : public CJimmyGame										// vtables 0x4cc570 / 0x4cc55c / 0x4cc10c / 0x4cc0fc
{
	public:
	CLevel06AGame();														// N2:0x48ce90
	virtual ~CLevel06AGame();												// 0x455450
	virtual void InitGame(void);											// ESLOT 45 0x4554e0
};

class CLevel07Game : public CJimmyGame										// vtables 0x4cd150 / 0x4cd13c / 0x4cccec / 0x4cccdc
{
	public:
	CLevel07Game();															// 0x455ad0
	virtual ~CLevel07Game();												// 0x455bf0
	virtual void InitGame(void);											// ESLOT 45 0x455c80
};

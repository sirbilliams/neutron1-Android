/*  The game types of the level 2 maps:
        CLevel02Game   'LEV2'  level2.gam   sky and terrain tint by the story stage (task "SCENE"):
                                            day blue sky, dusk sky, night sky (still); no fog set
        CLevel02AGame  'LV2A'  level2a.gam  no sky, level2a.omt tinted, dark grey fog and clear colour
        CLevel02BGame  'LV2B'  level2b.gam  as CLevel02Game, other tints
    0x5c0 bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. Reconstructed from Neutron.exe 0x450950..0x451720 (and 0x4513d0).            */
#pragma once
#include "../CJimmyGame.h"

class CLevel02Game : public CJimmyGame										// vtables 0x4c6c50 / 0x4c6c3c / 0x4c67ec / 0x4c67dc
{
	public:
	CLevel02Game();															// 0x451220
	virtual ~CLevel02Game();												// 0x451340
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0
	virtual void InitGame(void);											// ESLOT 45 0x4513e0
};

class CLevel02AGame : public CJimmyGame										// vtables 0x4c6070 / 0x4c605c / 0x4c5c0c / 0x4c5bfc
{
	public:
	CLevel02AGame();														// 0x450950
	virtual ~CLevel02AGame();												// 0x450a70
	virtual void InitGame(void);											// ESLOT 45 0x450ab0
};

class CLevel02BGame : public CJimmyGame										// vtables 0x4c6660 / 0x4c664c / 0x4c61fc / 0x4c61ec
{
	public:
	CLevel02BGame();														// 0x450cd0
	virtual ~CLevel02BGame();												// 0x450df0
	virtual void InitGame(void);											// ESLOT 45 0x450e30
};

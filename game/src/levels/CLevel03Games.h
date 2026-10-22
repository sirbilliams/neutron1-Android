/*  The game types of the level 3 maps (the Retroland arcade and the Powerplant streets):
        CLevel03Game   'LEV3'  level3.gam   night sky, level3.omt lit, blue fog colour, no fog
        CLevel03AGame  'LV3A'  level3a.gam  night sky, level3A.omt unlit, dark fog from 30000
        CLevel03BGame  'LV3B'  level3b.gam  night sky, level3B.omt lit, no fog
        CLevel03CGame  'LV3C'  level3c.gam  as LV3B with level3C.omt
        CLevel03DGame  'LV3D'  level3d.gam  as LV3B with level3D.omt
        CLevel03EGame  'LV3E'  level3e.gam  as LV3B with level3E.omt
    0x5c0 bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. They set the layers inline. Reconstructed from Neutron.exe 0x451730..0x452ea0.   */
#pragma once
#include "../CJimmyGame.h"

class CLevel03Game : public CJimmyGame										// vtables 0x4c8ff0 / 0x4c8fdc / 0x4c8b8c / 0x4c8b7c
{
	public:
	CLevel03Game();															// 0x452a70
	virtual ~CLevel03Game();												// 0x452b90
	virtual void InitGame(void);											// ESLOT 45 0x452c20
};

class CLevel03AGame : public CJimmyGame										// vtables 0x4c7240 / 0x4c722c / 0x4c6ddc / 0x4c6dcc
{
	public:
	CLevel03AGame();															// 0x451730
	virtual ~CLevel03AGame();												// 0x451850
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x451890
};

class CLevel03BGame : public CJimmyGame										// vtables 0x4c7830 / 0x4c781c / 0x4c73cc / 0x4c73bc
{
	public:
	CLevel03BGame();															// 0x451af0
	virtual ~CLevel03BGame();												// 0x451c10
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x451c50
};

class CLevel03CGame : public CJimmyGame										// vtables 0x4c7e20 / 0x4c7e0c / 0x4c79bc / 0x4c79ac
{
	public:
	CLevel03CGame();															// 0x451ed0
	virtual ~CLevel03CGame();												// 0x451ff0
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x452030
};

class CLevel03DGame : public CJimmyGame										// vtables 0x4c8410 / 0x4c83fc / 0x4c7fac / 0x4c7f9c
{
	public:
	CLevel03DGame();															// 0x4522b0
	virtual ~CLevel03DGame();												// 0x4523d0
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x452410
};

class CLevel03EGame : public CJimmyGame										// vtables 0x4c8a00 / 0x4c89ec / 0x4c859c / 0x4c858c
{
	public:
	CLevel03EGame();															// 0x452690
	virtual ~CLevel03EGame();												// 0x4527b0
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x4527f0
};

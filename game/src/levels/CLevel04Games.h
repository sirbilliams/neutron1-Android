/*  The game types of the level 4 maps:
        CLevel04Game   'LEV4'  level4.gam   sky by the story stage (task "SCENE"), level4.omt lit,
                                            fog colour without fog, far plane 30000
        CLevel04AGame  'LV4A'  level4a.gam  night sky, level4a.omt unlit, green fog
        CLevel04BGame  'LV4B'  level4b.gam  level4b.omt unlit, orange fog; the game type has the
                                            task name "POWER" (0x694 bytes: two members of its own)
        CLevel04CGame  'LV4C'  level4c.gam  sky by stage, level4c.omt unlit, blue fog colour, no fog
        CLevel04DGame  'LV4D'  level4d.gam  night sky, level4d.omt unlit, green fog colour, no fog
    The others are 0x68c bytes (CJimmyGame's layout). See CLevel01DGame.h for what their
    constructors share. Reconstructed from Neutron.exe 0x452eb0..0x4547a0.

    LEV4, LV4C and LV4D set the layers inline instead of calling SetFog; they write the world
    layer through both of its globals (0x509a30 and 0x509a3c, g_layer_world here).          */
#pragma once
#include "../CJimmyGame.h"

class CLevel04Game : public CJimmyGame										// vtables 0x4cadb0 / 0x4cad9c / 0x4ca94c / 0x4ca93c
{
	public:
	CLevel04Game();															// 0x4542f0
	virtual ~CLevel04Game();												// 0x454410
	virtual void InitGame(void);											// ESLOT 45 0x4544a0
};

class CLevel04AGame : public CJimmyGame										// vtables 0x4c95e0 / 0x4c95cc / 0x4c917c / 0x4c916c
{
	public:
	CLevel04AGame();														// 0x452eb0
	virtual ~CLevel04AGame();												// 0x452fd0
	virtual void InitGame(void);											// ESLOT 45 0x453060
};

class CLevel04BGame : public CJimmyGame										// vtables 0x4c9bd0 / 0x4c9bbc / 0x4c976c / 0x4c975c
{
	public:
	CLevel04BGame();														// 0x4532e0
	virtual ~CLevel04BGame();												// 0x453430
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0
	virtual void Process(float dt);											// SLOT 240 0x4508d0
	virtual void v260(void);												// SLOT 260 0x4538b0 after a level load
	virtual void v267(int a2);												// SLOT 267 N2:0x48af30 its task ("POWER") changed state
	virtual void InitGame(void);											// ESLOT 45 0x4534c0

	int				f_684;				// 0x684  0 (constructor; not used by the class)
	unsigned char	f_688;				// 0x688  0 (constructor; not used by the class)
};

class CLevel04CGame : public CJimmyGame										// vtables 0x4ca1d0 / 0x4ca1bc / 0x4c9d6c / 0x4c9d5c
{
	public:
	CLevel04CGame();														// 0x453960
	virtual ~CLevel04CGame();												// 0x453a80
	virtual void InitGame(void);											// ESLOT 45 0x453b10
};

class CLevel04DGame : public CJimmyGame										// vtables 0x4ca7c0 / 0x4ca7ac / 0x4ca35c / 0x4ca34c
{
	public:
	CLevel04DGame();														// 0x453ec0
	virtual ~CLevel04DGame();												// 0x453fe0
	virtual void InitGame(void);											// ESLOT 45 0x454070
};

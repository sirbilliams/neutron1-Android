/*  The game types of the bonus maps, virtual reality 1..8:
        CLevelVR01..08 'VR01'..'VR08'  vr01.gam..vr08.gam  vrNN.omt unlit, blue fog (inline)
    One copy of the same InitGame each (the exe repeats it per class), differing in the terrain
    database only; their pickup index bases are 3100, 3200 .. 3800. 0x5c0 bytes each
    (CJimmyGame's layout). See CLevel01DGame.h for what their constructors share.
    Reconstructed from Neutron.exe 0x455ef0..0x457ae0.                                      */
#pragma once
#include "../CJimmyGame.h"

class CLevelVR01 : public CJimmyGame										// vtables 0x4cd740 / 0x4cd72c / 0x4cd2dc / 0x4cd2cc
{
	public:
	CLevelVR01();														// 0x455ef0
	virtual ~CLevelVR01();												// 0x456010
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x456050
};

class CLevelVR02 : public CJimmyGame										// vtables 0x4cdd30 / 0x4cdd1c / 0x4cd8cc / 0x4cd8bc
{
	public:
	CLevelVR02();														// 0x456270
	virtual ~CLevelVR02();												// 0x456390
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x4563d0
};

class CLevelVR03 : public CJimmyGame										// vtables 0x4ce320 / 0x4ce30c / 0x4cdebc / 0x4cdeac
{
	public:
	CLevelVR03();														// 0x4565f0
	virtual ~CLevelVR03();												// 0x456710
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x456750
};

class CLevelVR04 : public CJimmyGame										// vtables 0x4ce910 / 0x4ce8fc / 0x4ce4ac / 0x4ce49c
{
	public:
	CLevelVR04();														// 0x456970
	virtual ~CLevelVR04();												// 0x456a90
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x456ad0
};

class CLevelVR05 : public CJimmyGame										// vtables 0x4cef00 / 0x4ceeec / 0x4cea9c / 0x4cea8c
{
	public:
	CLevelVR05();														// 0x456cf0
	virtual ~CLevelVR05();												// 0x456e10
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x456e50
};

class CLevelVR06 : public CJimmyGame										// vtables 0x4cf4f0 / 0x4cf4dc / 0x4cf08c / 0x4cf07c
{
	public:
	CLevelVR06();														// 0x457070
	virtual ~CLevelVR06();												// 0x457190
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x4571d0
};

class CLevelVR07 : public CJimmyGame										// vtables 0x4cfae0 / 0x4cfacc / 0x4cf67c / 0x4cf66c
{
	public:
	CLevelVR07();														// 0x4573f0
	virtual ~CLevelVR07();												// 0x457510
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x457550
};

class CLevelVR08 : public CJimmyGame										// vtables 0x4d00d0 / 0x4d00bc / 0x4cfc6c / 0x4cfc5c
{
	public:
	CLevelVR08();														// 0x457770
	virtual ~CLevelVR08();												// 0x457890
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0 (CGameType's)
	virtual void InitGame(void);											// ESLOT 45 0x4578d0
};

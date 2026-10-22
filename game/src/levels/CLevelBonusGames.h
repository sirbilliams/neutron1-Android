/*  The game types of the bonus maps:
        CLevelPromotion 'LPRO'  promotion.gam  the promotion screen: Bonus01.omt unlit, black fog
        CLevelVR01      'VR01'  vr01.gam       virtual reality 1: sky "skyvr01", vr01.omt unlit
        CLevelVR02      'VR02'  vr02.gam       virtual reality 2: vr02.omt unlit, blue fog (inline)
    0x68c bytes each (CJimmyGame's layout). See CLevel01DGame.h for what their constructors
    share. Reconstructed from Neutron.exe N2:0x48da20..0x4565e0.                           */
#pragma once
#include "../CJimmyGame.h"

class CLevelPromotion : public CJimmyGame									// vtables N2:0x5146a4 / N2:0x514690 / N2:0x514214 / N2:0x514204
{
	public:
	CLevelPromotion();														// N2:0x48da20
	virtual ~CLevelPromotion();												// N2:0x48db40
	virtual void InitGame(void);											// ESLOT 45 N2:0x48db80
};

class CLevelVR01 : public CJimmyGame										// vtables N2:0x514d04 / N2:0x514cf0 / N2:0x514874 / N2:0x514864
{
	public:
	CLevelVR01();															// 0x457770
	virtual ~CLevelVR01();													// 0x456010
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0
	virtual void InitGame(void);											// ESLOT 45 0x456050
};

class CLevelVR02 : public CJimmyGame										// vtables 0x4cdd30 / 0x4cdd1c / 0x4cd8cc / 0x4cd8bc
{
	public:
	CLevelVR02();															// 0x456270
	virtual ~CLevelVR02();													// 0x456390
	virtual void UnInitObject(void);										// SLOT 8   0x4513d0
	virtual void InitGame(void);											// ESLOT 45 0x4563d0
};

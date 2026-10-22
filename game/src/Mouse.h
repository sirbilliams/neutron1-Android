/*  The mouse pointer helpers of Neutron.exe (CGfx 0x476d70..0x476df0, CAweReal 0x46a460), shared.
    Defined in Input.cpp; the menu, C3DPlayer, C3DFlyingObject and CEditor use them. On the Thor the right stick moves the pointer (engine/sdl), so
    GetMouseDelta is also the right stick's camera turn.                                          */
#pragma once

namespace neg
{
void GetMousePos(short *x, short *y);		// 0x476d70  the pointer, logical window coordinates
void SetMousePos(short x, short y);			// 0x476d90
short MouseCentreX(void);					// 0x476dd0  half the viewport's width
short MouseCentreY(void);					// 0x476df0  half its height
void CenterMouse(void);						// N2:0x4b1d50  the pointer to the viewport's centre
void GetMouseDelta(short *dx, short *dy);	// 0x46a460  centre - pointer, then re-centred
}

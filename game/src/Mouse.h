/*  The mouse pointer helpers of Neutron.exe (CGfx 0x476d70..0x476df0, 0x46a400, 0x46a460), shared.
    Defined in Input.cpp; the menu, C3DPlayer, C3DFlyingObject and CEditor use them. On the Thor the right stick moves the pointer (engine/sdl), so
    GetMouseDelta is also the right stick's camera turn.                                          */
#pragma once

namespace neg
{
void GetMousePos(short *x, short *y);		// 0x476d70  the pointer, logical window coordinates
void SetMousePos(short x, short y);			// 0x476d90
short MouseCentreX(void);					// 0x476dd0  half the viewport's width
short MouseCentreY(void);					// 0x476df0  half its height
void CenterMouse(void);						// 0x476db0  the pointer to the viewport's centre
void CaptureMouse(bool capture);			// 0x46a400  capture: remember the pointer, centre it; release: put it back
void GetMouseDelta(short *dx, short *dy);	// 0x46a460  centre - pointer, then re-centred
}

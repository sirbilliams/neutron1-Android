/*  The C3DJimmy slots that the PDA menu methods (jimmy/C3DJimmy_menu.cpp) call but do not own.
    Weak defaults (a NEG_TODO each) in C3DJimmy_menu.cpp; C3DJimmy's own port defines the real
    ones (a strong definition replaces the weak one at link time).                          */
#pragma once

class C3DJimmy;

namespace neg {
void JimmyAnimEnded(C3DJimmy *j);				// element slot 65  (+0x104, C3DPlayer 0x43a900 "AnimEnded")
void JimmyStopLoopSound(C3DJimmy *j);			// element slot 82  (+0x148, 0x429a00: channel +0xa44)
void JimmySelectGadget(C3DJimmy *j, short g);	// element slot 93  (+0x174, 0x428d50)
void JimmyLeaveMenu(C3DJimmy *j);				// element slot 94  (+0x178, 0x427ff0: by g_4f0588)
short JimmyMove7e8(C3DJimmy *j);				// element +0x7e8 (short; > 2 shows the power bar again)
void JimmyHideA10(C3DJimmy *j);					// element +0xa10 (a picture hidden after a booth trip)
}

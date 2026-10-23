/*  Score and the interface element stub.
    Reconstructed from Neutron.exe (AddScore 0x42adc0; g_score 0x4f83c0, g_score_popup 0x4f83c4,
    g_score_popup_time 0x4f83c8).

    Boy Genius has no InterfaceElement / effect-queue system (that was Negatron's menu support;
    its manager, effect queue and Queue* functions were removed: nothing in Neutron.exe or the
    port called them).                                                                          */
#pragma once

class OMediaCanvasElementWrapper;

// port: only the part BillsUi.cpp reads (the help picture frame). Nothing creates these in
// Boy Genius, so canvas_elt stays NULL and the help pieces stay hidden.
struct InterfaceElt
{
	OMediaCanvasElementWrapper	*canvas_elt;
	short						 x;
	short						 y;
	InterfaceElt() : canvas_elt(0), x(0), y(0) {}
};

namespace neg {
extern InterfaceElt		g_elt_544908[14];		// port: see InterfaceElt
// port: the help pictures / current gadget the port UI (BillsUi.cpp) shows. Negatron's menu set
// them; nothing in Boy Genius does, so they stay empty (no pictures, index 0).
extern OMediaCanvasElementWrapper	*g_help_icons[14];
extern int				g_help_index;
extern InterfaceElt		*g_help_menu_elt;

// Score (shown with "%6.0d").
extern int		g_score;				// 0x4f83c0
extern int		g_score_popup;			// 0x4f83c4  points of the "+N" pop-up (0x424d10 draws it)
extern float	g_score_popup_time;		// 0x4f83c8  pop-up timer, 1.2 when points are added

// Add (or, negative, remove) points. Positive amounts start the "+N" pop-up; the score never
// goes below 0.
void AddScore(int points);				// 0x42adc0
}

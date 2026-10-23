/*  Score - see InterfaceElement.h.  Reconstructed from Neutron.exe 0x42adc0.  */

#include "InterfaceElement.h"

namespace neg {
InterfaceElt	g_elt_544908[14];		// port: see InterfaceElement.h

int		g_score = 0;					// 0x4f83c0
int		g_score_popup = 0;				// 0x4f83c4
float	g_score_popup_time = 0.0f;		// 0x4f83c8

void AddScore(int points)								// 0x42adc0
{
	g_score += points;
	if (points > 0)
	{
		g_score_popup = points;
		g_score_popup_time = 1.2f;
	}
	if (g_score < 0) g_score = 0;
}
}

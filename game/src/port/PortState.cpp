/*  port: see PortState.h. Not in the original. */
#include "PortState.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../Globals.h"

int neg::PortGameMode(void)
{
	if (!g_gametype) return 9;
	C3DJimmy *jim = Jimmy();
	if (!jim) return 1;
	if (g_camera && g_camera != static_cast<CGameObject *>(jim)) return 3;	// a cut scene camera
	if (jim->f_55f) return 4;												// the PDA is open
	return 5;
}

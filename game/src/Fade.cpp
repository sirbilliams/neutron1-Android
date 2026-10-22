/*  The screen fades - see Fade.h. Reconstructed from Neutron.exe 0x403c10. */
#include "Fade.h"
#include "gen/globals.h"
#include "CEditor.h"
#include "CGameType.h"
#include "Globals.h"
#include "C2DInGameMenu.h"
#include "OMediaViewPort.h"

void neg::StartFade(int type, float time)					// 0x403c10
{
	g_fade_type = type;
	if (time < 0.1f) time = 0.1f;							// 0x48d920
	g_fade_length = time;
	g_fade_left = time;
	g_fade_off = 0;
}

void neg::FadeTick(float dt)								// 0x403c50, the parts outside the menu
{
	if (g_menu) return;										// port: the menu's own v311 runs the fade (C2DInGameMenu_b.cpp)
	if (g_fade_off) return;
	if (g_view_restore)
	{
		g_viewport->place(g_view_restore_x, g_view_restore_y, g_view_restore_z);
		g_viewport->set_angle((short)(int)g_view_restore_ax, (short)(int)g_view_restore_ay, 0);	// 0x47f940
	}
	if (g_camera) g_camera->v176(0, 0, 0);					// vtable +0x2c4
	// (TODO(F): the fade sprites, by g_fade_type)
	g_fade_left -= dt;
	if (g_fade_left >= 0.0f) return;
	g_fade_left = 0.0f;
	switch (g_fade_type)
	{
		case 0:												// out: load, then fade in
			g_editor->m_load_pending = 1;
			StartFade(1, 1.0f);
			break;
		case 1:
			g_fade_off = 1;
			g_editor->m_load_pending = 0;
			break;
		case 2:												// out to the picture: load, then back
			g_editor->m_load_pending = 1;
			StartFade(3, 1.0f);
			break;
		case 3:
			g_fade_off = 1;
			g_editor->m_load_pending = 0;
			break;
		case 4:
		case 5:
			g_fade_off = 1;
			break;
	}
}

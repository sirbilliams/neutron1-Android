#include "Globals.h"

namespace neg {

CAweReal			*g_app = 0;
unsigned int		 g_granny = 0;
unsigned short		 g_granny_scale = 0;
OMediaWindow		*g_window = 0;
OMediaVideoEngine	*g_video = 0;
OMediaSoundEngine	*g_sound = 0;
OMediaInputEngine	*g_input_engine = 0;
CEditor				*g_editor = 0;
CGameType			*g_gametype = 0;

OMediaWorld			*g_world = 0;
OMediaLayer			*g_layer_clear = 0;
OMediaLayer			*g_layer_back2d = 0;
OMediaLayer			*g_layer_world = 0;
OMediaLayer			*g_layer_hud2d = 0;
OMediaLayer			*g_layer_top2d = 0;
OMediaViewPort		*g_viewport = 0;

unsigned short		 g_screen_w = 640;
unsigned short		 g_screen_h = 480;
unsigned short		 g_screen_depth = 16;
float				 g_world_far_clip = 15000.0f;
bool				 g_windowed = false;
bool				 g_display_ok = false;
bool				 g_minimized = false;

unsigned char		 g_keys[256];
bool				 g_cheat_keys = false;

unsigned int		*g_random_seed = 0;

}	// namespace neg

// --------------------------------------------------------------------------- port

#include "gen/globals.h"

// port: the Negatron globals that are not located in Neutron.exe yet live in a zero-filled block
// (gen/n2globals.h). Those the Negatron code expects to start non-zero get Negatron's start
// values here (they were initialised data in Neutron2.exe). TODO(BG): each goes away as it is
// found in Neutron.exe (its value then comes from the exe) or its Negatron code is replaced.
void neg::N2GlobalsInit(void)
{
	g_difficulty = 1;							// normal
	g_game_mode = 9;							// None
	g_prev_game_mode = 9;
	g_mode_before_pause = 9;
	g_has_focus = 1;
	g_enemy_time_scale = 1.0f;
	g_fade_done = 1;
	g_fade_duration = 1.0f;
	g_fade_in_time = 1.0f;
	g_fade_setting = 1;
	g_hud_picture_id = -1;
	g_queued_sfx_after = (unsigned int)-1;
	g_score_box_y = 386;
	g_teletype_rate = 12.0f;
	g_tex_scroll_on = 1;
	g_N2_5324f8 = 3;
	g_N2_532508 = 5.0f;
	g_N2_537634 = -1.0f;
	g_N2_53f27c = 9;
}

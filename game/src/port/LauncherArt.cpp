/*  port: the Android launcher's pictures - see LauncherArt.h. Not in the original.

    The launcher (android LauncherView) shows a picture for each menu option. They are the game's
    own full-screen pictures (screens.omt canvases, 640 x 480), read from the player's installed
    files here and written as PNGs into the app's files folder (NEG_LAUNCHER_ART, set by
    NegatronActivity), so the launcher can show them from its next start. Nothing of the game's art
    is in the repository. Canvases (Boy Genius's screens.omt, by id, as `neg_hdtool list` shows them):
      menu.png      the main menu page's background (the menu's BACKGROUND MAINMENU object; else 20 "menu")
      loadsave.png  239 "MenuLSBackground"  options.png  236 "OptionsBackground"
      newgame.png   149 "LoadingNeighborhood" (Retroville, where the story starts)
      loading.png   252 "loadscreen"
      inventory.png 53 "menucollectedItems2" credits.png 246 "Credits"
    Negatron wrote level-<gam>.png (each exit's "LoadScreen" picture); Boy Genius's CLoadLevel
    has no LoadScreen field (its .gam files have none) and it has no dat\menu.dat: it shows the one
    loading picture, canvas 0xf4 (C2DInGameMenu::ShowLoadingPicture 0x4073b0), so instead the game takes a
    picture of each level as the player first plays it (LauncherArtLevelLoaded: level-<gam>.bmp, at most
    640 wide, from the player's own run; none is shipped), and there is nothing
    to scan (ScanExits) and the main menu background is the PDA page's canvas (MainMenuBackground). */

#include "LauncherArt.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../CViewPort.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#include "../levels/CLoadLevel.h"
#include "OMediaCanvas.h"
#include "OMediaDataBase.h"
#include "OMediaFileStream.h"
#include "OMediaFilePath.h"

#include <SDL.h>
#include <ctype.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

extern void omf_SDLRequestArtShot(const char *path);		// engine/sdl/OMediaSDLMain.cpp

namespace
{

std::string s_dir;
std::string s_level_file;					// level-<gam>.bmp of the level being played, "" when done
int s_level_wait = 0;
bool s_env_read = false, s_fixed_done = false;
int s_scan_wait = 0;
std::set<std::string> s_done;			// files written or found this run

bool Exists(const std::string &file)
{
	if (FILE *f = fopen(file.c_str(), "rb")) { fclose(f); return true; }
	return false;
}

// Canvas `id` of screens.omt -> <dir>/<name>, unless it is there already.
void Save(int id, const std::string &name)
{
	if (id < 0 || s_done.count(name)) return;
	s_done.insert(name);
	const std::string file = s_dir + "/" + name;
	if (Exists(file)) return;
	if (!neg::g_menu || !neg::g_menu->f_4b4) return;
	OMediaCanvas *c = neg::LoadCanvasID(neg::g_menu->f_4b4, id);
	if (!c || c->get_width() < 64 || c->get_height() < 64) return;
	try
	{
		OMediaFilePath path(file.c_str());
		OMediaFileStream f;
		f.setpath(&path);
		f.open(omcfp_Write, true);
		c->png_export(f);
		f.close();
		SDL_Log("LAUNCHER ART: screens.omt canvas %d -> %s", id, file.c_str());
	}
	catch (...)
	{
		SDL_Log("LAUNCHER ART: cannot write %s", file.c_str());
	}
}

// The background of the PDA's main page (Boy Genius: page 3, canvas 0xd9 of screens.omt; it
// has no dat\menu.dat).
int MainMenuBackground()
{
	return 0xd9;
}

// The loading picture of every exit in the loaded level, as level-<gam>.png. port: Boy Genius's
// exits (CLoadLevel) have no "LoadScreen": nothing to save.
void ScanExits()
{
}

} // namespace

void neg::LauncherArtLevelLoaded(const char *gam)
{
	s_level_file.clear();
	if (!gam || !*gam) return;
	std::string n(gam);
	const size_t slash = n.find_last_of("/\\");
	if (slash != std::string::npos) n = n.substr(slash + 1);
	const size_t dot = n.rfind('.');
	if (dot != std::string::npos) n.resize(dot);
	for (size_t i = 0; i < n.size(); i++) n[i] = (char)tolower((unsigned char)n[i]);
	if (n.empty()) return;
	s_level_file = "level-" + n + ".bmp";
	s_level_wait = 150;						// let the level settle (and any start cut scene end)
}

void neg::LauncherArtTick(void)
{
	if (!s_env_read)
	{
		s_env_read = true;
		const char *d = getenv("NEG_LAUNCHER_ART");
		if (d && *d)
		{
			s_dir = d;
			mkdir(s_dir.c_str(), 0755);
		}
	}
	if (s_dir.empty() || !g_menu || !g_menu->f_4b4) return;
	// A picture of the level as Jimmy plays it (the launcher's Continue shows the newest save's level).
	if (!s_level_file.empty() && --s_level_wait <= 0)
	{
		const std::string file = s_dir + "/" + s_level_file;
		if (Exists(file)) s_level_file.clear();
		else if (g_hud_on && g_camera && g_camera->NameContains((char *)"C3DJIMMY"))
		{
			omf_SDLRequestArtShot(file.c_str());
			s_level_file.clear();
		}
		else s_level_wait = 60;
	}
	if (!s_fixed_done)
	{
		s_fixed_done = true;
		const int menu = MainMenuBackground();
		Save(menu >= 0 ? menu : 20, "menu.png");
		Save(239, "loadsave.png");
		Save(236, "options.png");
		Save(149, "newgame.png");
		Save(252, "loading.png");
		Save(53, "inventory.png");
		Save(246, "credits.png");
	}
	if (--s_scan_wait > 0) return;
	s_scan_wait = 120;							// the exits: every couple of seconds
	ScanExits();
}

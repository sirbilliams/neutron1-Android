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
    and level-<gam, lower case>.png: the loading picture (CLoadLevel "LoadScreen") of every exit
    of the levels played, so Continue can show the autosave's level.
    TODO(BG): BG's CLoadLevel has no LoadScreen field (its .gam files have none) and BG has no
    dat\menu.dat: find how Neutron.exe picks a level's loading picture (the "Loading<Area>"
    canvases 146..155) and its main menu background, then fill ScanExits / MainMenuBackground. */

#include "LauncherArt.h"
#include "../C2DInGameMenu.h"
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

namespace
{

std::string s_dir;
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
	OMediaCanvas *c = neg::LoadCanvasID(g_screens_db, id);
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

int MainMenuBackground()
{
	int id = -1;
	for (std::list<C2DInGameMenu::MenuObject *>::iterator i = C2DInGameMenu::s_objects.begin();
		 i != C2DInGameMenu::s_objects.end(); ++i)
		if (*i && (*i)->type == C2DInGameMenu::MOBJ_BACKGROUND && strcasecmp((*i)->name, "MAINMENU") == 0)
			id = (*i)->canvas_hi;
	return id;
}

// The loading picture of every exit in the loaded level, as level-<gam>.png. port: Boy Genius's
// exits (CLoadLevel) have no "LoadScreen": nothing to save.
void ScanExits()
{
}

} // namespace

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
	if (s_dir.empty() || !g_screens_db || C2DInGameMenu::s_objects.empty()) return;
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

/*  port: NEG_START, slots.txt and the menu test hooks - see StartMode.h. Not in the original.  */
#include "StartMode.h"
#include "AssetView.h"
#include "Companion.h"
#include "../C2DInGameMenu.h"
#include "../C3DJimmy.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../Globals.h"
#include "../objects/C3DStartPoint.h"
#include "../CViewPort.h"
#include "../gen/globals.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include <SDL.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <string>

std::string omf_SDLResolvePath(const std::string &in);		// engine/sdl: DOS path -> real file

namespace {

char	s_task[32] = "";			// the save to request once its level has loaded
int		s_loads = 0;				// levels loaded since start-up
int		s_test_frames = 0;

// The header of a .tsk (the fields ReadTaskFile 0x460210 reads), without touching the game's
// globals. False for a missing file.
bool ReadHeader(const char *name, int &level, int &score, int &story)
{
	OMediaFilePath fp(name, 'null', 'null');
	OMediaFileStream fs;
	fs.setpath(&fp);
	if (!fs.fileexists()) { fs.close(); return false; }
	fs.open(omcfp_Read, false, false);
	if (!fs.isopen()) return false;
	float health;
	short lives;
	fs >> level;
	fs >> health;
	fs >> lives;
	fs >> score;
	fs >> story;
	fs.close();
	return true;
}

long MTime(const char *name)
{
	struct stat st;
	return stat(omf_SDLResolvePath(name).c_str(), &st) == 0 ? (long)st.st_mtime : 0;
}

// The level file of a level id (as TaskGameLoad 0x4603f0 maps them).
const char *LevelFile(int id)
{
	static char buf[16];
	const char c[5] = { (char)(id >> 24), (char)(id >> 16), (char)(id >> 8), (char)id, 0 };
	if (c[0] == 'V' && c[1] == 'R') { snprintf(buf, sizeof buf, "VR%c%c.gam", c[2], c[3]); return buf; }
	if (c[0] == 'L' && c[1] == 'E' && c[2] == 'V') { snprintf(buf, sizeof buf, "level%c.gam", c[3]); return buf; }
	if (c[0] == 'L' && c[1] == 'V') { snprintf(buf, sizeof buf, "level%c%c.gam", c[2], (char)(c[3] | 0x20)); return buf; }
	return NULL;
}

}

const char *neg::StartModeChoose(char *level_name)
{
	const char *s = getenv("NEG_START");
	s_task[0] = 0;
	strcpy(level_name, "level1d.gam");						// the original start
	int slot = 0;
	if (s && !strcasecmp(s, "continue"))
	{
		long best = -1;
		for (int n = 1; n <= 10; n++)
		{
			char name[32];
			snprintf(name, sizeof name, "JimmyGame%d.tsk", n);
			int level, score, story;
			if (!ReadHeader(name, level, score, story)) continue;
			const long t = MTime(name);
			if (t > best) { best = t; slot = n; }
		}
	}
	else if (s && !strncasecmp(s, "load:", 5))
		slot = atoi(s + 5);
	if (slot >= 1 && slot <= 10)
	{
		char name[32];
		snprintf(name, sizeof name, "JimmyGame%d.tsk", slot);
		int level, score, story;
		const char *file;
		if (ReadHeader(name, level, score, story) && (file = LevelFile(level)) != NULL)
		{
			strcpy(level_name, file);
			strcpy(s_task, name);
		}
	}
	WriteSaveSlotList();
	WriteLevelList();										// levels.txt (AssetView.h)
	SDL_Log("NEG_START: %s -> %s%s%s", s && *s ? s : "(original)", level_name, s_task[0] ? " + " : "", s_task);
	return NULL;
}

void neg::StartModeLevelLoaded(void)
{
	++s_loads;
	if (AssetViewLevel()) s_task[0] = 0;
	if (!s_task[0] || s_loads != 1) return;
	// The saved level is only the stage for the load (TaskGameLoad has Jimmy travel to it again,
	// at his saved start point): its start sequence is not run, Jimmy plays at once (as
	// NEG_TEST_PLAY does, TestHooks.cpp).
	C3DJimmy *jim = Jimmy();
	if (!jim || !g_gametype) return;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (C3DStartPoint *sp = dynamic_cast<C3DStartPoint *>(*i)) sp->m_trigger_armed = 0;
	g_gametype->SetCamera(jim);
	SetGameMode(5);
}

void neg::StartModeFrame(void)
{
	// The save is loaded as the PDA's Load does: while Jimmy plays (TaskGameLoad reads his part
	// only when he is the player; a start-up cut scene runs first).
	if (s_task[0] && s_loads == 1 && g_hud_on && g_camera && g_camera->NameContains((char *)"C3DJIMMY"))
	{
		RequestTask(s_task);								// 0x460e70: CTaskList loads it next frame
		SDL_Log("NEG_START: loading %s", s_task);
		s_task[0] = 0;
	}
	if (s_loads > 1) s_task[0] = 0;
	// port, test-only: open a page / a box, or save a slot, a while after Jimmy plays
	static const char *menu = getenv("NEG_TEST_MENU");
	static const char *confirm = getenv("NEG_TEST_CONFIRM");
	static const char *save = getenv("NEG_TEST_SAVE");
	if (!(menu && *menu) && !(confirm && *confirm) && !(save && *save)) return;
	if (!s_loads || !g_hud_on || !g_menu) return;
	C3DJimmy *j = Jimmy();
	// TODO(BG): C3DJimmy's update calls MenuInput every frame (agent E); until then the test does
	if (menu && *menu && j && s_test_frames > 30) j->MenuInput(g_frame_time);
	if (++s_test_frames != 30) return;
	if (save && *save)
	{
		g_save_slot = atoi(save) + 5;
		SaveSlot();
		SDL_Log("NEG_TEST_SAVE: slot %d", atoi(save));
	}
	if (menu && *menu && j)
	{
		const int page = atoi(menu);
		if (page == C2DInGameMenu::MENU_SAVE) ReadSaveSlots();
		j->ActivateActiveMenu(page);
		if (page == C2DInGameMenu::MENU_MAIN || page == C2DInGameMenu::MENU_SAVE) j->f_1e4c = 3;
		SDL_Log("NEG_TEST_MENU: page %d", page);
	}
	if (confirm && *confirm)
	{
		OpenConfirm(atoi(confirm));
		SDL_Log("NEG_TEST_CONFIRM: box %d", atoi(confirm));
	}
}

void neg::WriteSaveSlotList(void)
{
	FILE *f = fopen(omf_SDLResolvePath("slots.txt").c_str(), "w");
	if (!f) return;
	fputs("# slot\tlevel id\tlevel name\tgam file\tscore\tmtime\tstory (written by the game, see game/src/port/StartMode.h)\n", f);
	for (int k = 1; k <= 10; k++)
	{
		char name[32];
		snprintf(name, sizeof name, "JimmyGame%d.tsk", k);
		int level = 0, score = 0, story = 0;
		if (!ReadHeader(name, level, score, story)) continue;
		const char id[5] = { (char)(level >> 24), (char)(level >> 16), (char)(level >> 8), (char)level, 0 };
		const char *gam = LevelFile(level);
		const char *lname = gam ? CompanionLevelName(gam) : NULL;
		fprintf(f, "%d\t%s\t%s\t%s\t%d\t%ld\t%d\n", k, id, lname ? lname : "", gam ? gam : "", score, MTime(name), story);
	}
	fclose(f);
}

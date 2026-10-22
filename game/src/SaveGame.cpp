/*  The savegame system and the task list: Neutron.exe 0x45f8f0..0x460e70 and the save-slot
    helpers 0x42aff0..0x42b180. See SaveGame.h for the .tsk format.

    The original's debug-log calls (0x472970, compiled empty in the shipped build) are kept as
    Trace(): silent unless NEG_SAVE_TRACE is set in the environment.                        */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "SaveGame.h"
#include "port/StartMode.h"
#include "port/AssetView.h"
#include "CGameObject.h"
#include "CLocalGameObject.h"
#include "CGameType.h"
#include "CEditor.h"
#include "C2DInGameMenu.h"
#include "C3DPlayer.h"
#include "InterfaceElement.h"
#include "Game.h"
#include "Globals.h"
#include "Todo.h"
#include "gen/globals.h"

#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include "OMedia3DPolygon.h"
#pragma GCC diagnostic pop

#include <SDL.h>
#include <math.h>
#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

std::string omf_SDLResolvePath(const std::string &in);		// engine/sdl: DOS path -> real file

namespace neg {
std::list<Task *>		g_tasks;			// 0x4fc5f8
std::list<TexScroll *>	g_tex_scrolls;		// N2:0x54e680
}

// The original's empty debug log.
static void Trace(const char *fmt, ...)
{
	static int on = -1;
	if (on < 0) on = getenv("NEG_SAVE_TRACE") != NULL;
	if (!on) return;
	char buf[512];
	va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
	SDL_Log("[save] %s", buf);
}

// String buffers. The file's strings are a length byte and up to 255 chars; the original reads
// them into smaller stack buffers. These are 256 so a long one cannot overrun.
enum { STR_MAX = 256 };

// --------------------------------------------------------------------------- helpers

static void DeleteFile(const char *dos_path)	// KERNEL32 DeleteFileA
{
	remove(omf_SDLResolvePath(dos_path).c_str());
}

static CLocalGameObject *Local(CGameObject *o)	{ return static_cast<CLocalGameObject *>(o); }

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it && strcasecmp((*it)->m_tag, tag) == 0) return *it;
	return NULL;
}

// The path of a task file: the (empty) folder prefix g_empty_string (0x4f81a8) + the name.
static void TaskPath(char *path, const char *name)
{
	strcpy(path, g_empty_string);
	strcat(path, name);
}

// Jimmy's strings and position that a save keeps (element +0x724, +0x774, +0x88c, +0x8f0,
// +0x9a8). TODO(BG): C3DPlayer still has Negatron's layout; these are its members for the
// same things (start point, return start point, next start point, return game, position).
static C3DPlayer *Player(CGameObject *o)		{ return o ? dynamic_cast<C3DPlayer *>(o) : NULL; }

// --------------------------------------------------------------------------- task list

void neg::AddTask(const char *name, int state)					// 0x45f8f0
{
	// ("AddItem"; the CTaskList constructor has more copies of it inlined)
	Task *t = new (std::nothrow) Task;
	if (!t)
	{
		ErrorLog("ERROR: Unable to allocate variable %s in AddItem", name);
		return;
	}
	strcpy(t->name, name);
	t->state = state;
	g_tasks.push_back(t);
}

// 0x45f990: SCENE values that have a story message (ShowStoryMessage 0x406f50).
static int SceneMessage(int state)
{
	switch (state)
	{
		case 0x23: return 1;	case 0x32: return 2;	case 0x3c: return 3;	case 0x46: return 4;
		case 0x5a: return 5;	case 100: return 6;		case 0x6c: return 7;
		case 0x8c: case 0x8d: return 8;
		case 0xa0: return 9;	case 0xa2: return 10;	case 0xa8: return 11;	case 0xb2: return 12;
		case 200: return 13;	case 0xcd: return 14;	case 0xd2: return 15;	case 0xdc: return 16;
		case 0xe6: return 17;	case 0xfa: return 18;	case 0x104: return 19;	case 0x10e: return 20;
		case 0x118: return 21;	case 0x122: return 22;	case 300: return 23;	case 0x136: return 24;
		case 0x140: return 25;	case 0x14a: return 26;	case 0x154: return 27;	case 0x15e: return 28;
		case 0x168: return 29;	case 0x17c: return 30;	case 0x186: return 31;	case 400: return 32;
		case 0x19a: return 33;	case 0x1a4: return 34;	case 0x1ae: return 35;	case 0x1cc: return 36;
		case 0x1d6: return 37;	case 0x1e0: return 38;	case 500: return 39;	case 0x1fe: return 40;
		case 0x208: return 41;	case 0x21c: return 42;	case 0x226: return 43;
	}
	return 0;
}

void neg::SetTaskState(const char *name, int state)				// 0x45f990
{
	Trace("SETTING task %s to %d", name, state);
	Trace("SETTING task %s to %d", name, state);			// (logged twice)
	if (strcasecmp("SCENE", name) == 0)
	{
		const int m = SceneMessage(state);
		if (m) ShowStoryMessage(m, 1);
	}
	for (std::list<Task *>::iterator i = g_tasks.begin(); i != g_tasks.end(); i++)
		if (strcasecmp((*i)->name, name) == 0) (*i)->state = state;	// every match, no break
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		// (no NULL test of the object; its m_task_name is an array, so the original's
		// "ERROR: invalid TaskName for %s" never shows)
		CLocalGameObject *o = Local(*i);
		if (strcasecmp(name, o->m_task_name) == 0) o->v267(state);	// slot 265 (+0x424)
	}
}

int neg::GetTaskState(const char *name)							// 0x45fea0
{
	for (std::list<Task *>::iterator i = g_tasks.begin(); i != g_tasks.end(); i++)
		if (strcasecmp((*i)->name, name) == 0) return (*i)->state;
	return 0;
}

// --------------------------------------------------------------------------- 0x45fee0

void neg::WriteTaskFile(const char *name)
{
	if (AssetViewLevel()) return;							// port: the Asset Viewer never saves
	char path[STR_MAX];
	TaskPath(path, name);
	OMediaFilePath fp(path, 'null', 'null');
	OMediaFileStream fs;
	fs.setpath(&fp);
	fs.open(omcfp_Write, true, true);
	int level = g_gametype->m_object_id;					// (element +0x490)
	fs << level;
	fs << g_health;
	fs << g_lives;
	int score = g_score;
	fs << score;
	fs << g_story_scene;
	for (int i = 0; i < 4200; i++) fs << g_object_states[i];
	for (int i = 0; i < 29; i++) fs << g_pickup_counts[2 * i];
	if (C3DPlayer *p = Player(FindObjectByTag("JIM1")))
	{
		fs << p->f_724;										// +0x724 (GO +0x664)
		fs << p->f_774;										// +0x774
		fs << p->f_88c;										// +0x88c
		fs << p->f_8f0;										// +0x8f0
		fs << p->f_9a8.x;// +0x9a8..
		fs << p->f_9a8.y;
		fs << p->f_9a8.z;
	}
	for (int i = 0; i < 9; i++)
	{
		int t = MenuGetItem(0, (short)i);
		fs << t;
	}
	for (int i = 0; i < 10; i++)
	{
		int t = MenuGetItem(1, (short)i);
		fs << t;
	}
	int ntasks = 0;
	for (std::list<Task *>::iterator i = g_tasks.begin(); i != g_tasks.end(); i++) ntasks++;
	fs << ntasks;
	for (std::list<Task *>::iterator i = g_tasks.begin(); i != g_tasks.end(); i++)
	{
		fs << (*i)->name;
		fs << (*i)->state;
	}
	fs.close();
	WriteSaveSlotList();									// port: slots.txt for the launcher (StartMode.h)
}

// --------------------------------------------------------------------------- 0x460210

bool neg::ReadTaskFile(const char *name)
{
	char path[STR_MAX];
	TaskPath(path, name);
	OMediaFilePath fp(path, 'null', 'null');
	OMediaFileStream fs;
	fs.setpath(&fp);
	if (!fs.fileexists())
	{
		Trace("WARNING file %s does not exist for TaskGameLoad", name);
		fs.close();
		return false;
	}
	fs.open(omcfp_Read, false, false);
	if (!fs.isopen())
	{
		Trace("ERROR opening file %s for TaskGameLoad", name);
		return false;
	}
	fs >> g_save_level_id;
	fs >> g_save_health;
	fs >> g_save_lives;
	fs >> g_save_score;
	fs >> g_save_scene;
	fs.close();
	return true;
}

// --------------------------------------------------------------------------- 0x4603f0

// The level file of a level id (the switch of TaskGameLoad), NULL when it has none.
static const char *LevelFileOf(int id)
{
	switch (id)
	{
		case 'LEV1': return "level1.gam";	case 'LV1A': return "level1a.gam";
		case 'LV1B': return "level1b.gam";	case 'LV1C': return "level1c.gam";
		case 'LV1D': return "level1d.gam";	case 'LV1E': return "level1e.gam";
		case 'LV1F': return "level1f.gam";
		case 'LEV2': return "level2.gam";	case 'LV2A': return "level2a.gam";
		case 'LV2B': return "level2b.gam";
		case 'LEV3': return "level3.gam";	case 'LV3A': return "level3a.gam";
		case 'LV3B': return "level3b.gam";	case 'LV3C': return "level3c.gam";
		case 'LV3D': return "level3d.gam";	case 'LV3E': return "level3e.gam";
		case 'LEV4': return "level4.gam";	case 'LV4A': return "level4a.gam";
		case 'LV4B': return "level4b.gam";	case 'LV4C': return "level4c.gam";
		case 'LV4D': return "level4d.gam";
		case 'LEV5': return "level5.gam";	case 'LV5A': return "level5a.gam";
		case 'LV5B': return "level5b.gam";
		case 'LEV6': return "level6.gam";	case 'LV6A': return "level6a.gam";
		case 'LEV7': return "level7.gam";	case 'LEV8': return "level8.gam";
		case 'VR01': return "VR01.gam";		case 'VR02': return "VR02.gam";
		case 'VR03': return "VR03.gam";		case 'VR04': return "VR04.gam";
		case 'VR05': return "VR05.gam";		case 'VR06': return "VR06.gam";
		case 'VR07': return "VR07.gam";		case 'VR08': return "VR08.gam";
	}
	return NULL;
}

bool neg::TaskGameLoad(const char *name)
{
	char path[STR_MAX];
	TaskPath(path, name);
	OMediaFilePath fp(path, 'null', 'null');
	OMediaFileStream fs;
	Trace("LoadTask 1");
	fs.setpath(&fp);
	if (!fs.fileexists())
	{
		Trace("WARNING file %s does not exist for TaskGameLoad", name);
		fs.close();
		return false;
	}
	Trace("LoadTask 2");
	fs.open(omcfp_Read, false, false);
	if (!fs.isopen())
	{
		Trace("ERROR opening file %s for TaskGameLoad", name);
		return false;
	}
	Trace("LoadTask 3");
	int level;
	fs >> level;
	fs >> g_health;
	fs >> g_lives;
	int score;
	fs >> score;
	g_score = score;
	fs >> g_story_scene;
	for (int i = 0; i < 4200; i++) fs >> g_object_states[i];
	for (int i = 0; i < 29; i++)
	{
		MenuClearItem(2, (short)i);
		fs >> g_pickup_counts[2 * i];
		if (g_pickup_counts[2 * i] > 0) MenuSetItem(2, (short)i, 1);
	}
	if (const char *file = LevelFileOf(level)) strcpy(g_editor->m_level_name, file);	// +0x74d
	if (g_camera && g_camera->NameContains((char *)"C3DJIMMY"))
	{
		C3DPlayer *p = Player(g_camera);
		char s[STR_MAX];
		fs >> s;											// +0x724 and +0x774 both get the first string
		if (p) { strcpy(p->f_724, s); strcpy(p->f_774, s); }
		fs >> s;											// (the second is read and dropped)
		fs >> s;
		if (p) strcpy(p->f_88c, s);							// +0x88c
		fs >> s;
		if (p) strcpy(p->f_8f0, s);							// +0x8f0
		float x, y, z;
		fs >> x;
		fs >> y;
		fs >> z;
		if (p)
		{
			p->f_9a8.x = x; p->f_9a8.y = y; p->f_9a8.z = z;	// +0x9a8..
			char g[STR_MAX], st[STR_MAX];
			strcpy(g, g_editor->m_level_name);
			strcpy(st, p->f_724);
			p->PlayerLoadLevel(g, st, g_editor->m_level_name, p->f_724);	// element slot 90 (+0x168)
		}
	}
	for (int i = 0; i < 9; i++)
	{
		MenuClearItem(0, (short)i);
		int t;
		fs >> t;
		MenuSetItem(0, (short)i, (short)t);
	}
	for (int i = 0; i < 10; i++)
	{
		MenuClearItem(1, (short)i);
		int t;
		fs >> t;
		MenuSetItem(1, (short)i, (short)t);
	}
	int ntasks = 0;
	fs >> ntasks;
	if (ntasks > 0)
	{
		Trace("LoadTask 4");
		for (int i = 0; i < ntasks; i++)
		{
			char tname[STR_MAX];
			fs >> tname;
			int state;
			fs >> state;
			if (state < 0) break;
			Trace("LoadTask 5  %s  %d", tname, state);
			SetTaskState(tname, state);
			for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
			{
				CLocalGameObject *o = Local(*it);
				if (strcasecmp(o->m_task_name, "none") != 0 && strcasecmp(o->m_task_name, tname) == 0)
				{
					o->v267(state);								// slot 265 (+0x424)
					Trace("TaskState %s found, state set to %d", tname, state);
				}
			}
		}
		Trace("LoadTask 6");
		for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
			if (*it) Local(*it)->v266();						// slot 264 (+0x420)
	}
	fs.close();
	return false;											// (eax is never set on this path)
}

// --------------------------------------------------------------------------- 0x460e70

void neg::RequestTask(const char *name)
{
	g_task_requested = 1;
	strcpy(g_task_request, name);
}

// --------------------------------------------------------------------------- 0x42aff0..0x42b180

// g_save_slot is the selected item of the load / save page: 6..15 = slots 1..10.
static const char *SlotFile(void)
{
	static const char *const kFiles[10] = {
		"JimmyGame1.tsk", "JimmyGame2.tsk", "JimmyGame3.tsk", "JimmyGame4.tsk", "JimmyGame5.tsk",
		"JimmyGame6.tsk", "JimmyGame7.tsk", "JimmyGame8.tsk", "JimmyGame9.tsk", "JimmyGame10.tsk" };
	if (g_save_slot < 6 || g_save_slot > 15) return NULL;
	return kFiles[g_save_slot - 6];
}

void neg::DeleteSlot(void)										// 0x42aff0
{
	if (const char *f = SlotFile())
	{
		DeleteFile(f);										// DeleteFileA
		WriteSaveSlotList();								// port: slots.txt
	}
}

void neg::SaveSlot(void)										// 0x42b0b0
{
	if (const char *f = SlotFile()) WriteTaskFile(f);
}

void neg::LoadSlot(void)										// 0x42b180
{
	if (const char *f = SlotFile()) RequestTask(f);
}

// C3DJimmy 0x428870 case 3: the ten slot headers ("JimmyGame%d.tsk", 0x4ef4a8).
void neg::ReadSaveSlots(void)
{
	C2DInGameMenu::SaveSlotInfo *slot = (C2DInGameMenu::SaveSlotInfo *)g_save_slot_info;
	for (int i = 1; i <= 10; i++, slot++)
	{
		char name[32];
		snprintf(name, sizeof name, "JimmyGame%d.tsk", i);	// sprintf
		if (!ReadTaskFile(name)) slot->score = -1;
		else
		{
			slot->score = g_save_score;
			slot->scene = g_save_scene;
		}
	}
}

// --------------------------------------------------------------------------- port: Negatron API

void neg::LoadTask(const char *name, bool new_level)
{
	if (!new_level) RequestTask(name);
	else g_editor->RequestLevel((char *)name);
}

bool neg::RestoreTask(const char *name)		{ (void)name; return false; }
void neg::SaveTask(const char *name)			{ (void)name; }

// --------------------------------------------------------------------------- texture scrolling

void neg::ClearTexScrolls(void)									// 0x45f4c0
{
	while (g_tex_scrolls.size() != 0)
	{
		if (g_tex_scrolls.front()) delete g_tex_scrolls.front();
		g_tex_scrolls.pop_front();
	}
}

void neg::UpdateTexScrolls(float dt)							// N2:0x4970e0
{
	if (!g_gametype) return;								// port: the Asset Viewer's model stage has none
	if (g_gametype->IsPaused()) return;						// vtable +0x114
	if (!g_tex_scroll_on) return;
	g_tex_scroll_time = g_tex_scroll_time + dt;
	for (std::list<TexScroll *>::iterator it = g_tex_scrolls.begin(); it != g_tex_scrolls.end(); ++it)
	{
		TexScroll *s = *it;
		if (!s || !s->polygon) continue;					// port: (the original has no checks)
		// (a copy of the vertex list is made and thrown away, as shipped)
		omt_3DPolygonVertexList copy = s->polygon->get_vertices();
		(void)copy;
		unsigned int du, dv;
		memcpy(&du, &s->du, 4);
		memcpy(&dv, &s->dv, 4);
		for (short i = 0; i < (int)s->polygon->get_vertices().size(); i++)
		{
			OMedia3DPolygonVertex &v = s->polygon->get_vertices()[i];
			if (du == TEXSCROLL_WAVE_BITS && dv == TEXSCROLL_WAVE_BITS)
			{
				// x87 (53-bit): v first (kept on the stack), then u, then v is stored
				const double t = g_tex_scroll_time;
				const double nv = ((cos(t * 3.0) + 1.0) * 0.5 - 0.5) * (double)dt * (double)0.12f + (double)v.v;	// 0x48e5a0, N2:0x516aec
				v.u = (float)(((cos(t * (double)3.2f) + 1.0) * 0.5 - 0.5) * (double)dt * (double)0.15f + (double)v.u);	// N2:0x516ae8, N2:0x4d7f7c
				v.v = (float)nv;
			}
			else
				v.v = dt * s->dv + v.v;						// (du is not used)
		}
	}
}

void neg::AddTexScroll(OMedia3DPolygon *polygon, float du, float dv)	// 0x45f610
{
	TexScroll *s = new TexScroll;
	s->polygon = polygon;
	s->du = du;
	s->dv = dv;
	g_tex_scrolls.push_back(s);
}

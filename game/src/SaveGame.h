/*  The savegame system (JimmyGame1..10.tsk, NewGame.tsk, RestartLevel.tsk in the game folder),
    the story task list and the texture scrolling of CJimmyGame: free functions of Neutron.exe
    0x45f4c0..0x460e70 and the save-slot helpers 0x42aff0..0x42b180. Reconstructed from the
    decompilation; each function names its original address.

    A .tsk file (OMediaStreamOperators, big-endian; strings are a length byte + the chars):
        int      level id           the game type's 4cc ('LV1B' ...)
        float    health             g_health
        short    lives              g_lives
        int      score              g_score
        int      story message      g_story_scene (the SCENE task's last message)
        int[4200] object states     g_object_states
        int[29]  pickup counts      g_pickup_counts[2 * i]
        Jimmy (when there is a "JIM1"): strings start point, return start point, next start
        point, return game file; float x, y, z (his position)
        int[9]   page 0 item types  (the gadgets he has)
        int[10]  page 1 item types  (the phone booth places open)
        int      task count, then per task: string name, int state
    ReadTaskFile reads the header up to the story message. There is no version or placeholder:
    the shipped NewGame.tsk is a real save (level1b.gam, Jimmy's lab) that New Game loads.
    Loading is asynchronous: RequestTask names the file, CTaskList::Process (Jimmy's task list,
    every frame) calls TaskGameLoad, which restores the state and has Jimmy go to the level.  */
#pragma once
#include <list>

class OMedia3DPolygon;

namespace neg {

// --------------------------------------------------------------------------- task list

// One task (0x68 bytes): the name objects give as "TaskName", and its state.
struct Task
{
	char	name[100];			// 0x00
	int		state;				// 0x64
};
extern std::list<Task *> g_tasks;							// 0x4fc5f8 (head 0x4fc5fc, count 0x4fc600)

void AddTask(const char *name, int state);					// 0x45f8f0 ("AddItem")
void SetTaskState(const char *name, int state);				// 0x45f990 (tasks and objects; SCENE: the story message)
int  GetTaskState(const char *name);						// 0x45fea0 (0 if unknown)

// --------------------------------------------------------------------------- savegames

void WriteTaskFile(const char *name);						// 0x45fee0
bool ReadTaskFile(const char *name);						// 0x460210 the header into g_save_level_id .. g_save_scene
bool TaskGameLoad(const char *name);						// 0x4603f0 restore it, Jimmy goes to its level
void RequestTask(const char *name);							// 0x460e70 load it next frame (CTaskList)
void DeleteSlot(void);										// 0x42aff0 JimmyGame<g_save_slot - 5>.tsk
void SaveSlot(void);										// 0x42b0b0
void LoadSlot(void);										// 0x42b180
// (C3DJimmy 0x428870 case 3, port: a function) the headers of the ten slots into g_save_slots.
void ReadSaveSlots(void);

// --------------------------------------------------------------------------- texture scrolling

// A polygon whose texture coordinates move: v by dv per second. 12 bytes in the original.
struct TexScroll
{
	OMedia3DPolygon	*polygon;
	float			du, dv;		// du is never used for a plain scroll
};
extern std::list<TexScroll *> g_tex_scrolls;				// (head 0x4fc5e4, count 0x4fc5e8)

void ClearTexScrolls(void);									// 0x45f4c0 (CJimmyGame, level end)
void UpdateTexScrolls(float dt);							// 0x45f510 (CJimmyGame::Process)
void AddTexScroll(OMedia3DPolygon *polygon, float du, float dv);	// 0x45f610 (CJimmyGame slot 368)

}

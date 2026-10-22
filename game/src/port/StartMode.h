/*  port: how the game starts (NEG_START, set by the Android launcher) and the save-slot list the
    launcher shows. Not in the original.

    Boy Genius (Neutron.exe) always starts the same way: the splash screens, then level1d.gam
    (the opening: the Yokian ships, then Retroville). Its saves are JimmyGame1..10.tsk in the game
    folder, written and read only from the PDA's load / save page (C3DJimmy MenuInput); "New
    game" there loads NewGame.tsk (Jimmy's lab). There is no autosave. Loading a save is the
    original's own path: TaskGameLoad (SaveGame.cpp) restores the state and Jimmy travels to the
    saved level; it needs Jimmy, so the port first loads the saved level, then requests the task.

    NEG_START (environment; read once at start-up; NEG_TEST_PLAY and the Asset Viewer win over it):
        (unset), menu, original, new   the original start (level1d.gam).
        continue                       the newest save (JimmyGame<n>.tsk by modification time);
                                       the original start when there is none.
        load:<n>                       save slot n (1..10, JimmyGame<n>.tsk); the original start
                                       when it is empty.

    slots.txt (in the game folder; written by the game at start-up and whenever a slot is written
    or deleted). UTF-8 text, '\n' line ends, lines starting with '#' are comments, then one line
    per saved game, TAB-separated:
        <slot>  <level id>  <level name>  <gam file>  <score>  <mtime>  <story>
    slot: 1..10; level id: the 4cc ('LEV1'); level name: the port's display name for it
    (Companion.cpp LevelNameOf; empty when none); gam file: "level1.gam"; score: integer; mtime:
    the file's modification time, Unix seconds; story: the story message number (g_story_scene).

    Test hooks (port, test-only; once Jimmy is playing): NEG_TEST_MENU=<page> opens PDA page
    0..6 (C2DInGameMenu.h), NEG_TEST_CONFIRM=<box> opens yes/no box <box>, NEG_TEST_SAVE=<n>
    writes slot n.                                                                             */
#pragma once

namespace neg {
// CEditor start-up, with the level to start (80 chars): applies NEG_START. Returns NULL (the
// original has no task to restore at start-up; a save is requested once its level has loaded).
const char *StartModeChoose(char *level_name);
// End of CEditor::LoadGame (a level has loaded).
void StartModeLevelLoaded(void);
// Every frame (CEditor::UpdateGame).
void StartModeFrame(void);
// Writes slots.txt (see above).
void WriteSaveSlotList(void);
}

/*  The screen fades of Neutron.exe: StartFade (0x403c10) arms one; C2DInGameMenu's v311
    (0x403c50) runs it every frame. Fade type 0 fades out and then loads the level the editor has
    (g_editor->m_load_pending) and fades in (type 1); 2 / 3 fade to and from a picture; 4 and 5
    fade out / in without a load. Globals (gen/globals.h): g_fade_type 0x4f8190, g_fade_length
    0x4ec490, g_fade_left 0x4f818c, g_fade_off 0x4ec494 (1 = none running).               */
#pragma once

namespace neg {
void StartFade(int type, float time);						// 0x403c10
// port: the timing and level side of C2DInGameMenu v311 (0x403c50), without its sprites, until
// the menu's own is ported (agent F); CEditor::UpdateGame runs it every frame.
void FadeTick(float dt);
}

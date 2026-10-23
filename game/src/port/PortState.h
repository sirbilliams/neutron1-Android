/*  port: the game state the port's own UI (BillsUi, Companion) needs. Not in the original.

    Negatron kept one "game mode" global (0 Starting, 1 Loading, 3 CutScene, 4 Menu, 5 Playing, 9
    None) that its menu code switched; Neutron.exe has no such variable. The port's UI still asks
    for those modes, so they are derived here from Boy Genius's own state.                     */
#pragma once

namespace neg {

// 9: no game type; 1: no player yet (a level loading); 3: a cut scene camera is the camera;
// 4: the PDA is open; 5: playing.
int PortGameMode(void);

}

/*  port: the Vox world picker - see VoxPicker.h. Negatron only: Boy Genius has no museum or Vox
    terminal, so here every entry point does nothing and the menu code runs as the original's.
    (The picker itself: sirbilliams/neutron2-android, game/src/port/VoxPicker.cpp.)            */
#include "VoxPicker.h"

void neg::VoxPickerOpened(void) {}

bool neg::VoxPickerFrame(C2DInGameMenu *) { return false; }

bool neg::VoxPickerPointer(short &, short &) { return false; }

int neg::VoxPickerCards(VoxCard *, int, int *sel)
{
	if (sel) *sel = -1;
	return 0;
}

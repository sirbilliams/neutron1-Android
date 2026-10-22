/*  C2DInGameMenu - Jimmy's 2D interface: the HUD (health and power bars, the current gadget's
    picture, the pickup pop-up, the story messages), the PDA menu pages (phone booth map,
    gadgets, pickups, main page, load / save, options, credits), the yes/no boxes, the screen
    fades and the end-of-level bonus count.
    Reconstructed from Neutron.exe: vtable 0x48d414 (316 slots: CLocalGameObject's 275 +
    275..315), constructor 0x401430, destructor 0x4029c0 (slot 0 0x4029a0 is the compiler's
    deleting destructor around it). Made by C3DJimmy's constructor and kept in Jimmy's +0xa18
    (port: g_menu). The art is omt\screens.omt.

    Input and actions are split as in the original: this class draws, hit-tests (v297) and runs
    the yes/no boxes (Process); C3DJimmy's menu methods (jimmy/C3DJimmy_menu.cpp) open / close
    the pages and act on the clicked items.

    Pages (g_menu_pages[i], built by v293):
      0 gadgets (11 items)           1 phone booth map (13) 2 pickups (29, counts drawn)
      3 the PDA main page (16)       4 load / save (15: 1..5 buttons, 6..15 the ten slots)
      5 options (volumes, 5)         6 credits (1)
    Yes/no boxes (OpenConfirm 0x406f90, g_confirm_open; g_confirm_box = what it asks):
      0..6, 10 a VR level (phone booth)   7 quit   8 save over a slot   9 load   11 new game
      12 (no action)                      13 delete a slot

    The class is written in parts (menu/C2DInGameMenu_<part>.{inc,cpp}); each part declares the
    slots it implements in its .inc with `// SLOT <n>`. Mechanical slots are generated.      */
#pragma once
#include "CLocalGameObject.h"
#include "gen/globals.h"
#include "InterfaceElement.h"		// (g_score; callers rely on it)

class OMediaCanvas;
class OMediaCanvasElement;
class OMediaDataBase;
class OMediaFileStream;

class C2DInGameMenu : public CLocalGameObject
{
	public:
	C2DInGameMenu();														// 0x401430
	virtual ~C2DInGameMenu();												// SLOT 0  0x4029a0 / 0x4029c0

#include "menu/C2DInGameMenu_types.inc"
#include "menu/C2DInGameMenu_a.inc"
#include "menu/C2DInGameMenu_b.inc"
#include "gen/C2DInGameMenu_slots.inc"
#include "gen/C2DInGameMenu_fields.inc"
};

namespace neg {
// ---- free functions of the menu code (menu/C2DInGameMenu_b.cpp)
void MenuSetItem(short page, short item, short type);		// 0x4038c0 the type of an item ("Activating Item")
void MenuToggleItem(short page, short item, short on);		// 0x403870 type 1 <-> 4
void MenuClearItem(short page, short item);					// 0x403910 type 0 ("Deactivating Item")
short MenuGetItem(short page, short item);					// 0x403950
void MenuStartFade(int mode, float time);					// 0x403c10 the screen fade (v311)
int  MenuPickupCount(int pic);								// 0x4061b0
void MenuAddPickupCount(int pic, int n);					// 0x4061c0
void AnnouncePickup(int pic, unsigned int index);			// 0x4061d0 the top-right pickup pop-up
void ShowStoryMessage(int scene, unsigned int on);			// 0x406f50 the PDA message of a SCENE value
void OpenConfirm(int box);									// 0x406f90 a yes/no box (once), with the cursor
bool ConfirmOpen(void);										// 0x407060
float MenuPulse(void);										// 0x4073a0 sin of the pulse clock (v313)
void ShowLoadingPicture(bool show);							// 0x4073b0 the bottom-right picture 0xf4

// ---- port: the Negatron (Neutron2.exe) menu API that callers not yet ported still use. None of
// these exist in Neutron.exe; they do nothing or the nearest Boy Genius equivalent. TODO(BG):
// remove once CJimmyGame, the cut scene cameras, C3DPlayer and the pickups are ported.
void SetGameMode(int mode);			// N2:0x485360: 0 Starting 1 Loading 2 Saving 3 CutScene 4 Menu 5 Playing 6 Paused 9 None
void SetLevelItems(int which);		// N2:0x40a860 (nothing)
void CutsceneLetterbox(bool on);	// N2:0x40c0f0 (the game modes only; Boy Genius has no letterbox)
void MenuInterfaceEffect(const char *name, float t);	// N2:0x405420 (nothing)
void GivePendingInvention(void);	// N2:0x409b20 (nothing)
}

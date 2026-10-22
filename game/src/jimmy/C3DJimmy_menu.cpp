/*  C3DJimmy: the PDA menu methods - opening and closing the pages (ActivateActiveMenu 0x425ef0,
    DeactivateActiveMenu 0x425b20, the Esc and phone booth openers 0x42ab60 / 0x42abb0), the
    per-frame input of an open page (MenuInput 0x427370), the main page's buttons (0x428870)
    and the phone booth trips (0x4289a0). The menu draws and hit-tests (C2DInGameMenu); these
    decide what a click does. Pages: C2DInGameMenu.h.

    Element offsets as in the original object (this = the element; its CGameObject part at
    +0xc0). Keys: g_keys[] Windows virtual keys.                                              */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMT headers)
#include "../C3DJimmy.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../Bridge.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Mouse.h"
#include "../Sound.h"
#include "../Todo.h"
#include "JimmyMenu.h"
#pragma GCC diagnostic pop

#include <SDL.h>
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- hooks (weak)

#define WEAK __attribute__((weak))
WEAK void neg::JimmyAnimEnded(C3DJimmy *j)			{ (void)j; NEG_TODO(0x43a900, "C3DJimmy slot 65 (AnimEnded)"); }
WEAK void neg::JimmyStopLoopSound(C3DJimmy *j)		{ (void)j; NEG_TODO(0x429a00, "C3DJimmy slot 82 (stop sound +0xa44)"); }
WEAK void neg::JimmySelectGadget(C3DJimmy *j, short g)	{ (void)j; (void)g; NEG_TODO(0x428d50, "C3DJimmy slot 93 (select gadget)"); }
WEAK void neg::JimmyLeaveMenu(C3DJimmy *j)			{ (void)j; NEG_TODO(0x427ff0, "C3DJimmy slot 94"); }
WEAK short neg::JimmyMove7e8(C3DJimmy *j)			{ (void)j; return 0; }
WEAK void neg::JimmyHideA10(C3DJimmy *j)			{ (void)j; }

// --------------------------------------------------------------------------- helpers

static C2DInGameMenu *Menu(void)		{ return g_menu; }	// the original: this +0xa18

// USER32 ShowCursor. port: SDL's cursor.
static void ShowCursor(bool on)			{ SDL_ShowCursor(on ? SDL_ENABLE : SDL_DISABLE); }

// g_gametype element slot 90 (CJimmyGame 0x44e450): the menu holds the game (inlined, see
// C2DInGameMenu_b.cpp).
static void GameMenuHold(bool on)
{
	g_5099ab = on ? 1 : 0;
	g_4f641c = 0;
}

// 0x46a3e0: was the key pressed? (clears it)
static bool KeyPressed(int vk)
{
	if (!g_keys[vk]) return false;
	g_keys[vk] = 0;
	return true;
}

// 0x42a730: a "menu" key: I / pad 3 or (in the free states of g_4f0588) the right button,
// Insert, Shift, Num 0 or pad 1.
static bool MenuKey(void)
{
	if (g_keys['I'] || g_joy_button3) return false;
	const short st = g_4f0588;
	if (st == -1 || st == 0 || st == 3 || st == 2 || st == 6 || st == 7)
		if (g_mouse_rdown || g_keys[0x2d] || g_keys[0x10] || g_keys[0x60] || g_joy_button1) return true;
	return false;
}

// C3DPlayer 0x42a7a0 (element slot 87): a click: the left button, Enter, Ctrl or pad 0 (not
// while 0x4f83e0).
static bool Click(void)
{
	if (g_4f83e0) return false;
	return g_mouse_down || g_keys[0x0d] || g_keys[0x11] || g_joy_button0;
}

static bool InVrLevel(void)
{
	const int id = g_gametype->m_object_id;
	return id > 'VR00' && id < 'VR09';
}

static bool SlotUsed(void)
{
	const C2DInGameMenu::SaveSlotInfo *s = (const C2DInGameMenu::SaveSlotInfo *)g_save_slot_info;
	return s[g_save_slot - 6].score != -1;
}

// --------------------------------------------------------------------------- 0x425ef0 / 0x425b20

void C3DJimmy::ActivateActiveMenu(int page)
{
	C2DInGameMenu *m = Menu();
	if (!g_hud_on || g_menu_active) return;
	CLocalGameObject::PauseAll(true);						// (GO +0x448)
	Log("Calling ActivateActiveMenu()");
	m->v305();
	JimmyAnimEnded(this);									// element +0x104
	f_1e34 = 0;
	m->v290();
	m->v279();
	m->v299((short)page);
	GameMenuHold(true);										// g_gametype +0x168
	JimmyStopLoopSound(this);								// element +0x148
	g_menu_active = 1;
	m->v301(false);
	f_55f = 1;
	if (page != 2) ShowCursor(true);
	m->f_4c0 = 1;
	m->f_4c4 = 0;
	f_13e = 0;												// GO slot 133 (+0x214, 0x471a00): +0x13e
	m->v280();
	StopAllChannels();										// 0x47d850
	g_4f8434 = 0;
	g_story_time = 20.0f;
	m->v315();
}

void C3DJimmy::DeactivateActiveMenu(void)
{
	C2DInGameMenu *m = Menu();
	if (!g_menu_active) return;
	Log("Calling DeactivateActiveMenu()");
	g_menu_active = 0;
	if (0 < g_4eefd0 || 0 < g_4eefc8) m->v301(true);
	if (g_4f0588 == 2 || JimmyMove7e8(this) > 2) m->v289();
	GameMenuHold(false);
	f_55f = 0;
	m->v279();
	if (m->f_4d4 != 2) ShowCursor(false);
	m->f_4c4 = 0;
	m->f_4c0 = 1;
	g_4f8434 = 1;
	f_13e = 1;										// GO slot 133
	g_menu_input_lock = 0;
	CLocalGameObject::PauseAll(false);
	m->v314(-1);
}

// --------------------------------------------------------------------------- 0x42ab60 / 0x42abb0

void C3DJimmy::ToggleMainMenu(void)
{
	if (!Menu()) return;
	if (g_menu_active)
	{
		DeactivateActiveMenu();
		return;
	}
	if (g_hud_on)
	{
		ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
		f_1e4c = 3;
	}
}

void C3DJimmy::OpenPhoneMenu(void)
{
	if (!Menu()) return;
	if (g_menu_active)
	{
		DeactivateActiveMenu();
		return;
	}
	if (g_4f0588 != -1)
	{
		JimmyLeaveMenu(this);								// element +0x178
		g_4f0588 = -1;
		Menu()->v288();
	}
	ActivateActiveMenu(C2DInGameMenu::MENU_PHONE);
}

// --------------------------------------------------------------------------- 0x428870

void C3DJimmy::MainMenuAction(short item)
{
	switch (item)
	{
		case 1:												// quit
			OpenConfirm(7);
			g_menu_input_lock = 1;
			return;
		case 3:												// load / save
			ActivateActiveMenu(C2DInGameMenu::MENU_SAVE);
			ReadSaveSlots();								// (inline: JimmyGame%d.tsk headers)
			f_1e25 = 1;
			return;
		case 4:												// options
			ActivateActiveMenu(C2DInGameMenu::MENU_OPTIONS);
			f_1e25 = 1;
			return;
		case 6:
			ActivateActiveMenu(C2DInGameMenu::MENU_CREDITS);
			break;
		case 12:
			ActivateActiveMenu(C2DInGameMenu::MENU_GADGETS);
			break;
		case 13:											// pickups
			ActivateActiveMenu(C2DInGameMenu::MENU_PICKUPS);
			f_1e25 = 1;
			return;
		default:
			return;
	}
	f_1e25 = 1;
}

// --------------------------------------------------------------------------- 0x4289a0

void C3DJimmy::MenuTravel(short place)
{
	static const struct { int id; const char *file; const char *start; int pic; } kPlaces[9] = {
		{ 'LV1B', "level1b.gam", "PHONEBOOTH", 0x94 },		// Jimmy's lab
		{ 'LEV1', "level1.gam", "PHONEBOOTH", 0x95 },		// Retroville
		{ 'LEV2', "level2.gam", "PHONEBOOTH", 0x99 },
		{ 'LEV4', "level4.gam", "PHONEBOOTH", 0x93 },
		{ 'LEV3', "level3.gam", "PHONEBOOTH", 0x97 },
		{ 'LV4C', "level4c.gam", "PHONEBOOTH", 0x96 },
		{ 'LEV5', "level5.gam", "PHONEBOOTH", 0x9a },
		{ 'LV5A', "level5a.gam", "phonebooth", 0x98 },
		{ 0, "level6.gam", "PHONEBOOTH", 0x92 },			// (no "already there" test)
	};
	C2DInGameMenu *m = Menu();
	if (place >= 0 && place <= 8)
	{
		if (kPlaces[place].id && g_gametype->m_object_id == kPlaces[place].id) return;
		if (m) m->v302(kPlaces[place].pic, true, 0, -1);
		char g[100], s[100], rg[100], rs[100];
		strcpy(g, kPlaces[place].file); strcpy(s, kPlaces[place].start);
		strcpy(rg, f_b89); strcpy(rs, f_9c8);				// +0x955 / +0x724 TODO(BG): Negatron's members
		n2_ev121(g, s, rg, rs, 0);								// element slot 90 (+0x168) "PlayerLoadLevel"
	}
	else if (place == 10 || place == 12)
	{
		m->v314(-1);
		m->v314(0x91);
	}
	else if (place == 11)
	{
		m->v314(-1);
		m->v314(0x90);
	}
	JimmyHideA10(this);
}

// --------------------------------------------------------------------------- 0x427370

unsigned char C3DJimmy::MenuInput(float dt)
{
	C2DInGameMenu *m = Menu();
	if (!m) return 0;
	if (!g_menu_active)
	{
		// the HUD numbers: health, lives (a while after they change), score
		if (g_confirm_shown) return 0;
		if (g_hud_on != 1) return 0;
		if (!GameIsRunning()) return 0;
		if (!g_camera) return 0;
		if (g_4f8430) return 0;
		m->v282(g_health);
		DrawText(0x28, 0x43, 'BIG', "%d", (int)g_health);	// _ftol
		if (0 < g_4f83d0)
		{
			g_4f83d0 = g_4f83d0 - dt;
			DrawText(300, 0x1e, 'BIG', "%d", (int)g_lives);
		}
		if (g_score_popup_time == 0)
		{
			if (g_story_time < 7.0f) return 0;				// 0x48e5b8
			DrawText(0x20d, (short)(m->f_4ec + 0x1c2), 'MED', "%6.0d", g_score);
			return 0;
		}
		if (g_story_time < 7.0f) return 0;
		DrawText(0x1a9, (short)(m->f_4ec + 0x1b3), 'BIG', "%6.0d", g_score);
		return 0;
	}

	if (m->v298() == C2DInGameMenu::MENU_PICKUPS)			// any click closes the pickups page
	{
		if (!MenuKey() && !Click() && !KeyPressed(0x0d) && !g_keys[0x11])
		{
			f_1e25 = 0;
			return 1;
		}
		if (!f_1e25)
		{
			if (f_1e4c != -1)
			{
				f_13e = 1;										// GO slot 133
				f_1e25 = 1;
				g_menu_input_lock = 1;
				DeactivateActiveMenu();
				ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
				f_1e4c = 3;
				return 1;
			}
			DeactivateActiveMenu();
			f_1e25 = 1;
		}
		return 1;
	}
	if (m->v298() == C2DInGameMenu::MENU_SAVE)				// the ten slots: score and story message
	{
		MenuSetItem(4, (short)g_save_slot, 2);
		const C2DInGameMenu::SaveSlotInfo *s = (const C2DInGameMenu::SaveSlotInfo *)g_save_slot_info;
		int y = 0x5f;
		for (int i = 0; i < 10; i++, y += 0x1e)
		{
			if (s[i].score == -1) continue;
			DrawText(0x19f, (short)y, 'MED', "%6d", s[i].score);
			DrawText(0x145, (short)y, 'MED', "%4.d", s[i].scene);
		}
	}
	if (m->v298() == C2DInGameMenu::MENU_OPTIONS)			// the two volumes
	{
		DrawText(g_music_volume < 10 ? 0xa0 : 0x91, 200, 'BIG', "%d", g_music_volume);
		DrawText(g_sfx_volume < 10 ? 0x1ad : 0x19e, 200, 'BIG', "%d", g_sfx_volume);
	}
	short mx, my;
	GetMousePos(&mx, &my);									// 0x476d70
	const short hit = m->v297(mx, my);
	if (hit == -1)
	{
		if (m->v298() == C2DInGameMenu::MENU_MAIN)
		{
			MenuSetItem(3, 7, 3); MenuSetItem(3, 8, 3); MenuSetItem(3, 9, 3); MenuSetItem(3, 10, 3);
			MenuSetItem(3, 14, 3); MenuSetItem(3, 15, 3); MenuSetItem(3, 16, 3);
		}
		else if (m->v298() != C2DInGameMenu::MENU_PHONE)
			return 1;
		m->v314(-1);
		return 1;
	}
	if (m->v298() == C2DInGameMenu::MENU_MAIN)
	{
		// the main page: the caption of the button under the pointer, and its sound
		MenuSetItem(3, 7, 3); MenuSetItem(3, 8, 3); MenuSetItem(3, 9, 3); MenuSetItem(3, 10, 3);
		MenuSetItem(3, 14, 3); MenuSetItem(3, 15, 3); MenuSetItem(3, 16, 3);
		switch (hit)
		{
			case 1:  MenuSetItem(3, 7, 2);  m->v314(0x95); break;
			case 2:  MenuSetItem(3, 10, 2); m->v314(0x0d); break;
			case 3:  MenuSetItem(3, 9, 2);  m->v314(0x85); break;
			case 4:  MenuSetItem(3, 8, 2);  m->v314(0x88); break;
			case 6:  MenuSetItem(3, 16, 2); m->v314(0xcc); break;
			case 11: break;
			case 12: MenuSetItem(3, 14, 2); m->v314(0x75); break;
			case 13: MenuSetItem(3, 15, 2); m->v314(200); break;
			default: m->v314(-1); break;
		}
	}
	else if (m->v298() == C2DInGameMenu::MENU_PHONE && hit == 9)
		m->v314(0xb5);

	if (!Click() && !MenuKey() && !KeyPressed(0x0d) && !g_keys[0x11])
	{
		f_1e25 = 0;
		return 1;
	}
	if (!m->v286(hit)) return 1;
	if (m->v298() == C2DInGameMenu::MENU_GADGETS && m->f_4c8 == 9)
	{
		if (GetTaskState("SCENE") > 499)
		{
			m->v314(0xb7);
			return 1;
		}
		if (f_1e25) return 1;
		f_1e25 = 1;
		if (InVrLevel()) return 1;
		if (MenuGetItem(0, 9) != 2) MenuSetItem(0, 9, 2);
		else MenuSetItem(0, 9, 3);
		return 1;
	}
	switch (m->v298())
	{
		case C2DInGameMenu::MENU_GADGETS:
			if (f_1e25) break;
			if (m->f_4c8 != 10)
			{
				DeactivateActiveMenu();
				JimmyLeaveMenu(this);
				JimmySelectGadget(this, m->f_4c8);
				MenuSetItem(0, 9, 3);
				f_13e = 1;										// GO slot 133
				f_1e25 = 1;
				return 1;
			}
			f_13e = 1;										// GO slot 133
			DeactivateActiveMenu();
			if (f_1e4c != -1)
			{
				ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
				f_1e4c = 3;
			}
			f_1e25 = 1;
			MenuSetItem(0, 9, 3);
			return 1;

		case C2DInGameMenu::MENU_PHONE:
		{
			const short item = m->f_4c8;
			if (item < 10 || item > 12)
			{
				DeactivateActiveMenu();
				MenuTravel(item);
				goto handled;
			}
			if (!f_1e25)
			{
				MenuTravel(item);
				f_1e25 = 1;
				return 1;
			}
			break;
		}

		case C2DInGameMenu::MENU_PICKUPS:
			if (f_1e25) break;
			f_13e = 1;										// GO slot 133
			f_1e25 = 1;
			g_menu_input_lock = 1;
			DeactivateActiveMenu();
			ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
			f_1e4c = 3;
			return 1;

		case C2DInGameMenu::MENU_MAIN:
			if (f_1e25) break;
			DeactivateActiveMenu();
			MainMenuAction(m->f_4c8);
			goto handled;

		case C2DInGameMenu::MENU_SAVE:
			if (f_1e25) return 1;
			switch (m->f_4c8)
			{
				case 1:										// back
					f_13e = 1;										// GO slot 133
					f_1e25 = 1;
					g_menu_input_lock = 1;
					DeactivateActiveMenu();
					if (f_1e4c != -1) ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
					goto handled;
				case 2:										// new game
					JimmyLeaveMenu(this);
					f_13e = 1;										// GO slot 133
					f_1e25 = 1;
					g_menu_input_lock = 1;
					DeactivateActiveMenu();
					JimmyLeaveMenu(this);
					OpenConfirm(11);
					return 1;
				case 3:										// load
					JimmyLeaveMenu(this);
					f_13e = 1;										// GO slot 133
					f_1e25 = 1;
					g_menu_input_lock = 1;
					DeactivateActiveMenu();
					if (SlotUsed())
					{
						JimmyLeaveMenu(this);
						OpenConfirm(9);
					}
					return 1;
				case 4:										// save (not in a VR level)
					if (InVrLevel())
					{
						m->v314(8);
						goto handled;
					}
					DeactivateActiveMenu();
					if (SlotUsed())
					{
						OpenConfirm(8);
						g_menu_input_lock = 1;
						goto handled;
					}
					SaveSlot();								// 0x42b0b0
					m->v314(0x87);
					break;
				case 5:										// delete
					DeactivateActiveMenu();
					if (SlotUsed())
					{
						OpenConfirm(13);
						m->v314(0xa5);
						g_menu_input_lock = 1;
						goto handled;
					}
					break;
				default:									// a slot: select it
					MenuSetItem(4, (short)g_save_slot, 1);
					g_save_slot = m->f_4c8;
					MenuSetItem(4, (short)g_save_slot, 2);
					f_1e25 = 1;
					return 1;
			}
			g_menu_input_lock = 1;
			goto handled;

		case C2DInGameMenu::MENU_OPTIONS:
			if (f_1e25) break;
			f_1e25 = 1;
			switch (m->f_4c8)
			{
				case 1:
					if (++g_music_volume > 9) g_music_volume = 10;
					g_695f80 = g_music_volume;
					return 1;
				case 2:
					if (--g_music_volume < 1) g_music_volume = 0;
					g_695f80 = g_music_volume;
					return 1;
				case 3:
					goto back_to_main;
				case 4:
					if (++g_sfx_volume > 9) g_sfx_volume = 10;
					return 1;
				case 5:
					if (--g_sfx_volume < 1) g_sfx_volume = 0;
					return 1;
			}
			break;

		case C2DInGameMenu::MENU_CREDITS:
			if (f_1e25) break;
		back_to_main:
			f_13e = 1;										// GO slot 133
			f_1e25 = 1;
			g_menu_input_lock = 1;
			DeactivateActiveMenu();
			ActivateActiveMenu(C2DInGameMenu::MENU_MAIN);
			f_1e4c = 3;
			return 1;
	}
	return 1;

handled:
	f_13e = 1;										// GO slot 133
	f_1e25 = 1;
	return 1;
}

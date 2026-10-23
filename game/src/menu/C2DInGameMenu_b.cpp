/*  C2DInGameMenu part b: Process (slot 241, 0x404080: the pad cursor, the pickup pop-up, the HUD
    and the yes/no boxes with their actions), the level-start slot (259), the story message
    panel (308), the screen fade (311), the end-of-level bonus count (312), the pulse (313),
    the menu's sound (314), and the free functions of the menu code. See C2DInGameMenu.h.

    Keys are Windows virtual keys in g_keys[]: Enter 0x0d, Esc 0x1b, Space 0x20, Insert 0x2d,
    Page Down 0x22, the arrows 0x25..0x28, 'I', 'N', 'O', 'Y', Num 0 (0x60); g_mouse_down /
    g_mouse_rdown the mouse buttons; g_joy_button0.. the pad.
    Constants: 0x48d914 = 0, 0x48d924 = 1, 0x48d920 = 0.1, 0x48d940 = 1.7, 0x48d944 = 80,
    0x48d948 = 0.5, 0x48d94c = -0.5, 0x48d950 = 5, 0x48d93c / 0x48d938 / 0x48d934 / 0x48d930 /
    0x48d92c / 0x48d928 = 225 / 305 / 240 / 300 / 315 / 395 (the yes / no buttons),
    0x48d968 = 380, 0x48d96c = 100, 0x48d970 = 1/0.7, 0x48d974 = 0.7, 0x48d958 = 4.0,
    0x48d960 = 1.3 (doubles), 0x4ec498 = 7 (int, the message's seconds).                    */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMT headers)
#include "../C2DInGameMenu.h"
#include "../C3DPlayer.h"
#include "../CEditor.h"
#include "../CGameType.h"
#include "../SaveGame.h"
#include "../Fade.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Mouse.h"
#include "../Sound.h"
#include "../InterfaceElement.h"
#include "../Todo.h"
#include "../JimmySlots.h"
#include "OMediaCanvasElement.h"
#include "OMediaViewPort.h"
#pragma GCC diagnostic pop

#include <SDL.h>
#include <math.h>
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

static inline void Show(OMediaCanvasElement *e)	{ e->force_show(); }
static inline void Hide(OMediaCanvasElement *e)	{ e->hide(true); }

static void SetAlpha(OMediaCanvasElement *e, float a)
{
	OMediaFARGBColor c;
	c.alpha = a; c.red = a; c.green = a; c.blue = a;
	e->set_diffuse(c);
}

// USER32 ShowCursor. port: SDL's cursor.
static void ShowCursor(bool on)		{ SDL_ShowCursor(on ? SDL_ENABLE : SDL_DISABLE); }

// 0x474070: the object whose tag ("ObjectTag") is `tag`, case-insensitive.
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it && strcasecmp((*it)->m_tag, tag) == 0) return *it;
	return NULL;
}

// 0x42adc0: points (and the "+N" pop-up).
static void AddPoints(int points)
{
	g_score += points;										// 0x4f83c0
	if (points > 0)
	{
		g_score_popup = points;								// 0x4f83c4
		g_score_popup_time = 1.2f;							// 0x4f83c8
	}
	if (g_score < 0) g_score = 0;
}

// g_gametype element slot 90 (CJimmyGame 0x44e450, +0x168): the menu holds the game.
// port: inlined (CGameType does not declare the slot yet).
static void GameMenuHold(bool on)
{
	g_5099ab = on ? 1 : 0;
	g_4f641c = 0;
}

// The player's element slot 90 (+0x168, C3DPlayer "PlayerLoadLevel"): go to `game` at start
// point `start`, coming back to its current game file / start point (+0x955 / +0x724).
// TODO(BG): Boy Genius's slot has no picture argument; C3DPlayer still has Negatron's.
static void PlayerGoTo(CGameObject *player, const char *game, const char *start)
{
	C3DPlayer *p = player ? dynamic_cast<C3DPlayer *>(player) : NULL;
	if (!p) return;
	char g[100], s[100], rg[100], rs[100];
	strcpy(g, game); strcpy(s, start); strcpy(rg, p->f_955); strcpy(rs, p->f_724);
	p->PlayerLoadLevel(g, s, rg, rs);
}

// C3DJimmy element slot 94 (0x427ff0, +0x178): put the item in hand away (C3DJimmy::DeactivateGadget).
static void JimmyAfterMenu(CGameObject *jim)
{
	JimmyElementSlot94(jim);
}

// 0x46a2d0: the application's quit flag (+0x18).
static void RequestQuit(void)		{ g_app->quit(); }

static const char *const kVrLevels[11] = {
	"VR04.gam", "VR05.gam", "VR07.gam", "VR06.gam", "VR08.gam", "VR02.gam", "VR03.gam",
	NULL, NULL, NULL, "VR01.gam" };

// --------------------------------------------------------------------------- free functions

void neg::MenuSetItem(short page, short item, short type)				// 0x4038c0
{
	if (item < 0 || item >= C2DInGameMenu::MENU_ITEMS) return;
	C2DInGameMenu::Page(page)->items[item].type = type;		// ("Activating Item %d for %d IsActive:%d")
}

void neg::MenuToggleItem(short page, short item, short on)				// 0x403870
{
	if (item < 0 || item >= C2DInGameMenu::MENU_ITEMS) return;
	short &t = C2DInGameMenu::Page(page)->items[item].type;
	if (!on) { if (t == 4) t = 1; }
	else if (t == 1) t = 4;
}

void neg::MenuClearItem(short page, short item)							// 0x403910
{
	if (item < 0 || item >= C2DInGameMenu::MENU_ITEMS) return;
	C2DInGameMenu::Page(page)->items[item].type = 0;		// ("Deactivating Item %d for %d IsActive:%d")
}

short neg::MenuGetItem(short page, short item)							// 0x403950
{
	if (item < 0 || item >= C2DInGameMenu::MENU_ITEMS) return 0;
	return C2DInGameMenu::Page(page)->items[item].type;
}

int neg::MenuPickupCount(int pic)											// 0x4061b0
{
	return g_pickup_counts[pic * 2];
}

void neg::MenuAddPickupCount(int pic, int n)								// 0x4061c0
{
	g_pickup_counts[pic * 2] += n;
}

void neg::AnnouncePickup(int pic, unsigned int index)					// 0x4061d0
{
	if (g_popup_time <= 0)
	{
		g_popup_next = -1;
		g_popup_pic = pic;
		g_popup_time = 3.0f;
		g_popup_index = index;
	}
	else if (g_popup_pic != -1)
	{
		g_popup_next = pic;
		g_pickup_counts[pic * 2]++;
		return;
	}
	g_pickup_counts[pic * 2]++;
}

void neg::ShowStoryMessage(int scene, unsigned int on)					// 0x406f50
{
	if (scene <= 0) return;
	for (int i = 0; i < 44; i++) g_story_flags[i] = 0;
	g_story_scene = scene;
	g_story_time = 0;
	g_story_flags[scene] = on & 0xff;
}

void neg::OpenConfirm(int box)											// 0x406f90
{
	if (g_confirm_open) return;
	g_confirm_open = 1;
	ShowCursor(true);
	OMediaCanvasElement *e;
	switch (box)
	{
		case 7:  e = g_box_quit; break;
		case 8:
		case 11: e = g_box_save; break;
		case 9:  e = g_box_load; break;
		case 12: e = g_box_12; break;
		case 13: e = g_box_delete; break;
		default: e = g_box_level; break;
	}
	Show(e);
	GameMenuHold(true);										// g_gametype +0x168 (1)
	g_confirm_box = box;
}

bool neg::ConfirmOpen(void)												// 0x407060
{
	return g_confirm_open != 0;
}

float neg::MenuPulse(void)												// 0x4073a0
{
	return g_pulse;
}

void neg::ShowLoadingPicture(bool show)									// 0x4073b0
{
	if (!show) C2DInGameMenu::s_loading_pic.Delete();
	else C2DInGameMenu::s_loading_pic.Show();
}

// --------------------------------------------------------------------------- 0x404080 Process

void C2DInGameMenu::Process(float dt)
{
	CGameObject::Process(dt);								// (0x473240, CGameObject's)
	const float sx = (float)(short)g_screen_w * 0.0015625f;
	const float sy = (float)(short)g_screen_h * 0.0020833334f;
	short mx, my;
	GetMousePos(&mx, &my);									// 0x476d70

	// the pad moves the pointer while the PDA or a box is open, faster the longer it is held
	if (!g_menu_active && !g_confirm_open)
	{
		f_514 = 0;
		f_518 = 0;
	}
	else
	{
		if (g_joy_x < -0.5f || g_keys[0x25]) f_514 = f_514 - dt * 80.0f;
		else if (g_joy_x > 0.5f || g_keys[0x27]) f_514 = dt * 80.0f + f_514;
		else f_514 = 0;
		if (g_joy_y < -0.5f || g_keys[0x28]) f_518 = dt * 80.0f + f_518;
		else if (g_joy_y > 0.5f || g_keys[0x26]) f_518 = f_518 - dt * 80.0f;
		else f_518 = 0;
		mx = (short)(int)((float)mx + f_514);				// _ftol
		my = (short)(int)((float)my + f_518);
		SetMousePos(mx, my);								// 0x476d90
	}
	v313(dt);
	v311(dt);

	// the pickup pop-up (top right) and its count
	bool lock_down = false;
	if (g_popup_time > 0)
	{
		g_popup_time = g_popup_time - dt;
		if (g_keys['I'] || g_joy_button3) g_popup_time = 0;
		if (g_hud_on) DrawText(540, 15, 'MED', "%d", g_pickup_counts[g_popup_pic * 2]);	// 0x468660
		MenuPage *pk = Page(MENU_PICKUPS);
		if (g_popup_next != -1)
		{
			MenuItem &it = pk->items[g_popup_pic];
			v275(it.active, it.x, it.y);
			Hide(it.active);
			g_popup_time = 3.0f;
			g_popup_pic = g_popup_next;
			g_popup_next = -1;
		}
		if (g_popup_pic != -1)
		{
			MenuItem &it = pk->items[g_popup_pic];
			v275(it.active, 540.0f, 10.0f);
			Show(it.active);
			if (g_popup_time <= 0)
			{
				v275(it.active, it.x, it.y);
				Hide(it.active);
				g_popup_time = 0;
				g_popup_pic = -1;
			}
		}
	}
	(void)lock_down;
	if (g_menu_active) v305();
	v312(dt);

	if (!g_camera)											// no player: no HUD
	{
		v305();
		Hide(g_health_bar);
		Hide(g_hud_lives);
		Hide(g_health_frame);
		for (int i = 0; i < 44; i++) v307(i, 0, false);
	}
	else
	{
		g_story_time = g_story_time + sy;					// sic: the screen's y scale per frame, not dt
		v308();
		if (g_menu_active)
		{
			Hide(g_health_bar);
			Hide(g_hud_lives);
			Hide(g_health_frame);
		}
		else if (!g_4f8430)
		{
			Show(g_health_bar);
			Show(g_hud_lives);
			Show(g_health_frame);
		}
	}

	if (!g_confirm_open) goto done;
	if (g_confirm_box != 12)
	{
		g_story_time = (float)g_4ec498 + 1.7f;
		Hide(g_story_lines[0]);
		for (int i = 0; i < 44; i++) v307(i, 0, false);
	}
	PauseAll(true);											// +0x448

	{
		const bool in_yes = (float)mx > sx * 225.0f && (float)mx < sx * 305.0f &&
							(float)my > sy * 240.0f && (float)my < sy * 300.0f;
		if (!in_yes && !g_keys['Y'])
		{
			// not on YES: the NO button's highlight; NO / 'N' / Enter there closes the box
			bool cancel = false;
			Hide(g_box_yes);
			if (((float)mx > sx * 315.0f && (float)mx < sx * 395.0f &&
				 (float)my > sy * 240.0f && (float)my < sy * 300.0f) || g_keys[0x0d])
			{
				Show(g_box_no);
				v314(0x75);
				if (g_mouse_down || g_keys[0x0d] || g_joy_button0) cancel = true;
			}
			else
			{
				Hide(g_box_no);
				v314(-1);
			}
			if (g_keys['N'] || cancel)
			{
				Hide(g_box_level); Hide(g_box_quit); Hide(g_box_save); Hide(g_box_load);
				Hide(g_box_delete); Hide(g_box_no);
				g_confirm_open = 0;
				ShowCursor(false);
				g_menu_input_lock = 1;
				GameMenuHold(false);
				PauseAll(false);
				g_story_time = 0;
				return;
			}
			if (!g_keys[0x22] && !g_mouse_rdown && !g_keys[0x2d] && !g_keys[0x10] &&
				!g_keys[0x60] && !g_keys[0x0d] && !g_joy_button0 && !g_joy_button1)
				g_menu_input_lock = 0;
			goto done;
		}

		// on YES (or 'Y')
		Show(g_box_yes);
		Hide(g_box_no);
		v314(0x75);
		if (!g_mouse_down && !g_keys['Y'] && !g_keys[0x0d] && !g_joy_button0)
		{
			if (g_keys['N'])
			{
				Hide(g_box_level); Hide(g_box_quit); Hide(g_box_save); Hide(g_box_load);
				Hide(g_box_delete); Hide(g_box_no); Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				g_menu_input_lock = 1;
				GameMenuHold(false);
				PauseAll(false);
				g_story_time = 0;
				return;
			}
			goto release;
		}
		if (g_menu_input_lock) goto done;
		switch (g_confirm_box)
		{
			case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 10:	// a VR level
				Hide(g_box_level);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				v302(0x9b, true, 0, -1);
				if (g_camera) PlayerGoTo(g_camera, kVrLevels[g_confirm_box], "PHONEBOOTH");
				break;
			case 7:													// quit
				Hide(g_box_quit);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				RequestQuit();
				break;
			case 8:													// save over the selected slot
				Hide(g_box_save);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				SaveSlot();											// 0x42b0b0
				GameMenuHold(false);
				PauseAll(false);
				v314(0x87);
				break;
			case 9:													// load the selected slot
				Hide(g_box_load);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				v302(200, true, 0, -1);
				LoadSlot();											// 0x42b180
				GameMenuHold(false);
				PauseAll(false);
				break;
			case 11:												// new game: Jimmy's lab, the start state
				Hide(g_box_save);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				v302(0xef, true, 0, -1);
				RequestTask("NewGame.tsk");							// 0x460e70
				GameMenuHold(false);
				PauseAll(false);
				break;
			case 13:												// delete the selected slot
				Hide(g_box_delete);
				Hide(g_box_yes);
				g_confirm_open = 0;
				ShowCursor(false);
				DeleteSlot();										// 0x42aff0
				GameMenuHold(false);
				PauseAll(false);
				break;
		}
	}
done:
	if (g_mouse_down) return;
release:
	if (!g_mouse_rdown && !g_keys[0x2d] && !g_keys[0x10] && !g_keys[0x60] && !g_keys[0x1b] &&
		!g_keys[0x0d] && !g_joy_button0)
		g_menu_input_lock = 0;
}

// --------------------------------------------------------------------------- slot 259

void C2DInGameMenu::v260()												// 0x4072d0
{
	Log("");												// (debug log)
	g_editor->m_load_pending = 0;							// +0x74c
	const int level = g_gametype->m_object_id;				// +0x490
	if (g_story_scene > 0 && (level < 'VR01' || level > 'VR08'))
	{
		g_story_time = 0.7f;
		v308();
		Show(g_story_lines[0]);
	}
	if (g_hud_on)
	{
		if (g_fade_mode == 4) { StartFade(5, 1.0f); return; }
		StartFade(3, 1.0f);
	}
}

// --------------------------------------------------------------------------- 0x407070

void C2DInGameMenu::v308()
{
	if (!g_keys['O'] && !g_509ac9)
		g_4f7c30 = 0;
	else if (g_4f7c30 == 0)
	{
		// 'O' (or pad button 5) shows the message again, or hides it
		g_4f7c30 = 1;
		if (g_story_time < 0 || (float)g_4ec498 <= g_story_time)
		{
			g_story_time = 0;
			goto slide_in;
		}
		g_story_time = (float)g_4ec498;
	}
	if (0.7f <= g_story_time)
	{
		if ((float)g_4ec498 <= g_story_time)
		{
			if (g_story_time < (float)g_4ec498 + 1.7f)		// slide out
			{
				for (int i = 1; i < 44; i++) v307(i, 0, false);
				v276(g_story_lines[0], 384.0f,
					 (g_story_time - (float)g_4ec498) * (1.0f / 0.7f) * 100.0f + (float)f_4ec + 380.0f,
					 0x9c, 1.0f, true);
			}
			return;
		}
		f_4e8 = 0;
		int n = 0;
		for (int i = 0; i < 44; i++)
			if (g_story_flags[i]) v307(i, n++, true);
		DrawText(0x189, (short)(f_4ec + 0x1c4), 'SMAL', "%d", g_story_scene);
		v276(g_story_lines[0], 384.0f, (float)(f_4ec + 0x17c), 0x9c, 1.0f, true);
		return;
	}
slide_in:
	if (!f_4e8)
	{
		f_4e8 = 1;
		PlayGameSoundIndex(-1, 0xd8, false);				// 0x458980
	}
	const float a = g_story_time * (1.0f / 0.7f);
	Show(g_story_lines[0]);
	v276(g_story_lines[0], 384.0f, (1.0f - a) * 100.0f + (float)f_4ec + 380.0f, 0x9c, 1.0f, true);
}

// --------------------------------------------------------------------------- 0x403c50 fades

void C2DInGameMenu::v311(float dt)
{
	if (g_hud_on) return;
	if (g_4f8430)											// the view is held at its saved position
	{
		g_viewport->setx(g_4f8418);
		g_viewport->sety(g_4f841c);
		g_viewport->setz(g_4f8420);
		g_viewport->set_anglex((omt_WAngle)(int)g_4f8428);	// +0x50..+0x54 (_ftol)
		g_viewport->set_angley((omt_WAngle)(int)g_4f842c);
		g_viewport->set_anglez(0);
	}
	if (g_camera)											// slot 177 (+0x2c4, 0x471f40): the player stands still
	{
		g_camera->f_1a4 = 0; g_camera->f_1a8 = 0; g_camera->f_1ac = 0;
	}
	switch (g_fade_mode)
	{
		case 0:
			g_story_time = 0.7f;
			v308();
			Show(g_story_lines[0]);
			// fall through
		case 1:
			Show(g_fade_quad);
			break;
		case 2:
		case 3:
			s_fade_pic.Show();
			break;
		case 4:
			Show(g_fade_quad);
			break;
	}
	g_fade_left = g_fade_left - dt;
	if (g_fade_left < 0)
	{
		g_fade_left = 0;
		switch (g_fade_mode)
		{
			case 0:											// faded out: the level loads, fade in
				g_editor->m_load_pending = 1;
				StartFade(1, 1.0f);
				DrawText(0x3c, 0x39, 'BIG', "%6.0d", g_score);
				break;
			case 1:
				g_hud_on = 1;
				g_editor->m_load_pending = 0;
				Hide(g_fade_quad);
				break;
			case 2:
				g_editor->m_load_pending = 1;
				s_pic_4f8008.Show();							// (never given a picture)
				StartFade(3, 1.0f);
				break;
			case 3:
				g_hud_on = 1;
				g_editor->m_load_pending = 0;
				s_fade_pic.Delete();
				s_pic_4f7cb8.Delete();
				break;
			case 4:
				g_hud_on = 1;
				Show(g_fade_quad);
				DrawText(0x3c, 0x39, 'BIG', "%6.0d", g_score);
				break;
			case 5:
				g_hud_on = 1;
				Hide(g_fade_quad);
				break;
		}
	}
	float a;
	switch (g_fade_mode)
	{
		case 0:
		case 4:
			a = 1.0f - g_fade_left / g_fade_time;
			DrawText(0x3c, 0x39, 'BIG', "%6.0d", g_score);
			SetAlpha(g_fade_quad, a);
			return;
		case 1:
		case 5:
			a = g_fade_left / g_fade_time;
			SetAlpha(g_fade_quad, a);
			return;
		case 2:
			a = 1.0f - g_fade_left / g_fade_time;
			s_pic_4f7cb8.Show();
			if (s_fade_pic.elt) SetAlpha(s_fade_pic.elt, a);
			return;
		case 3:
			a = g_fade_left / g_fade_time;
			if (s_fade_pic.elt) SetAlpha(s_fade_pic.elt, a);
			return;
	}
}

// --------------------------------------------------------------------------- 0x406690 bonus

// After the last level: the lives left count as 500 points each (fast-forwarded by a click or
// a key), then nine pictures (the ending), then level1f.gam; or (the first stage running out)
// RestartLevel.tsk.
void C2DInGameMenu::v312(float dt)
{
	if (!f_4f0) return;
	g_story_time = (float)g_4ec498 + 1.7f;
	if (f_508 == 0)
	{
		if (f_4f4 == 0)
		{
			GameMenuHold(true);
			f_4fc = g_lives;
			f_504 = 0;
			s_pic_4f7c98.Show();
			f_500 = f_500 - f_4fc;
			f_4f4 = (float)((double)f_4fc * 1.3 + 4.0);		// 0x48d960, 0x48d958
			return;
		}
		const float t = f_4f4;
		f_4f4 = t - dt;
		if (t - dt <= 0)
		{
			s_pic_4f7c98.Delete();
			f_4f4 = 0;
			f_504 = 0;
			f_508 = 11;
			f_4f8 = 0.1f;
			return;
		}
		if (f_4fc > 0)
		{
			const float u = f_4f8;
			f_4f8 = u - dt;
			if (u - dt < 0 || g_mouse_down || g_keys[0x1b] || g_keys[0x20] || g_keys[0x0d])
			{
				f_4f8 = 1.3f;
				f_4fc = f_4fc - 1;
				f_504 = f_504 + 500;
				AddPoints(500);
				PlayGameSoundIndex(-1, 0xf9, false);
			}
		}
		DrawText(0x7d, 0x8c, 'BIG', "%3.0d", f_500);
		DrawText(400, 0x8c, 'BIG', "%3.0d", f_4fc);
		DrawText(0x185, 0x10e, 'BIG', "%5.0d", f_504);
		DrawText(0x1a9, 0x1b3, 'BIG', "%6.0d", g_score);
		return;
	}
	if (g_menu_input_lock)
	{
		if (g_mouse_down) return;
		g_menu_input_lock = 0;
	}
	static const int kPics[9] = { 0xff, 0x100, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0xfe };
	if (f_508 >= 1 && f_508 <= 10)
	{
		if (!(0 < f_4f8)) return;
		const float t = f_4f8;
		f_4f8 = t - dt;
		if (!(t - dt <= 0 || g_mouse_down || g_keys[0x1b] || g_keys[0x20] || g_keys[0x0d])) return;
		g_menu_input_lock = 1;
		if (f_508 <= 9)
		{
			v302(kPics[f_508 - 1], true, 0, -1);
			f_508 = f_508 + 1;
			f_4f8 = 4.0f;
			return;
		}
		f_508 = 0;
		f_4f0 = 0;
		GameMenuHold(false);
		if (CGameObject *jim = FindObjectByTag("JIM1")) JimmyAfterMenu(jim);
		RequestTask("RestartLevel.tsk");					// 0x460e70
		return;
	}
	if (f_508 == 11)
	{
		if (!(0 < f_4f8)) return;
		const float t = f_4f8;
		f_4f8 = t - dt;
		if (!(t - dt <= 0)) return;
		g_menu_input_lock = 1;
		f_508 = 0;
		f_4f0 = 0;
		GameMenuHold(false);
		if (CGameObject *jim = FindObjectByTag("JIM1")) JimmyAfterMenu(jim);
		g_editor->RequestLevel((char *)"level1f.gam");		// +0x114
	}
}

// --------------------------------------------------------------------------- 313..315

void C2DInGameMenu::v313(float dt)										// 0x407370
{
	g_pulse_time = g_pulse_time + dt;
	g_pulse = (float)sin((double)(g_pulse_time) * 5.0);		// 0x48d950
}

void C2DInGameMenu::v314(int sound)										// 0x407410
{
	if (sound == -1)
	{
		f_50c = -1;
		StopChannel(f_510, false);							// 0x47d7a0
		f_510 = -1;
	}
	if (f_50c != sound)
	{
		if (IsChannelPlaying(f_510)) StopChannel(f_510, false);
		f_510 = PlaySoundIndex(-1, sound, false);			// 0x47d390
		f_50c = sound;
	}
}

void C2DInGameMenu::v315()												// 0x407490
{
	for (int i = 0; i < 44; i++) v307(i, 0, false);
}

// --------------------------------------------------------------------------- port: Negatron API

void neg::SetGameMode(int mode)
{
	if (mode == g_game_mode) return;
	g_prev_game_mode = g_game_mode;
	g_game_mode = mode;
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		if (*it) static_cast<CLocalGameObject *>(*it)->v270(g_prev_game_mode, mode);
}

void neg::SetLevelItems(int which)					{ (void)which; }
void neg::CutsceneLetterbox(bool on)				{ if (on) SetGameMode(3); else SetGameMode(5); }
void neg::MenuInterfaceEffect(const char *n, float t)	{ (void)n; (void)t; }
void neg::GivePendingInvention(void)				{}

/*  CAweReal, omt_main and start-up helpers.  Reconstructed from Neutron.exe.
    0x46a4a0 omt_main   0x469f90 CAweReal::listen_to_message   (CInput: Input.cpp)
    0x45e770 SetWindowTitle   0x45e860 CheckCD   0x46a310 LoadNewLevel */

#include "DatabaseCache.h"
#include "Game.h"
#include "CGameType.h"
#include "CMainMenu.h"
#include "CViewPort.h"
#include "Todo.h"
#include "Physics.h"
#include "gen/globals.h"
#include "CEditor.h"
#include "Sound.h"
#include "OMediaWindow.h"
#include "OMediaEventManager.h"
#include "OMediaDefMessages.h"
#include "OMediaError.h"
#include <SDL.h>
#include <string.h>
#include <stdlib.h>
#include "OMediaPeriodical.h"

using namespace neg;

// --------------------------------------------------------------------------- CAweReal

CAweReal::CAweReal() : OMediaApplication(omchint_BestInit, true)		// OMediaApplication(2, 1)
{
	SDL_Log("CAweReal()");
	memset(g_keys, 0, sizeof(g_keys));			// 0x509838, 0x40 dwords
	g_509938 = 100;							// 0x509938 (a ushort nothing reads)
	g_app = this;								// 0x509944
}

CAweReal::~CAweReal() {}

// port: WM_ACTIVATEAPP came once per change; SDL reports the focus and the background separately
// (both can come for one change), so the activate / deactivate messages are sent on changes only,
// to the application's own handler below (as the original's window procedure did; sending them to
// OMediaApplication::listen_to_message, as before, reached nothing).
static void AppActive(CAweReal *app, bool on)
{
	static bool s_active = true;
	if (on == s_active) return;
	s_active = on;
	app->listen_to_message(on ? omsg_neg_Activate : omsg_neg_DeActivate, NULL);
}

// 0x469f90. The original reacts to raw Windows messages that OMT's event filter forwards
// (WM_KEYDOWN/UP fill g_keys[VK], WM_ACTIVATEAPP restores the display, WM_USER+n = AWEMSG_*).
// On this port the filter carries SDL events, translated here to the same effects.
void CAweReal::listen_to_message(omt_Message msg, void *param)
{
	switch (msg)
	{
		// WM_ACTIVATEAPP (0x469f90): losing the focus pauses the game once, getting it back (after
		// restoring the display mode; port: SDL keeps it) un-pauses it.
		case omsg_neg_DeActivate:
			SDL_Log("omsg_DeActivate");
			if (!g_minimized)
			{
				// vtable +0x110: in Neutron.exe CJimmyGame doesn't override it, so this is
				// CGameType's 0x475a00 (it only logs). Our CJimmyGame::PauseGame is Negatron's menu
				// pause (it reaches g_menu, which doesn't exist yet at start-up: the device crash).
				if (g_gametype) g_gametype->CGameType::PauseGame(true);
				g_minimized = 1;
			}
			break;

		case omsg_neg_Activate:
			SDL_Log("omsg_Activate");
			if (g_minimized)
			{
				if (g_gametype) g_gametype->CGameType::PauseGame(false);	// vtable +0x110 (see above)
				g_minimized = 0;
			}
			break;

		case omsg_LowlevelEventFilter:
		{
			OMediaFilterLocalEvent *f = (OMediaFilterLocalEvent*)param;
			SDL_Event *e = f ? (SDL_Event*)f->local_event : NULL;
			if (!e) break;
			if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
			{
				if (!e->key.repeat) SDLKeyToVK(e->key.keysym.scancode, e->type == SDL_KEYDOWN);
				// F4 with the cheat/debug flag set: "EXIT REQUEST, CALLING QUIT (CLEAN EXIT)"
				if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_F4 && g_cheat_keys) quit();
			}
			else if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
				AppActive(this, true);
			else if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_FOCUS_LOST)
				AppActive(this, false);
			// port: suspend (engine/sdl/OMediaSDLInput.cpp; docs/controls.md): going to the background
			// (Android: screen off, lid shut, another app; desktop: minimised) deactivates the game as
			// losing the focus does, coming back activates it with no game time passed.
			else if (e->type == SDL_APP_WILLENTERBACKGROUND
					 || (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_MINIMIZED))
				AppActive(this, false);
			else if (e->type == SDL_APP_DIDENTERFOREGROUND
					 || (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESTORED))
			{
				ResetFrameTimer();				// 0x47e470: the time away is not game time
				AppActive(this, true);
			}
			else if (e->type == SDL_QUIT)
				SDL_Log("EXIT REQUEST, CALLING QUIT (CLEAN EXIT)");
			break;
		}

		case omsg_ViewPortClicked:			// -600: picks go to the editor while it is on
			if (g_editor && g_editor->m_editing) g_editor->ViewportClicked((OMediaPickRequest *)param);	// vtable +0xcc
			break;

		case omsg_Event:					// -0x65: the mouse buttons (the SDL event manager sends these, as OMT's window procedure did)
			switch (((OMediaEvent *)param)->type)
			{
				case omtet_LMouseDown: g_mouse_down = 1; break;		// 0x509834
				case omtet_LMouseUp:   g_mouse_down = 0; break;
				case omtet_RMouseDown: g_mouse_rdown = 1; break;	// 0x50993a
				case omtet_RMouseUp:   g_mouse_rdown = 0; break;
				default: break;
			}
			break;
	}
	OMediaApplication::listen_to_message(msg, param);
}

// --------------------------------------------------------------------------- helpers

void neg::SetWindowTitle(void)			// 0x45e770
{
	if (g_window) g_window->set_title("Jimmy Neutron");
}

bool neg::CheckCD(void)					// 0x45e860: "PLEASE INSERT THE JIMMY NEUTRON CDROM" / "CD REQUIRED"
{
	return true;						// no CD on this port
}


// 0x4736c0: every object is un-initialised, then loses its databases, then is deleted (each
// destructor takes itself off g_objects). The running game type is one of them; g_gametype is left
// pointing at it until the next game type is made.
void neg::DeleteAllObjects(void)
{
	SDL_Log("+DeleteAllObjects()");
	// "+UnInitObjects()"
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		(*i)->UnInitObject();				// vtable +0x20
	// "-UnInitObjects()"  "+UnInitObjectDatabases()"
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		(*i)->UnInitDatabases();			// vtable +0x34
	// "-UnInitObjectDatabases()"
	while (g_objects.size() != 0)
		if (g_objects.front()) delete g_objects.front();	// (a NULL entry would loop for ever, as in the original)
	SDL_Log("-DeleteAllObjects()");
}

// 0x46a310 (called by omt_main and by CAweReal::listen_to_message for AWEMSG_LOAD, 0x469f90).
void neg::LoadNewLevel(unsigned int which)
{
	SDL_Log("+LoadNewLevel(%c%c%c%c)", (which >> 24) & 0xff, (which >> 16) & 0xff, (which >> 8) & 0xff, which & 0xff);
	CGfx::UnInitCaptions();				// 0x4766e0
	PhysicsReset();						// 0x479000
	DeleteAllObjects();					// 0x4736c0
	CGfx::UnInitAlpha();				// 0x4685f0 = UnInitAlpha + InitAlpha
	CGfx::InitAlpha();
	ClearWayPoints();					// 0x47ec30
	CreateGameType(which);				// 0x44d0e0: a CMainMenu, whatever the id
	ResetFrameTimer();					// 0x47e470
	SDL_Log("-LoadNewLevel()");
}

// Windows virtual-key codes for the keys the game reads (keys.txt)
void neg::SDLKeyToVK(int sc, bool down)
{
	int vk = 0;
	if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) vk = 'A' + (sc - SDL_SCANCODE_A);
	else if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9) vk = '1' + (sc - SDL_SCANCODE_1);
	else if (sc == SDL_SCANCODE_0) vk = '0';
	else if (sc >= SDL_SCANCODE_F1 && sc <= SDL_SCANCODE_F12) vk = 0x70 + (sc - SDL_SCANCODE_F1);
	else switch (sc)
	{
		case SDL_SCANCODE_RETURN: case SDL_SCANCODE_KP_ENTER: vk = 0x0d; break;	// VK_RETURN
		case SDL_SCANCODE_ESCAPE: case SDL_SCANCODE_AC_BACK:  vk = 0x1b; break;	// VK_ESCAPE
		case SDL_SCANCODE_SPACE:  vk = 0x20; break;
		case SDL_SCANCODE_LEFT:   vk = 0x25; break;
		case SDL_SCANCODE_UP:     vk = 0x26; break;
		case SDL_SCANCODE_RIGHT:  vk = 0x27; break;
		case SDL_SCANCODE_DOWN:   vk = 0x28; break;
		case SDL_SCANCODE_LSHIFT: case SDL_SCANCODE_RSHIFT: vk = 0x10; break;
		case SDL_SCANCODE_LCTRL:  case SDL_SCANCODE_RCTRL:  vk = 0x11; break;
		case SDL_SCANCODE_LALT:   case SDL_SCANCODE_RALT:   vk = 0x12; break;
		case SDL_SCANCODE_TAB:    vk = 0x09; break;
		case SDL_SCANCODE_BACKSPACE: vk = 0x08; break;
		default: break;
	}
	if (vk) g_keys[vk] = down ? 1 : 0;
}

// --------------------------------------------------------------------------- port test hook
// NEG_TEXT_TEST=1 draws a line in every font each frame (until the menu is reconstructed).
class CTextTest : public OMediaPeriodical
{
	public:
	virtual void spend_time(void)
	{
		ClearText();
		DrawText(-1, 40, 'BIG', "JIMMY NEUTRON");
		DrawText(-1, 110, 'LARG', "VS JIMMY NEGATRON");
		DrawText(40, 200, 0, "DEFAULT 0123456789");
		DrawText(40, 250, 'SMAL', "SMALL: HELLO, RETROVILLE!");
		DrawText(40, 300, 'MED', "MEDIUM 0123456789");
		DrawText(40, 360, 'TIME', "TIME 12345");
		g_text_center = 1;
		DrawText(320, 430, 'SMAL', "CENTRED AT X=320");
		g_text_center = 0;
	}
};

// --------------------------------------------------------------------------- 0x46a4a0 omt_main

void omt_main(void)
{
	// Not in the original (Windows loaded the exe's data for it): the data section's initial
	// values come from the player's own Neutron.exe. See ExeData.cpp.
	{
		char why[512];
		if (!neg::LoadExeData(why, sizeof why))
		{
			SDL_Log("ExeData: %s", why);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Jimmy Neutron vs. Jimmy Negatron", why, NULL);
			return;
		}
	}
	// (Windows: refuses to start twice - FindWindow("OMT SingleWindow"), "A game is already in progress")
	try
	{
		CAweReal *app = new CAweReal;
		CGfx *gfx = new CGfx;					// 0x475ee0 (creates window, world, layers)
		SetWindowTitle();						// 0x45e770
		OMediaEventManager *em = OMediaEventManager::get_event_manager();
		if (em)
		{
			em->get_msg_filter_broadcaster()->addlistener(app);	// event_manager +0x2c
			em->getkeybroadcaster()->addlistener(app);				// event_manager +0x1c
		}
		if (!CheckCD()) { delete gfx; delete app; return; }

		// (**(window vtable + 0x50))(): show
		g_window->show();
		InitSoundEngine();						// 0x47cf60
		if (g_windowed) { g_screen_w = 400; g_screen_h = 300; }
		CGfx::SetDisplayMode(g_screen_w, g_screen_h, g_screen_depth, g_windowed);	// 0x476950
		CInput *input = new CInput;				// 0x478200

		g_editor = new CEditor;					// 0x46b160 (0x82c bytes): level loader / editor; sets g_editor itself
		// (the original runs the rest only when g_editor was created, and the main loop only when
		// LoadNewLevel produced a game type - not reconstructed yet, so the loop runs regardless)
		LoadNewLevel('MAIN');
		SDL_Log("omtmain 1");

		CTextTest *text_test = getenv("NEG_TEXT_TEST") ? new CTextTest : NULL;
		if (g_gametype)
		{
			SDL_Log("omtmain 2: entering main loop");
			app->start();						// OMediaApplication::start (vtable +0x10)
		}
		SDL_Log("omtmain 3");
		DeleteAllObjects();						// 0x4736c0
		CGfx::UnInitAlpha();					// 0x468510

		SDL_Log("EXITING GAME");
		neg::CloseAllDatabases();				// 0x46b000
		neg::CloseAllPermanentDatabases();		// port (Neutron.exe leaves them to the process end)
		delete text_test;
		if (g_editor)
		{
			delete g_editor;
			g_editor = NULL;
		}
		delete input;
		delete gfx;
		delete app;
	}
	catch (OMediaError &e)
	{
		SDL_Log("OMT error %d (data %d) at %s:%d", (int)e.errorcode, (int)e.errordata, e.file.c_str(), e.line);
	}
	SDL_Log("CLEAN EXIT");
}

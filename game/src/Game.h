/*  Negatron - reconstructed classes of Jimmy Neutron vs. Jimmy Negatron (Neutron.exe).
    Each method names the address it was reconstructed from. Unfinished parts say TODO(address). */
#pragma once
#include "OMediaApplication.h"
#include "OMediaListener.h"
#include "Globals.h"
#include <string>

// --------------------------------------------------------------------------- CAweReal (0x38 bytes)
// The application. vtables 0x4d5e60 (OMediaApplication) and 0x4d5e4c (OMediaListener at +0x24).
class CAweReal : public OMediaApplication, public OMediaListener
{
	public:
	CAweReal();												// inline in omt_main 0x46a4a0
	virtual ~CAweReal();									// 0x469f10
	virtual void listen_to_message(omt_Message msg, void *param = NULL);	// 0x469f90
};

// --------------------------------------------------------------------------- CGfx (4 bytes)
class CGfx
{
	public:
	CGfx();													// 0x475ee0
	virtual ~CGfx();

	void InitDisplay(void);									// 0x4762b0
	void InitWorld(void);									// 0x4762b0 (second half)
	static void InitLayers(void);							// 0x4767c0
	static void InitViewPort(void);							// 0x476490
	static void SetDisplayMode(int w, int h, int depth, bool windowed);	// 0x476950
	static void InitFont(void), UnInitFont(void);
	static void InitCaptions(void), UnInitCaptions(void);	// 0x4766e0
	static void InitAlpha(void), UnInitAlpha(void);			// 0x4683e0 / 0x468510
};

// --------------------------------------------------------------------------- CInput (4 bytes)
// vtable 0x4d69f8. Made once by omt_main; owns g_input_engine and the joystick FindDevice locked
// (Input.cpp). Slots 1 and 2 are never called.
class CInput
{
	public:
	CInput();												// 0x478200
	virtual ~CInput();										// N2:0x4b3370 (slot 0, deleting destructor)
	virtual void CreateInputEngine(void);					// 0x478350 (slot 1)
	virtual void Release(void);								// 0x478370 (slot 2)
};

// --------------------------------------------------------------------------- free functions
class OMediaFileStream;
class OMediaDataBase;
class OMediaCanvas;
class OMediaCanvasElement;

namespace neg {
enum { ALPHA_SPRITES = 0xfa, FONT_COUNT = 6, FONT_GLYPHS = 64 };
extern OMediaCanvasElement	 g_alpha[ALPHA_SPRITES];			// 0x4fc6c0  text sprite pool
extern OMediaCanvas			*g_font[FONT_COUNT][FONT_GLYPHS];	// 0x5091e0  glyph canvases, [font][ch - ' ']
extern unsigned short		 g_alpha_used;						// 0x5097e0

OMediaFileStream *InitFileStream(const char *dos_path);		// 0x476170
OMediaDataBase *InitDatabase(OMediaFileStream *fs);			// 0x476000
void UnInitDatabase(OMediaDataBase *db);					// 0x476080
void UnInitFileStream(OMediaFileStream *fs);				// 0x476290
OMediaCanvas *LoadCanvasID(OMediaDataBase *db, int id);		// 0x477890

// text (Text.cpp)
extern float g_text_rgb[3], g_text_alpha, g_text_width;		// 0x4f4ed4 / 0x4f4ed4 / 0x509800
extern int   g_text_center;									// 0x509804
void ClearText(void);										// 0x468600
void DrawText(short x, short y, int font, const char *fmt, ...);	// N2:0x4a0f60  font: 'SMAL' 'MED' 'LARG' 'BIG' 'TIME'
OMediaCanvasElement *PlaceGlyph(short x, short y, OMediaCanvas *c);	// 0x4689f0

void DeleteAllObjects(void);								// 0x4736c0
void OpenDebugLog(void);									// N2:0x4b3ac0
void SetWindowTitle(void);									// 0x45e770
bool CheckCD(void);											// N2:0x496200
void LoadNewLevel(unsigned int which);						// omt_main / AWEMSG_LOAD
void SDLKeyToVK(int sdl_scancode, bool down);				// SDL replacement for WM_KEYDOWN/UP

// input (Input.cpp); the joystick state the poll fills is in gen/globals.h (g_joy_*)
extern std::string g_input_device_name;						// 0x509a78  FindDevice skips the device of this name ("")
bool FindDevice(int type);									// 0x4783c0  type: omt_HIDType (2 = joystick)
bool UpdateInput(void);										// 0x478530  the per-frame joystick poll
}

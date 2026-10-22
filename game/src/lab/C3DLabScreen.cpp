/*  C3DLabScreen - see C3DLabScreen.h. Reconstructed from Neutron.exe 0x42bc00..0x42c6f0. */
#include "C3DLabScreen.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../gen/globals.h"

using namespace neg;

C3DLabScreen::C3DLabScreen()								// 0x42bc00
{
	SetName((char *)"C3DLABSCREEN");
	Log("C3DLabScreen()");
	C3DLabScreen::InitObject();								// 0x42bda0 (called directly)
	C3DObject::SetObjectID('3SCR');							// 0x462220
	f_924 = 0;
	C3DObject::InitPhysics();								// 0x4623d0
	v42(0);
	v40(0);
	v46(0);
	v52(0);
	// (then a call to the empty 0x472970)
}

C3DLabScreen::~C3DLabScreen()								// 0x42bd50
{
}

namespace neg {
CGameObject *New_C3DLabScreen(void) { return new C3DLabScreen; }		// '3SCR' (0x930 bytes)
}

void C3DLabScreen::InitObject(void)							// 0x42bda0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// N2:0x41b0a0
	InitAnim();												// element +0x110
	CreateAnim((char *)"DEFAULT", (char *)"screen.ase");	// +0xe0
	CreateTexture((char *)"screen1.png", 0);				// +0xfc
	CreateTexture((char *)"screen0.png", 1);
	CreateTexture((char *)"screen2.png", 2);
	CreateTexture((char *)"screen3.png", 3);
	CreateTexture((char *)"screen4.png", 4);
	v67(20.0f);												// CGameObject +0x10c: radius
	ev58((char *)"DEFAULT", 1);								// +0xe8
	f_924 = 0;
	// (then a call to the empty 0x472970)
}

void C3DLabScreen::v23(CGameObject *o)						// 0x42bfc0
{
	CGameObject::v23(o);									// 0x4732a0 (empty)
}

void C3DLabScreen::Process(float dt)						// 0x42be70
{
	if (g_gametype->IsPaused()) return;						// +0x114
	C3DAnimated::Process(dt);								// N2:0x41c120
	f_924 = dt + f_924;
	if (f_924 < 1.0f)		{ ApplyTexture(f_590, 0); return; }	// element +0x100
	if (f_924 < 3.0f)		{ ApplyTexture(f_590, 1); return; }
	if (f_924 < 5.0f)		{ ApplyTexture(f_590, 2); return; }
	if (f_924 < 7.0f)		{ ApplyTexture(f_590, 3); return; }
	if (f_924 < 10.0f)		{ ApplyTexture(f_590, 4); return; }
	if (f_924 < 13.0f)												// (sic: past 13 s the clock never restarts)
	{
		ApplyTexture(f_590, 0);
		f_924 = 0;
	}
}

/*  C3DGraplingHook - see C3DGraplingHook.h. */
#include "C3DGraplingHook.h"
#include "Game.h"

using namespace neg;

C3DGraplingHook::C3DGraplingHook()							// 0x41eeb0
{
	SetName((char *)"C3DGRAPLINGHOOK");
	Log("C3DGraplingHook()");
	C3DGraplingHook::InitObject();							// 0x41f060 (called directly)
	v64(0.3f);
	C3DObject::SetObjectID('3GRA');							// 0x462220
	v107(0);
	v36(0);
	v52(0);
	hide(true);												// element +0x58 (BG; C3DObject::HideAgain only after a ShowAgain)
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DGraplingHook::~C3DGraplingHook()							// 0x41f010
{
}

void C3DGraplingHook::InitObject(void)						// 0x41f060
{
	Log("C3DGraplingHook::InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x110
	CreateAnim((char *)"HIROPE", (char *)"rope01.ase");		// +0xe0
	CreateTexture((char *)"jimycarl.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0x100
	v67(10.0f);												// +0x10c
	ev58((char *)"ROPE", 1);								// +0xe8
	float r[4];
	v201(r);												// +0x324 (three times: z, y, x)
	v203(r[0], r[1], r[2] + 180.0f);						// +0x32c; 0x48e5b0
	// (then a call to the empty 0x472970)
}

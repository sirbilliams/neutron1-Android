/*  C3DCursor - see C3DCursor.h. Reconstructed from Neutron.exe. */
#include "C3DCursor.h"
#include "Game.h"
#include "OMedia3DShape.h"

using namespace neg;

C3DCursor::C3DCursor()										// 0x415770
{
	SetName((char *)"C3DCURSOR");
	Log("C3DCURSOR()");
	C3DCursor::InitObject();								// 0x415900 (called directly)
	v64(0.3f);
	C3DObject::SetObjectID('3CUR');							// 0x462220
	v107(0);
	v36(0);
	v52(0);
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DCursor::~C3DCursor()										// 0x4158b0
{
}

void C3DCursor::InitObject(void)							// 0x415900
{
	Log("C3DGraplingHook::InitObject()");					// (sic: copied from C3DGraplingHook)
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x110
	CreateAnim((char *)"HIARROW", (char *)"3darrow.ase");		// +0xe0
	CreateTexture((char *)"3darrow.png", 0);				// +0xfc
	ev65(0, 6, 1);											// +0x104: blend Src_Alpha, One
	ApplyTexture(f_590, 0);									// +0x100
	v67(40.0f);												// +0x10c
	ev58((char *)"ARROW", 1);								// +0xe8
	// (then a call to the empty 0x472970)
}

/*  C3DSparkWire - see C3DSparkWire.h. Reconstructed from Neutron.exe 0x441260..0x441590. */
#include "C3DSparkWire.h"
#include "../Game.h"
#include "../Globals.h"

using namespace neg;

C3DSparkWire::C3DSparkWire()								// 0x441260
{
	SetName((char *)"C3DSPARKWIRE");
	f_6bc = 0;
	C3DSparkWire::InitObject();								// 0x441440 (called directly)
	C3DObject::SetObjectID('3SPA');							// 0x462220
	v52(1);
	v46(0);
	v34(1);
	f_6c0 = 0;
	f_6c4 = 1;												// ItemActive
	f_6c8 = 1;
	force_show();											// (if hidden: hide count 1, then hide(false))
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DSparkWire::~C3DSparkWire()								// 0x4413b0
{
}

namespace neg {
CGameObject *New_C3DSparkWire(void) { return new C3DSparkWire; }		// '3SPA' (0x6d8 bytes)
}

void C3DSparkWire::InitObject(void)							// 0x441440
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ItemActive", &f_6c4, PROP_LONG, 0);
	InitAnim();												// element +0x108
	f_4bc = 0;												// (BG +0x570) InitAnim done: cleared again
	CreateAnim((char *)"HIWALK", (char *)"powerline.ase");		// +0xd8
	CreateAnim((char *)"HISTOP", (char *)"powerlinestop.ase");
	CreateTexture((char *)"powerline01.png", 0);			// +0xf0
	CreateTexture((char *)"powerline02.png", 1);
	CreateTexture((char *)"powerline01.png", 2);
	CreateTexture((char *)"powerline02.png", 3);
	ApplyTexture(f_590, (short)f_6bc);						// +0xf4
	v67(60.0f);												// radius
	ev58((char *)"WALK", 1);								// +0xe0
}

void C3DSparkWire::ResetObject(void)						// 0x441400
{
	C3DTesla::ResetObject();								// 0x445a20
	if (f_6c8 == 0) OMediaElement::hide(true);
	else force_show();
}

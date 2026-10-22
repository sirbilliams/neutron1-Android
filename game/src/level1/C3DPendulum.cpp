/*  C3DPendulum - see C3DPendulum.h. Reconstructed from Neutron.exe 0x434f60..0x435290. */
#include "C3DPendulum.h"
#include "../Game.h"
#include "../Globals.h"
#include <math.h>

using namespace neg;

C3DPendulum::C3DPendulum()									// 0x434f60
{
	SetName((char *)"C3DPENDULUM");
	Log("C3DPENDULUM");
	C3DPendulum::InitObject();								// 0x435160 (called directly)
	C3DObject::SetObjectID('3PEN');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	f_924 = (float)(((*seed >> 16) * 501) >> 16) * 0.005f;	// 0x4ad424: phase 0..2.5
	C3DObject::InitPhysics();								// 0x4623d0
	f_928 = 60.0f;											// SwingHeight
	v94(2);													// (BG slot 95)
	C3DObject::InitPhysics();
	// (then a call to the empty 0x472970)
}

C3DPendulum::~C3DPendulum() {}								// 0x435110

namespace neg {
CGameObject *New_C3DPendulum(void) { return new C3DPendulum; }	// '3PEN' (0x6d4 bytes)
}

void C3DPendulum::InitObject(void)							// 0x435160
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("SwingHeight", &f_928, PROP_FLOAT, 0);
	InitAnim();												// element +0x110
	CreateAnim((char *)"HIDEFAULT", (char *)"chain.ase");	// +0xd8
	CreateTexture((char *)"chain.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(100.0f);											// +0x110: radius
	ev58((char *)"DEFAULT", 1);								// +0xe0 (sic: the animation was made as "HIDEFAULT")
}

void C3DPendulum::Process(float dt)							// 0x435210
{
	C3DAnimated::Process(dt);								// 0x40e050
	f_924 = dt + f_924;
	const float z = (float)(sin((double)f_924) * f_928);	// fsin
	float rot[4];
	v201(rot);												// +0x328 (rotation)
	v201(rot);												// (called again for the y / x values)
	v203(rot[0], rot[1], z);								// +0x330: swings about z
	v224(0, -1700.0f, 0);									// +0x384 (result unused in the original)
}

/*  C3DYokHelmet - see C3DYokHelmet.h. Reconstructed from Neutron.exe 0x44a440..0x44a730. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokHelmet.h"
#include "../Globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DYokHelmet::C3DYokHelmet()								// 0x44a440
{
	SetName((char *)"C3DYOKHELMET");
	Log("C3DYokHelmet()");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DYokHelmet::InitObject();								// 0x44a640 (called directly)
	C3DObject::SetObjectID('3YHE');							// 0x462220
	v36(1);
	v107(0);												// (BG slot 108)
	v52(0);
	v46(0);
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DYokHelmet::~C3DYokHelmet()								// 0x44a590
{
}

void C3DYokHelmet::InitObject(void)							// 0x44a640
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	v67(10.0f);												// +0x110: radius
	ev71(g_windowed ? 0.2f : 0.8f);							// element +0x114: alpha blending (0x509a13)
	force_show();											// (2002 +0x70 hide count: 1, then hide(false))
	// (then a call to the empty 0x472970)
}

void C3DYokHelmet::ev69(void)								// 0x44a5e0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"yokcaphelm.ase");	// element +0xd8
	CreateTexture((char *)"yokcaptn.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4 (BG +0x57c)
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

/*  C3DShrinkRay - see C3DShrinkRay.h. Reconstructed from Neutron.exe 0x43fe20..0x440290. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DShrinkRay.h"
#include "OMedia3DMaterial.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DShrinkRay::C3DShrinkRay()								// 0x43fe20
{
	SetName((char *)"C3DSHRINKRAY");
	Log("C3DShringRay()");									// (sic)
	C3DShrinkRay::InitObject();								// 0x440000 (called directly)
	v64(0.3f);
	C3DObject::SetObjectID('3SHR');							// element slot 49
	v107(0);												// +0x1b0
	v36(0);
	v52(0);
	f_6bc = 0;
	f_6c4 = 0;
	f_008 = 1;												// element +0xc8
}

C3DShrinkRay::~C3DShrinkRay() {}							// 0x43ff70

namespace neg {
CGameObject *New_C3DShrinkRay(void)	{ return new C3DShrinkRay; }	// factory '3SHR' (0x6d0 bytes)
}

void C3DShrinkRay::InitObject(void)							// 0x440000
{
	Log("C3DShrinkRay::InitObject()");
	C3DAnimated::InitObject();
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIRAY", (char *)"ray.ase");			// element +0xd8
	static const char *const kTex[3] = { "ray0000.png", "ray0001.png", "ray0002.png" };
	for (short i = 0; i < 3; i++)
	{
		CreateTexture((char *)kTex[i], i);					// element +0xf0
		ev65(i, 6, 1);										// element +0xfc: the material's blend
	}
	ApplyTexture(f_590, 0);									// element +0xf4 (BG +0x57c)
	for (int i = 0; i < 3; i++)								// (BG +0x5c0..): white, half transparent
		if (f_5d4[i]) f_5d4[i]->set_diffuse(OMediaFARGBColor(0.5f, 1.0f, 1.0f, 1.0f));	// +0x4c.. (r, g, b, a = 1, 1, 1, 0.5)
	ev71(0.5f);												// element +0x114
	v67(40.0f);												// +0x110: radius
	ev58((char *)"RAY", 1);									// element +0xe0
	f_6c0 = 0;
}

void C3DShrinkRay::Process(float dt)						// 0x440180
{
	C3DAnimated::Process(dt);
	if (!f_6c4)
	{
		f_6c4 = 1;
		InitObject();										// +0x1c
	}
	f_6c0 += dt;
	if (f_6c0 > 0.1)										// 0x49cfb8 (double)
	{
		f_6bc++;
		ev71(0.5f);											// element +0x114
		if (f_6bc > 2) f_6bc = 0;
		ApplyTexture(f_590, f_6bc);							// element +0xf4
		f_6c0 = 0;
	}
}

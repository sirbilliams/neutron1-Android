/*  C3DHydrant - see C3DHydrant.h. Reconstructed from Neutron.exe 0x420df0..0x421190. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DHydrant.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DHydrant::C3DHydrant()									// 0x420df0
{
	SetName((char *)"C3DHYDRANT");							// 0x4702d0
	C3DHydrant::InitObject();								// 0x420f70 (called directly)
	v64(0.3f);												// 0x4711e0
	C3DObject::SetObjectID('3HYD');							// 0x462220
	v52(0);
	f_6c0 = 0;
}

C3DHydrant::~C3DHydrant()									// 0x420f20
{
}

namespace neg {
CGameObject *New_C3DHydrant(void) { return new C3DHydrant; }			// '3HYD' (0x6cc bytes)
}

void C3DHydrant::InitObject(void)							// 0x420f70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"waterhydrant.ase");	// +0xd8
	static const char *const tex[4] = { "whydrant0000.png", "whydrant0001.png",
										"whydrant0002.png", "whydrant0003.png" };
	for (short i = 0; i < 4; i++)
	{
		CreateTexture((char *)tex[i], i);					// +0xf0
		ev65(i, 6, 1);										// +0xfc: blend (src alpha, one)
	}
	f_6bc = 0;
	ApplyTexture(f_590, (short)f_6bc);						// +0xf4
	v67(20.0f);												// CGameObject +0x110: radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
	ev71(0.3f);												// +0x114: alpha blending
}

void C3DHydrant::Process(float dt)							// 0x4210a0
{
	C3DAnimated::Process(dt);								// 0x40e050
	f_6c0 = dt + f_6c0;
	if (!(f_6c0 > 0.2)) return;								// 0x4a2ad0 (double)
	ApplyTexture(f_590, (short)f_6bc);						// +0xf4
	ev71(0.3f);												// +0x114
	if (++f_6bc > 3) f_6bc = 0;
}

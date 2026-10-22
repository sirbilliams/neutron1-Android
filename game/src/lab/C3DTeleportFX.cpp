/*  C3DTeleportFX - see C3DTeleportFX.h. Reconstructed from Neutron.exe 0x445600..0x445870. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DTeleportFX.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DTeleportFX::C3DTeleportFX()								// 0x445600
{
	SetName((char *)"C3DTELEPORTFX");
	Log("C3DTELEPORTFX()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	InitAnim();												// element +0x108
	CreateAnim((char *)"HITEL", (char *)"sfxteleport.ase");	// +0xd8
	CreateTexture((char *)"sfxteleport.png", 0);			// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4 (BG +0x57c: the base shape)
	ev58((char *)"TEL", 1);									// +0xe0 (0x4f10e4: the tail of "HITEL")
	C3DObject::SetObjectID('3TEL');							// 0x462220
	v64(0.05f);
	v66(100.0f);
	v46(0);
	v52(0);
	C3DObject::InitPhysics();								// 0x4623d0
	f_6bc = 0;
	ev71(0.5f);												// element +0x114 (BG slot 69): alpha blending
	v107(1);												// (BG slot 108)
	f_008 = 1;												// CGameObject +0x8
	// (then a call to the empty 0x472970)
}

C3DTeleportFX::~C3DTeleportFX()								// 0x4457c0
{
}

namespace neg {
CGameObject *New_C3DTeleportFX(void) { return new C3DTeleportFX; }	// '3TEL' (0x6c8 bytes)
}

void C3DTeleportFX::Process(float dt)						// 0x448bc0
{
	C3DAnimated::Process(dt);								// 0x40e050
}

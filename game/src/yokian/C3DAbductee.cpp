/*  C3DAbductee - see C3DAbductee.h. Reconstructed from Neutron.exe 0x4076e0..0x407a10. */
#include "C3DAbductee.h"
#include "../Globals.h"
#include "../Physics.h"

using namespace neg;

C3DAbductee::C3DAbductee()									// 0x4076e0
{
	SetName((char *)"C3DABDUCTEE");
	Log("C3DABDUCTEE()");
	C3DAbductee::InitObject();								// 0x407880 (called directly)
	C3DObject::SetObjectID('3ABD');							// 0x462220
	C3DObject::InitPhysics();								// 0x4623d0
	v107(0);
	v52(0);
	v42(0);
	v40(0);
}

C3DAbductee::~C3DAbductee()									// 0x407830
{
}

namespace neg {
CGameObject *New_C3DAbductee(void) { return new C3DAbductee; }		// '3ABD' (0x6c4 bytes)
}

void C3DAbductee::InitObject(void)							// 0x407880
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HISTRUGGLE", (char *)"abductstruggle.ase");	// +0xd8
	CreateAnim((char *)"HISTOP", (char *)"abductstop.ase");
	CreateTexture((char *)"abducted.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(20.0f);												// radius
	ev58((char *)"STRUGGLE", 1);							// +0xe0
}

void C3DAbductee::Process(float dt)							// 0x407930
{
	C3DAnimated::Process(dt);								// 0x40e050
	float a[4], b[4];
	v155(a);												// +0x270 constant acceleration (BG slot 156)
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	AddRotation(b[1] * 300.0f, 0, 0);						// +0x334 (0x48d930)
}

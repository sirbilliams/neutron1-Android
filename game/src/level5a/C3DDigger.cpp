/*  C3DDigger - see C3DDigger.h. Reconstructed from Neutron.exe 0x416eb0..0x4170f0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DDigger.h"
#include "../Game.h"
#include "../Globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DDigger::C3DDigger()										// 0x416eb0
{
	SetName((char *)"C3DDIGGER");
	Log("C3DDIGGER()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDRILL", (char *)"drill.ase");		// +0xd8
	CreateTexture((char *)"drill.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DRILL", 1);								// +0xe0
	v94(2);													// +0x17c
	C3DObject::SetObjectID('3DIG');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DDigger::~C3DDigger()										// 0x417050
{
}

namespace neg {
CGameObject *New_C3DDigger(void) { return new C3DDigger; }				// '3DIG' (0x6c4 bytes)
}

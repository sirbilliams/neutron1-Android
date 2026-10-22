/*  C3DFlag - see C3DFlag.h. Reconstructed from Neutron.exe 0x419550..0x419750. */
#include "C3DFlag.h"

using namespace neg;

C3DFlag::C3DFlag()											// 0x419550
{
	SetName((char *)"C3DFLAG");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	InitAnim();												// 0x40e270
	CreateAnim((char *)"HIDEFAULT", (char *)"flag.ase");	// 0x40d4a0
	CreateTexture((char *)"flag.png", 0);					// 0x40db20
	ApplyTexture(f_590, 0);									// 0x40dd60
	ev58((char *)"DEFAULT", 1);								// 0x40dd90
	SetObjectID('3FLA');									// 0x462220
	v46(0);
	v52(0);
}

C3DFlag::~C3DFlag() {}										// 0x4196b0

namespace neg {
CGameObject *New_C3DFlag(void)	{ return new C3DFlag; }		// factory '3FLA' (0x6c4 bytes)
}

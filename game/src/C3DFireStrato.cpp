/*  C3DFireStrato - see C3DFireStrato.h. Reconstructed from Neutron.exe 0x4192e0..0x419540. */
#include "C3DFireStrato.h"

C3DFireStrato::C3DFireStrato()								// 0x4192e0
{
	SetName((char *)"C3DFIRESTRATO");
	C3DAnimated::InitObject();								// 0x40d530 (called directly)
	C3DAnimated::InitAnim();								// 0x40e270
	C3DAnimated::CreateAnim((char *)"HIDEFAULT", (char *)"firestrato.ase");	// 0x40d4a0 (Boy Genius: no afterburner)
	C3DAnimated::CreateTexture((char *)"fire.png", 0);
	C3DAnimated::ApplyTexture(f_590, 0);					// 0x40dd60
	C3DAnimated::ev58((char *)"DEFAULT", 1);				// 0x40dd90
	C3DAnimated::ev65(0, 6, 1);								// 0x40dd40 (called directly): material 0 blend Src_Alpha, One
	C3DObject::SetObjectID('3FST');							// 0x462220
	v107(0);
	v36(0);
	v64(0);
	v66(1000.0f);
	v46(0);
	v52(0);
	f_664 = 1;												// SecondPass
}

C3DFireStrato::~C3DFireStrato()								// 0x419480
{
}

void C3DFireStrato::ResetObject(void)						// 0x4194d0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	hide(true);											// element +0x58
}

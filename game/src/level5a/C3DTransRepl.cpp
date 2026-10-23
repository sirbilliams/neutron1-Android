/*  C3DTransRepl - see C3DTransRepl.h. Reconstructed from Neutron.exe 0x4466c0..0x446a00. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DTransRepl.h"
#include "../Game.h"
#include "../Globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DTransRepl::C3DTransRepl()								// 0x4466c0
{
	SetName((char *)"C3DTransRepl");
	Log("C3DSwingDoor()");									// (sic)
	v42(0);
	v40(0);
	v46(0);
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3TRA');							// 0x462220
	strcpy(m_ase_file, "doorfowl.ase");
	strcpy(m_png_file, "doorfowl.png");
	C3DTransRepl::InitObject();								// 0x446930 (called directly)
	v42(0);
	v40(0);
	v34(0);
	v107(0);												// +0x1b0
	v52(0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIRAY", m_ase_file);				// +0xd8
	CreateTexture(m_png_file, 0);							// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(40.0f);												// +0x110: radius
	ev58((char *)"RAY", 1);									// +0xe0
	v42(0);
	v40(0);
}

C3DTransRepl::~C3DTransRepl()								// 0x4468e0
{
}

namespace neg {
CGameObject *New_C3DTransRepl(void) { return new C3DTransRepl; }		// '3TRA' (0x78c bytes)
}

void C3DTransRepl::InitObject(void)							// 0x446930
{
	Log("C3DShrinkRay::InitObject()");						// (sic)
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ASEFile", m_ase_file, PROP_ALPHA, 0);
	RegisterProperty("PNGFile", m_png_file, PROP_ALPHA, 0);
}

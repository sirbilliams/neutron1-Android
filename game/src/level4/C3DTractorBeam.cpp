/*  C3DTractorBeam - see C3DTractorBeam.h. Reconstructed from Neutron.exe 0x4462c0..0x446680. */
#include "C3DTractorBeam.h"
#include "../Game.h"
#include "../Globals.h"
#include "OMedia3DMaterial.h"
#include <math.h>

using namespace neg;

C3DTractorBeam::C3DTractorBeam()							// 0x4462c0
{
	SetName((char *)"C3DTRACTORBEAM");
	Log("C3DTRACTORBEAM()");
	C3DTractorBeam::InitObject();							// 0x446470 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	v107(1);
	v46(0);
	v52(0);
	C3DObject::SetObjectID('3TRC');							// 0x462220
	f_6bc = 0;
	f_6c0 = 0;
	OMediaElement::hide(true);
	f_6c4 = 0;
}

C3DTractorBeam::~C3DTractorBeam()							// 0x446420
{
}

namespace neg {
CGameObject *New_C3DTractorBeam(void) { return new C3DTractorBeam; }	// '3TRC' (0x6d0 bytes)
}

void C3DTractorBeam::InitObject(void)						// 0x446470
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIUP", (char *)"yrayupdown.ASE");	// +0xd8
	CreateAnim((char *)"HISTOP", (char *)"yraystop.ASE");
	CreateTexture((char *)"yray1.png", 0);					// +0xf0
	CreateTexture((char *)"yray2.png", 1);
	ApplyTexture(f_590, 0);									// +0xf4
	v67(10.0f);												// radius
	ev58((char *)"STOP", 1);								// +0xe0
	ev71(0.5f);												// +0x114
}

void C3DTractorBeam::Process(float dt)						// 0x446540
{
	C3DAnimated::Process(dt);								// 0x40e050
	f_6c0 = dt + f_6c0;
	f_6c4 = dt + f_6c4;
	if ((float)0.15 < f_6c0)								// 0x4bbc00 (double)
	{
		f_6c0 = 0;
		f_6bc = (f_6bc == 0);
		ev71(0.5f);
		const float s = sinf(f_6c4);
		OMedia3DMaterial *m = f_5d4[f_6bc];
		m->set_diffuse(OMediaFARGBColor(0.2f, s, s, 1.0f));	// +0x4c..+0x58: r, g, b, a
		ApplyTexture(f_590, (short)f_6bc);					// +0xf4
	}
}

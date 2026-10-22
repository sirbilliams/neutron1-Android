/*  C3DTesla - see C3DTesla.h. Reconstructed from Neutron.exe 0x445870..0x445f20. */
#include "C3DTesla.h"
#include "../C3DPlayer.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include "OMedia3DVector.h"
#include <string>

using namespace neg;

C3DTesla::C3DTesla()										// 0x445870
{
	SetName((char *)"C3DTESLA");
	f_6bc = 0;
	C3DTesla::InitObject();									// 0x445a70 (called directly)
	C3DObject::SetObjectID('3TES');							// 0x462220
	v52(1);
	v46(0);
	v34(1);
	f_6c0 = 0;
	f_6c4 = 1;												// ItemActive
	f_6c8 = 1;
	f_6cc = -1;
	force_show();											// (if hidden: hide count 1, then hide(false))
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DTesla::~C3DTesla()										// 0x4459d0
{
}

namespace neg {
CGameObject *New_C3DTesla(void) { return new C3DTesla; }				// '3TES' (0x6d8 bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DTesla::InitObject(void)								// 0x445a70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ItemActive", &f_6c4, PROP_LONG, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"tesla.ase");	// +0xd8
	CreateTexture((char *)"tesla0000.png", 0);				// +0xf0
	ev65(0, omblendfc_Src_Alpha, omblendfc_One);			// +0xfc: added onto the scene
	CreateTexture((char *)"tesla0001.png", 1);
	ev65(1, omblendfc_Src_Alpha, omblendfc_One);
	CreateTexture((char *)"tesla0002.png", 2);
	ev65(2, omblendfc_Src_Alpha, omblendfc_One);
	CreateTexture((char *)"tesla0003.png", 3);
	ev65(3, omblendfc_Src_Alpha, omblendfc_One);
	ApplyTexture(f_590, (short)f_6bc);						// +0xf4
	v67(80.0f);												// radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DTesla::ResetObject(void)							// 0x445a20
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	v34(1);
	f_6c8 = f_6c4;											// on = ItemActive
	if (f_6c4 == 0) OMediaElement::hide(true);
	else force_show();
}

void C3DTesla::AddTouching(CGameObject *other)				// 0x445ba0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (f_6c8 == 0) return;
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	float a[4], b[4];
	other->v195(a);											// +0x310
	v195(b);
	OMedia3DVector d(a[0] - b[0], a[1] - b[1], a[2] - b[2], 0);
	if (d.quick_magnitude() > 1.0f)							// 0x48d924
	{
		const float k = 1.0f / d.quick_magnitude();
		C3DPlayer *p = dynamic_cast<C3DPlayer *>(other);	// port: the original calls element slot 88 of whatever is named C3DJIMMY
		if (p) p->TouchedShock(OMedia3DVector(k * d.x, d.y * k, d.z * k, 0));	// +0x160 (C3DJimmy 0x42a7e0)
	}
}

void C3DTesla::RemoveTouching(CGameObject *other)			// 0x44a100
{
	CGameObject::RemoveTouching(other);						// 0x470b20
}

void C3DTesla::Process(float dt)							// 0x445ca0
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (f_6c8 == 0) return;
	f_6c0 = dt + f_6c0;
	if (!(f_6c0 > 0.1)) return;								// 0x49cfb8 (double)
	f_6c0 = 0;
	f_6bc++;
	if (f_6bc > 3) f_6bc = 0;
	ApplyTexture(f_590, (short)f_6bc);						// element +0xf4
	ev71(0.7f);												// +0x114: 0x3f333333
}

void C3DTesla::v260(void)									// 0x445dd0
{
	C3DAnimated::v260();									// 0x40e7b0
	if (f_6c8 != 0) ev94();									// element +0x120
	else ev95();											// +0x124
}

void C3DTesla::v268(int state)								// 0x445d30
{
	if (state == 0) f_6c8 = 0;
	else if (state == 1)
	{
		f_6c8 = 1;
		force_show();
		ev94();
		return;
	}
	else
	{
		f_6c8 = (f_6c8 == 0);
		if (f_6c8 != 0)
		{
			force_show();
			ev94();
			return;
		}
	}
	OMediaElement::hide(true);
	ev95();
}

void C3DTesla::v277(void)									// 0x445e70
{
	C3DAnimated::v277();									// 0x40e770
	if (f_6cc != -1) StopChannel(f_6cc, false);				// 0x47d7a0
}

void C3DTesla::v278(void)									// 0x445e90
{
	C3DAnimated::v278();									// 0x40e790
	if (f_6cc != -1)
		f_6cc = PlayGameSound3DIndex(this, -1, 0xb0, true);	// 0x4589c0
}

// --------------------------------------------------------------------------- element family

void C3DTesla::ev94(void)									// 0x445e00
{
	if (f_6cc == -1)
		f_6cc = PlayGameSound3DIndex(this, -1, 0xb0, true);	// 0x4589c0
}

void C3DTesla::ev95(void)									// 0x445e40
{
	if (f_6cc == -1) return;
	StopGameSound(f_6cc, false);							// 0x458a00
	f_6cc = -1;
}

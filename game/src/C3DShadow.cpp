/*  C3DShadow - see C3DShadow.h. Reconstructed from Neutron.exe. */
#include "C3DShadow.h"
#include "Game.h"
#include "gen/globals.h"
#include <string.h>

using namespace neg;

C3DShadow::C3DShadow()										// 0x43f640
{
	SetName((char *)"C3DSHADOW");
	Log("C3DShadow()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3SHA');							// 0x462d80
	strcpy(m_sprite_db, "retainedsprites.omt");
	m_sprite_index = 9;
	m_sprite_size = 150;
	v42(1);
	v40(1);
	v52(0);
	v107(0);
	v36(0);
	f_600 = 70;
	f_602 = 30;
	// (then a call to the empty 0x472970)
}

C3DShadow::~C3DShadow()										// 0x43f7d0
{
}

namespace neg {
CGameObject *New_C3DShadow(void)	{ return new C3DShadow; }				// factory '3SHA' (0x60c bytes)
}

void C3DShadow::v260(void)									// 0x43f810
{
	C3DPermanentSprite::v260();								// 0x4354c0
	set_size((float)f_600, (float)f_602);					// 2002 +0xac / +0xb0
	set_blend(omblendfc_Dst_Color, omblendfc_Zero);			// +0xb4 = 2, +0xb8 = 0
	if (g_windowed) hide(true);							// element +0x58 (slot 22)
}

void C3DShadow::SetSize(short w, short h)					// 0x43f870
{
	f_600 = w;
	f_602 = h;
	set_size((float)w, (float)h);							// 2002 +0xac / +0xb0
}

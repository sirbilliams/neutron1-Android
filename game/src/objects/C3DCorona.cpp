/*  C3DCorona - see C3DCorona.h. Reconstructed from Neutron.exe 0x4154b0..0x415750. */
#include "C3DCorona.h"
#include "../Game.h"
#include "../Globals.h"
#include <string.h>

using namespace neg;

C3DCorona::C3DCorona()										// 0x4154b0
{
	SetName((char *)"C3DCORONA");
	Log("C3DCorona()");
	C3DSprite::InitObject();								// 0x464130 (it has no InitObject of its own)
	SetObjectID('3COR');									// element slot 48
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 36;
	m_sprite_size = 100;
	v42(0);
	v40(0);
	v52(0);
	v107(0);
	v46(0);
	m_red = 1.0f;
	m_green = 0.1f;
	m_blue = 0.1f;
	// (an empty debug log)
}

C3DCorona::~C3DCorona() {}									// 0x415640

namespace neg {
CGameObject *New_C3DCorona(void) { return new C3DCorona; }	// '3COR' (0x5fc bytes)
}

void C3DCorona::v260(void)									// 0x415680
{
	C3DSprite::v260();										// 0x464810 (not C3DSpriteType's)
	set_blend(omblendfc_Src_Alpha, omblendfc_One);			// +0xb4 = 6, +0xb8 = 1
	set_diffuse(OMediaFARGBColor(1.0f, m_red, m_green, m_blue));	// +0x90.. (r, g, b, a)
}

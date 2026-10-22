/*  C3DCone - see C3DCone.h. Reconstructed from Neutron.exe 0x4152a0..0x4154a0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DCone.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DCone::C3DCone()											// 0x4152a0
{
	SetName((char *)"C3DCONE");
	Log("C3DCone()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3CON');
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 41;									// 0x29
	m_sprite_size = 70;										// 0x46
	v42(0);
	v40(0);
	v52(0);
	v107(1);												// raw v108 (slot 108, 0x472640)
	v46(0);
	// (then a call to the empty 0x472970)
}

C3DCone::~C3DCone() {}										// 0x415420

namespace neg {
CGameObject *New_C3DCone(void)	{ return new C3DCone; }	// factory '3CON' (0x5f0 bytes)
}

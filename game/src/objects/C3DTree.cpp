/*  C3DTree - see C3DTree.h. Reconstructed from Neutron.exe 0x446c50..0x446e40. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DTree.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DTree::C3DTree()											// 0x446c50
{
	SetName((char *)"C3DTREE");
	Log("C3DTree()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3TRE');
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 16;
	m_sprite_size = 150;
	v42(0);
	v40(0);
	v52(0);
	v46(0);
	// (then a call to the empty 0x472970)
}

C3DTree::~C3DTree() {}										// 0x446dc0

namespace neg {
CGameObject *New_C3DTree(void)	{ return new C3DTree; }	// factory '3TRE' (0x5f0 bytes)
}

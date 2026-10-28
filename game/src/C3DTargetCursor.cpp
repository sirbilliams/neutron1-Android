/*  C3DTargetCursor - see C3DTargetCursor.h. Reconstructed from Neutron.exe. */
#include "C3DTargetCursor.h"
#include "Game.h"
#include <string.h>

using namespace neg;

C3DTargetCursor::C3DTargetCursor()							// 0x4453b0
{
	SetName((char *)"C3DSHADOW");							// (sic: copied from C3DShadow)
	Log("C3DShadow()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3TAR');							// 0x462d80
	strcpy(m_sprite_db, "retainedsprites.omt");
	m_sprite_index = 8;
	m_sprite_size = 150;
	v42(1);
	v40(1);
	v52(0);
	v107(0);
	v36(0);
	f_600 = 110;
	f_602 = 110;
	// (then a call to the empty 0x472970)
}

C3DTargetCursor::~C3DTargetCursor()							// 0x445540
{
}

void C3DTargetCursor::v260(void)							// 0x445580
{
	C3DPermanentSprite::v260();								// 0x4354c0
	set_size((float)f_600, (float)f_602);					// 2002 +0xac / +0xb0
}

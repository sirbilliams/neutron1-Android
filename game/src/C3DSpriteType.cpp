/*  C3DSpriteType - see C3DSpriteType.h. Reconstructed from Neutron.exe. */
#include "C3DSpriteType.h"
#include "gen/globals.h"
#include "OMediaLayer.h"
#include <string.h>

C3DSpriteType::C3DSpriteType()								// 0x441980
{
	set_canvas_flags((omt_CanvasElementFlags)(omcanef_FreeWorldSize | 2));	// +0xc4 = 3 (2: an OMT 2002 flag 2.5 does not have)
}

C3DSpriteType::~C3DSpriteType() {}							// 0x441a60

void C3DSpriteType::AddTouching(CGameObject *other)						// 0x43cf10
{
	CGameObject::AddTouching(other);
}

void C3DSpriteType::Process(float dt)						// 0x40ff40
{
	C3DSprite::Process(dt);
}

void C3DSpriteType::v242(float t)							// 0x441aa0
{
	(void)t;
	if (!getvisible())										// element +0x70 hide count != 0
	{
		force_show();										// hide count = 1, hide(false) (element +0x58)
	}
}

void C3DSpriteType::v260(void)								// 0x441ac0 (BG slot 259)
{
	LoadSprite(false);										// (inlined, as C3DSprite's)
	set_canvas_flags((omt_CanvasElementFlags)(omcanef_FreeWorldSize | 2));	// +0xc4 = 3 on every path (2: an OMT 2002 flag 2.5 does not have)
}

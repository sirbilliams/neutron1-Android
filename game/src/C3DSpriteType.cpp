/*  C3DSpriteType - see C3DSpriteType.h. */
#include "C3DSpriteType.h"
#include "gen/globals.h"
#include "OMediaLayer.h"
#include <string.h>

C3DSpriteType::C3DSpriteType()								// N2:0x473420
{
	set_canvas_flags(omcanef_FreeWorldSize);				// +0xc4 = 1
	f_600 = 0;
	strcpy(f_604, g_empty_string);							// 0x4f81a8
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

void C3DSpriteType::v282(OMediaMatrix_4x4 &m, float *w, float *h)	// N2:0x473630
{
	get_layer()->prepare_projection_matrix(0, m, *w, *h);	// element +0x64 its_layer, vtable +0xc
}

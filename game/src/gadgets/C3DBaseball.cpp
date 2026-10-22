/*  C3DBaseball - see C3DBaseball.h. Reconstructed from Neutron.exe 0x40fc50..0x410080. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBaseball.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMediaCanvas.h"
#include <string.h>

using namespace neg;

C3DBaseball::C3DBaseball()									// 0x40fc50
{
	f_5f4 = 0;
	SetName((char *)"C3DBASEBALL");
	Log("C3DBaseball()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3BAS');							// 0x462d80
	strcpy(m_sprite_db, "retainedsprites.omt");
	m_sprite_index = 2;
	m_sprite_size = 25;
	OMediaDataBase *db = GetPermanentDatabase("retainedsprites.omt");	// 0x46acc0
	OMediaCanvas **c[4] = { &f_5f8, &f_5fc, &f_600, &f_604 };
	for (int i = 0; i < 4; i++)
	{
		*c[i] = LoadCanvasID(db, 2 + i);					// 0x477890
		if (*c[i]) (*c[i])->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60, +0x64 = 1, +0x24 vtable +0x10 (port: checked)
	}
	f_608 = 1.0f;
	f_60c = 0;
	f_610 = 0;
	v46(1);
	InitSpriteProperties(25.0f);							// element slot 52
	v60(3.2f);
	v67(15.0f);												// BG slot 68
	v107(0);												// BG slot 108
	v36(0);
	v52(0);
	v62(0.46f);
	// (then a call to the empty 0x472970)
}

C3DBaseball::~C3DBaseball() {}								// 0x40fea0

namespace neg {
CGameObject *New_C3DBaseball(void)	{ return new C3DBaseball; }	// factory '3BAS' (0x61c bytes)
}

// (The original fetches the velocity into a local and copies the arguments, then calls the base.)
void C3DBaseball::v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly)	// 0x40ff50
{
	CGameObject::v219(nx, ny, nz, nw, poly);				// 0x472690
}

void C3DBaseball::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x40fee0
{
	CGameObject::v220(a, b, c, d, poly);					// 0x458890
}

// Landing: the impulse is the velocity with the vertical part reversed and damped (-0.65).
void C3DBaseball::v222(void)								// 0x40ff80
{
	float v[4];
	v174(v);												// the velocity, BG slot 175
	v161(v[0], v[1] * -0.65f, v[2], 0);						// 0x49195c; the impulse, BG slot 162
}

void C3DBaseball::v260(void)								// 0x410000
{
	C3DPermanentSprite::v260();								// 0x4354c0
	InitPhysics();											// BG slot 11
	v67(15.0f);												// BG slot 68
}

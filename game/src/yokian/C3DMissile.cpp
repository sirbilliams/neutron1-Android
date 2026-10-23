/*  C3DMissile - see C3DMissile.h. Reconstructed from Neutron.exe 0x42f500..0x42f9e0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMissile.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DMissile::C3DMissile()									// 0x42f500
{
	SetName((char *)"C3DMISSILE");
	Log("C3DMissile()");
	C3DMissile::InitObject();								// 0x42f6a0 (called directly)
	C3DObject::SetObjectID('3MIS');							// 0x462220
	v107(0);												// +0x1b0
	v52(0);
	m_flying = 0;
	m_hit = 0;
	m_timer = 0;
	C3DObject::InitPhysics();								// 0x4623d0
	v34(1);
	m_target = NULL;
}

C3DMissile::~C3DMissile()									// 0x42f650
{
}

namespace neg {
CGameObject *New_C3DMissile(void) { return new C3DMissile; }			// (0x6d4 bytes; created by C3DYokTurret / C3DHarrier)
}

void C3DMissile::InitObject(void)							// 0x42f6a0
{
	Log("C3DGraplingHook::InitObject()");					// (sic)
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"missile.ase");	// +0xd8
	CreateTexture((char *)"missile.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev65(0, 6, 1);											// +0xfc: the material's blend (src alpha, one)
	v67(40.0f);												// +0x110: radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
	v44(0);													// +0xb0
	ev71(0.6f);												// +0x114: translucent
}

void C3DMissile::Process(float dt)							// 0x42f760
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (!m_flying) return;
	float a[4], b[4];
	v155(a);												// +0x270: the acceleration
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);							// +0x278: gravity cancelled (0x5cfc04)
	const float t = m_timer;
	m_timer = dt + t;
	if (0.5 < (double)m_timer)								// 0x48e5d0
	{
		m_timer = 0;
		v42(1);												// +0xa8
		v40(1);												// +0xa0
	}
}

void C3DMissile::AddTouching(CGameObject *other)			// 0x42f800
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DROCKET") && !other->NameContains((char *)"C3DJIMMY")
		&& !other->NameContains((char *)"C3DJEEP")) return;
	ev72();													// element +0x120
	m_target = other;
}

void C3DMissile::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x42f860
{
	CGameObject::v220(a, b, c, d, poly);					// 0x458890
	ev72();													// element +0x120
}

void C3DMissile::v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly)	// 0x42f8a0
{
	CGameObject::v219(nx, ny, nz, nw, poly);				// 0x472690
	ev72();													// element +0x120
}

void C3DMissile::v222(void)									// 0x42f8e0
{
	ev72();													// element +0x120
}

void C3DMissile::ev72(void)									// 0x42f900
{
	m_hit = 1;
	m_flying = 0;
	OMediaElement::hide(true);								// element +0x58
	v46(0);													// +0xb8
	v52(0);													// +0xd0
	m_timer = 0;
	v176(0, 0, 0);											// +0x2c4
	v42(0);													// +0xa8
	v40(0);													// +0xa0
}

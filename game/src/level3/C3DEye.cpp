/*  C3DEye - see C3DEye.h. Reconstructed from Neutron.exe 0x418090..0x418390. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DEye.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DEye::C3DEye()											// 0x418090
{
	SetName((char *)"C3DEYE");
	Log("C3DEYE()");
	f_649 = 0;												// animations not loaded
	C3DEye::InitObject();									// 0x418290 (called directly)
	C3DObject::SetObjectID('3EYE');							// 0x462220
	m_speed = 300.0f;										// speed
	CGameObject::v42(0);									// 0x470fd0 (direct calls)
	CGameObject::v40(0);									// 0x470fa0
	CGameObject::v52(0);									// 0x4710c0
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DEye::~C3DEye()											// 0x4181e0
{
}

namespace neg {
CGameObject *New_C3DEye(void) { return new C3DEye; }					// '3EYE' (0x9b8 bytes)
}

void C3DEye::InitObject(void)								// 0x418290
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// element +0x108
}

void C3DEye::ev69(void)										// 0x418230
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"eye.ase");		// +0xd8
	CreateTexture((char *)"eyemap.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DEye::Process(float dt)								// 0x4182d0
{
	C3DAICar::Process(dt);									// 0x40aa50
	float a[4], b[4];
	v155(a);												// +0x270: the acceleration
	v155(b);
	v157(b[0], -g_gravity.y, a[2]);						// +0x278: gravity cancelled (0x5cfc04)
}

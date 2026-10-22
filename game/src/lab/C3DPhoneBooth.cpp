/*  C3DPhoneBooth - see C3DPhoneBooth.h. Reconstructed from Neutron.exe 0x4355a0..0x4358b0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DPhoneBooth.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Todo.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DPhoneBooth::C3DPhoneBooth()								// 0x4355a0
{
	SetName((char *)"C3DPHONEBOOTH");
	Log("C3DPHONEBOOTH()");
	C3DPhoneBooth::InitObject();							// 0x435780 (called directly)
	C3DObject::SetObjectID('3PHO');							// 0x462220
	v52(1);
	v42(0);
	v40(0);
	v46(0);
	v34(1);
	C3DObject::InitPhysics();								// 0x4623d0
	v67(40.0f);												// (BG slot 68) radius
	f_6bc = 0;
	// (then a call to the empty 0x472970)
}

C3DPhoneBooth::~C3DPhoneBooth()								// 0x435710
{
}

namespace neg {
CGameObject *New_C3DPhoneBooth(void) { return new C3DPhoneBooth; }	// '3PHO' (0x6cc bytes)
}

void C3DPhoneBooth::InitObject(void)						// 0x435780
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	m_db = GetDatabase("phone.omt");						// 0x46a910
	f_648 = 0;												// animations off (BG +0x634)
	set_shape(Load3DShape(m_db, 0));						// element +0xac (0x477ba0)
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	ev51();													// element +0xcc
	// (then a call to the empty 0x472970)
}

void C3DPhoneBooth::ResetObject(void)						// 0x435760
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	v67(40.0f);												// +0x110: radius
}

void C3DPhoneBooth::AddTouching(CGameObject *other)			// 0x435800
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	// Jimmy's element slot 117 (0x42abb0, +0x1d4 of his element vtable).
	// TODO(core): call it once C3DJimmy has BG's slot 117.
	NEG_TODO(0x42abb0, "C3DJimmy ev117: the phone booth");
}

void C3DPhoneBooth::Process(float dt)						// 0x448bc0
{
	C3DAnimated::Process(dt);								// 0x40e050
}

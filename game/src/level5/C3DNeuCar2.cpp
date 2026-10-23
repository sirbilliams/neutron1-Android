/*  C3DNeuCar2 - see C3DNeuCar2.h. Reconstructed from Neutron.exe 0x432730..0x4328b0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DNeuCar2.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DNeuCar2::C3DNeuCar2()										// 0x432730
{
	SetName((char *)"C3DNEUCAR2");
	Log("C3DNEUCAR2()");
	C3DNeuCar2::InitObject();									// 0x4328b0
	C3DObject::SetObjectID('3NC2');							// 0x462220
	m_speed = 20.0f;										// speed
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DNeuCar2::~C3DNeuCar2()
{
}

namespace neg {
CGameObject *New_C3DNeuCar2(void) { return new C3DNeuCar2; }	// '3NC2' (0x9b8 bytes)
}

void C3DNeuCar2::InitObject(void)							// 0x4328b0
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0 (C3DAICar has none)
	InitAnim();
	CreateAnim((char *)"HIDEFAULT", (char *)"car.ase");
	CreateTexture((char *)"cargreen.png", 0);
	ApplyTexture(f_590, 0);
	v67(300.0f);
	ev58((char *)"DEFAULT", 1);
}

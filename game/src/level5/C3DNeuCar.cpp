/*  C3DNeuCar - see C3DNeuCar.h. Reconstructed from Neutron.exe 0x4324c0..0x432640. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DNeuCar.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DNeuCar::C3DNeuCar()										// 0x4324c0
{
	SetName((char *)"C3DNEUCAR");
	Log("C3DNEUCAR()");
	C3DNeuCar::InitObject();									// 0x432640
	C3DObject::SetObjectID('3NCA');							// 0x462220
	m_speed = 20.0f;										// speed
	C3DObject::InitPhysics();								// 0x4623d0
	// (then a call to the empty 0x472970)
}

C3DNeuCar::~C3DNeuCar()
{
}

namespace neg {
CGameObject *New_C3DNeuCar(void) { return new C3DNeuCar; }	// '3NCA' (0x9b8 bytes)
}

void C3DNeuCar::InitObject(void)							// 0x432640
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0 (C3DAICar has none)
	InitAnim();
	CreateAnim((char *)"HIDEFAULT", (char *)"car.ase");
	CreateTexture((char *)"car.png", 0);
	ApplyTexture(f_590, 0);
	v67(300.0f);
	ev58((char *)"DEFAULT", 1);
}

/*  C3DFerris, C3DFerChair - see C3DFerris.h. Reconstructed from Neutron.exe 0x418a20..0x4192e0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DFerris.h"
#pragma GCC diagnostic pop

using namespace neg;

// --------------------------------------------------------------------------- C3DFerChair

C3DFerChair::C3DFerChair()									// 0x418a20
{
	SetName((char *)"C3DFERCHAIR");
	Log("C3DFERCHAIR()");
	f_649 = 0;												// animations not loaded
	C3DFerChair::InitObject();								// 0x418bd0 (called directly)
	C3DObject::SetObjectID('3CHA');							// 0x462220
	v64(0.05f);
	v66(10.0f);
	v46(0);
	v52(0);
	C3DObject::InitPhysics();								// 0x4623d0
	v107(0);												// slot 108
}

C3DFerChair::~C3DFerChair()									// 0x418b80
{
}

void C3DFerChair::InitObject(void)							// 0x418bd0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	f_649 = 1;												// animations loaded
	CreateAnim((char *)"HIDEFAULT", (char *)"chair.ase");	// +0xd8
	CreateTexture((char *)"wheel.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

// --------------------------------------------------------------------------- C3DFerris

C3DFerris::C3DFerris()										// 0x418cc0
{
	SetName((char *)"C3DFERRIS");
	Log("C3DFERRIS()");
	f_649 = 0;												// animations not loaded
	C3DFerris::InitObject();								// 0x418fc0 (called directly)
	C3DObject::SetObjectID('3FER');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(0);
	C3DObject::InitPhysics();								// 0x4623d0
	for (int i = 0; i < 4; i++) m_chair[i] = new C3DFerChair;
}

C3DFerris::~C3DFerris()										// 0x418f10
{
}

namespace neg {
CGameObject *New_C3DFerris(void) { return new C3DFerris; }				// '3FER' (0x6ec bytes)
}

void C3DFerris::InitObject(void)							// 0x418fc0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
}

void C3DFerris::ev69(void)									// 0x418f60
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"wheel.ase");	// +0xd8
	CreateTexture((char *)"wheel.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DFerris::Process(float dt)							// 0x419000
{
	C3DAnimated::Process(dt);								// 0x40e050
	AddRotation(0, 0, dt * 20.0f);							// +0x334; 0x495324
	static const float spot[4][3] = { { 432.0f, 0, 0 }, { -432.0f, 0, 0 }, { 0, 432.0f, 0 }, { 0, -432.0f, 0 } };
	for (int i = 0; i < 4; i++)
	{
		const OMedia3DVector p = v224(spot[i][0], spot[i][1], spot[i][2]);	// +0x384
		m_chair[i]->v196(p.x, p.y, p.z, p.w);				// +0x314
		float r[4];
		v201(r);											// +0x328
		m_chair[i]->v203(0, r[1], 0);						// +0x330: hangs upright
	}
}

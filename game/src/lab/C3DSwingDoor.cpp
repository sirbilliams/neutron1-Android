/*  C3DSwingDoor - see C3DSwingDoor.h. Reconstructed from Neutron.exe 0x444450..0x4449c0. */
#include "C3DSwingDoor.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include <string.h>
#include <string>

using namespace neg;

C3DSwingDoor::C3DSwingDoor()								// 0x444450
{
	SetName((char *)"C3DDOORSWING");
	Log("C3DSwingDoor()");
	v52(1);
	v42(0);
	v40(0);
	v94(2);
	v46(0);
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3SWN');							// 0x462220
	strcpy(m_ase_file, "doorfowl.ase");
	strcpy(m_png_file, "doorfowl.png");
	C3DSwingDoor::InitObject();								// 0x4446a0 (called directly)
	m_open_speed = 50.0f;											// OpenSpeed
	m_time_to_open = 1.8f;											// TimeToOpen
	m_time_left = 0;
	v42(0);
	v40(0);
	v34(0);
	m_touch_activated = 1;												// TouchActivated
	m_turning = 0;
	m_open = 0;
	v52(1);
	// (then a call to the empty 0x472970)
}

C3DSwingDoor::~C3DSwingDoor()								// 0x444650
{
}

namespace neg {
CGameObject *New_C3DSwingDoor(void) { return new C3DSwingDoor; }		// '3SWN' (0x7a4 bytes)
}

void C3DSwingDoor::InitObject(void)							// 0x4446a0
{
	Log("C3DShrinkRay::InitObject()");						// (sic)
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("ASEFile", m_ase_file, PROP_ALPHA, 0);
	RegisterProperty("PNGFile", m_png_file, PROP_ALPHA, 0);
	RegisterProperty("TimeToOpen", &m_time_to_open, PROP_FLOAT, 0);
	RegisterProperty("OpenSpeed", &m_open_speed, PROP_FLOAT, 0);
	RegisterProperty("TouchActivated", &m_touch_activated, PROP_LONG, 0);
	// (then a call to the empty 0x472970)
}

void C3DSwingDoor::AddTouching(CGameObject *other)			// 0x444750
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DJIMMY") && m_touch_activated != 0) v268(1);	// +0x18, +0x428
}

void C3DSwingDoor::v268(int state)							// 0x444790
{
	(void)state;
	if (m_turning) return;
	m_turning = 1;
	m_time_left = m_time_to_open;											// TimeToOpen
	PlayGameSound(-1, std::string("squeak"), false);		// 0x458b40
}

void C3DSwingDoor::Process(float dt)						// 0x4447e0
{
	if (!m_turning) return;										// (no pause check)
	m_time_left -= dt;
	if (!m_open)
	{
		AddRotation(0, dt * m_open_speed, 0);						// +0x330
		if (m_time_left <= 0)
		{
			m_turning = 0;
			m_open = 1;
			m_time_left = 0;
		}
	}
	else
	{
		AddRotation(0, -(dt * m_open_speed), 0);
		if (m_time_left <= 0)
		{
			m_turning = 0;
			m_open = 0;
			m_time_left = 0;
		}
	}
}

void C3DSwingDoor::v260(void)								// 0x4448a0
{
	C3DAnimated::v260();									// 0x40e7b0
	InitAnim();												// element +0x110
	CreateAnim((char *)"HIRAY", m_ase_file);						// +0xd8 (sic: played as "RAY")
	CreateTexture(m_png_file, 0);								// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	v67(40.0f);												// CGameObject +0x10c: radius
	ev58((char *)"RAY", 1);									// +0xe8
	v42(0);													// +0xa8
	v40(0);													// +0xa0
}

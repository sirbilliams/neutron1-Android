/*  C3DBus - see C3DBus.h. Reconstructed from Neutron.exe 0x411060..0x4115c0 and the shared
    InitObject 0x418290. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBus.h"
#include "../SaveGame.h"
#include "../Sound.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DBus::C3DBus()											// 0x411060
{
	SetName((char *)"C3DBUS");
	Log("C3DBUS()");
	f_649 = 0;												// animations not loaded
	Log("InitObject()");									// 0x418290 inlined
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// +0x110
	C3DObject::SetObjectID('3SBU');							// 0x462220
	m_speed = 400.0f;											// speed
	CGameObject::v64(0.05f);								// 0x4711e0 (direct calls)
	CGameObject::v66(1000.0f);
	CGameObject::v94(2);
	CGameObject::v34(0);
	m_bus_sound = -1;
	C3DObject::InitPhysics();								// 0x4623d0
}

C3DBus::~C3DBus()											// 0x4111d0
{
}

namespace neg {
CGameObject *New_C3DBus(void) { return new C3DBus; }		// '3SBU' (0x9bc bytes)
}

void C3DBus::InitObject(void)								// 0x418290
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// +0x110
}

void C3DBus::ev69(void)										// 0x411220
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"bus.ase");		// +0xe0
	CreateTexture((char *)"bus.png", 0);					// +0xfc
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"STOP", 1);								// +0xe8 looped
}

void C3DBus::v260(void)										// 0x411280
{
	C3DAICar::v260();										// 0x40ac30
	if (GetTaskState("DINO") < 10)							// 0x45fea0
	{
		if (GetTaskState("SCENE") > 0x103)
			m_bus_sound = PlayGameSound3DIndex(this, -1, 0xe0, true);	// 0x4589c0
	}
}

void C3DBus::v267(int level)								// 0x4112e0
{
	C3DAnimated::v267(level);								// 0x40e340
	if (GetTaskState("DINO") >= 10 && m_bus_sound != -1)
	{
		StopGameSound(m_bus_sound, false);					// 0x458a00
		m_bus_sound = -1;
	}
}

void C3DBus::v277(void)										// 0x411330
{
	C3DAICar::v277();										// 0x40ac60
	if (m_bus_sound != -1) StopChannel(m_bus_sound, false);	// 0x47d7a0
}

void C3DBus::v278(void)										// 0x411350
{
	C3DAICar::v278();										// 0x40ac80
	if (m_bus_sound != -1)
		m_bus_sound = PlayGameSound3DIndex(this, -1, 0xe0, true);	// 0x4589c0
}

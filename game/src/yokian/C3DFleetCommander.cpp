/*  C3DFleetCommander - see C3DFleetCommander.h. Reconstructed from Neutron.exe. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DFleetCommander.h"
#include "C3DYokHelmet.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DFleetCommander::C3DFleetCommander()								// 0x419760
{
	SetName((char *)"C3DFLEETCOMMANDER");
	Log("C3DFLEETCOMMANDER()");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DFleetCommander::InitObject();						// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	m_helmet = NULL;
	strcpy(f_ac4, "STOP");									// 0x407fa0
	strcpy(f_aec, "WALK");
	strcpy(f_b3c, "STOP");
	strcpy(f_b64, "ATTACK");
	m_mouth = 0;
	m_mouth_time = 0;
	C3DObject::SetObjectID('3FLE');							// 0x462220
	f_008 = 1;												// CGameObject +0x8
	f_bd4 = 0;
	// (then a call to the empty 0x472970)
}

C3DFleetCommander::~C3DFleetCommander()								// 0x4198f0
{
}

namespace neg {
CGameObject *New_C3DFleetCommander(void) { return new C3DFleetCommander; }			// '3FLE' (0x9c8 bytes)
}

void C3DFleetCommander::InitObject(void)						// 0x4173c0 (shared code)
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
	// (then a call to the empty 0x472970)
}

void C3DFleetCommander::Process(float dt)					// 0x419940
{
	C3DYokian::Process(dt);									// 0x44aa00
	if (g_camera) return;									// 0x5099e4: only in cut scenes
	if (!IsAnim((char *)"TALK")) return;					// 0x40e1d0
	m_mouth_time = dt + m_mouth_time;
	if ((double)m_mouth_time > 0.1)							// 0x49cfb8 (double)
	{
		m_mouth_time = 0;
		m_mouth = m_mouth == 0;
		ApplyTexture(f_590, (short)m_mouth);				// element +0xf4 (BG +0x57c)
	}
}

void C3DFleetCommander::ev69(void)							// 0x4199d0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HITALK", (char *)"commandertalk.ase");	// element +0xd8
	CreateAnim((char *)"HIWALK", (char *)"commanderwalk.ase");
	CreateAnim((char *)"HISTOP", (char *)"commanderstop.ase");
	CreateTexture((char *)"comander.png", 0);				// +0xf0
	CreateTexture((char *)"comandertalk.png", 1);
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"WALK", 1);								// +0xe0
	v67(60.0f);												// +0x110: radius
	v64(1.265f);												// +0x100
}

void C3DFleetCommander::v266(void)							// 0x419aa0 (shared code)
{
	// (an empty debug log)
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990, 0x45fea0 (sic: no change)
}

void C3DFleetCommander::v267(int level)						// 0x419ac0
{
	C3DPickupType::v267(level);								// 0x436b80
	ev70(1);												// element +0x110 (BG ESLOT 68): shown
}

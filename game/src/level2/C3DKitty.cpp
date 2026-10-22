/*  C3DKitty - see C3DKitty.h. Reconstructed from Neutron.exe 0x42b800..0x42bbf0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DKitty.h"
#include "../SaveGame.h"
#include "../Sound.h"
#pragma GCC diagnostic pop

using namespace neg;

C3DKitty::C3DKitty()										// 0x42b800
{
	SetName((char *)"C3DKITTY");
	Log("C3DKITTY");
	C3DKitty::InitObject();									// 0x42b9d0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	ev72(m_anim_wag, (char *)"STOP");						// 0x407fa0 (strcpy)
	ev72(m_anim_run, (char *)"WALK");
	ev72(m_anim_sit, (char *)"STOP");
	ev72(m_anim_walk, (char *)"ATTACK");
	C3DObject::SetObjectID('3KIT');							// 0x462220
	m_hover = 0;											// 0x908
	f_008 = 1;
	m_sound = -1;
	// (then a call to the empty 0x472970)
}

C3DKitty::~C3DKitty() {}									// 0x42b980

namespace neg {
CGameObject *New_C3DKitty(void) { return new C3DKitty; }	// '3KIT' (0x9a0 bytes)
}

void C3DKitty::InitObject(void)								// 0x42b9d0
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIATTACK", (char *)"catrun.ase");	// +0xd8
	CreateAnim((char *)"HIWALK", (char *)"catrun.ase");
	CreateAnim((char *)"HISTOP", (char *)"catsit.ase");
	CreateTexture((char *)"cat.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(10.0f);												// CGameObject +0x110: radius
	ev58((char *)"STOP", 1);								// +0xe0
	// (then a call to the empty 0x472970)
}

void C3DKitty::v260(void)									// 0x42ba90
{
	C3DAI::v260();											// 0x409480
	if (GetTaskState(m_task_name) > 9)						// 0x45fea0
	{
		ev70(0);											// element +0x110
		return;
	}
	ev70(1);
	if (m_sound == -1)
		m_sound = PlayGameSound3DIndex(this, -1, 0x53, true);	// 0x4589c0
}

void C3DKitty::v266(void)									// 0x419aa0
{
	// (a call to the empty 0x472970)
	GetTaskState(m_task_name);								// 0x45fea0 (the result is not used)
}

void C3DKitty::v267(int level)								// 0x42bb00
{
	C3DAnimated::v267(level);								// 0x40e340
	if (GetTaskState(m_task_name) > 9)
	{
		StopChannel(m_sound, false);						// 0x47d7a0
		m_sound = -1;
	}
}

void C3DKitty::v277(void)									// 0x42bb40
{
	C3DAnimated::v277();									// 0x40e770
	if (m_sound != -1) StopChannel(m_sound, false);
}

void C3DKitty::v278(void)									// 0x42bb60
{
	C3DAnimated::v278();									// 0x40e790
	if (m_sound != -1)
		m_sound = PlayGameSound3DIndex(this, -1, 0x53, true);
}

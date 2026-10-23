/*  C3DSparrow - see C3DSparrow.h. Reconstructed from Neutron.exe 0x4415a0..0x441970. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSparrow.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DSparrow::C3DSparrow()									// 0x4415a0
{
	SetName((char *)"C3DSPARROW");
	Log("C3DSPARROW");
	C3DSparrow::InitObject();								// 0x4417a0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	v50(0);
	strcpy(m_anim_wag, "STOP");								// 0x407fa0
	strcpy(m_anim_run, "WALK");
	strcpy(m_anim_sit, "STOP");
	strcpy(m_anim_walk, "WALK");
	strcpy(m_anim_fly, "WALK");
	m_ai_state = 6;
	m_keep_fleeing = 0.5f;
	C3DObject::SetObjectID('3SPW');							// 0x462220
	m_hover = 1;
	m_958 = 0;
}

C3DSparrow::~C3DSparrow()									// 0x441750
{
}

namespace neg {
CGameObject *New_C3DSparrow(void) { return new C3DSparrow; }			// '3SPW' (0x99c bytes)
}

void C3DSparrow::InitObject(void)							// 0x4417a0
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	InitAnim();												// element +0x108
	const int level = static_cast<CGameObject *>(g_gametype)->m_object_id;	// g_gametype +0x490
	if (level == 'LEV5')
	{
		CreateAnim((char *)"HIWALK", (char *)"vulture02.ase");	// element +0xd8
		CreateAnim((char *)"HISTOP", (char *)"vulture01.ase");
		CreateTexture((char *)"vulture.png", 0);			// +0xf0
	}
	else if (level < 'LV5A' || level > 'LV5B')
	{
		CreateAnim((char *)"HIWALK", (char *)"sparrow2.ase");
		CreateAnim((char *)"HISTOP", (char *)"sparrow.ase");
		CreateTexture((char *)"sparrow.png", 0);
	}
	else
	{
		CreateAnim((char *)"HIWALK", (char *)"batsfly.ase");
		CreateAnim((char *)"HISTOP", (char *)"batstop.ase");
		CreateTexture((char *)"bats.png", 0);
	}
	ApplyTexture(f_590, 0);									// +0xf4
	v67(10.0f);												// +0x110: radius
	ev58((char *)"WALK", 1);								// +0xe0
}

void C3DSparrow::v267(int level)							// 0x4418e0
{
	C3DAnimated::v267(level);								// 0x40e340
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
	ev70(1);												// element +0x110
}

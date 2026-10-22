/*  C3DYokianSoldier - see C3DYokianSoldier.h. Reconstructed from Neutron.exe. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokianSoldier.h"
#include "C3DYokHelmet.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DYokianSoldier::C3DYokianSoldier()								// 0x44bee0
{
	SetName((char *)"C3DYOKIANSOLDIER");
	Log("C3DYOKIANSOLDIER()");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DYokianSoldier::InitObject();							// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	strcpy(f_7f8, "WALK");									// 0x407fa0
	strcpy(f_820, "WALK");
	strcpy(f_870, "WALK");
	strcpy(f_898, "STOP");
	C3DObject::SetObjectID('3SOL');							// 0x462220
	m_mouth = 0;
	m_mouth_time = 0;
	// (then a call to the empty 0x472970)
}

C3DYokianSoldier::~C3DYokianSoldier()								// 0x44c060
{
}

namespace neg {
CGameObject *New_C3DYokianSoldier(void) { return new C3DYokianSoldier; }			// '3SOL' (0x9c8 bytes)
}

void C3DYokianSoldier::InitObject(void)						// 0x4173c0 (shared code)
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
	// (then a call to the empty 0x472970)
}

void C3DYokianSoldier::Process(float dt)					// 0x44c1c0
{
	C3DYokian::Process(dt);									// 0x44aa00
	if (g_camera) return;									// 0x5099e4: only in cut scenes
	if (!IsAnim((char *)"TALK")) return;					// 0x40e1d0
	m_mouth_time = dt + m_mouth_time;
	if ((double)m_mouth_time > 0.075)						// 0x4c1ff0 (double)
	{
		m_mouth_time = 0;
		m_mouth = m_mouth == 0;
		ApplyTexture(f_590, (short)m_mouth);				// element +0xf4 (BG +0x57c)
	}
}

void C3DYokianSoldier::ev69(void)							// 0x44c0b0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"soldwalk.ase");	// element +0xd8
	CreateAnim((char *)"HISHRINK", (char *)"soldshrink.ASE");
	CreateAnim((char *)"HITALK", (char *)"soldatak.ASE");
	CreateAnim((char *)"HISTOP", (char *)"soldatak.ASE");
	const int level = static_cast<CGameObject *>(g_gametype)->m_object_id;	// g_gametype +0x490
	if (level < 'VR01' || level > 'VR08')
	{
		CreateTexture((char *)"yoksold.png", 0);			// +0xf0
		CreateTexture((char *)"yoksoldtalk.png", 1);
	}
	else
	{
		CreateTexture((char *)"vryoksoldier.png", 0);
		CreateTexture((char *)"vryoksoldier.png", 1);
	}
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"WALK", 1);								// +0xe0
	v67(90.0f);												// +0x110: radius
	v64(1.0f);												// +0x100
}


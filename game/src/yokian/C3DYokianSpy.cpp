/*  C3DYokianSpy - see C3DYokianSpy.h. Reconstructed from Neutron.exe. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokianSpy.h"
#include "C3DYokHelmet.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DYokianSpy::C3DYokianSpy()								// 0x44c2b0
{
	SetName((char *)"C3DYOKIANSPY");
	Log("C3DYOKIANSPY()");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DYokianSpy::InitObject();								// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	m_helmet = new C3DYokHelmet;							// 0x6c4 bytes, 0x44a440
	strcpy(f_ac4, "STOP");									// 0x407fa0
	strcpy(f_aec, "WALK");
	strcpy(f_b3c, "STOP");
	strcpy(f_b64, "ATTACK");
	m_mouth = 0;
	m_mouth_time = 0;
	C3DObject::SetObjectID('3SPY');							// 0x462220
	// (then a call to the empty 0x472970)
}

C3DYokianSpy::~C3DYokianSpy()								// 0x44c480
{
}

namespace neg {
CGameObject *New_C3DYokianSpy(void) { return new C3DYokianSpy; }			// '3SPY' (0x9c8 bytes)
}

void C3DYokianSpy::InitObject(void)						// 0x4173c0 (shared code)
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
	// (then a call to the empty 0x472970)
}

void C3DYokianSpy::Process(float dt)						// 0x44c5d0 (its debug log "SPYANIM %s %s %s" is compiled out)
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

void C3DYokianSpy::ev69(void)								// 0x44c4d0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIATTACK", (char *)"yokcapatak.ase");	// element +0xd8
	CreateAnim((char *)"HIWALK", (char *)"yokcaplook.ase");
	CreateAnim((char *)"HITALK", (char *)"yokcaplook.ase");
	CreateAnim((char *)"HISHRINK", (char *)"yokcapshrink.ASE");
	CreateAnim((char *)"HIBROKE", (char *)"YOKCAPTNBROKEN.ASE");
	CreateAnim((char *)"HISTOP", (char *)"yokcaptnstop.ase");
	CreateTexture((char *)"yokcaptn.png", 0);				// +0xf0
	CreateTexture((char *)"yokcaptntalk.png", 1);
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"WALK", 1);								// +0xe0
	v67(60.0f);												// +0x110: radius
	v64(1.265f);												// +0x100
}


/*  C3DYokianGuard - see C3DYokianGuard.h. Reconstructed from Neutron.exe. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DYokianGuard.h"
#include "../Game.h"
#include "../Globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

C3DYokianGuard::C3DYokianGuard()									// 0x44b220
{
	SetName((char *)"C3DYOKIANGUARD");
	Log("C3DYOKIANGUARD()");
	f_649 = 0;												// animations not loaded (BG +0x635)
	C3DYokianGuard::InitObject();							// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	strcpy(m_anim_wag, "WALK");									// 0x407fa0
	strcpy(m_anim_run, "WALK");
	strcpy(m_anim_sit, "WALK");
	strcpy(m_anim_walk, "STOP");
	C3DObject::SetObjectID('3GUA');							// 0x462220
}

C3DYokianGuard::~C3DYokianGuard()									// 0x44b3a0
{
}

namespace neg {
CGameObject *New_C3DYokianGuard(void) { return new C3DYokianGuard; }		// '3GUA' (0x9c0 bytes)
}

void C3DYokianGuard::InitObject(void)						// 0x4173c0 (shared code)
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
}

void C3DYokianGuard::ev69(void)								// 0x44b3f0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"guardwalk.ASE");	// element +0xd8
	CreateAnim((char *)"HISHRINK", (char *)"guardshrink.ASE");
	CreateAnim((char *)"HISTOP", (char *)"guardatak.ASE");
	CreateTexture((char *)"yokguard.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"WALK", 1);								// +0xe0
	v67(90.0f);												// +0x110: radius
	v64(1.0f);												// +0x100
}

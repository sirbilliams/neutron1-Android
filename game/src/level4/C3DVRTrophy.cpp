/*  C3DVRTrophy - see C3DVRTrophy.h. Reconstructed from Neutron.exe 0x448c60..0x448f80. */
#include "C3DVRTrophy.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../Sound.h"

using namespace neg;

C3DVRTrophy::C3DVRTrophy()									// 0x448c60
{
	SetName((char *)"C3DTROPHY");
	Log("C3DTrophy()");
	f_649 = 0;												// ev69 loads the animation later (BG +0x635)
	C3DVRTrophy::InitObject();								// 0x448e70 (called directly)
	C3DObject::SetObjectID('3TRO');							// 0x462220
	C3DObject::InitPhysics();								// 0x4623d0
	v42(0);
	v40(0);
	v46(0);
	v52(1);
	v34(1);
	f_6bc = 0;
}

C3DVRTrophy::~C3DVRTrophy()									// 0x448dc0
{
}

namespace neg {
CGameObject *New_C3DVRTrophy(void) { return new C3DVRTrophy; }			// '3TRO' (0x6c8 bytes)
}

void C3DVRTrophy::ev69(void)								// 0x448e10
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIDEFAULT", (char *)"trophy.ase");	// element +0xd8
	CreateTexture((char *)"trophy.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DVRTrophy::InitObject(void)							// 0x448e70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	InitAnim();												// element +0x108
	v67(20.0f);												// radius
}

void C3DVRTrophy::Process(float dt)							// 0x448ec0
{
	C3DAnimated::Process(dt);								// 0x40e050
	AddRotation(0, dt * 90.0f, 0);							// +0x334; 0x4a7388
}

void C3DVRTrophy::ResetObject(void)							// 0x448ef0
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	force_show();
}

void C3DVRTrophy::AddTouching(CGameObject *other)			// 0x448f20
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other == g_camera)
	{
		OMediaElement::hide(true);
		AddScore(300);										// 0x42adc0
		PlayGameSoundIndex(-1, 0xe6, false);				// 0x458980
		v136(0);											// +0x224
	}
}

/*  C3DRocketFuel - see C3DRocketFuel.h. Reconstructed from Neutron.exe 0x43d530..0x43d830. */
#include "C3DRocketFuel.h"
#include "../C2DInGameMenu.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include <string.h>

using namespace neg;

C3DRocketFuel::C3DRocketFuel()								// 0x43d530
{
	SetName((char *)"C3DROCKETFUEL");
	Log("C3DROCKETFUEL");
	C3DSprite::InitObject();								// 0x464130 (it has no InitObject of its own)
	SetObjectID('3FUE');									// element slot 48
	strcpy(m_sprite_db, "sprites.omt");						// SpriteDatabase
	m_sprite_index = 40;									// SpriteIndex
	m_sprite_size = 50;										// SpriteSize
	v42(0);
	v40(0);
	v52(1);
	v67(150.0f);											// radius
	v46(0);
	C3DSprite::InitPhysics();								// 0x464210
}

C3DRocketFuel::~C3DRocketFuel()								// 0x43d6c0
{
}

namespace neg {
CGameObject *New_C3DRocketFuel(void) { return new C3DRocketFuel; }	// '3FUE' (0x5f0 bytes)
}

void C3DRocketFuel::AddTouching(CGameObject *other)			// 0x43d700
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;	// +0x18
	OMediaElement::hide(true);								// element +0x58
	const int scene = GetTaskState("SCENE");				// 0x45fea0
	MenuSetItem(2, 26, 1);									// 0x4038c0
	AnnouncePickup(26, 0);									// 0x4061d0
	AddScore(100);											// 0x42adc0
	if (scene == 470)
	{
		SetTaskState("SCENE", 480);							// 0x45f990
		MenuAddPickupCount(25, -1);							// 0x4061c0
		if (MenuPickupCount(25) < 1) MenuClearItem(2, 25);	// 0x4061b0, 0x403910
	}
	v52(0);													// +0xd0
}

void C3DRocketFuel::v266(void)								// 0x43d7b0
{
	force_show();											// (if hidden: hide count 1, then hide(false))
	v234();													// +0x3ac: v136(1)
	v132(0);												// +0x214
}

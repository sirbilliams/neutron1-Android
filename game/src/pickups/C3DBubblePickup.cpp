/*  C3DBubblePickup - see C3DBubblePickup.h. Reconstructed from Neutron.exe 0x410d90..0x411000. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBubblePickup.h"
#include "../C2DInGameMenu.h"
#include "../Game.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// 0x4038c0: set the type of item `item` (0..28) of menu page `page`.
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 29) return;
	// ("Activating Item %d for %d IsActive:%d": debug log)
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;
}

C3DBubblePickup::C3DBubblePickup()							// 0x410d90
{
	SetName((char *)"C3DBUBBLEPICKUP");
	Log("C3DBubblePickup()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3BUP');							// 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 26;
	m_sprite_size = 50;
	v42(0);
	v40(0);
	v52(1);
	v34(1);
	v67(150.0f);											// BG slot 68
	v46(0);
	C3DSprite::InitPhysics();								// 0x464210
	// (then a call to the empty 0x472970)
}

C3DBubblePickup::~C3DBubblePickup() {}						// 0x410f20

namespace neg {
CGameObject *New_C3DBubblePickup(void)	{ return new C3DBubblePickup; }	// factory '3BUP' (0x6d4 bytes)
}

void C3DBubblePickup::AddTouching(CGameObject *other)		// 0x410f60
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DJIMMY"))
	{
		OMediaElement::hide(true);							// element +0x58
		SetItemState(0, 0, 1);									// 0x4038c0: the bubble gadget
		v136(0);												// BG slot 137
		OpenConfirm(10);										// 0x406f90: "try it in VR?"
	}
}

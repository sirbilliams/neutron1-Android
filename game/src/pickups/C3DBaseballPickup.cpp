/*  C3DBaseballPickup - see C3DBaseballPickup.h. Reconstructed from Neutron.exe 0x410090..0x410310. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBaseballPickup.h"
#include "../C2DInGameMenu.h"
#include "../Game.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// 0x403950 / 0x4038c0: the type of item `item` (0..28) of menu page `page`, and its setter.
static short ItemType(short page, short item)
{
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p && item >= 0 && item < 29) return p->items[item].type;
	return 0;
}
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 29) return;
	// ("Activating Item %d for %d IsActive:%d": debug log)
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;
}

C3DBaseballPickup::C3DBaseballPickup()						// 0x410090
{
	SetName((char *)"C3DBASEBALLPICKUP");
	Log("C3DBaseballPickup()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3BPU');							// 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 32;
	m_sprite_size = 50;
	v42(0);
	v40(0);
	v52(1);
	v67(150.0f);											// BG slot 68
	v46(0);
	C3DSprite::InitPhysics();								// 0x464210
	// (then a call to the empty 0x472970)
}

C3DBaseballPickup::~C3DBaseballPickup() {}					// 0x410220

namespace neg {
CGameObject *New_C3DBaseballPickup(void)	{ return new C3DBaseballPickup; }	// factory '3BPU' (0x5f0 bytes)
}

void C3DBaseballPickup::AddTouching(CGameObject *other)		// 0x410260
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DJIMMY") && ItemType(0, 6) == 0)	// 0x403950
	{
		OMediaElement::hide(true);							// element +0x58
		SetItemState(0, 6, 1);								// 0x4038c0: the baseball launcher
		v52(0);												// BG slot 52
		PlayGameSoundIndex(-1, 18, false);					// 0x458980
	}
}

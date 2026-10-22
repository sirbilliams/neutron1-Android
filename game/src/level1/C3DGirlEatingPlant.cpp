/*  C3DGirlEatingPlant - see C3DGirlEatingPlant.h. Reconstructed from Neutron.exe 0x41c2d0..0x41c640. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DGirlEatingPlant.h"
#include "../C2DInGameMenu.h"
#include "../Game.h"
#include "../CGameType.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// 0x4061d0: a pickup collected: the top-right pop-up for picture `pic` (3 s; queued behind the
// one showing), and its count under the pickup icon.
static void PickupPopup(int pic, int index)
{
	if (g_4f815c <= 0)
	{
		g_4f8158 = -1;
		g_4f8154 = pic;
		g_4f815c = 3.0f;
		g_4f8160 = (unsigned int)index;
	}
	else if (g_4f8154 != -1)
	{
		g_4f8158 = pic;
		g_4f7db8[pic * 2]++;
		return;
	}
	g_4f7db8[pic * 2]++;
}

// 0x4038c0: set the type of item `item` (0..28) of menu page `page` ("Activating Item %d for %d
// IsActive:%d" is a compiled-out log).
static void SetItemState(short page, short item, short state)
{
	if (item < 0 || item >= 0x1d) return;
	C2DInGameMenu::MenuPage *p = C2DInGameMenu::Page(page);
	if (p) p->items[item].type = state;						// port: the original does not check the page
}

C3DGirlEatingPlant::C3DGirlEatingPlant()					// 0x41c2d0
{
	SetName((char *)"C3DGIRLEATINGPLANT");
	Log("C3DGIRLEATINGPLANT");
	f_649 = 0;												// animations not loaded (BG +0x635)
	m_is_pickup = 1;
	C3DGirlEatingPlant::InitObject();						// 0x4173c0 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	strcpy(m_anim_wag, "STOP");									// 0x407fa0
	strcpy(m_anim_run, "WALK");
	strcpy(m_anim_sit, "STOP");
	strcpy(m_anim_walk, "WALK");
	m_keep_fleeing = 0.5f;
	C3DObject::SetObjectID('3GIR');							// 0x462220
	m_hover = 0;
	m_958 = 1;
	m_pic_number = 2;
	// (then a call to the empty 0x472970)
}

C3DGirlEatingPlant::~C3DGirlEatingPlant()					// 0x41c470
{
}

namespace neg {
CGameObject *New_C3DGirlEatingPlant(void) { return new C3DGirlEatingPlant; }	// '3GIR' (0x9a8 bytes)
}

void C3DGirlEatingPlant::InitObject(void)					// 0x4173c0 (shared code)
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
	// (then a call to the empty 0x472970)
}

void C3DGirlEatingPlant::ev69(void)							// 0x41c4c0
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"HIWALK", (char *)"plantwalk.ase");	// element +0xd8
	CreateAnim((char *)"HISHRINK", (char *)"plantshrink.ASE");
	CreateAnim((char *)"HISTOP", (char *)"plantwait.ase");
	CreateTexture((char *)"plant.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4 (BG +0x57c)
	ev58((char *)"STOP", 1);								// +0xe0
}

void C3DGirlEatingPlant::AddTouching(CGameObject *other)	// 0x41c550
{
	C3DAI::AddTouching(other);								// (BG C3DAI slot 16)
	// ("MEBTEMP GEP, %d" and three more compiled-out logs)
	if (!m_state_locked) return;										// not shrunk
	if (other != g_camera) return;
	if (g_object_states[m_pickup_index] != 0) return;		// 0x4f8438: already collected
	hide(true);												// element +0x58
	v232();													// +0x3a4 (BG slot 233): no collision
	AddScore(50);											// BG 0x42adc0
	if (m_pic_number < 0) return;
	PickupPopup(m_pic_number, m_pickup_index);				// 0x4061d0
	SetItemState(2, (short)m_pic_number, 1);				// 0x4038c0
	g_object_states[m_pickup_index] = 1;
}

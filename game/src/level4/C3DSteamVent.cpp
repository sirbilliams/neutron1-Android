/*  C3DSteamVent - see C3DSteamVent.h. Reconstructed from Neutron.exe 0x442920..0x4432e0. */
#include "C3DSteamVent.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../Sound.h"
#include "../level1/C3DLightCone.h"
#include "OMedia3DMaterial.h"
#include <list>
#include <string.h>
#include <strings.h>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// The button's material (2002 +0x4c..+0x58, stored r, g, b, a).
static void SetButtonColour(OMedia3DMaterial *m, float r, float g, float b)
{
	m->set_diffuse(OMediaFARGBColor(1.0f, r, g, b));
}

C3DSteamVent::C3DSteamVent()								// 0x442920
{
	SetName((char *)"C3DSTEAMVENT");
	C3DSteamVent::InitObject();								// 0x442b70 (called directly)
	C3DObject::SetObjectID('3STE');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	C3DObject::InitPhysics();								// 0x4623d0
	m_green = 0.1f;
	m_blue = 0.1f;
	m_red = 1.0f;
	m_available = 0;
	m_na_sound = -1;
	m_avail_sound = -1;
	strcpy(m_next_button, "none");
	m_lightcone = new C3DLightCone;							// 0x42d410 (0x6dc bytes)
	m_lightcone->v107(0);									// +0x1b0
	v34(1);
}

C3DSteamVent::~C3DSteamVent()								// 0x442b20
{
}

namespace neg {
CGameObject *New_C3DSteamVent(void) { return new C3DSteamVent; }		// '3STE' (0x748 bytes)
}

void C3DSteamVent::InitObject(void)							// 0x442b70
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("Red", &m_red, PROP_FLOAT, 0);
	RegisterProperty("Green", &m_green, PROP_FLOAT, 0);
	RegisterProperty("Blue", &m_blue, PROP_FLOAT, 0);
	RegisterProperty("ButtonAvailable", &m_available, PROP_LONG, 0);
	RegisterProperty("NASound", &m_na_sound, PROP_LONG, 0);
	RegisterProperty("AvailSound", &m_avail_sound, PROP_LONG, 0);
	RegisterProperty("NextButton", m_next_button, PROP_ALPHA, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDOWN", (char *)"buttondown.ase");	// +0xd8
	CreateAnim((char *)"HIUP", (char *)"buttonup.ase");
	CreateTexture((char *)"switch.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(500.0f);											// radius
	ev58((char *)"UP", 1);									// +0xe0
}

void C3DSteamVent::AddTouching(CGameObject *other)			// 0x442cd0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!g_camera) return;
	if (other != g_camera && !other->NameContains((char *)"C3DBASEBALL")) return;
	if (m_available == 0)
	{
		if (m_na_sound != -1) PlayGameSoundIndex(-1, m_na_sound, false);	// 0x458980
		return;
	}
	if (strcasecmp(GetAnim(), "HIUP") != 0)					// +0xe8: not up (pressed): up again
	{
		ev58((char *)"UP", 1);
		SetButtonColour(f_5d4[0], m_red, m_green, m_blue);
		return;
	}
	AddScore(50);											// 0x42adc0
	ev58((char *)"DOWN", 1);
	if (m_avail_sound != -1) PlayGameSoundIndex(-1, m_avail_sound, false);
	SetButtonColour(f_5d4[0], m_red * 0.5f, m_green * 0.5f, m_blue * 0.5f);	// 0x48d948
	m_available = 0;
	CGameObject *target = FindObjectByTag(m_next_button);	// 0x474070
	m_lightcone->hide(true);								// element +0x58
	m_lightcone->m_off = 1;									// +0x6d0
	if (target)
	{
		if (!target->NameContains((char *)"C3DSTEAMVENT"))
		{
			CLocalGameObject *t = dynamic_cast<CLocalGameObject *>(target);
			if (t) t->v268(0);								// +0x428
		}
		else
		{
			C3DSteamVent *v = dynamic_cast<C3DSteamVent *>(target);
			if (v) v->ev73();								// element +0x124
		}
	}
}

void C3DSteamVent::ev73(void)								// 0x442ed0
{
	m_available = 1;
	m_lightcone->force_show();								// (if hidden: hide count 1, then hide(false))
	SetButtonColour(f_5d4[0], m_red, m_green, m_blue);
}

void C3DSteamVent::Process(float dt)						// 0x442f50
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (m_available == 0)
	{
		m_lightcone->m_red = 1.0f;
		m_lightcone->m_green = 0.1f;
		m_lightcone->m_blue = 0.1f;
		SetButtonColour(f_5d4[0], 0.2f, 0.2f, 0.2f);
	}
	else
	{
		m_lightcone->m_off = 0;
		m_lightcone->m_red = 0.1f;
		m_lightcone->m_green = 1.0f;
		m_lightcone->m_blue = 0.1f;
		const float pulse = MenuPulse();					// 0x4073a0
		SetButtonColour(f_5d4[0], (float)(pulse * 0.5f + 0.5), 0.2f, 0.2f);	// 0x48d948, 0x48e5d0
	}
	const OMedia3DVector p = v224(0, 127.0f, -16.0f);		// +0x384: the cone's place
	m_lightcone->v196(p.x, p.y, p.z, p.w);					// cone +0x314
	(void)v224(0, -19.0f, -62.3f);							// (the result is not used)
}

void C3DSteamVent::v258(void)								// 0x443100
{
	CLocalGameObject::v258();								// 0x458940
	m_lightcone->m_red = m_red;
	m_lightcone->m_green = m_green;
	m_lightcone->m_blue = m_blue;
	if (strcasecmp(GetAnim(), "HIUP") == 0)
	{
		if (m_available == 0) SetButtonColour(f_5d4[0], 0.2f, 0.0f, 0.0f);
		else SetButtonColour(f_5d4[0], m_red, m_green, m_blue);
	}
	if (strcasecmp(GetAnim(), "HIDOWN") == 0)
	{
		ev58((char *)"UP", 1);
		SetButtonColour(f_5d4[0], m_red * 0.5f, m_green * 0.5f, m_blue * 0.5f);
	}
	m_lightcone->m_off = 0;
}

void C3DSteamVent::v260(void)								// 0x4432c0
{
	C3DAnimated::v260();									// 0x40e7b0
	v258();													// +0x404
	v67(100.0f);											// radius
}

/*  C3DSwitch - see C3DSwitch.h. Reconstructed from Neutron.exe 0x4449c0..0x4450e0. */
#include "C3DSwitch.h"
#include "../C2DInGameMenu.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../Sound.h"
#include "../objects/C3DCorona.h"
#include "OMedia3DMaterial.h"
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

C3DSwitch::C3DSwitch()										// 0x4449c0
{
	SetName((char *)"C3DSWITCH");
	C3DSwitch::InitObject();								// 0x444cb0 (called directly)
	C3DObject::SetObjectID('3SWI');							// 0x462220
	v64(0.05f);
	v66(1000.0f);
	v46(0);
	v52(1);
	C3DObject::InitPhysics();								// 0x4623d0
	strcpy(m_switch_object, "none");
	m_my_state = 0;
	m_corona = new C3DCorona;								// 0x4154b0 (0x5fc bytes)
	m_corona->m_red = 0.1f;									// corona 0x5e8..0x5f0
	m_corona->m_green = 1.0f;
	m_corona->m_blue = 0.1f;
	m_corona->set_diffuse(OMediaFARGBColor(1.0f, 0.2f, 1.0f, 0.2f));	// +0x90..+0x9c (r, g, b, a)
	v34(1);
	m_time = 0;
	m_toggle = -1;
}

C3DSwitch::~C3DSwitch()										// 0x444c40
{
}

namespace neg {
CGameObject *New_C3DSwitch(void) { return new C3DSwitch; }	// '3SWI' (0x734 bytes)
}

void C3DSwitch::ResetObject(void)							// 0x444c90
{
	CLocalGameObject::ResetObject();						// 0x4587f0
	v67(140.0f);											// radius
}

void C3DSwitch::InitObject(void)							// 0x444cb0
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("MyState", &m_my_state, PROP_LONG, 0);
	RegisterProperty("SwitchObject", m_switch_object, PROP_ALPHA, 0);
	RegisterProperty("Toggle", &m_toggle, PROP_LONG, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"switch.ase");	// +0xd8
	CreateTexture((char *)"switch.png", 0);					// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(300.0f);											// radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

void C3DSwitch::Process(float dt)							// 0x444d90
{
	C3DAnimated::Process(dt);								// 0x40e050
	float p[4];
	v195(p);												// +0x310
	m_corona->v196(p[0], p[1] + 210.0f, p[2], p[3]);		// corona +0x314: 0x4b9900
	m_time = dt + m_time;
	const float pulse = MenuPulse();						// 0x4073a0
	f_5d4[0]->set_diffuse(OMediaFARGBColor(1.0f, (float)(pulse * 0.5f + 0.5), 0.2f, 0.2f));	// +0x4c..+0x58 (r, g, b, a); 0x48d948, 0x48e5d0
}

void C3DSwitch::AddTouching(CGameObject *other)				// 0x444e60
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other != g_camera && !other->NameContains((char *)"C3DBASEBALL")) return;
	int arg;
	if (m_toggle == -1)
	{
		if (m_my_state == 0)
		{
			m_my_state = 1;
			v268(1);										// +0x428
			PlayGameSoundIndex(-1, 0x75, false);			// 0x458980
			return;
		}
		m_my_state = 0;
		arg = 0;
	}
	else
	{
		if (m_my_state == 1)
		{
			AddScore(25);									// 0x42adc0
			PlayGameSoundIndex(-1, 0x75, false);
		}
		arg = m_toggle;
	}
	v268(arg);
	PlayGameSoundIndex(-1, 0x75, false);
}

void C3DSwitch::v268(int state)								// 0x444f20
{
	CGameObject *target;
	CLocalGameObject *t;
	if (state == 0)
	{
		m_my_state = 0;
		target = FindObjectByTag(m_switch_object);			// 0x474070
		if (target)
		{
			if ((t = dynamic_cast<CLocalGameObject *>(target)) != NULL) t->v268(0);	// +0x428
			m_corona->hide(true);
			OMediaElement::hide(true);
			if (target->NameContains((char *)"C3DFAN"))
				PlayGameSoundIndex(-1, 0x29, false);
		}
	}
	else
	{
		m_my_state = 1;
		target = FindObjectByTag(m_switch_object);
		if (target)
		{
			if ((t = dynamic_cast<CLocalGameObject *>(target)) != NULL) t->v268(1);
			m_corona->set_diffuse(OMediaFARGBColor(1.0f, 0.2f, 1.0f, 0.2f));	// +0x90..+0x9c
			m_corona->force_show();
			if (target->NameContains((char *)"C3DFAN"))
				PlayGameSoundIndex(-1, 0x20, false);
		}
	}
}

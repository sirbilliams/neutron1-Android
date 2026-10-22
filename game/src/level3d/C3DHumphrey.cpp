/*  C3DHumphrey - see C3DHumphrey.h. Reconstructed from Neutron.exe 0x420730..0x420de0 (and
    the shared 0x419aa0). */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DHumphrey.h"
#include "../Game.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <stdio.h>
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

// CLONE1..CLONE7 (unrolled in the original): shown (element +0x110), then state `state` (+0x124).
static void ShowClones(int state)
{
	for (int i = 1; i <= 7; i++)
	{
		char tag[8];
		snprintf(tag, sizeof tag, "CLONE%d", i);
		C3DAI *c = dynamic_cast<C3DAI *>(FindObjectByTag(tag));	// (o - 0xc0; port: checked)
		if (!c) continue;
		c->ev70(1);
		c->ev73(state);
	}
}

C3DHumphrey::C3DHumphrey()									// 0x420730
{
	SetName((char *)"C3DHUMPHREY");
	Log("C3DHUMPHREY()");
	C3DHumphrey::InitObject();								// 0x420950 (called directly)
	C3DObject::InitPhysics();								// 0x4623d0
	v64(0.6f);												// 0x4711e0
	C3DAI::ev72(m_anim_wag, (char *)"STOP");
	C3DAI::ev72(m_anim_run, (char *)"WALK");
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	C3DAI::ev72(m_anim_walk, (char *)"ATTACK");
	C3DObject::SetObjectID('3HUM');							// 0x462220
	m_hover = 0;												// no hovering
	m_958 = 1;
	f_008 = 1;
}

C3DHumphrey::~C3DHumphrey()									// 0x4208c0
{
	// ("Deleting Humphrey": debug log)
}

namespace neg {
CGameObject *New_C3DHumphrey(void) { return new C3DHumphrey; }			// '3HUM' (0x9a8 bytes)
}

void C3DHumphrey::InitObject(void)							// 0x420950
{
	Log("InitObject()");
	C3DPickupType::InitObject();							// 0x436ac0
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIATTACK", (char *)"humprun.ase");	// +0xd8
	CreateAnim((char *)"HIWALK", (char *)"humpwalk.ase");
	CreateAnim((char *)"HISHRINK", (char *)"humpshrink.ASE");
	CreateAnim((char *)"HIRUNSHRUNK", (char *)"humprunshrunk.ASE");
	CreateAnim((char *)"HIGROW", (char *)"humpgrow.ASE");
	CreateAnim((char *)"HISTOP", (char *)"humpsleep.ase");
	CreateAnim((char *)"HISTOP2", (char *)"humpstop.ase");
	CreateTexture((char *)"humphrey.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	v67(20.0f);												// CGameObject +0x110: radius
	ev58((char *)"STOP", 1);								// +0xe0
}

void C3DHumphrey::v260(void)								// 0x420d60
{
	C3DPickupType::v260();									// 0x436b10
	GetTaskState("SCENE");									// 0x45fea0 (the result is not used)
	ev70(0);												// element +0x110
}

void C3DHumphrey::v266(void)								// 0x419aa0 (shared code)
{
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990 / 0x45fea0 (writes back what it read)
}

void C3DHumphrey::v267(int level)							// 0x420a60
{
	C3DAnimated::v267(level);								// 0x40e340
	if (GetTaskState("SCENE") == 90) ShowClones(1);
}

void C3DHumphrey::ev90(void)								// 0x420bf0
{
	if (GetTaskState("SCENE") == 90) ShowClones(0);
}

/*  C3DToolChest - see C3DToolChest.h. Reconstructed from Neutron.exe 0x445f30..0x4462c0. */
#include "C3DToolChest.h"
#include "../C2DInGameMenu.h"
#include "../C3DTriggerType.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
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

C3DToolChest::C3DToolChest()								// 0x445f30
{
	SetName((char *)"C3DTOOLCHEST");
	Log("C3DToolChest()");
	C3DToolChest::InitObject();								// 0x4460d0 (called directly)
	C3DObject::SetObjectID('3TOL');							// 0x462220
	v107(1);												// +0x1b0
	v52(1);
	C3DObject::InitPhysics();								// 0x4623d0
	v44(0);
	v34(1);
	// (then a call to the empty 0x472970)
}

C3DToolChest::~C3DToolChest()								// 0x446080
{
}

namespace neg {
CGameObject *New_C3DToolChest(void) { return new C3DToolChest; }		// '3TOL' (0x6c4 bytes)
}

void C3DToolChest::InitObject(void)							// 0x4460d0
{
	Log("C3DToolchest::InitObject()");
	C3DAnimated::InitObject();
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIOPEN", (char *)"toolchestopen.ase");		// +0xd8
	CreateAnim((char *)"HINOTOOLS", (char *)"toolchestopen2.ase");
	CreateAnim((char *)"HICLOSED", (char *)"toolchestclosed.ase");
	CreateTexture((char *)"toolchest.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev58((char *)"CLOSED", 1);								// +0xe0
	v67(70.0f);												// +0x110: radius
	// (then a call to the empty 0x472970)
}

void C3DToolChest::AddTouching(CGameObject *other)			// 0x446190
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (!other->NameContains((char *)"C3DJIMMY")) return;
	if (GetTaskState("SCENE") != 0xdc) return;				// 0x45fea0
	ev58((char *)"NOTOOLS", 1);
	SetTaskState("SCENE", 0xe6);							// 0x45f990
	MenuSetItem(2, 0x12, 1);								// 0x4038c0
	MenuSetItem(0, 5, 1);
	AnnouncePickup(0x12, 0);								// 0x4061d0
	CGameObject *cam = FindObjectByTag("GOTKEY");			// 0x474070
	if (cam && cam->NameContains((char *)"C3DCUTSCENECAMERA"))
	{
		C3DTriggerType *t = dynamic_cast<C3DTriggerType *>(cam);
		if (t) t->CallNextTrigger("GOTKEY", 0);				// element +0xd4 (BG slot 53)
	}
}

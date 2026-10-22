/*  CPickupType - see CPickupType.h. Reconstructed from Neutron.exe 0x45f040..N2:0x496fe0 (and the
    shared v266 0x419aa0). */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CPickupType.h"
#include "../CJimmyGame.h"
#include "../SaveGame.h"
#include "../Game.h"
#include "../Globals.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

CPickupType::CPickupType()									// 0x45f040
{
	SetName((char *)"CPICKUPTYPE");
	m_pickup_index = 0;
	m_pic_number = -1;
	m_required_level = 0;
	m_exact_level = -1;
}

CPickupType::~CPickupType() {}								// 0x45f130

int neg::PickupIndexOf(CGameObject *o)
{
	CPickupType *p = o ? dynamic_cast<CPickupType *>(o) : NULL;
	return p ? p->m_pickup_index : -1;
}

void CPickupType::InitObject(void)							// 0x45f170
{
	C3DTriggerType::InitObject();
	RegisterProperty("PickupIndex", &m_pickup_index, PROP_LONG, 0);
	RegisterProperty("PIC_NUMBER", &m_pic_number, PROP_LONG, 0);
	RegisterProperty("RequiredLevel", &m_required_level, PROP_LONG, 0);
	RegisterProperty("ExactLevel", &m_exact_level, PROP_LONG, 0);
}

void CPickupType::v242(float t)								// 0x45f280
{
	C3DTriggerType::v242(t);								// 0x447a70
	ShowAgain();											// element +0xd4
}

void CPickupType::v260(void)								// 0x45f1f0
{
	C3DSpriteType::v260();									// 0x464810
	CJimmyGame *g = static_cast<CJimmyGame *>(g_gametype);	// every level game type is one
	m_pickup_index = g->NextPickupIndex();					// element +0x160
	// (debug log "PostLoad::PI=%d")
	if (g_object_states[m_pickup_index] == 0)				// 0x4f8438
	{
		ShowAgain();										// element +0xd4
		v124(1);											// +0x1f0
		v234();												// +0x3a8
		return;
	}
	HideAgain();											// element +0xd8
	v124(0);
	v232();													// +0x3a0
}

void CPickupType::v266(void)								// 0x419aa0
{
	// (an empty debug log)
	SetTaskState(m_task_name, GetTaskState(m_task_name));	// 0x45f990, 0x45fea0 (sic: no change)
}

// 0x45f2b0: with a TaskName (and not collected), shown while the task's state is ExactLevel, or
// (ExactLevel -1) at least RequiredLevel (-1: leave it); otherwise hidden. The level argument
// is not used.
void CPickupType::v267(int level)
{
	CLocalGameObject::v267(level);
	if (strcasecmp(m_task_name, "none") == 0) return;
	if (g_object_states[m_pickup_index] != 0) return;
	const int state = GetTaskState(m_task_name);
	bool show;
	if (m_exact_level == -1)
	{
		if (m_required_level == -1) return;
		show = m_required_level <= state;
	}
	else show = m_exact_level == state;
	if (show)
	{
		ShowAgain();
		v124(1);
		v234();
		return;
	}
	HideAgain();
	v124(0);
	v232();
}

/*  C3DPickupType - see C3DPickupType.h. Reconstructed from Neutron.exe 0x436980..0x436c40. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DPickupType.h"
#include "../CJimmyGame.h"
#include "../Globals.h"
#include "../SaveGame.h"
#pragma GCC diagnostic pop
#include <strings.h>

using namespace neg;

C3DPickupType::C3DPickupType()								// 0x436980
{
	m_pickup_index = 0;
	m_pic_number = -1;
	m_is_pickup = 0;
}

C3DPickupType::~C3DPickupType()								// 0x436a70
{
}

void C3DPickupType::InitObject(void)						// 0x436ac0
{
	C3DAI::InitObject();									// 0x407ee0
	if (!m_is_pickup) return;
	RegisterProperty("PickupIndex", &m_pickup_index, PROP_LONG, 0);
	RegisterProperty("PIC_NUMBER", &m_pic_number, PROP_LONG, 0);
}

void C3DPickupType::v260(void)								// 0x436b10
{
	C3DAI::v260();											// (BG slot 259)
	if (!m_is_pickup) return;
	CJimmyGame *g = static_cast<CJimmyGame *>(g_gametype);	// every level game type is one
	m_pickup_index = g->NextPickupIndex();					// element +0x154
	// (debug log "PostLoad::PI=%d")
	if (g_object_states[m_pickup_index] == 0)				// 0x4f8438
	{
		ev70(1);											// element +0x110 (BG slot 68): shown, with collision
		return;
	}
	ev70(0);
}

void C3DPickupType::v267(int level)							// 0x436b80
{
	C3DAnimated::v267(level);								// (BG slot 265)
	if (!m_is_pickup) return;
	if (strcasecmp(m_task_name, "none") != 0 && g_object_states[m_pickup_index] != 0)	// 0x487c20
		ev70(0);											// element +0x110
}

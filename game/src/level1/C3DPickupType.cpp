/*  C3DPickupType - see C3DPickupType.h. Reconstructed from Neutron.exe 0x436980..0x436c30. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DPickupType.h"
#include "../CGameType.h"
#include "../CJimmyGame.h"
#include "../SaveGame.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <strings.h>

using namespace neg;

C3DPickupType::C3DPickupType()								// 0x436980
{
	f_994 = 0;
	f_998 = -1;
	f_99c = 0;
}

C3DPickupType::~C3DPickupType()								// 0x436a70
{
}

void C3DPickupType::InitObject(void)						// 0x436ac0
{
	C3DAI::InitObject();									// 0x407ee0
	if (!f_99c) return;
	RegisterProperty("PickupIndex", &f_994, PROP_LONG, 0);
	RegisterProperty("PIC_NUMBER", &f_998, PROP_LONG, 0);
}

void C3DPickupType::v260(void)								// 0x436b10
{
	C3DAI::v260();											// 0x409480
	if (!f_99c) return;
	CJimmyGame *g = static_cast<CJimmyGame *>(g_gametype);	// every level game type is one
	f_994 = g->NextPickupIndex();							// element +0x154
	// ("PostLoad::PI=%d": debug log)
	ev70(g_object_states[f_994] == 0);						// element +0x110
}

void C3DPickupType::v267(int level)							// 0x436b80
{
	C3DAnimated::v267(level);								// 0x40e340
	if (!f_99c) return;
	if (strcasecmp(m_task_name, "none") != 0 && g_object_states[f_994] != 0) ev70(0);
}

/*  C3DAIOmtObj - see C3DAIOmtObj.h. Reconstructed from Neutron.exe 0x40ae30..0x40b1a0. */
#include "C3DAIOmtObj.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Globals.h"
#include "OMedia3DShape.h"
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- construction

C3DAIOmtObj::C3DAIOmtObj()									// 0x40ae30
{
	SetName((char *)"C3DAIOMTOBJ");							// 0x4702d0
	Log("C3DAIOMTOBJ()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	RegisterProperty("OmtDatabase", m_omt_database, PROP_ALPHA, 0);
	RegisterProperty("OmtIndex", &m_omt_index, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	RegisterProperty("TerrainColl", &m_terrain_coll, PROP_LONG, 0);
	C3DObject::InitPhysics();								// 0x4623d0 (called directly)
	C3DObject::SetObjectID('3AIO');							// 0x462220
	v67(5.0f);
	v152(5.0f);
	v52(1);
	strcpy(m_omt_database, "objects.omt");
	m_omt_index = 4;
	f_648 = 0;												// 0x634 in Neutron.exe: C3DAnimated's "animations on" (tree: Negatron's layout)
	m_terrain_coll = -1;
	v34(1);
	// (then a call to the empty 0x472970)
}

C3DAIOmtObj::~C3DAIOmtObj()									// 0x40b020
{
}

namespace neg {
CGameObject *New_C3DAIOmtObj(void) { return new C3DAIOmtObj; }			// '3AIO' (0xa08 bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DAIOmtObj::v260(void)								// 0x40b070
{
	C3DAnimated::v260();									// 0x40e7b0 (sic: not C3DAI's)
	m_db = GetDatabase(m_omt_database);						// 0x46a910
	set_shape(Load3DShape(m_db, m_omt_index));				// element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0
	v67(f_034);												// the radius
	if (m_terrain_coll == 0)								// no terrain collision: hover
	{
		m_hover = 1;
		v42(0);
		v40(0);
	}
	if (m_terrain_coll == 1)
	{
		v42(1);
		v40(1);
	}
}

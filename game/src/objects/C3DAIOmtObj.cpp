/*  C3DAIOmtObj - see C3DAIOmtObj.h. Reconstructed from Neutron.exe 0x40ae30..0x40b1a0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DAIOmtObj.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include <string.h>

using namespace neg;

C3DAIOmtObj::C3DAIOmtObj()									// 0x40ae30
{
	SetName((char *)"C3DAIOMTOBJ");							// 0x4702d0
	Log("C3DAIOMTOBJ()");
	C3DAI::InitObject();									// 0x407ee0 (called directly)
	RegisterProperty("OmtDatabase", m_omt_database, PROP_ALPHA, 0);
	RegisterProperty("OmtIndex", &m_omt_index, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	RegisterProperty("TerrainColl", &m_terrain_coll, PROP_LONG, 0);
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3AIO');							// 0x462220
	v67(5.0f);
	v152(5.0f);
	v52(1);
	strcpy(m_omt_database, "objects.omt");
	m_omt_index = 4;
	f_648 = 0;												// animations off
	m_terrain_coll = -1;
	v34(1);
}

C3DAIOmtObj::~C3DAIOmtObj()									// 0x40b020
{
}

namespace neg {
CGameObject *New_C3DAIOmtObj(void) { return new C3DAIOmtObj; }			// '3AIO' (0xa08 bytes)
}

void C3DAIOmtObj::v260(void)								// 0x40b070
{
	C3DAnimated::v260();									// 0x40e7b0 (not C3DAI's)
	m_db = GetDatabase(m_omt_database);						// 0x46a910
	set_shape(Load3DShape(m_db, m_omt_index));				// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	v67(f_034);												// +0x110: radius = "Radius"
	if (m_terrain_coll == 0)
	{
		f_bd4 = 1;											// floating: no gravity
		v42(0);
		v40(0);
	}
	if (m_terrain_coll == 1)
	{
		v42(1);
		v40(1);
	}
}

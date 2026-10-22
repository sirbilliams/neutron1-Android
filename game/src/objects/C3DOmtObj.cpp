/*  C3DOmtObj - see C3DOmtObj.h. Reconstructed from Neutron.exe 0x434530..0x434870. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DOmtObj.h"
#include "../CGameType.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include <string.h>

using namespace neg;

C3DOmtObj::C3DOmtObj()										// 0x434530
{
	SetName((char *)"C3DOMTOBJ");							// 0x4702d0
	Log("C3DOMTOBJ()");
	C3DAnimated::InitObject();								// 0x40d3c0 (called directly)
	v67(5.0f);												// radius
	RegisterProperty("OmtDatabase", f_924, PROP_ALPHA, 0);
	RegisterProperty("OmtIndex", &f_988, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	C3DObject::InitPhysics();								// 0x4623d0
	C3DObject::SetObjectID('3OMT');							// 0x462220
	v67(5.0f);
	v152(5.0f);
	v52(1);
	strcpy(f_924, "objects.omt");
	f_988 = 4;
	f_648 = 0;												// animations off
	v34(1);
}

C3DOmtObj::~C3DOmtObj()										// 0x434700
{
}

namespace neg {
CGameObject *New_C3DOmtObj(void) { return new C3DOmtObj; }				// '3OMT' (0x72c bytes)
}

void C3DOmtObj::v260(void)									// 0x434750
{
	m_db = GetDatabase(f_924);								// 0x46a910
	set_shape(Load3DShape(m_db, f_988));					// 0x477ba0, element +0xac
	MakeShapeUnLit(get_shape());							// element +0xb0, 0x477550
	if (f_664 == 1) AlphaSecondPass();								// "SecondPass": element +0x118
	if (f_658 == 0)											// "HasCollision" 0
	{
		v136(0);											// +0x224
		v42(0);												// +0xa8
		v40(0);												// +0xa0
		v132(0);											// +0x214
	}
	C3DAnimated::v260();									// 0x40e7b0
}

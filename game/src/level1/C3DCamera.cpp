/*  C3DCamera - see C3DCamera.h. Reconstructed from Neutron.exe 0x4610e0..0x461300. */
#include "C3DCamera.h"
#include "../gen/globals.h"

using namespace neg;

C3DCamera *neg::CameraProbe(void)
{
	return (C3DCamera *)g_4fc69c;
}

C3DCamera::C3DCamera()										// 0x4610e0
{
	v46(0);
	v52(0);
	InitCube(OMedia3DVector(1.0f, 1.0f, 1.0f, 1.0f), 75.0f);	// element +0xe4
	HideAgain();											// N2:0x49b120
	v107(0);
	g_4fc69c = (C3DObject *)this;
}

C3DCamera::~C3DCamera()										// N2:0x499510
{
	g_4fc69c = NULL;
}

void C3DCamera::v219(float, float, float, float, OMedia3DPolygon *) {}	// 0x461290
void C3DCamera::v220(float, float, float, float, OMedia3DPolygon *) {}	// 0x461290

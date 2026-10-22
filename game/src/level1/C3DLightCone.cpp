/*  C3DLightCone - see C3DLightCone.h. Reconstructed from Neutron.exe 0x42d410..0x42db90. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DLightCone.h"
#include "../objects/C3DCorona.h"
#include "../Globals.h"
#include "../Game.h"
#pragma GCC diagnostic pop
#include "OMedia3DMaterial.h"
#include "OMediaViewPort.h"

using namespace neg;

C3DLightCone::C3DLightCone()								// 0x42d410
{
	SetName((char *)"C3DLIGHTCONE");						// 0x4702d0
	Log("C3DLIGHTCONE()");
	C3DLightCone::InitObject();								// 0x42d620 (called directly)
	C3DObject::SetObjectID('3LCO');							// 0x462220
	v46(0);
	v52(0);
	v232();
	C3DObject::InitPhysics();								// 0x4623d0
	m_rot_speed = 80.0f;
	m_corona = new C3DCorona;								// 0x4154b0 (0x5fc bytes)
	m_red = 1.0f;
	m_green = 1.0f;
	m_blue = 1.0f;
	m_off = 0;
}

C3DLightCone::~C3DLightCone()								// 0x42d5d0
{
}

void C3DLightCone::InitObject(void)							// 0x42d620
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d3c0
	RegisterProperty("RotSpeed", &m_rot_speed, PROP_FLOAT, 0);
	RegisterProperty("Red", &m_red, PROP_FLOAT, 0);
	RegisterProperty("Green", &m_green, PROP_FLOAT, 0);
	RegisterProperty("Blue", &m_blue, PROP_FLOAT, 0);
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"lightcone.ase");	// +0xd8
	CreateTexture((char *)"lightcone.png", 0);				// +0xf0
	ApplyTexture(f_590, 0);									// +0xf4
	ev65(0, 6, 1);											// +0xfc: blend (src alpha, one)
	ev71(0.5f);												// +0x114: alpha
	v67(100.0f);											// CGameObject +0x110: radius
	ev58((char *)"DEFAULT", 1);								// +0xe0
}

// The beam's material diffuse (2002 +0x4c, stored r, g, b, a).
static void SetBeamColour(OMedia3DMaterial *m, float r, float g, float b)
{
	if (m) m->set_diffuse(OMediaFARGBColor(1.0f, r, g, b));
}

void C3DLightCone::Process(float dt)						// 0x42d740
{
	C3DAnimated::Process(dt);								// 0x40e050
	if (m_off)
	{
		OMediaElement::hide(true);							// +0x58
		m_corona->hide(true);
		return;
	}
	force_show();											// inline: hide_count (+0x70) = 1, hide(false)
	m_corona->force_show();
	float r[4];
	v201(r);												// +0x328: our rotation
	const float rz = r[2];
	const float yaw = dt * m_rot_speed;
	v201(r);
	AddRotation(r[0], yaw, rz);								// +0x334 (sic: x and z added again)
	const OMedia3DVector cam(g_viewport->get_x(), g_viewport->get_y(), g_viewport->get_z(), 0);	// 0x509a50 +0x44..
	float p[4];
	v195(p);												// +0x310: our position
	OMedia3DVector d(cam.x - p[0], cam.y - p[1], cam.z - p[2], 0);
	OMedia3DVector f = v226(0, 0, 1.0f);					// +0x388: the way the beam points
	float inv = 1.0f / f.quick_magnitude();
	f.x *= inv; f.y *= inv; f.z *= inv;
	inv = 1.0f / d.quick_magnitude();
	d.x *= inv; d.y *= inv; d.z *= inv;
	float c = f.z * d.z + f.y * d.y + f.x * d.x;
	if (c < 0) c = -c;
	if (c > 0.8f)											// 0x49fac0
	{
		m_corona->force_show();
		float q[4];
		v195(q);
		m_corona->v196(q[0], q[1], q[2], q[3]);				// corona +0xc8 +0x314
		m_corona->set_diffuse(OMediaFARGBColor((float)((c - 0.8) * 5.0), m_red, m_green, m_blue));	// 0x4a6850, 0x4a6848
		return;
	}
	m_corona->hide(true);
	SetBeamColour(f_5d4[0], m_red, m_green, m_blue);		// (0x5c0: the texture's material)
}

void C3DLightCone::v258(void)								// 0x42da50
{
	CLocalGameObject::v258();								// 0x458940
	m_corona->m_red = m_red;								// corona 0x5e8..0x5f0
	m_corona->m_green = m_green;
	m_corona->m_blue = m_blue;
	SetBeamColour(f_5d4[0], m_red, m_green, m_blue);
	m_corona->set_diffuse(OMediaFARGBColor(1.0f, m_red, m_green, m_blue));	// corona +0x90..+0x9c
}

void C3DLightCone::v260(void)								// 0x42db00
{
	C3DAnimated::v260();									// 0x40e7b0
	v258();													// +0x404
}

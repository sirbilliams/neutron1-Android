/*  C3DMine - see C3DMine.h. Reconstructed from Neutron.exe 0x42efa0..0x42f4e0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DMine.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../Globals.h"
#pragma GCC diagnostic pop
#include "OMediaCanvas.h"
#include <string.h>

using namespace neg;

C3DMine::C3DMine()											// 0x42efa0
{
	SetName((char *)"C3DMINE");
	Log("C3DMine()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	SetObjectID('3MIN');									// element slot 48
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 0x2c;
	m_sprite_size = 300;
	v42(0);
	v40(0);
	v52(0);
	v107(0);
	OMediaDataBase *db = GetDatabase("sprites.omt");		// 0x46a910
	for (int i = 0; i < 10; i++)
	{
		m_canvas[i] = LoadCanvasID(db, 0x58 + i);			// 0x477890
		if (m_canvas[i]) m_canvas[i]->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60, +0x64 = 1, +0x24 vtable +0x10
	}
	m_index = 0;
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	m_state = 0;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	m_time = (float)((double)r * 0.001f);					// 0x48e5d8: starts 0..1 s into the blink
	v67(200.0f);											// radius
	v46(0);
	v34(1);
	C3DSprite::InitPhysics();								// 0x464210
}

C3DMine::~C3DMine()											// 0x42f2d0
{
}

namespace neg {
CGameObject *New_C3DMine(void) { return new C3DMine; }					// '3MIN' (0x638 bytes)
}

void C3DMine::Process(float dt)								// 0x42f320
{
	C3DSprite::Process(dt);									// 0x463090
	m_time = dt + m_time;
	if (m_state == 0)
	{
		if ((float)0.8 < m_time)							// 0x4a6850 (double)
		{
			m_state = 1;
			set_canvas(m_canvas[9]);						// element +0xac
			m_time = 0;
		}
	}
	else if (m_state == 1)
	{
		if ((float)0.8 < m_time)
		{
			m_state = 0;
			set_canvas(m_canvas[0]);
			m_time = 0;
		}
	}
	else if (m_state == 2 && (float)0.08 < m_time)			// 0x4a7e38 (double)
	{
		m_time = 0;
		set_canvas(m_canvas[m_index]);
		m_index++;
		if (m_index > 1) set_diffuse(OMediaFARGBColor(0.5f, 1.0f, 1.0f, 1.0f));	// +0x90.. (r, g, b, a = 1, 1, 1, 0.5)
		if (m_index > 8)
		{
			OMediaElement::hide(true);						// +0x58
			m_state = 3;
		}
	}
}

/*  C3DBubble - see C3DBubble.h. Reconstructed from Neutron.exe 0x410840..0x410d80. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DBubble.h"
#include "../DatabaseCache.h"
#include "../Game.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMediaCanvas.h"
#include <math.h>
#include <string.h>

using namespace neg;

C3DBubble::C3DBubble()										// 0x410840
{
	SetName((char *)"C3DBUBBLE");
	Log("C3DBubble()");
	C3DSprite::InitObject();								// 0x464130 (called directly)
	C3DSprite::SetObjectID('3BUB');							// 0x462d80
	strcpy(m_sprite_db, "RetainedSprites.omt");
	m_sprite_size = 240;
	m_sprite_index = 1;
	OMediaDataBase *db = GetPermanentDatabase("RetainedSprites.omt");	// 0x46acc0
	m_canvas = LoadCanvasID(db, 1);							// 0x477890
	if (m_canvas) m_canvas->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60, +0x64 = 1, +0x24 vtable +0x10 (port: checked)
	f_5fc = 0;
	v46(0);
	m_time = 0;
	f_604 = 0;
	InitSpriteProperties(200.0f);							// element slot 52
	C3DSprite::InitPhysics();								// 0x464210
	v67(150.0f);											// BG slot 68
	v107(0);												// BG slot 108
	v36(0);
	v52(0);
	set_blend(omblendfc_Src_Alpha, omblendfc_One);			// +0xb4 = 6, +0xb8 = 1
	f_608 = 1;
	m_bob = 0;
	m_floating = 0;
	m_fade_in = 0;
	m_fade_out = 0;
	m_alpha = 0;
	// (then a call to the empty 0x472970)
}

C3DBubble::~C3DBubble() {}									// 0x410a40

namespace neg {
CGameObject *New_C3DBubble(void)	{ return new C3DBubble; }	// factory '3BUB' (0x620 bytes)
}

void C3DBubble::v260(void)									// 0x410a80
{
	C3DPermanentSprite::v260();								// 0x4354c0
	set_canvas(m_canvas);									// element +0xac
	set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// +0xb4 = 6, +0xb8 = 7 / 1
}

// Fading in (1 s of alpha 0..1, then it floats); floating: the height bobs by a sine (period
// 2 pi / 10 s, 30 units); fading out (1 s), then hidden. No pause test and no base Process.
void C3DBubble::Process(float dt)							// 0x410ae0
{
	dt = dt + m_time;										// (the time since Start / Pop)
	m_time = dt;
	if (m_floating)
	{
		if (g_windowed) set_diffuse(OMediaFARGBColor(0.5f, 1.0f, 1.0f, 1.0f));	// +0x90.. (r, g, b, a = 1, 1, 1, 0.5)
		const double s = sin((double)(m_time * 10.0f));		// 0x48e5ec
		const double amp = s * 30.0;							// 0x492a60
		m_bob = (float)amp;
		set_size((float)m_sprite_size, (float)((double)m_sprite_size + amp));	// +0xac, +0xb0
		return;
	}
	if (m_fade_in)
	{
		m_alpha = dt;
		if (1.0 < dt)											// 0x492a58
		{
			m_alpha = 1.0f;
			m_floating = 1;
			m_fade_in = 0;
		}
		set_diffuse(OMediaFARGBColor(m_alpha, m_alpha, m_alpha, m_alpha));
		return;
	}
	if (m_fade_out)
	{
		dt = 1.0f - dt;											// 0x48d924
		m_alpha = dt;
		if (dt < 0.0)											// 0x492a50
		{
			m_alpha = 0;
			m_fade_out = 0;
			hide(true);											// element +0x58
		}
		set_diffuse(OMediaFARGBColor(m_alpha, m_alpha, m_alpha, m_alpha));
	}
}

// Jimmy starts the bubble gadget: fade in, shown.
void C3DBubble::Start(void)									// 0x410ca0
{
	m_fade_in = 1;
	set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// +0xb4 = 6, +0xb8 = 7 / 1
	force_show();											// hide_count = 1; hide(false)
	m_time = 0;
}

// Jimmy leaves the bubble: fade out.
void C3DBubble::Pop(void)									// 0x410d00
{
	m_fade_out = 1;
	m_floating = 0;
	m_time = 0;
}

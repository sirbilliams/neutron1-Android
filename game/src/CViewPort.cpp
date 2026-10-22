/*  CViewPort and the per-frame object passes (0x473740..0x473770). See CViewPort.h.
    The original uses QueryPerformanceCounter/Frequency; SDL's performance counter replaces them. */

#include "CViewPort.h"
#include "port/Companion.h"
#include "port/AssetView.h"
#include "CGameObject.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "TextureAnims.h"
#include "Physics.h"
#include "Sound.h"
#include <SDL.h>
#include <string.h>

namespace neg {
unsigned long long	g_perf_counter = 0;
unsigned long long	g_perf_frequency = 0;
int					g_frames_rendered = 0;
float				g_frame_time = 0;
int					g_logic_updates = 0;
}
using namespace neg;

// --------------------------------------------------------------------------- not reconstructed yet / inlined helpers

static void UpdateFrameRate(float dt)		{ (void)dt; NEG_TODO(0x477cb0, "frame rate average / debug overlay"); }
// 0x47e5a0 (inlined in render_viewport): the clear layer (the sky) gets a view matrix of the
// viewport's rotation only: OMT's world transform at (0, 0, 0) - inverse rotation (-ax, -ay, -az)
// in z, x, y order times a zero translation, hint ommc_RotateTranslate - copied to its custom
// view matrix (+0x7c, 17 dwords).
static void SetClearLayerViewMatrix(void)
{
	OMediaMatrix_4x4 m;
	m.set_world_transform(0.0f, 0.0f, 0.0f, g_viewport->get_anglex(), g_viewport->get_angley(), g_viewport->get_anglez());
	g_layer_clear->set_custom_view_matrix(m);
}

// --------------------------------------------------------------------------- the per-frame passes over g_objects

// 0x462a20: C3DParticle's per-frame update of the particles in the list 0x4fc6a4 (rise, fade,
// hide when their time is out). Nothing in Boy Genius adds to that list (see ResetParticles in
// CGameType.cpp), so there is nothing to update.
static void UpdateParticles(float dt)	{ (void)dt; }

static void UpdateObjects(void)												// 0x473740
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		(*i)->v4(g_frame_time);				// vtable +0x10 -> Process(dt)
}

static void PostUpdateObjects(void)											// 0x473770
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		(*i)->v247();						// vtable +0x3dc
		(*i)->v248();						// vtable +0x3e0
	}
}

// --------------------------------------------------------------------------- CViewPort

CViewPort::CViewPort(OMediaSupervisor *super) : OMediaViewPort(super)		// 0x47e310
{
	g_perf_frequency = SDL_GetPerformanceFrequency();
	g_perf_counter = SDL_GetPerformanceCounter();
	g_frame_time = 0;
}

CViewPort::~CViewPort() {}													// 0x47e410

void CViewPort::update_logic(float millisecs_elapsed)						// 0x47e450
{
	OMediaViewPort::update_logic(millisecs_elapsed);
	g_logic_updates++;
}

float CViewPort::GetFrameTime(void)											// 0x47e490
{
	unsigned long long now = SDL_GetPerformanceCounter();
	double dt = (double)(long long)(now - g_perf_counter) / (double)(long long)g_perf_frequency;
	if (dt != 0.0) g_perf_counter = now;
	if (dt > 0.2) return 0.2f;
	return (float)dt;
}

void CViewPort::render_viewport(OMediaRendererInterface *rdr_i)				// 0x47e4f0
{
	g_frame_time = GetFrameTime();			// vtable +0xd4
	if (g_frame_time > 0.0f && g_frame_time < 1.0f)
	{
		const bool explorer = AssetViewFrame(g_frame_time);	// port: the Asset Viewer's explorer: the level stays frozen
		if (!explorer && GameIsRunning() && g_editor && !EditorIsEditing())
		{
			PhysicsProcess(g_frame_time);	// 0x479bc0
			UpdateParticles(g_frame_time);	// 0x462a20
			UpdateTextureAnims(g_frame_time);	// 0x4693a0
		}
		UpdateSounds();						// 0x47d8f0 (the joystick poll is CJimmyGame::Process's)
		if (!explorer)						// port: (always, in the original)
		{
			UpdateObjects();				// 0x473740
			PostUpdateObjects();			// 0x473770
		}
		CompanionTick(g_frame_time);		// port: the dual-screen companion snapshot (port/Companion.h; read-only)
		if (g_layer_clear) SetClearLayerViewMatrix();
		UpdateFrameRate(g_frame_time);		// 0x477db0 -> 0x477cb0 (+ "MEM=" with the debug flag 0x509a12)
	}
	// else: "LASTDELTA=%f" (debug log, empty in the shipped build)
	OMediaViewPort::render_viewport(rdr_i);
	ClearText();							// 0x468600: the text sprites are redrawn every frame
	g_frames_rendered++;
}

void neg::ResetFrameTimer(void)												// 0x47e470
{
	g_perf_counter = SDL_GetPerformanceCounter();
	g_frame_time = 0;
}

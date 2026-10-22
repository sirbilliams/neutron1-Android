/*  CViewPort - the game's viewport (0x120 bytes in the original; OMediaViewPort + game code).
    Reconstructed from Neutron.exe: constructor 0x47e310, destructor 0x47e410.

    It drives the game: each rendered frame, render_viewport (0x47e4f0) measures the frame time and,
    before drawing, runs the per-frame game passes over g_objects - v241(dt), v4(dt) (which calls
    Process(dt), or v242(dt) while editing) and v247/v248 - plus physics, texture animations, input
    and sounds. That is how every game object, the running game type included, is updated.       */
#pragma once
#include "OMediaViewPort.h"

class CViewPort : public OMediaViewPort
{
	public:
	CViewPort(OMediaSupervisor *super);									// 0x47e310
	virtual ~CViewPort();													// 0x47e410

	virtual void update_logic(float millisecs_elapsed);						// 0x47e450
	virtual void render_viewport(OMediaRendererInterface *rdr_i);			// 0x47e4f0
	virtual float GetFrameTime(void);										// 0x47e490 (element vtable +0xd4)
};

namespace neg {
extern unsigned long long	g_perf_counter;		// 0x696018  QueryPerformanceCounter at the last frame
extern unsigned long long	g_perf_frequency;	// 0x696020
extern int					g_frames_rendered;	// 0x696028
extern float				g_frame_time;		// 0x696030  seconds, at most 0.2
extern int					g_logic_updates;	// 0x696034

void ResetFrameTimer(void);						// 0x47e470
}

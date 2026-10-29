// port: seamless opt-in transition state; no engine/database objects cross threads.
#pragma once
#include <SDL.h>
#include <stdlib.h>
namespace PortSeamless {
inline bool enabled() { const char *e = getenv("NEG_SEAMLESS"); return e && e[0]=='1' && !e[1]; }
struct State { void *hud; bool pending, dissolve, armed; double start; unsigned texture; int width,height; State():hud(0),pending(false),dissolve(false),armed(false),start(0),texture(0),width(0),height(0) {} };
inline State &state() { static State s; return s; }
inline double now() { return 1000.0*SDL_GetPerformanceCounter()/SDL_GetPerformanceFrequency(); }
}

// port: seamless called with a current GL context immediately before the HUD layer.
void omf_SeamlessOverlay();

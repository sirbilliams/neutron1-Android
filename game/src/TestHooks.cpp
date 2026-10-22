/*  port: test-only hooks (not in the original; each does nothing unless its variable is set).
    The Android app may set them in the environment before the engine starts.

    NEG_TEST_PLAY=<level.gam>  The game starts with that level (instead of save\startlevel.txt's
                               choice: a new game in level1d.gam or the autosave). When it has loaded, its start trigger (the opening cut
                               scene) is not fired, Jimmy gets the camera and the game mode is 5
                               (Playing): his controls can be tried in a level whose story
                               sequence cannot run yet (level1d's logo intro, level5's museum
                               cut scene whose classes do not exist yet).
    NEG_TEST_POSLOG=<seconds>  Every <seconds> of game time, log Jimmy's position, rotation and
                               velocity ("POS x y z rot ... vel ...").                        */
#include "TestHooks.h"
#include "C3DJimmy.h"
#include "C2DInGameMenu.h"
#include "CGameType.h"
#include "objects/C3DStartPoint.h"
#include "Game.h"
#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

const char *neg::TestPlayLevel(void)
{
	const char *want = getenv("NEG_TEST_PLAY");
	if (!want || !*want || strlen(want) >= 64) return NULL;	// (m_level_name holds 80)
	return want;
}

void neg::TestPlayHook(const char *level)
{
	const char *want = getenv("NEG_TEST_PLAY");
	if (!want || strcasecmp(want, level) != 0) return;
	C3DJimmy *jim = Jimmy();
	if (!jim || !g_gametype) return;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (C3DStartPoint *s = dynamic_cast<C3DStartPoint *>(*i)) s->m_trigger_armed = 0;
	g_gametype->SetCamera(jim);
	SetGameMode(5);
	SDL_Log("NEG_TEST_PLAY: %s without its start trigger, Jimmy playing", level);
}

void neg::TestPosLog(CGameObject *o, float dt)
{
	static float every = -1, t = 0;
	if (every < 0) { const char *e = getenv("NEG_TEST_POSLOG"); every = e && *e ? (float)atof(e) : 0; }
	if (every <= 0 || !o) return;
	t += dt;
	if (t < every) return;
	t = 0;
	float p[4], r[4], v[4];
	o->v195(p);
	o->v201(r);
	o->v174(v);
	SDL_Log("POS %.1f %.1f %.1f rot %.1f %.1f %.1f vel %.1f %.1f %.1f", p[0], p[1], p[2], r[0], r[1], r[2], v[0], v[1], v[2]);
}

/*  port: test-only hooks (not in the original; each does nothing unless its variable is set).
    The Android app may set them in the environment before the engine starts.

    NEG_TEST_PLAY=<level.gam>  The game starts with that level (instead of save\startlevel.txt's
                               choice: a new game in level1d.gam or the autosave). When it has loaded, its start trigger (the opening cut
                               scene) is not fired, Jimmy gets the camera and the game mode is 5
                               (Playing): his controls can be tried in a level whose story
                               sequence cannot run yet (level1d's logo intro, level5's museum
                               cut scene whose classes do not exist yet).
    NEG_TEST_SEQUENCE=1        With NEG_TEST_PLAY: leave the level as it loads (its start trigger
                               and story sequence run, as in a new game).
    NEG_TEST_POSLOG=<seconds>  Every <seconds> of game time, log Jimmy's position, rotation and
                               velocity ("POS x y z rot ... vel ...").
    NEG_TEST_GOTO=<s>:<tag>[#n][@dx/dz],...  At <s> seconds of game time put Jimmy where the
                               (n-th) object tagged <tag> is (moved by dx, dz; 60 higher), e.g.
                               to try an exit or a phone booth without steering there.
    NEG_TEST_TASK=<name>=<n>,...  When the level is loaded, set those task states.             */
#include "TestHooks.h"
#include "C3DJimmy.h"
#include "C2DInGameMenu.h"
#include "CGameType.h"
#include "objects/C3DStartPoint.h"
#include "Game.h"
#include "SaveGame.h"
#include <stdio.h>
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
	if (const char *tasks = getenv("NEG_TEST_TASK"))
	{
		for (const char *p = tasks; *p; )
		{
			const char *e = strchr(p, ',');
			if (!e) e = p + strlen(p);
			const char *q = strchr(p, '=');
			if (q && q < e && q - p < 60)
			{
				char name[64];
				memcpy(name, p, (size_t)(q - p)); name[q - p] = 0;
				SetTaskState(name, atoi(q + 1));
				SDL_Log("NEG_TEST_TASK: %s = %d", name, atoi(q + 1));
			}
			p = *e ? e + 1 : e;
		}
	}
	const char *seq = getenv("NEG_TEST_SEQUENCE");
	if (seq && *seq && *seq != '0')
	{
		SDL_Log("NEG_TEST_PLAY: %s with its start trigger (NEG_TEST_SEQUENCE)", level);
		return;
	}
	C3DJimmy *jim = Jimmy();
	if (!jim || !g_gametype) return;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (C3DStartPoint *s = dynamic_cast<C3DStartPoint *>(*i)) s->m_trigger_armed = 0;
	g_gametype->SetCamera(jim);
	SetGameMode(5);
	SDL_Log("NEG_TEST_PLAY: %s without its start trigger, Jimmy playing", level);
}

// "TAG" or "TAG#n": the first (n-th, from 1) object tagged TAG
static CGameObject *TestFindTag(char *tag)
{
	int n = 1;
	if (char *h = strchr(tag, '#')) { *h = 0; n = atoi(h + 1); }
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0 && --n <= 0) return *i;
	return NULL;
}

static void TestGoto(CGameObject *o, float dt)
{
	static const char *spec = getenv("NEG_TEST_GOTO");
	static float t = 0;
	if (!spec || !*spec) return;
	const float before = t;
	t += dt;
	for (const char *p = spec; *p; )
	{
		const char *e = strchr(p, ',');
		if (!e) e = p + strlen(p);
		const float at = (float)atof(p);
		const char *c = strchr(p, ':');
		if (c && c < e && at > before && at <= t)
		{
			char tag[100] = "";
			float dx = 0, dz = 0;
			size_t n = (size_t)(e - c - 1) < sizeof tag - 1 ? (size_t)(e - c - 1) : sizeof tag - 1;
			memcpy(tag, c + 1, n); tag[n] = 0;
			if (char *at_sign = strchr(tag, '@')) { *at_sign = 0; sscanf(at_sign + 1, "%f/%f", &dx, &dz); }
			CGameObject *dest = TestFindTag(tag);
			if (!dest) SDL_Log("NEG_TEST_GOTO: no object %s", tag);
			else
			{
				float d[4];
				dest->v195(d);
				o->v196(d[0] + dx, d[1] + 60.0f, d[2] + dz, 1.0f);
				SDL_Log("NEG_TEST_GOTO: Jimmy to %s (%.0f %.0f %.0f)", tag, d[0] + dx, d[1] + 60.0f, d[2] + dz);
			}
		}
		p = *e ? e + 1 : e;
	}
}

void neg::TestPosLog(CGameObject *o, float dt)
{
	if (o) TestGoto(o, dt);
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
	C3DAnimated *a = dynamic_cast<C3DAnimated *>(o);
	SDL_Log("POS %.1f %.1f %.1f rot %.1f %.1f %.1f vel %.1f %.1f %.1f anim %s t %.2f", p[0], p[1], p[2], r[0], r[1], r[2], v[0], v[1], v[2],
		a ? a->GetAnim() : "-", a ? a->f_598 : 0.0f);
}

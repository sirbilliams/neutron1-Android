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
    NEG_TEST_EXITS=1           Log every CLoadLevel (exit) of the level: target, start point, task gate
                               (=2: and every object: tag, class ID, position; =3: the story objects' non-default properties).
    NEG_TEST_POSLOG_TAGS=<tag>,... With POSLOG: also log those objects ("OPOS tag x y z state"); a
                               hidden object shows "hidden".
    NEG_TEST_WATCH=<tag>       With NEG_TEST_POSLOG: also log where the object tagged <tag> is ("WATCH ..."); the camera's
                               object is logged as "CAM ..." when it is not Jimmy.
    NEG_TEST_TASK=<name>=<n>,...  When the level is loaded, set those task states.
    NEG_DUMP_OBJECTS=1         When the level is loaded, log every object: class, tag, position and
                               its registered properties (name=value), for tracing trigger chains.
    NEG_TEST_LIVES=<n>         When the level is loaded, set the lives (red neutrons found) to n.
    NEG_TEST_GADGETS=<i>,<i>,...  When the level is loaded, make those items of the PDA's
                               gadget page (0 bubble, 6 baseball, 7 grappler...) available.   */
#include "TestHooks.h"
#include "C3DJimmy.h"
#include "level1/C3DJeep.h"
#include "C3DAI.h"
#include "C2DInGameMenu.h"
#include "CGameType.h"
#include "levels/CLoadLevel.h"
#include "objects/C3DStartPoint.h"
#include "Game.h"
#include "SaveGame.h"
#include <stdio.h>
#include <typeinfo>
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

static void DumpObjects(void)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
	{
		CGameObject *o = *i;
		if (!o) continue;
		float p[4];
		o->v195(p);
		char line[1500];
		int n = snprintf(line, sizeof line, "OBJ %s tag=%s pos=%.0f,%.0f,%.0f", typeid(*o).name(), o->m_tag, p[0], p[1], p[2]);
		for (std::list<CProperty *>::iterator q = o->m_properties.begin(); q != o->m_properties.end() && n < 1300; ++q)
		{
			CProperty *pr = *q;
			if (!pr || !pr->data) continue;
			if (pr->type == PROP_ALPHA) n += snprintf(line + n, sizeof line - n, " %s=%s", pr->name, (char *)pr->data);
			else if (pr->type == PROP_FLOAT) n += snprintf(line + n, sizeof line - n, " %s=%g", pr->name, *(float *)pr->data);
			else if (pr->type == PROP_BOOL) n += snprintf(line + n, sizeof line - n, " %s=%d", pr->name, *(unsigned char *)pr->data);
			else if (pr->type == PROP_LONG) n += snprintf(line + n, sizeof line - n, " %s=%d", pr->name, *(int *)pr->data);
		}
		SDL_Log("%s", line);
	}
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
	if (const char *lv = getenv("NEG_TEST_LIVES")) g_lives = (short)atoi(lv);		// the lives (red neutrons) found
	if (const char *gad = getenv("NEG_TEST_GADGETS"))
	{
		for (const char *p = gad; *p; p++)
			if (*p >= '0' && *p <= '9')
			{
				MenuSetItem(0, (short)atoi(p), 1);
				while (p[1] >= '0' && p[1] <= '9') p++;
			}
	}
	if (const char *ex = getenv("NEG_TEST_EXITS"))
		if (*ex == '3')										// the story objects' properties that are not at their defaults
			for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
			{
				const unsigned int id = (unsigned int)(*i)->m_object_id;
				static const char *want[] = { "3AIT", "3MCA", "3CAM", "3DOR", "3BUT", "3RCK", "3KIT", "TRIG", "LOAD", "3ULT", "3FOW", "3HUG", "3SWI", "3SPA", "3PHO", NULL };
				char ids[5] = { (char)(id >> 24), (char)(id >> 16), (char)(id >> 8), (char)id, 0 };
				int w = 0;
				for (int k = 0; want[k]; k++) if (strcmp(want[k], ids) == 0) w = 1;
				if (getenv("NEG_TEST_EXITS")[1] == 'a' && strcmp(ids, "3PAT") && strcmp(ids, "3TER") && strcmp(ids, "3LIO")) w = 1;
				if (!w) continue;
				char line[1500];
				int n = snprintf(line, sizeof line, "PROPS %s %s:", (*i)->m_tag, ids);
				for (std::list<CProperty *>::iterator p = (*i)->m_properties.begin(); p != (*i)->m_properties.end(); ++p)
				{
					CProperty *q = *p;
					if (!strncmp(q->name, "Position", 8) || !strncmp(q->name, "Rotation", 8) || !strcmp(q->name, "ObjectTag") || !strcmp(q->name, "ObjectID")) continue;
					if (n > 1300) break;
					if (q->type == PROP_ALPHA && strcmp((char *)q->data, "none") && *(char *)q->data)
						n += snprintf(line + n, sizeof line - n, " %s=%s", q->name, (char *)q->data);
					else if (q->type == PROP_FLOAT && !strcmp(q->name, "Radius") && *(float *)q->data != 0)
						n += snprintf(line + n, sizeof line - n, " %s=%.0f", q->name, *(float *)q->data);
					else if (q->type == PROP_LONG && *(int *)q->data != 0 && *(int *)q->data != -1)
						n += snprintf(line + n, sizeof line - n, " %s=%d", q->name, *(int *)q->data);
				}
				SDL_Log("%s", line);
			}
	if (const char *ex = getenv("NEG_TEST_EXITS"))
		if (*ex == '2')										// every object: tag, object id, position
			for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
			{
				float p[4];
				(*i)->v195(p);
				const unsigned int id = (unsigned int)(*i)->m_object_id;
				SDL_Log("OBJ %s %c%c%c%c (%.0f %.0f %.0f)", (*i)->m_tag, id >> 24, id >> 16, id >> 8, id, p[0], p[1], p[2]);
			}
	if (getenv("NEG_TEST_EXITS"))
		for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
			if (CLoadLevel *e = dynamic_cast<CLoadLevel *>(*i))
				SDL_Log("EXIT %s -> %s @%s needs %s >= %d exact %d", e->m_tag, e->m_level_name, e->m_start_point,
						e->m_required_task, e->m_required_level, e->m_exact_level);
	if (getenv("NEG_DUMP_OBJECTS")) DumpObjects();
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
				if (C3DJimmy *jm = dynamic_cast<C3DJimmy *>(o))			// riding the scooter: it goes too
					if (jm->f_a30 && jm->f_a30->f_a84 == jm) jm->f_a30->v196(d[0] + dx, d[1] + 60.0f, d[2] + dz, 1.0f);
				SDL_Log("NEG_TEST_GOTO: Jimmy to %s (%.0f %.0f %.0f)", tag, d[0] + dx, d[1] + 60.0f, d[2] + dz);
			}
		}
		p = *e ? e + 1 : e;
	}
}

void neg::TestPosLog(CGameObject *o, float dt)
{
	if (o) TestGoto(o, dt);
	static const char *answer = getenv("NEG_TEST_CONFIRM");		// "y" or "n": answers the yes/no boxes
	if (answer && *answer) g_keys[(*answer == 'y' || *answer == 'Y') ? 'Y' : 'N'] = ConfirmOpen() ? 1 : 0;
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
	if (const char *tags = getenv("NEG_TEST_POSLOG_TAGS"))
		for (const char *q = tags; *q; )
		{
			const char *e = strchr(q, ',');
			if (!e) e = q + strlen(q);
			char tag[100];
			size_t n = (size_t)(e - q) < sizeof tag - 1 ? (size_t)(e - q) : sizeof tag - 1;
			memcpy(tag, q, n); tag[n] = 0;
			if (CGameObject *t = TestFindTag(tag))
			{
				float tp[4];
				t->v195(tp);
				C3DAI *ai = dynamic_cast<C3DAI *>(t);
				SDL_Log("OPOS %s %.0f %.0f %.0f state %d scene %d", tag, tp[0], tp[1], tp[2], ai ? ai->m_state : -1, GetTaskState("SCENE"));
			}
			else SDL_Log("OPOS %s none", tag);
			q = *e ? e + 1 : e;
		}
	SDL_Log("POS %.1f %.1f %.1f rot %.1f %.1f %.1f vel %.1f %.1f %.1f anim %s t %.2f", p[0], p[1], p[2], r[0], r[1], r[2], v[0], v[1], v[2],
		a ? a->GetAnim() : "-", a ? a->f_598 : 0.0f);
	if (const char *w = getenv("NEG_TEST_WATCH"))		// NEG_TEST_WATCH=<tag>: where that object is
		if (CGameObject *wo = TestFindTag((char *)w))
		{
			float q[4];
			wo->v195(q);
			SDL_Log("WATCH %s %.1f %.1f %.1f", w, q[0], q[1], q[2]);
		}
	if (g_camera && g_camera != o)						// riding something: where the camera's object is
	{
		g_camera->v195(p);
		SDL_Log("CAM %s %.1f %.1f %.1f", g_camera->m_tag, p[0], p[1], p[2]);
	}
}

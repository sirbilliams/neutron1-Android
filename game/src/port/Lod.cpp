/*  port: draw distance - see Lod.h. Not in the original.  */
#include "Lod.h"
#include "OMediaLayer.h"
#include <SDL.h>
#include <stdlib.h>

float neg::LodScale(void)
{
	static float scale = 0.0f;
	if (scale <= 0.0f)
	{
		const char *e = getenv("NEG_LOD");
		scale = e && *e ? (float)atof(e) : 2.0f;
		if (scale < 1.0f) scale = 1.0f;
		if (scale > 4.0f) scale = 4.0f;
		SDL_Log("Draw distance: %gx", scale);
	}
	return scale;
}

void neg::LodFarClip(OMediaLayer *layer, float far_clip)
{
	layer->set_far_clip(far_clip * LodScale());
}

void neg::LodFogRange(OMediaLayer *layer, float start, float end)
{
	layer->set_fog_range(start * LodScale(), end * LodScale());
}

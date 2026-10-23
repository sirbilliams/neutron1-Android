/*  port: draw distance (not in the original). Neutron.exe sets each level's far clip plane and fog range
    in its InitGame (e.g. level 1: far 28000, fog 13000..17000). Trees, bushes and other flat
    picture cards are drawn with the terrain and simply vanish at the far plane (the original shows them
    popping in as Jimmy approaches); the port multiplies the far plane and the fog range by a factor.
    NEG_LOD=<n> (Android: Settings, "Draw distance"): 1 = the exe's values, 2 = twice (default), up to 4.  */
#pragma once

class OMediaLayer;

namespace neg
{
	float LodScale(void);
	// What the level's InitGame passes for layer->set_far_clip / set_fog_range, times LodScale().
	void LodFarClip(OMediaLayer *layer, float far_clip);
	void LodFogRange(OMediaLayer *layer, float start, float end);
}

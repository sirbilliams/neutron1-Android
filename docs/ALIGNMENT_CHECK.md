# Neighborhood / School alignment measurement

2026-10-28, desktop RelWithDebInfo, Xvfb :96, 1280x720, default NEG_LOD=2. World coordinates are (x,y,z), y up, degrees. No gameplay or asset changes; only opt-in TestHooks.cpp diagnostics. Nothing committed.

## Exits and selection

Level1 has exactly three CLoadLevel objects targeting level2.gam, all tagged C3DLOADLEVEL, ordinal #1/#2/#3. No NextTrigger in Level1 references that tag, hence no additional C3DTriggerType LoadLevel route to School in the loaded data. Trigger dispatch resolves a tag, not a separate geometry: its LoadLevel case calls ev54 and PlayerLoadLevel and bypasses AddTouching's gate.

| Ordinal | Position | Rotation | Radius | Destination/start | Gate |
|---|---|---|---|---|---|
| 1 | 10540.460,596.022,-4619.645 | 0,0,0 | 500 | level2.gam / school1 | scene >=108, ExactLevel=-1 |
| 2 | 11026.343,141.982,-4119.272 | 0,0,0 | 500 | level2.gam / school1 | scene >=108, ExactLevel=-1 |
| 3 | 11528.733,552.003,-4423.256 | 0,0,0 | 500 | level2.gam / school1 | scene >=108, ExactLevel=-1 |

These are radius-based touch objects, not boxes. Touch eligibility also includes Jimmy's collision extent. RequiredTask is case insensitive. Missing task state (-1) logs an error then falls through (original behavior). Their ev54 names do not advance SCENE. Opening threshold 108 remains open for later states; no upper limit.

C3DJimmy::v260 uses the requested f_774 if not `none`, copies it to f_724, clears f_774, and scans C3DStartPoints by case-insensitive tag; it applies each match's stored position/rotation (last match wins). There is one school1 in Level2: (-3241.887,86.459,-834.101), yaw270, forward +X. Ground y=6.86, nominal start 79.60 above ground; settled Jimmy y=82.8. Its StartTrigger=yokian2 normally arms on entry (unless RETURN), so a story camera can intervene. Saved viewport=(-1843.04,507.646,-1029.49); the player camera is subsequently placed at local (0,200,-300).

Level2 return: C3DLOADLEVEL ordinal #4, (-4399.961,71.733,-854.019), rotation(0,0,0), radius700, target level1.gam / school1, RequiredTask=none, no story gate. Ground y=5.04; road width1777.79. It is 1158.25 units behind school1 along X and 19.918 sideways. Level1 return start school1=(11076.545,51.953,-3313.703), yaw40, StartTrigger=none.

## Terrain and road

Measurements ray-intersect exported terrain triangles; rendered polygons have a non-null material. Invisible blocking polygons are kept separately. Mesh coordinates are already world coordinates (terrain objects at origin). Cross sections use the asphalt material boundary, excluding sidewalks.

Central Level1 exit #2: approach -Z (yaw180); road x=[10434.648,11742.490], width1307.842. Centerline x=11026.343, signed distance d toward exit: z=-4119.272-d. At d=-2000,-1500,-1000,-500,0,+500,+1000,+1500,+2000, road heights are 2.88,2.88,2.89,2.90,2.90,2.89,2.90,2.92,2.92. At +1250 the dirt pile top is123.05; at +1500 it is104.64. Dirt road continues +2000 through +6000, and asphalt exists at +8000. This is not a cut/open level boundary: a pre-existing school backdrop, hedges, brick walls, barricades and dirt occupy the view. Invisible BLOCKING_07 begins around +500 along the sampled line. At the side exits #1/#3 the ground is2.87/2.94; their high object centers cover different portions of the same passage.

Level2 arrival road direction +X; asphalt z=[-1782.576,-6.177], width1776.399. Along x=startX+d at d=-2000,-1500,-1000,-500,0,+500,+1000,+1500,+2000: heights3.72,4.50,5.29,6.07,6.86,7.18,7.18,7.18,7.18. Sidewalk heights near arrival6.44–7.17; surrounding grass, buildings, intersection and school are present. Road continues behind the return trigger; invisible blockers also exist there.

## Transform and overlap

For arrival facing to match the Level1 road approach yaw180, use engine yaw -90 degrees: (x',y',z')=(z+11860.444, y+55.523, -x-7361.159). This maps school1 exactly onto central exit #2, including its nominal height. Exit sprites' stored yaw0 is not the road direction. Literal matching to sprite yaw0 instead requires +90 degrees and t=(10192.242,55.523,-877.385), pointing Jimmy away from the School approach.

Untransformed spawn-to-exit vertical difference is +55.523. After spawn alignment, road height is62.38 versus Level1's2.90: +59.48 mismatch. Matching the roads vertically would instead use ty=-3.957, leaving the nominal spawn59.48 below the exit center. Width mismatch468.557 (35.83% wider). Native road axes differ90 degrees; transformed axes agree0 degrees. Asphalt centerlines are displaced about122.5 sideways at the central section. Side exit spawn-alignment translations (same yaw -90): #1=(11374.561,509.563,-7861.532), #3=(12362.834,465.544,-7665.143), with ground mismatch about513.55/469.46 respectively.

| Bounds | X | Y | Z |
|---|---|---|---|
| Level1 rendered | -4489.14..23583.39 | -506.37..2352.79 | -16061.62..21372.41 |
| Level2 rendered, native | -19648.66..16473.13 | -1283.78..2337.28 | -22144.65..9041.15 |
| Level2 rendered, transformed | -10284.21..20901.59 | -1228.26..2392.80 | -23834.29..12287.50 |
| Level1 incl. invisible collision | -11080.31..34316.65 | -546.06..19685.04 | -18442.92..26509.07 |
| Level2 incl. invisible collision, native | -19648.66..21878.38 | -1283.78..9468.49 | -29937.88..13919.82 |

Rendered AABB intersection in XZ=719.805 million square units. Level1 rendered footprint inside transformed Level2 AABB is approximately334.50 million square units. Actual projected rendered-triangle footprint intersection is177.94 million square units (50-unit grid; 100-unit grid gives177.28 million, ~0.4% difference). Projection overlap alone is not proof every triangle intersects in 3D, but roads, existing backdrop and surrounding terrain demonstrably occupy the same corridor. A single rigid offset cannot remove the road width change and duplicated scenery. Verdict for both complete unmodified levels: **not without a fade**. A seamless composed backdrop would require clipping/selecting geometry or art edits beyond a per-exit offset.

## Environment

Both stage0 skies: omt/bluesky3.omt; both global ambient RGB=(0.2,0.2,0.2), terrain tint=(1,1,1), terrain materials made unlit. Fog disabled in both. Level1 stored fog RGB=(0.24,0.56,0.75), original range13000..17000, port default2x range26000..34000; far clip28000(original)/56000(port). Level2 stored fog RGB=(1,1,1), range100..1000 (layer defaults in direct load), far clip50000/100000; it does not set a fog range, so a real Level1->2 hop inherits Level1's stored range. Disabled fog makes these stored differences invisible. Daylight atmosphere matches; geometry differs substantially.

Time of day uses GetTaskValue(SCENE): <250 day, 250..319 dusk, >=320 night. Both use dusksky.omt/nightsky.omt. Dusk terrain tint differs: Level1(1,.9,.9), Level2(1,.9,.7); night Level1(.7,.7,.8), Level2(.7,.7,1). Thus blue-channel difference0.2 at dusk/night. No ambient difference measured in daylight; shading largely uses unlit baked textures.

## Captures / reproduction

Both PNGs are1280x720, captured from Jimmy's gameplay camera after settling, with opening story camera suppressed by NEG_TEST_PLAY. `before.png`: Jimmy(11026.3,78.8,-3819.3), yaw180, 300 units before central exit center in XZ; SCENE=0 prevents the radius500 exit firing during capture. `arrival.png`: recreated school1 arrival via GOTO, Jimmy(-3241.9,82.8,-834.1), yaw270, SCENE=108; this is not a live transition/cutscene frame. Both clean-exit twice, only known TODO0x477cb0.

Environment: DISPLAY=:96 SDL_AUDIODRIVER=dummy OMT_WINDOW_SIZE=1280x720 OMT_FRAMES=140 OMT_SCREENSHOT=/tmp/alignment-{before,arrival}.bmp NEG_TEST_PLAY={Level1,Level2}.gam NEG_TEST_TASK=SCENE={0,108} NEG_TEST_GOTO={0.2:C3DLOADLEVEL#2@0/300,0.2:school1} NEG_TEST_GOTO_YAW={180,270} NEG_TEST_GOTO_STOP=1 NEG_TEST_POSLOG=0.5. Run from .game with ../build/negatron. Export meshes with NEG_TEST_ALIGNMENT=/tmp and properties with NEG_DUMP_OBJECTS=1. Raw measurements /tmp/Level{1,2}.gam.mesh, /tmp/alignment-l{1,2}.log; analysis /tmp/align.py and /tmp/align-extra.py. BMP->PNG conversion uses stdlib struct/zlib (/tmp/bmp2png.py), no image dependencies.

Requested /home/bill/Projects/n1-lanes/seam-shots is outside writable roots; mkdir failed with Read-only file system. Captures saved instead to /home/bill/Projects/n1-lanes/seam/alignment-shots/{before,arrival}.png.

Live transition confirmation: alignment-shots/live.png is frame180 of an actual Level1->Level2 touch through #2 at SCENE108. Jimmy settles at(-3241.9,82.8,-834.1), yaw270, confirming the recreated arrival view; the original fade still darkens this frame. Raw log /tmp/alignment-live.log. All nine prescribed 240-frame smoke levels passed rc0, clean=2, todo=0 (excluding known477cb0).

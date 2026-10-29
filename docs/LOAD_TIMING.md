# Desktop level-transition measurements

Port-only instrumentation, 2026-10-28; story gates, asset lifetimes and operation order are unchanged. Always-on `SDL_Log("load: <phase> <ms>")` calls are marked `// port: load timing`. Implementation: `engine/sdl/PortLoadTiming.h` and its call sites. No game data is included.

## Path and loading picture

`CLoadLevel::AddTouching` (0x457ec0) checks Jimmy and the task/exact-state gate, runs `ev54` story steps, then calls `C3DPlayer::PlayerLoadLevel` (0x43b5a0). A `C3DTriggerType` LoadLevel case instead calls the exit's `ev54` and hands its destination/start point to the same player function. That preserves return/start information and calls `CEditor::LoadLevelNow` (0x46df70). Exits normally clear the pending flag and start their configured fade; the menu's `v311` (0x403c50) raises the flag when fade-out finishes. The measurement begins at the accepted load request, so `request-wait` includes the existing fade, usually one second. It ends after the destination's first `SDL_GL_SwapWindow`, before frame pacing. Fade-in continues afterward; this is first presentation, not completion of fade-in.

The loading picture comes from the player's `omt/screens.omt`: `C2DInGameMenu_a.cpp` creates `g_fade_quad` using canvas **252 / 0xfc**, and `C2DInGameMenu::v311` shows it and adjusts its alpha for fade modes 0/1. Modes 2/3 instead use `s_fade_pic`, canvas **247 / 0xf7**. The separate small loading icon is canvas 244 / 0xf4 (`ShowLoadingPicture`, 0x4073b0). `LauncherArt.cpp` exports canvas 252 as `loading.png`; this export is not the in-game rendering path. The screen is retained while the synchronous loader blocks. `loading-fade-cpu` measures menu fade updates, not GPU rasterization or the wall time the picture stays visible; `request-wait` captures the latter before teardown.

`CEditor::UpdateGame` -> `CheckLoadLevel` (0x46dff0): physics reset; eligible objects' `UnInitObject` and `UnInitDatabases`; flag/delete objects; rebuild alpha/font pool; `CloseAllDatabases`; reset surviving objects' physics flags. `LoadGame` (0x46e170) opens `gam/<destination>.gam`, creates its game type (whose `InitObject` calls `InitGame`), then creates or reuses each object and reads its properties. Game initialization and object property readers request OMT assets through `DatabaseCache` -> `InitFileStream` (0x476170) -> `InitDatabase` (0x476000). Database construction reads indexes; `OMediaDataBase::get_object_ck` lazily reads/decompresses shapes, canvases, sound and music. Animation objects also import ASE/PNG into memory databases through `CreateAnim`/`CreateTexture`, which are logged separately.

`ResetLevel` calls each object's `ResetObject` and `v260`. These calls initialize shapes, terrain/physics and object state. Jimmy's `v260` resolves the requested start point, places player/camera and selects music. Start-point `v260` arms its story trigger; `Process` fires it on a subsequent update. Finally the world renders; `OMediaGLCanvas::create_texture`/`create_subimage` prepare/upload textures on the GL context; swap presents the frame.

## Measurement conventions

All times are wall-clock milliseconds from SDL's performance counter. `unload`, `gam-open`, `objects-properties`, `reset-init` and `first-frame` are sequential regions; `request-wait` precedes them. Nested `game-init`, `terrain-build`, `object-physics`, `player-start`, `start-point`, database/asset and `texture-upload-cpu` times overlap those regions and must not be added again. Tiny gaps include test/UI hooks and logging. GL timing includes texture preparation/mipmaps and driver submission, not a forced GPU completion; no `glFinish` or extra rendering was added. Terrain and object physics counters sum calls. Menu fade-update CPU includes callbacks during fade and is also nested.

Each `database` line has filename, whole file bytes, elapsed open/index plus lazy database-read CPU/wall time, `previous` (open at the request boundary) and `reopened` (opened again during this transition). Recursive database reads are charged exclusively to their owning database, avoiding double counting. This does not attribute GL work to databases. Each `asset` line measures ASE/PNG import, with unique file size and whether the previous level imported that name. Shared reload totals count only `previous=1,reopened=1` databases and previous ASE/PNG imports; retained databases are excluded. MB means decimal whole-file footprint, **not measured physical I/O bytes**: an OMT can reopen and read only its index, and the OS page cache is warm. Assets first accessed after the first presented frame fall outside these totals.

## Threading boundaries

Keep GL context selection, texture creation/update/destruction, render-world linking, audio device/channel changes, object construction/deletion, global object lists, singleton/game/task pointers, physics terrain lists and start/camera/story operations on their current main thread. OMT databases/streams have mutable seek/chunk/link/cache state, global registries/cache accounting and static reentrancy guards. ASE converters use global conversion modes. Neither live database loading nor conversion through these existing globals is safe to move to workers unchanged. Timing state itself is deliberately main-thread-only.

Run ahead: independent raw file reads into owned buffers; independent compressed-block decompression and PNG decoding with isolated decoder state. OMT chunk decompression needs separation from live stream seeks and cache publication; ASE conversion needs isolated converter state. Publish decoded assets, create/link game objects, build/register current physics terrain, bind materials and upload textures on the main/GL thread. Any preload must leave story gates and initialization order intact. These measurements introduce no preloading.

## Reproduction and results

Desktop RelWithDebInfo, native OpenGL/Mesa under private Xvfb :96, 640x480, dummy audio, 60 fps cap. Single samples with warm OS caches; not Android/device latency. `NEG_TEST_PLAY`, `NEG_TEST_EXITS=1`, `NEG_TEST_TASK=SCENE=<state>`, `NEG_TEST_GOTO=0.2:<tag>`, `NEG_TEST_GOTO_STOP=1`, `OMT_FRAMES=160`, timeout 35 seconds. Task states are test setup via existing hooks, not gate changes. Destinations are loaded through their exits, not direct editor requests.

| Hop | Tag | SCENE |
|---|---|---:|
| level1b -> Level1 | LABTONEIGH | 60 |
| Level1 -> level1b | C3DLOADLEVEL#12 (riverlab) | 65 |
| Level1 -> Level2 (School) | C3DLOADLEVEL#1 | 108 |
| Level1 -> level4 (Downtown) | C3DLOADLEVEL#7 | 180 |
| Level1 -> Level3 (Retroland) | C3DLOADLEVEL#9 | 320 |
| level5 -> Level7 | 2space2 | 172 |

Local raw logs: `/tmp/seam-hop-<hop>.log`; parser `/tmp/seam-report.py`; machine-readable results `/tmp/seam-results.json`. No assets or screenshots are stored here.

Times below are milliseconds. Shared footprint is MB / measured reload ms. Nested values are not additive.

| Hop | Total | Wait / unload / GAM / objects / reset / frame | Init / terrain / object physics / player+start / GL / fade CPU | Shared MB/ms | Top five databases (ms) |
|---|---:|---|---|---|---|
| lab-neigh | 1150.5 | 998.84/9.20/0.30/94.90/2.10/45.12 | 17.89/6.96/0.02/0.49/26.36/0.67 | 20.10/17.58 | level1 9.14, alpha 1.60, bluesky3 1.06, sprites 0.79, musicneighborhood 0.41 |
| neigh-lab | 1060.9 | 1016.22/20.59/0.16/19.69/1.85/2.34 | 4.21/0.88/0.01/0.14/2.57/0.71 | 20.10/11.15 | level1b 3.09, alpha 0.71, sprites 0.49, objects 0.42, phone 0.34 |
| neigh-school | 1099.9 | 1016.46/13.16/0.15/64.69/1.32/4.09 | 7.25/3.16/0.02/0.27/4.04/0.69 | 29.10/38.40 | level2 3.35, sprites 0.51, alpha 0.49, bluesky3 0.42, phone 0.35 |
| neigh-downtown | 1070.1 | 999.65/13.58/0.11/29.02/0.91/26.76 | 5.62/1.81/0.01/0.07/2.57/0.73 | 20.39/10.69 | level4 3.20, alpha 0.50, yokianship 0.41, bluesky3 0.39, sprites 0.37 |
| neigh-retro | 1097.3 | 1016.14/12.02/0.16/31.60/1.85/35.46 | 3.96/1.20/0.01/0.11/21.58/0.74 | 19.53/8.15 | level3 2.09, nightsky 0.50, alpha 0.46, phone 0.34, sprites 0.30 |
| ship | 1064.0 | 999.82/15.68/0.17/36.57/1.01/10.75 | 3.66/0.56/0.01/0.05/2.72/0.68 | 17.23/2.87 | level7 2.26, alpha 0.75, space 0.75, sprites 0.43, yokianship 0.41 |

Jimmy ASE/PNG reload: **0 MB / 0 ms** on all six hops; his singleton and animation database survive. HUD `screens.omt` (13.23 MB), `icons.omt` and `RetainedSprites.omt` stay resident (`reopened=0`). Fonts (`alpha.omt`, 0.119 MB) and common sounds (`soundeffects.omt`, 10.575 MB) reopen on every hop. Their whole-file footprint does not imply all their contents are reread: common-sound reopening here costs only an index read. Shared totals also include common objects, sprites, phone, scooter and overlapping ASE/PNG imports. Database lines record all filenames individually.

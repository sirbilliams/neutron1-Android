# Handoff: Jimmy Neutron: Boy Genius port

Read this, then `docs/decomp-guide.md` (the conventions).

## The project

Bill (GitHub `sirbilliams`) wants the 2001 PC game **Jimmy Neutron: Boy Genius** (`Neutron.exe`,
MSVC 6, Open Media Toolkit engine, `OMT2.dll`) to run natively on his **AYN Thor** (arm64 Android,
two screens, gamepad). The approach: a faithful C++ reconstruction of the game's code (`game/src`)
on our SDL2 / gl4es port of OMT 2.5 (LGPL, `engine/`).

- Repo: **github.com/sirbilliams/neutron1-android**. App id `com.sirbilliams.neutron1` (installs
  next to the Negatron app, `com.sirbilliams.negatron`; the Java package stays
  `com.sirbilliams.negatron` because the native code's JNI names use it).
- Seeded from the finished sequel port, **sirbilliams/neutron2-android** (*Jimmy Neutron vs. Jimmy
  Negatron*, `Neutron2.exe`, 2002), which was built on Boy Genius's code: 166 of BG's 214 classes
  exist there, and `OMT2.dll` exports the same 1358 functions.
- CI (`.github/workflows/android.yml`) builds `Jimmy-Neutron-Boy-Genius.apk` on every push; pushes
  to `main` replace the **latest** pre-release:
  https://github.com/sirbilliams/neutron1-android/releases/download/latest/Jimmy-Neutron-Boy-Genius.apk
  Signing: secrets `NEG_KEYSTORE_B64` / `NEG_KEYSTORE_PASSWORD` (key alias `negatron`).
- Git identity: `git -c user.name=sirbilliams -c user.email=bravehooves@gmail.com commit`, messages
  ending with the attribution lines the session gives.

### Bill's standing rules

- **Never commit game files**, and no bytes from the exe or its DLLs (no dumped tables, strings,
  images). Values the code needs are read from the player's own `Neutron.exe` at run time
  (`game/src/ExeData.cpp`, `gen/globals.*`). Small constants the code itself uses are fine.
- No leaked source code of any kind; work only from the retail `Neutron.exe`.
- **Faithful**: every function must match `Neutron.exe` (same logic, order, quirks). Deviations are
  marked `// port:` with the reason.
- C++98, no x86 asm, no Windows APIs, never pointers in int (x86-64 Linux and arm64 Android).
- `game/src/gen/*` is generated: regenerate with the tools, never hand-edit.
- Short status notes, no running commentary.

## Setting up

1. `bash fetch-deps.sh` (SDL2, gl4es, unshield; pinned in `deps.lock`).
2. Desktop: `cmake -S engine -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo && make -C build -j8 negatron neg_import`
   (re-run cmake after adding files: `game/src` is globbed).
3. Game files (never in this repo): the BG PC CD image (`bg.iso`, InstallShield `data1.hdr` /
   `data1.cab`). Install with `build/neg_import bg.iso game/`: the cabinet's "Program Executable
   Files" group, i.e. `Neutron.exe`, `NeutronSW.exe`, `OMT2.dll`, MSVC DLLs, `alpha.omt`,
   `fontsmall.png`, `green_font.png`, `NewGame.tsk` and `omt/ gam/ ASE/ png/` (about 212 MB). The
   importer refuses the Negatron CD (`Neutron2.exe`).
4. Run headless from the game folder (start `Xvfb :99 &` first):
   `DISPLAY=:99 OMT_WINDOW_SIZE=640x480 OMT_FRAMES=60 OMT_SCREENSHOT=x.bmp timeout 60 ../build/negatron`
   (success = "CLEAN EXIT"; `OMT_SCREENSHOT` is needed or it never exits). Level test:
   `NEG_TEST_PLAY=<Level>.gam`; which classes a level lacks:
   `python3 tools/decomp/gamcensus.py <decomp1>/factory.json game/src <gam>`.
5. Android: push and let CI build, or `bash build-android.sh` (podman).
6. Tools: radare2 6.2.4 + r2ghidra `c1287cc` built into `/opt/r2` (r2pipe scripts:
   `PATH=/opt/r2/bin:$PATH LD_LIBRARY_PATH=/opt/r2/lib`); `pip install pefile capstone unicorn r2pipe`.

## The decompilation inputs (derived from the exes; not in the repo)

BG work folder `decomp1/` (Neutron.exe) and the Negatron one `decomp/` (Neutron2.exe), each with
`raw/<Class>.c`, `raw/_free.c`, `raw/functions.json` (r2ghidra), `classes.json` (RTTI), `abi.json`,
`factory.json`, `go_slots.json`, `lgo_slots.json`, `omt_layout.json`. Regenerate one from its exe
(details and options per class: `docs/decomp-guide.md`):

1. `tools/decomp/rtti.py Neutron.exe classes.json`
2. `tools/decomp/decompile_all.py Neutron.exe classes.json raw/` (functions analysis misses go in
   `raw/_extra.c`, same header format)
3. `tools/decomp/abi.py Neutron.exe raw/functions.json abi.json`
4. `tools/decomp/factory.py Neutron.exe factory.json abi.json`, then
   `tools/decomp/gen_factory.py factory.json game/src/gen/factory.cpp`
5. `tools/decomp/slot_names.py raw/ classes.json --family CGameObject --json go_slots.json` (and
   `lgo_slots.json` for CLocalGameObject)
6. `tools/decomp/globals.py raw/ Neutron.exe game/src` (after every merge)
7. per class: `tools/decomp/gen_class.py --class X --raw raw --classes classes.json --abi abi.json --src game/src --out game/src/gen ...`

**Retargeting the Negatron code** (how the seed was made):

- `tools/decomp/addrmap.py decomp/Neutron2.exe decomp/raw/functions.json decomp/classes.json
  decomp1/Neutron.exe decomp1/raw/functions.json decomp1/classes.json decomp1/addrmap.json`:
  N2 -> BG function / data / instruction matches (`func`: N2 -> [BG, score, why]).
- `tools/decomp/retarget.py decomp1/addrmap.json decomp1/retarget.tsv <files>`: rewrites the
  addresses in sources. Unmapped ones stay as `N2:0x4xxxxx` (comments, code) and `g_N2_xxxxxx`
  (data macros, which don't compile until resolved); `retarget.tsv` lists every unmapped or
  not-identical match (`check func~0.80` = the BG function is only 80% similar: port the differences).

## Status (Oct 22, end of session 2) - READ THIS FIRST

- **main = every class exists; all 35 .gam files load and reach CLEAN EXIT on desktop** (the only TODO
  left in any log is 0x477cb0, the frame-rate overlay). Last APK sent to Bill: main 0e3554d (ASan-clean
  on 13 levels); Bill's Thor report on the session-1 APK: "ran great, no complaints".
- **Done this session (lanes g..x, all merged):** C3DGoddard; gadgets (C3DBubble, C3DBaseball + pickups,
  grappling hook swing/fire, shrink ray verified); PDA gadget select (C2DInGameMenu bridges);
  merry-go-round, SUV (slot 94 = C3DJimmy::DeactivateGadget 0x427ff0); v197 sets z (no change);
  C3DObject, C3DAIOmtObj, C3DStartPoint, C3DSwingDoor, CLoadLevel; C3DRocketShip, C3DJeep; C3DSprite,
  C3DTriggerType (CallNextTrigger = slot 53, name only); C3DFlyingObject, C3DVehicle, C3DCar, C3DMine,
  C3DRock; C3DJimmy (only C3DSUB 0x422e60 / C3DTANK 0x423030 touch TODOs left: classes not in BG's
  levels); all CLevel*Game types (Negatron-only types removed; gen/CLocalGameObject_* regenerated);
  every missing class of School, Downtown, Retroland, Yokian/Area 51 levels; ending bonus screen.
- **Played to exits (mostly by teleporting between triggers, not real input):** level1d -> level1b ->
  Level1 -> level1c; School (Level2 kitty, book -> rocket -> level1e; races level2a/Level2b to the
  finish); Downtown (level4 Cindy/Sheen/kitty/subway/tunnel, level4a Hugh, level4b/4c power plant,
  level4d ending -> bonus -> credits level1f -> NewGame.tsk); Retroland (Level3 chain + 4 exits, Camel,
  Sumo/Octapuke/Pirate ticket rides). Level3B/Level3E: their .omt is not on the retail CD (no terrain, no
  exit leads there): not a bug.
- **Open:**
  - Late story levels (agent y surveyed, committed nothing): Level1F plays through (ships -> zoomout ->
    teleportjim -> restartgame -> NewGame.tsk); level1e rockspace2 chain loads level2 @rockstart
    (yoke2/4/5 entries -> gotoschool/gomidway/goretro untried); level5 at SCENE 162: invispart ->
    SCENE 168, the invisibility pickup fires invisibility1 -> gogoddard (170 untested). Not run:
    level5a, Level5b, level6, level6a, Level7 beyond a clean start; turret missiles unchecked. Exit chain
    (NEG_TEST_EXITS): Level7 goinghome2 (SCENE 200) -> level5 @escape; level5 (>=500) -> level5a @cave
    -> Level5b @cave; Level5b teleport (>=530) -> level6 @phonebooth -> level6a; candy -> level4d.
    Trigger-only pickups fire on pickup: NEG_TEST_GOTO the pickup tag, not the AITrigger. Left: C3DAI
    N2/TODO(BG) (C3DAI.cpp 189-205, 990; C3DAI.h 152, 168), C3DFireStrato N2:0x41bdc0.
  - The Opus core lane (agent z) committed nothing;
    its findings: CJimmyGame in Neutron.exe defines only element slots 45, 46, 57, 61, 62, 76, 85-91 and
    CLocalGameObject-family slots 10, 241, 259 (0x44d2b0..0x44ef20), all already in the port; the
    Negatron-only members (v242, v277/v278, WriteState/ReadState, OnActivate/OnDeactivate, slots
    94-108: UpdateTimers, music, StopwatchEnd ...) have no BG counterpart. SetFog (N2 0x484520) has no
    direct callers (levels set fog inline via WorldFog), only ReadState (Negatron code). Fix next:
    InitGame (0x44d3d0) must not set m_stopwatch_channel / m_reactor_touched; Process (0x44df80) reads
    the sky at +0x524/+0x528. C3DAnimated unmapped 0x41a9f0..0x41e960 (+ g_545f30, g_545ef8),
    C3DPlayer 0x460380..0x465ad0 (+ g_54a448, g_545d9c). `tools/decomp/vt.py <Class> <Base>` lists a
    class's vtable overrides.
  - Biggest remaining `N2:` files: CJimmyGame, InterfaceElement (35), C3DAnimated, CEditor (23),
    Input (17), C3DMovingTarget (15), C3DPlayer (15). Counts on main: N2: 426 (899 at session start),
    g_N2_ 75, TODO(BG) 36, n2_ev 54, N2SLOT 10; 137/206 .cpp "Reconstructed from Neutron.exe".
  - Not verified in play: Level3C 9-button chain n01..n09 -> sunticket (C3DButton 0x411bb0, needs a
    real walk); laser trigger touch (0x42c410); scooter steered by hand in the races; Level2 SCENE 162 ->
    168 continues in level5 (INVISPART 0x40caa0), followed no further; VR01-08 not played.
  - C3DSpriteType ctor sets canvas flags 1, exe 3 (v260 sets 3 on load). C3DVehicle::ev110 (0x465e00)
    TODO, never runs. Ferris chair offsets (432) not checked in motion.
  - Signing secrets still missing (APK uses a one-off key).
- **Session-2 process notes:** clone with `git clone` gives `backup/seed` (the repo's default branch):
  check out `origin/main`. `apt-get update` before the package install. The local merge branch is named
  `backup/integ` (the stop hook wants a local branch whose remote twin exists). Agent worktrees need the
  dependency links: `ln -s <main checkout>/android/app/jni/{SDL,gl4es,unshield}` into the worktree
  (excluded in .git/info/exclude). GitHub says the repo moved to `sirbilliams/neutron1-Android`
  (redirect works). Sonnet lanes did all class/level work well; Opus only for the core lane.
  Useful test hooks added: NEG_TEST_GOTO=<s>:<TAG>[#n]@dx/dz (by tag), NEG_TEST_EXITS=1|2|3|3a,
  NEG_DUMP_OBJECTS, NEG_TEST_POSLOG_TAGS, NEG_TEST_WATCH, NEG_TEST_GADGETS, NEG_TEST_LIVES.

## Game plan for the next sessions (updated end of session 2)

1. Ask Bill for his Thor notes on APK 0e3554d. Then play the late story levels (agent y's survey above).
2. Finish step 2 (core, one Opus lane): CJimmyGame, C3DAnimated, C3DPlayer (findings above), InterfaceElement, CEditor, Input, C3DMovingTarget, remaining `g_N2_`/`n2_ev`/
   `N2SLOT`; track the counts each session.
3. Step 3 leftovers: VR01-08 play-through; real-input walk of each story level (ideally Bill on the
   Thor); a full new-game run to the credits.
4. Step 4 (port features: dual-screen panel, Bill's UI gadget table, widescreen, HD pack, Asset Viewer
   .ase, launcher art), step 5 (signing secrets, Bill's play-through, README/CHANGELOG, tag v1.0).

## Status (Oct 22, end of session 1)

- **Playable on desktop, first build on the Thor boots** (main = this commit; CI builds the APK):
  new game -> THQ splash -> level1d (Yokian fleet intro, King Goobot) -> "MEANWHILE" -> level1b (lab:
  Carl, Goddard, shrink ray, "practice?" box) -> Level1 (Neighborhood: Carl's talks, neutrons/score,
  red neutrons, Nick's call to SCENE 100) -> front door -> level1c (Jimmy's house). Jimmy animates
  (BG names animations "HI"/"LOW" + name). PDA menu (Esc/Start), phone booth map, saves
  JimmyGame1..10.tsk, launcher Continue/Load on them. Bill's Thor: boots and plays the intro;
  "getting better, not quite there" (his screenshot: scooter-with-Goddard shown in the lab, now
  hidden but unverified; a black ball on the floor, unidentified).
- **How the port was made:** the Negatron source retargeted with tools/decomp/addrmap.py +
  retarget.py, then class by class against Neutron.exe. Unported Negatron code still exists:
  grep `N2:` / `g_N2_` / `TODO(BG)` / `n2_ev`; `/home/claude/decomp1/retarget.tsv` lists every
  address that wasn't an exact match (`check func~<1`). gen/n2globals.h holds zero-filled stand-ins
  for unresolved Negatron globals.
- **Ported to Neutron.exe:** CGameObject (BG slot numbers; Negatron-only marked N2SLOT),
  CLocalGameObject names (offsets still Negatron's), C3DAnimated, CGameType, CJimmyGame, CEditor
  (splash.omt, LoadGame, LoadLevelNow), CViewPort, CLoadLevel, fades, C2DInGameMenu + CMainMenu +
  saves + CTaskList, C3DPlayer, C3DJimmy (+ jimmy/C3DJimmy_menu.cpp), both cut-scene cameras, the
  Yokians, lab and Level1 object classes (agents C/D lists in git log).
- **Open (next session):**
  - C3DGoddard is still Negatron logic (BG 0x41c810.., decompiled notes were in scratch).
  - C3DBubble, C3DBaseball, grappling hook swing/fire, merry-go-round / SUV slot 94.
  - The lab's black ball (Bill's screenshot); verify the scooter stays hidden after the intro.
  - C3DObject; the .ase loader for the model viewer.
  - Level game types: done except level2 (LEV2/LV2A/LV2B, agent p). Every InitGame/ctor/dtor of
    LEV1, LV1A-F, LEV3, LV3A-E, LEV4, LV4A-D (LV4B fog sweep), LEV5, LV5A/B, LEV6, LV6A, LEV7 and
    VR01-08 now matches Neutron.exe (fog inline through the layer globals, pickup bases, terrain
    calls); Negatron-only L1_A/B/C, L5B1, LV5C, LV5F, LPRO removed. gen/CLocalGameObject_* regenerated
    from the BG decompile (275-slot table; fields at BG offsets). CJimmyGame::SetFog (N2:0x484520)
    is still used by LEV2/LV2x and CJimmyGame::ReadState.
  - v197: A says the generated code already sets x, y, z (E's report may be stale): verify.
  - Exit-time use-after-free in C2DInGameMenu's destructor; OMT shape reader new[]/delete.
  - APK is signed with a one-off key until Bill adds NEG_KEYSTORE_B64 / NEG_KEYSTORE_PASSWORD to
    this repo's secrets (same values as neutron2-android).
- **Debugging on the device:** omt-log.txt (launcher: Y on the main menu -> Test -> Share log) now
  carries a crash backtrace (library+offset; symbolise with llvm-addr2line on the APK's
  libnegatron.so) and the first frames' times. Android's allocator aborts on use-after-free that
  glibc tolerates: run an ASan desktop build (`-fsanitize=address,undefined`,
  ASAN_OPTIONS=detect_leaks=0:alloc_dealloc_mismatch=0) before shipping. The shared Xvfb :99 can
  steal focus: test on a private display.
- **Agent setup that worked:** docs/agent-brief.md; worktrees /home/claude/wt1/<x>, game copies
  /home/claude/gamefiles/bg-<x>; 2 CPUs / 7 GB: `make -j2`, at most 3 agents building. Decomp
  inputs backed up in the private negatron-gamefiles repo (decomp/bg-decomp-inputs.tar.gz).

## Game plan as brainstormed with Bill (Oct 22, session 1)

1. **Finish the first stretch** (intro -> lab -> Neighborhood -> Jimmy's house): Bill's Thor notes
   first (Share log, photos); C3DGoddard from Neutron.exe; the lab's black ball / scooter; verify
   v197 z; gadgets the first levels use (bubble gun, baseball, grappling hook, shrink ray beam);
   merry-go-round, SUV. ASan desktop run before every APK.
2. **Core clean-up, measured:** C3DObject; the level game types list above; regenerate
   gen/CLocalGameObject_*; burn down the Negatron leftovers - track the count of `N2:`, `g_N2_`,
   `n2_ev`, `TODO(BG)` and retarget.tsv `check` entries per session.
3. **The story levels, in the game's order** (from the CLoadLevel exits / SCENE gates): School
   (level2*), Downtown (level4*), Retroland (level3*), then Powerplant, Area 51, Rocket Maze, the
   Yokian ship (level5*/6*/7) and the ending; VR01-08 bonus levels last. Per level: census ->
   classes -> play its own trigger/task chain to the exit. One lane per level group.
4. **Port features adapted to Boy Genius:** dual-screen panel (BG gadgets/items/map), Bill's UI
   (BG gadget table), widescreen check, HD texture pack (texture zip for BG), Asset Viewer's .ase
   models, launcher art per level.
5. **Release:** signing secrets in this repo, Bill's full Thor play-through, README/CHANGELOG,
   tag `v1.0`.
Agents: at most 3 at once on this 2-CPU machine (make -j2): core/clean-up, Jimmy+gadgets+Goddard,
and one level lane; short briefs (docs/agent-brief.md), merge into backup/integ, main after
build + smoke test (+ ASan before an APK).

## Status (Oct 22, start of the port)

- `game/src` is the Negatron reconstruction with addresses retargeted to `Neutron.exe`: Negatron's
  logic, not yet Boy Genius's. Negatron-only classes were deleted; being made to build (agent A).
- Done outside the game classes: the importer (ISO and zip) installs BG; the app identity, icon,
  CI and docs; launcher / port strings; the Vox picker (Negatron only) is stubbed out.
- Open `TODO(BG)` items outside the game classes (grep `TODO(BG)`):
  - Done (agent F): the menus and saves are Boy Genius's (C2DInGameMenu, CTaskList, SaveGame,
    jimmy/C3DJimmy_menu.cpp); NEG_START continue / load:<n> and the launcher use JimmyGame1..10.tsk
    in the game folder (port/StartMode.h); level names: the game's own (phone booth keypad).
  - `port/LauncherArt.cpp`: per-level loading pictures (BG's CLoadLevel has no LoadScreen); the
    menu background is the PDA main page (screens.omt 0xd9).
  - `port/BillsUi.cpp`: the gadget table is Negatron's numbering.
- Goal of this round: the first levels playable: `level1d.gam` (the opening) -> `Level1.gam`
  (Retroville) <-> `level1b.gam` (Jimmy's lab, via the phone booth).

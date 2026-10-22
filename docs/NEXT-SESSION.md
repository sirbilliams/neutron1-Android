# Next session: start here (Jimmy Neutron: Boy Genius Android port)

You are the lead on a faithful C++ reconstruction of the 2001 PC game **Jimmy Neutron: Boy Genius**
(`Neutron.exe`, MSVC6, OMT2 engine) running natively on Android (Bill's AYN Thor, a dual-screen arm64
handheld). Bill (GitHub `sirbilliams`) tests on the Thor and gives short feedback. This file is
the whole briefing: read it, then `docs/HANDOFF.md` (status, rules, setup details) in the repo.

## 1. Bill's rules (non-negotiable)
- Never commit game files or bytes copied from the exe/DLLs (values are read from the player's own
  `Neutron.exe` at run time). No leaked source code. Faithful to `Neutron.exe`: same logic, order and
  quirks; every deviation marked `// port:` with the reason.
- Commit as `git -c user.name=sirbilliams -c user.email=bravehooves@gmail.com commit`, message ending
  with the session's Co-Authored-By / Claude-Session lines. Keep this identity even if a hook asks
  for another.
- Push to `main` only after a build + smoke test; every push to main builds the "latest" APK on CI.
  Before sending Bill an APK, also run the ASan desktop build (section 6).
- Send Bill the APK and pictures (screenshots) whenever there's something new to see. Status notes
  short; no running commentary. He asks "check in" / "percentage to goal": answer briefly.

## 2. Use usage efficiently (Bill's limits are tight)
- You (Opus) plan, merge, check and debug. **Agents run on Sonnet** (`model: "sonnet"` on the Agent
  call) for porting classes, levels and tests; an Opus agent only for hard core/debugging work.
- **At most 3 agents at once** (2-CPU, 7 GB machine: `make -j2`, never -j8: it OOM-killed the
  container once). One job per agent, a fresh agent per job, a short prompt that points at
  `docs/agent-brief.md`; agents report in <= 12 lines.
- Don't poll: wait for agent/CI notifications (Monitor on CI). Read slices of files, not whole files.
- End every session: update `docs/HANDOFF.md` (status, open items) and this file; push.

## 3. Repos and where things are
- **sirbilliams/neutron1-android** (public, this repo): the port. `engine/` (OMT 2.5 on SDL2/gl4es),
  `game/src/` (the reconstruction), `tools/decomp/` (decomp tooling incl. `addrmap.py`,
  `retarget.py`, `gamcensus.py`), `android/` (app id `com.sirbilliams.neutron1`), `docs/`.
- **sirbilliams/neutron2-android** (public): the finished Negatron port (v1.0). Boy Genius's code was
  seeded from it; it is the annotated reference for the shared classes. Don't push to it.
- **sirbilliams/negatron-gamefiles** (PRIVATE): release `iso` holds
  `Jimmy.Neutron.Boy.Genius.PC.CD-ROM.iso` (and Negatron's ISO, OMT 2.5 zip); the repo holds
  `decomp/bg-decomp-inputs.tar.gz` (Boy Genius decompile + address map) and
  `decomp/decomp-inputs.tar.gz` (Negatron's). Never copy any of it into a public repo.

## 4. Setting up a fresh machine (about 10 minutes)
1. Attach and clone: `add_repo` sirbilliams/neutron1-android (push), sirbilliams/negatron-gamefiles
   (read), sirbilliams/neutron2-android (read); clone each (depth 1 is fine). In neutron1-android run
   `git config remote.origin.fetch '+refs/heads/*:refs/remotes/origin/*' && git fetch origin`
   (otherwise agents' backup branches aren't fetched).
2. Packages: `apt-get install -y libsdl2-dev zlib1g-dev xvfb x11-utils imagemagick cmake gdb` and
   `pip install --break-system-packages pefile capstone unicorn r2pipe pycdlib`.
3. Decomp inputs: `mkdir -p /home/claude/decomp1 && tar xzf
   negatron-gamefiles/decomp/bg-decomp-inputs.tar.gz -C /home/claude/decomp1` (Negatron's set into
   `/home/claude/decomp` the same way, for reference). Copy `Neutron.exe` in after step 4.
4. Game files: download the ISO with the REST API (`gh release download` fails: GraphQL is blocked):
   `gh api repos/sirbilliams/negatron-gamefiles/releases/tags/iso --jq '.assets[]|"\(.id) \(.name)"'`
   then `gh api -H "Accept: application/octet-stream" repos/sirbilliams/negatron-gamefiles/releases/assets/<id> > bg.iso`.
   Install: `bash fetch-deps.sh` (once, main checkout only), `cmake -S engine -B build-desk
   -DCMAKE_BUILD_TYPE=RelWithDebInfo && make -C build-desk -j2 negatron neg_import`, then
   `build-desk/neg_import bg.iso /home/claude/gamefiles/bg` (gives omt/ gam/ ASE/ png/ + Neutron.exe,
   OMT2.dll). `cp /home/claude/gamefiles/bg/Neutron.exe /home/claude/decomp1/`.
5. radare2 6.2.4 + r2ghidra are only needed to regenerate the decompile (HANDOFF "Setting up");
   the tarball already has `raw/<Class>.c`, so usually skip them.
6. Display: `Xvfb :99 &`. Agents should each use their own display (e.g. `:57`): windows on a
   shared display steal focus.

## 5. Running and testing
- From the game folder: `DISPLAY=:99 OMT_WINDOW_SIZE=640x480 OMT_FRAMES=400 OMT_SCREENSHOT=x.bmp
  timeout 200 <repo>/build-desk/negatron` -> must end with `CLEAN EXIT`.
- Start modes: `NEG_START=new|continue|load:N`; test hooks: `NEG_TEST_PLAY=<Level>.gam`,
  `NEG_TEST_SEQUENCE=1` (keep the level's start trigger), `NEG_TEST_AT=<tag>`, `NEG_TEST_GOTO`,
  `NEG_TEST_TASK`, `NEG_TRIGGER_TRACE`, scripted pad `OMT_FAKE_PAD="<tick>:lefty=-32767,..."`,
  Asset Viewer `NEG_ASSET_VIEW=<level>.gam|models`.
- Real-time screenshots (cut scenes): `tools/test/run_port.sh <negatron> <game dir> <out dir>
  <seconds> [interval] [display] [ENV=...]` -> `shot-<s>.png`; `montage` them for Bill.
- What a level still lacks: `python3 tools/decomp/gamcensus.py /home/claude/decomp1/factory.json
  game/src <game>/gam/<Level>.gam`. Classes created at run time don't show: play and grep the log
  for `TODO`.

## 6. Thor-only bugs (learned the hard way)
- Android's allocator aborts on use-after-free that glibc tolerates. Before an APK: ASan build
  `cmake -S engine -B build-asan -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined
  -fno-omit-frame-pointer -fsigned-char" -DCMAKE_C_FLAGS="-fsanitize=address,undefined"
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"`, run a new game with
  `ASAN_OPTIONS=detect_leaks=0:alloc_dealloc_mismatch=0`. (The original deleted canvases before the
  elements using them: fixed twice, marked port.)
- The Thor window loses focus once at start-up: focus handlers must not touch objects that don't
  exist yet.
- Bill's log: launcher main menu -> press Y (or Select) -> Test -> Share log (`omt-log.txt`). It has a
  crash backtrace (`libnegatron.so+0x...` with symbol) and the first frames' times. Ask for it on
  any device problem.
- Android code is C++98 (no `T{...}`, `auto`, range-for, `nullptr`); Java compiles only on CI.

## 7. How the code got here (so you can read it)
- `game/src` started as the Negatron reconstruction with addresses rewritten to Neutron.exe
  (`addrmap.py` matched 2113 functions / ~3000 data addresses; `retarget.py` rewrote them).
  Leftovers still marked: `N2:0x...`, `g_N2_...` (gen/n2globals.h stand-ins), `n2_ev...`,
  `TODO(BG)`, `N2SLOT`. `/home/claude/decomp1/retarget.tsv` lists every address that wasn't an
  exact match (`check func~0.80` = port the differences).
- Already ported to Neutron.exe: see HANDOFF "Status". In short: core (CGameObject slots,
  C3DAnimated, CGameType, CJimmyGame, CEditor, CViewPort, CLoadLevel, fades), menus + saves
  (C2DInGameMenu, CMainMenu, CTaskList, SaveGame, JimmyGame1..10.tsk), C3DPlayer/C3DJimmy, cut-scene
  cameras, Yokians, lab + Neighborhood objects.
- Game flow: level1d (Yokian intro) -> level1b (Jimmy's lab) -> Level1 ("Neighborhood", the hub:
  exits by SCENE value to level1c house, level2 school, level4 downtown, level3 Retroland ...).
  Level names = the phone booth keypad: Lab, Neighborhood, School, Downtown, Retroland,
  Powerplant, Area 51, Rocket Maze, Yokian Ship.

## 8. Where to start
1. Ask Bill for his Thor notes on the latest APK (photos + Share log).
2. Game plan step 1 (HANDOFF "Game plan"): C3DGoddard from Neutron.exe; the lab's black ball and
   the hidden scooter; check `CGameObject::v197` sets z; gadgets (bubble gun C3DBubble, baseball
   C3DBaseball, grappling hook swing/fire, shrink ray beam); merry-go-round, SUV slot 94.
   Suggested lanes (Sonnet): "Goddard", "gadgets", and one Opus/Sonnet lane for core clean-up
   (C3DObject, level game types list in HANDOFF, gen/CLocalGameObject_*).
3. Then the story levels in order (step 3 of the plan), one lane per level group.
4. Merge workflow: agents push `backup/agent-<x>`; you merge into local `integ`, re-run cmake if files
   were added, `make -j2`, smoke test (new game, Level1, level1b), push `integ:main` and
   `integ:backup/integ`, watch CI, download the APK from release `latest` with the REST API, send it.
- Blocked from this environment (ask Bill): pushing tags, deleting branches, reading/writing
  Actions secrets. The APK uses a one-off key until Bill adds `NEG_KEYSTORE_B64` /
  `NEG_KEYSTORE_PASSWORD` to this repo (same values as neutron2-android).

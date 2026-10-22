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

## Status (Oct 22, start of the port)

- `game/src` is the Negatron reconstruction with addresses retargeted to `Neutron.exe`: Negatron's
  logic, not yet Boy Genius's. Negatron-only classes were deleted; being made to build (agent A).
- Done outside the game classes: the importer (ISO and zip) installs BG; the app identity, icon,
  CI and docs; launcher / port strings; the Vox picker (Negatron only) is stubbed out.
- Open `TODO(BG)` items outside the game classes (grep `TODO(BG)`):
  - `port/StartMode.*` and the launcher's Continue / slots: BG has no autosave, `StartLevel.txt` or
    `save\` folder (saves `JimmyGame1..10.tsk`, plus `NewGame.tsk`, `RestartLevel.tsk`).
  - Level display names: `Neutron.exe` has none; `port/Companion.cpp` shows file names until Bill
    picks names.
  - `port/LauncherArt.cpp`: per-level loading pictures (BG's CLoadLevel has no LoadScreen) and the
    menu background (BG has no `dat\menu.dat`).
  - `port/BillsUi.cpp`: the gadget table is Negatron's numbering.
- Goal of this round: the first levels playable: `level1d.gam` (the opening) -> `Level1.gam`
  (Retroville) <-> `level1b.gam` (Jimmy's lab, via the phone booth).

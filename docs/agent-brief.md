# Agent brief: Jimmy Neutron: Boy Genius (Neutron.exe) port. Token budget is tight: be economical.

Project: faithful C++ reconstruction of the 2001 PC game Jimmy Neutron: Boy Genius (`Neutron.exe`, MSVC6,
OMT2 engine) on our SDL2/gl4es OMT port, for Android (AYN Thor). This repo (sirbilliams/neutron1-android)
was seeded from the finished Negatron port (sirbilliams/neutron2-android, `Neutron2.exe`, 2002), which was
built on Boy Genius's code: 166 of BG's 214 classes exist there too, and OMT2.dll exports the same 1358
functions. Read `docs/decomp-guide.md` (conventions) and the parts of `docs/HANDOFF.md` you need
("Setting up", "Regenerating the decompilation inputs": it describes the Negatron run; paths below).

## What the seed is
- `game/src` = the Negatron reconstruction with its addresses rewritten to Neutron.exe by
  `tools/decomp/retarget.py` from `tools/decomp/addrmap.py`'s function/data matching. **It is Negatron's
  logic, not yet Boy Genius's.** Unmapped addresses are marked `N2:0x4xxxxx` (comments/code) and
  `g_N2_xxxxxx` (data macros, won't compile until resolved). Report of every unmapped or
  not-identical-match address: `/home/claude/decomp1/retarget.tsv` (file, line, address, kind; `check
  func~0.80` = the matched BG function is only 80% similar: port the differences).
- Classes only Negatron has were deleted (C3DPlayerObject, C3DAIGranny, granny objects, ...). In BG:
  C3DPlayer and C3DAI derive straight from C3DAnimated; CGameObject has 275 game slots (Negatron 286),
  C3DAnimated 72, C3DAI 95, C3DPlayer 93, C3DJimmy 131, CJimmyGame 92 (counts incl. inherited). Object
  sizes all differ (factory.json). Our C++ layouts don't need the exe's byte offsets; the logic does.
- The rule: every function must match Neutron.exe. Where the BG function is identical (addrmap score 1),
  keep the code; otherwise port it from BG's decompile, using the Negatron code as the annotated guide.
  When a file is verified, change its header note to "Reconstructed from Neutron.exe" and list remaining
  `N2:` / TODOs.

## Inputs (read-only, shared)
- BG: `/home/claude/decomp1/` = `Neutron.exe`, `raw/<Class>.c` + `raw/_free.c` + `raw/functions.json`
  (r2ghidra), `classes.json` (RTTI), `abi.json`, `factory.json` (ID -> class/size/ctor), `go_slots.json`,
  `lgo_slots.json`, `omt_layout.json`, `addrmap.json` (`func`: N2 -> [BG, score, why]; `data`; `insn`).
- Negatron reference: `/home/claude/decomp/` (same files for Neutron2.exe) and the Negatron source
  `/home/claude/neutron2-android/game/src` (don't edit).
- `r2` on PATH (radare2 6.2.4 + r2ghidra; for r2pipe scripts use `PATH=/opt/r2/bin:$PATH
  LD_LIBRARY_PATH=/opt/r2/lib`); python pefile/capstone/unicorn.
- Game data: master install `/home/claude/gamefiles/bg` (omt/ gam/ ASE/ png/ + Neutron.exe, OMT2.dll).
  The ISO: `/home/claude/gamefiles/jimmy1/bg.iso` (InstallShield `data1.cab`; unpacked in `.../jimmy1/game`).

## Hard rules
- Never commit game files or bytes from the exe/DLLs (values are read from the player's exe at run time:
  `game/src/ExeData.cpp`, `gen/globals.*`). No leaked source code of any kind. Faithful; deviations marked
  `// port:` with the reason. Builds for x86-64 Linux and arm64 Android: C++98 (no `T{...}`, `auto`,
  range-for, `nullptr`), no x86 asm, no Windows APIs, never pointers in int.
- `game/src/gen/*` is generated: regenerate with the tools (globals.py, gen_class.py, gen_factory.py,
  all take the BG inputs), never hand-edit. Run globals.py after merges.
- Commit only what builds: `git -c user.name=sirbilliams -c user.email=bravehooves@gmail.com commit`,
  message ending with a blank line and:
  Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
  Claude-Session: https://claude.ai/code/session_015zEU1ZTRDWHNzdA6Fa1a1r
- After each commit: `git push -f origin HEAD:backup/agent-<x>`. Never push main or other branches.
- Don't touch other agents' worktrees, build dirs, game installs or scratch. Don't run fetch-deps.sh.

## Your workspace (<x> = your letter)
- Worktree `/home/claude/wt1/<x>` (branch `agent-<x>`, from `backup/seed`). Build:
  `cmake -S engine -B build-<x> -DCMAKE_BUILD_TYPE=RelWithDebInfo && make -C build-<x> -j8 negatron`
  (re-run cmake after adding files). Game install `/home/claude/gamefiles/bg-<x>`; scratch
  `/home/claude/scratch1-<x>`.
- Run from the game folder (Xvfb :99 is running):
  `DISPLAY=:99 OMT_WINDOW_SIZE=640x480 OMT_FRAMES=60 OMT_SCREENSHOT=x.bmp timeout 60 <worktree>/build-<x>/negatron`
  Success = "CLEAN EXIT". Level test: `NEG_TEST_PLAY=<Level>.gam`; scripted pad: `OMT_FAKE_PAD`.
  Which classes a level lacks: `python3 tools/decomp/gamcensus.py /home/claude/decomp1/factory.json game/src <gam>`.
- New classes: own new files, define `New_<Class>` (overrides the factory's weak stub). Match the style
  of the existing classes. Shared files: minimal edits, list them in your report.
- Testing economy: build + smoke test before each commit, plus one quick check that your change works.
  No long captures unless something is wrong. Before reporting, check CI on your backup branch:
  `gh api "repos/sirbilliams/neutron1-android/actions/runs?branch=backup/agent-<x>&per_page=1" --jq '.workflow_runs[0]|.status+" "+(.conclusion//"")'`
- Report in <= 12 lines: commits, what works, what's left (with addresses), shared files touched.

## Goal of this round: the first levels playable
Start chain (from CEditor/C2DInGameMenu): `level1d.gam` (the opening: Yokian ships, logos?) ->
`Level1.gam` (Retroville) <-> `level1b.gam` (Jimmy's lab, via the phone booth). "Done" = a level loads
all its objects and plays from start to exit (objectives, cut scenes, exits load the next level).

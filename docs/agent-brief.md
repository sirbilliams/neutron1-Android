# Agent brief: Jimmy Neutron: Boy Genius

Faithful C++ reconstruction of the 2001 PC game `Neutron.exe` on SDL2/gl4es OMT,
for Linux and arm64 Android (AYN Thor). Read the latest status in [HANDOFF.md](HANDOFF.md)
and the conventions in [decomp-guide.md](decomp-guide.md).

## Current state

All 35 .gam files load on desktop. 170/207 source files are verified; a header saying
`Reconstructed from Neutron.exe` means every function was read line by line against BG.
The port began as a Negatron seed, but verified files now implement Boy Genius's logic.
Unmarked files still need review; loading cleanly does not prove a full play-through.
Late-story exits load in order. Real-input story and VR play-throughs remain to check.

## Inputs and faithfulness

- Read-only BG inputs: `~/Projects/neutron1-private/decomp1/`, especially `raw/<Class>.c`,
  `classes.json`, `abi.json`, `addrmap.json`. Disassemble `.game/Neutron.exe` with capstone
  when the decompile is unclear. Negatron is a reference, never the authority.
- Match Neutron.exe's logic, order, constants and quirks. Mark port deviations `// port: <reason>`.
  Add the verified header only after checking every function; list anything left.
- Never copy or commit game files, exe/DLL bytes or leaked source. Runtime tables come from
  the player's exe (`ExeData.cpp`, `gen/globals.*`).
- C++98 only; no asm, Windows APIs or pointers in ints. Never hand-edit `game/src/gen/*`;
  regenerate with the decomp tools, including globals after merges.
- Edit only the assigned files (and their headers). Do not commit unless asked.

## Local workflow

Use your assigned worktree under `~/Projects/n1-lanes/<lane>`, its own build directory,
and your own game copy (`.game/`). Leave other lanes' worktrees and scratch alone.
Link existing SDL/gl4es/unshield dependencies into a fresh worktree rather than fetching them again.
Build and test inside the `omtbuild` distrobox, which shares the home directory.
Local shell helpers: `cx -b` for the build workflow, `fl` for logs.
Explicit build (run from the worktree inside the container):

```sh
distrobox enter omtbuild
cmake -S engine -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -C build -j4 negatron
```

Headless smoke (inside the container, from the worktree):

```sh
cd .game
for lv in "" level1b.gam Level1.gam Level2.gam Level3.gam level4.gam level5.gam level6.gam Level7.gam; do
  SDL_VIDEODRIVER=offscreen SDL_AUDIODRIVER=dummy LIBGL_ALWAYS_SOFTWARE=1 \
    OMT_WINDOW_SIZE=640x480 OMT_FRAMES=240 OMT_SCREENSHOT=/tmp/cx-$$.bmp \
    NEG_TEST_PLAY=$lv timeout 120 ../build/negatron > /tmp/cx-$$.log 2>&1
  echo "${lv:-start} clean=$(grep -c 'CLEAN EXIT' /tmp/cx-$$.log) todo=$(grep TODO /tmp/cx-$$.log | grep -vc 477cb0)"
done
```

Expected per level: `clean=2 todo=0` (the frame-rate overlay 0x477cb0 is excluded).
If Mesa/EGL crashes offscreen, start a private Xvfb inside the container and use its
`DISPLAY` instead of `SDL_VIDEODRIVER=offscreen LIBGL_ALWAYS_SOFTWARE=1`.
Never use the real desktop display. Keep `OMT_SCREENSHOT` set so frame-limited runs exit.
Use `NEG_TEST_GOTO`, `NEG_TEST_EXITS` and `OMT_FAKE_PAD` for focused checks;
trigger-only pickups need the pickup tag, not the AITrigger tag.
Report changes, remaining addresses and smoke results briefly.

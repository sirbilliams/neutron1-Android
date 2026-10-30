# Jimmy Neutron: Boy Genius Recomp
Faithful C++ reconstruction of the 2001 PC game for Android/AYN Thor, reusing the Negatron port engine.

## Layout
- `engine/`: OMT2 engine source (SDL2/gl4es port), `README.md` details changes.
- `android/app/`: Gradle project; `src/main/jni/` holds fetched SDL2, gl4es, unshield.
- `tools/decomp/`: `addrmap.py`, `retarget.py`, `gamcensus.py` for reconstruction logic.
- `game/src/gen/`: Generated code from `globals.py`; never hand-edit.

## Build
Use distrobox `omtbuild` (Ubuntu 24.04, SDL2, Xvfb) or local Linux dependencies.
```bash
bash build-android.sh          # Creates podman image if missing; builds APK to ~/BoyGenius-Android/
# Desktop check:
sudo apt install build-essential cmake libsdl2-dev libgl-dev zlib1g-dev
cmake -S engine -B build && cmake --build build -j
```

## Test
Smoke test under Xvfb (headless): success prints "CLEAN EXIT".
`NEG_TEST_PLAY=<Level>.gam` jumps to level; `OMT_FAKE_PAD` scripts input.
GitHub Actions (`android.yml`) builds APK per push; pushes to `main` update `latest`.
Desktop: `/path/to/build/negatron` runs engine with game folder containing `Neutron.exe`.

## Rules
- **No game files**: Never commit ROMs, ISOs, or extracted game assets. Player provides own CD `.iso`.
- **Licensing**: Engine under LGPL 2.1; respect SDL2, gl4es, unshield licenses (`THIRD_PARTY.md`).
- **Code**: C++98 only (no `auto`, range-for, `nullptr`, `T{...}`); no x86 asm, no Windows APIs, never pointers in ints.
- **Signing**: Keystore secrets (`NEG_KEYSTORE_B64`/`_PASSWORD`) are repository secrets; never in git.
- **Decompilation**: Verify function-by-function against `Neutron.exe` decompile; match scores <1 require porting differences.

## Release
Tag `v*` on `main` to create permanent release with CHANGELOG notes.
CI signs APK using repository secrets; fallbacks to debug key if missing.
App ID: `com.sirbilliams.neutron1`.

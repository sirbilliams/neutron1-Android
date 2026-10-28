# HD texture pack

The game can use upscaled copies of its own textures. The pack is made from the player's own game
files and is never part of this repository (it is the game's art).

## Layout

A folder `hd/` in the game folder (Android: the game data folder, next to `omt/`):

| file | stands in for |
|---|---|
| `hd/omt/<database>/<id>.png` | canvas `<id>` of `omt/<database>.omt` (lower case, no extension): menus, HUD, font, sprites, the 3D levels' textures |
| `hd/png/<name>.png` | the loose texture `png/<name>.png` |
| `hd/grn/<model>_<n>.png` | Negatron only (Granny models): texture `n` of `grn/<model>.grn`. Boy Genius has no .grn files; its models are `ASE/*.ASE` and take their textures from `png/` (below) |

Boy Genius (Neutron.exe): the game folder holds `omt/` (menus, HUD, sprites, level and sky textures: `hd/omt/level1/12.png` ...), `png/` (the character and object textures the ASE models use, 126 files such as `png/jimycarl.png`: `hd/png/jimycarl.png`) and two loose pictures next to the exe, `fontsmall.png` and `green_font.png` (`hd/png/fontsmall.png`, `hd/png/green_font.png`, in the same `hd/png/`). The `.omt` database names are the file names in `omt/` (`alpha.omt` in the game folder too), lower case without extension.

Each picture is an integer multiple (2x, 3x, 4x ...) of the original size, same aspect; alpha is
used as is (keep transparent pixels transparent). Names are matched without regard to case. Any PNG
works (all colour types and bit depths, palette transparency, interlaced).

## Install

- Android: launcher Settings, "HD texture pack: install (.zip)" (file picker; progress in the item;
  the counts are reported), "HD texture pack: remove".
- Desktop: `neg_hdtool install <game dir> <pack.zip>`, `neg_hdtool remove <game dir>`.

The zip may have `omt/ png/` (and `grn/` for Negatron) at its top or inside one folder; other files (README,
manifest.tsv) are left out. `neg_hdtool extract <game dir> <out> [db.omt ...]` writes the omt canvases
with these names (plus manifest.tsv) for upscaling; with no database named it takes every `.omt` in `omt/` (Boy Genius: about 1600 canvases, 34 MB) and the `alpha.omt` next to the exe. The loose `png/` textures and the two font pictures are plain PNGs: copy and upscale them yourself into `png/`.

## How it works (engine/sdl/OMediaHDPack.cpp)

- `hd/` is scanned once at start (`NEG_HD_DIR` overrides the folder).
- A canvas read from an .omt database (OMediaCanvas::read_class, the database's file name in
  OMediaDataBase::hd_name), from a loose png file (Boy Genius: C3DAnimated's `png\\<name>` textures, and the two in the game folder), or filled from a granny texture (Negatron only, C3DAnimated::BindTex,
  GrannyCompatTextureName) is tagged with its pack name when the pack has it.
- When its GL texture is made (OMediaGLCanvas::create_texture), the picture is uploaded instead of the
  canvas pixels: one texture (exact / scaled canvases: normalised coordinates) or the 2D subdivided tiles,
  each 2^k times larger (the factor is rounded down to a power of two there). The canvas keeps its size and
  pixels, so every size and coordinate the game uses stays the original.
- A canvas the game draws into (a write lock) loses its tag and goes back to its own pixels.
- Limits: GL_MAX_TEXTURE_SIZE (the largest factor that fits, else the original, with a log line); a memory
  budget for the HD textures, `NEG_HD_BUDGET_MB` (default 1536; past it the rest stays original, logged).
- Log: `HD: texture pack with N pictures` at start, `HD: N textures replaced` at exit; `NEG_HD_LOG=1` lists
  each replaced picture; a wrong-size or unreadable picture is logged once and skipped.
- Without `hd/` nothing changes.

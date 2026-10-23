/*  Texture animations (0x468d50..0x469400).

    A shape's material whose texture chunk is named "a-<digit>..." is animated: the canvases with the
    same name and digits 1..9 in place of the third character (up to the first one missing) are
    loaded and shown in turn, each for the material's ambient red (+0x6c) seconds (0.5 when 0).
    One animation per texture name: a second material with the same texture keeps its first
    frame (quirk).                                                                              */
#pragma once

class OMediaDataBase;
class OMedia3DShape;

namespace neg {
void AddShapeTextureAnims(OMediaDataBase *db, OMedia3DShape *shape);	// 0x468ec0 ("a-" textures)
void UpdateTextureAnims(float dt);										// 0x4693a0
void ResetTextureAnims(void);											// 0x468d50
}

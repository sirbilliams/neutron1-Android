/*  C3DSprite - the base of every billboard game object (pickups, triggers, effects, markers): an
    OMT canvas element facing the camera that is also a CLocalGameObject.
    Reconstructed from Neutron.exe: constructor 0x463f10, destructor 0x464070. Original layout:
    OMediaCanvasElement at +0 (element vtable 0x4d4bdc, 53 slots: OMT's 48 + game slots 48..52),
    listener vtable at +0x30, CLocalGameObject at +0xc8 (vtable 0x4d4778, 275 slots), own members
    from +0x57c, then the virtual-base streamer (vtordisp at +0x5e8, streamer at +0x5ec).
    Neutron.exe's sprite has no ShowAgain / HideAgain (Negatron's element slots 53 / 54) and no
    "visible" flag: C3DSpriteType's v242 shows a hidden sprite with the element's own hide.
    No factory ID of its own ('3SPR' is only the element/object id the constructor sets).

    The sprite's picture: "SpriteDatabase" (an .omt file name, default "icons.omt") and
    "SpriteIndex" (canvas chunk id) select the canvas, "SpriteSize" its world size (v260 loads it).

    Element-family game slots are marked `// ESLOT n`, CGameObject-family slots `// SLOT n`.  */
#pragma once
#include <string>
#include "OMediaCanvasElement.h"
#include "CLocalGameObject.h"

class OMediaDataBase;
class OMediaStreamOperators;
class OMediaMatrix_4x4;

class C3DSprite : public OMediaCanvasElement, public CLocalGameObject
{
	public:
	C3DSprite();															// 0x463f10
	virtual ~C3DSprite();													// SLOT 0  0x464070 (thunks 0x4648d0 / 0x464040 / 0x4648e0)

	// ---- OMT element virtuals overridden
	virtual void update_logic(float millisecs_elapsed);						// element slot 15 0x464120
	virtual void read_class(OMediaStreamOperators &stream);					// streamer slot 1 0x410320
	virtual void write_class(OMediaStreamOperators &stream);				// streamer slot 2 0x410330
	// (every sprite class repeats these two in its streamer vtable, e.g. 0x410320 / 0x410330)

	// ---- element-family game slots
	virtual void SetObjectID(int id);										// ESLOT 48 0x462d80
	virtual void InitSpriteName(OMediaDataBase *db, std::string name, float size);	// ESLOT 49 0x464490
	virtual void InitSpriteID(OMediaDataBase *db, int id, float size);		// ESLOT 50 0x464430
	virtual void InitSpriteFile(std::string file, float size);				// ESLOT 51 0x4642a0
	virtual void InitSpriteProperties(float size);							// ESLOT 52 0x4645f0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x464130
	virtual void UnInitObject(void);										// SLOT 8  0x4641d0
	virtual void InitPhysics(void);											// SLOT 11 0x464210
	virtual void Process(float dt);											// SLOT 241 0x463090 (= C3DPolygon's: a log, then CLocalGameObject's)
	virtual void v247(void);												// SLOT 247 0x4646c0 element position from the object
	virtual void v248(void);												// SLOT 248 0x464700 element angles from the object
	virtual void v258(void);												// SLOT 257 0x464780 properties changed: reset + reload sprite
	virtual void v260(void);												// SLOT 259 0x464810 (re)load the sprite canvas

#include "gen/C3DSprite_slots.inc"

	protected:
	void LoadSprite(bool permanent);										// inlined in v258/v260 of the sprite classes
	public:

	// FIELD 0x57c 0x6c
	int				m_sprite_size;			// 0x57c "SpriteSize" (world units, = 100)
	int				m_sprite_index;			// 0x580 "SpriteIndex" (canvas chunk id, = 4)
	char			m_sprite_db[100];		// 0x584 "SpriteDatabase" (= "icons.omt")
#include "gen/C3DSprite_fields.inc"
};

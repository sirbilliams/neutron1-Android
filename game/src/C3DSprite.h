/*  C3DSprite - the base of every billboard game object (pickups, triggers, effects, markers): an
    OMT canvas element facing the camera that is also a CLocalGameObject.
    Reconstructed from Neutron.exe: constructor 0x463f10, destructor 0x464070. Original layout:
    OMediaCanvasElement at +0 (element vtable 0x4d4bdc, 55 slots: OMT's 48 + game slots 48..54),
    listener vtable at +0x30, CLocalGameObject at +0xc8 (vtable 0x4d4778, 286 slots), own members
    from +0x590, size 0x600 + the virtual-base streamer (vtordisp at +0x600, streamer at +0x604).
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
	virtual void read_class(OMediaStreamOperators &stream);					// streamer slot 1 N2:0x45d880
	virtual void write_class(OMediaStreamOperators &stream);				// streamer slot 2 N2:0x45d890
	// (every sprite class repeats these two in its streamer vtable, e.g. 0x410320 / 0x410330)

	// ---- element-family game slots
	virtual void SetObjectID(int id);										// ESLOT 48 0x462d80
	virtual void InitSpriteName(OMediaDataBase *db, std::string name, float size);	// ESLOT 49 0x464490
	virtual void InitSpriteID(OMediaDataBase *db, int id, float size);		// ESLOT 50 0x464430
	virtual void InitSpriteFile(std::string file, float size);				// ESLOT 51 0x4642a0
	virtual void InitSpriteProperties(float size);							// ESLOT 52 0x4645f0
	virtual void ShowAgain(void);											// ESLOT 53 N2:0x49d060
	virtual void HideAgain(void);											// ESLOT 54 N2:0x49d080

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x464130
	virtual void UnInitObject(void);										// SLOT 8  0x4641d0
	virtual void InitPhysics(void);											// SLOT 11 0x464210
	virtual void Process(float dt);											// SLOT 240 0x463090
	virtual void v247(void);												// SLOT 247 0x4646c0 element position from the object
	virtual void v248(void);												// SLOT 248 0x464700 element angles from the object
	virtual void v258(void);												// SLOT 258 0x464780 properties changed: reset + reload sprite
	virtual void v260(void);												// SLOT 260 N2:0x49cfd0 (re)load the sprite canvas

#include "gen/C3DSprite_slots.inc"

	protected:
	void LoadSprite(bool permanent);										// inlined in v258/v260 of the sprite classes
	public:

	// FIELD 0x590 0x70
	int				m_sprite_size;			// 0x590 "SpriteSize" (world units, = 100)
	int				m_sprite_index;			// 0x594 "SpriteIndex" (canvas chunk id, = 4)
	char			m_sprite_db[100];		// 0x598 "SpriteDatabase" (= "icons.omt")
	unsigned char	m_visible;				// 0x5fc ShowAgain / HideAgain (= 1)
#include "gen/C3DSprite_fields.inc"
};

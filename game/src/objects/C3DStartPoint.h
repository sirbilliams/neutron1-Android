/*  C3DStartPoint ('STRT') - where Jimmy enters a level: a sprite ("Icons.omt" canvas 5) whose
    ObjectTag is the name a CLoadLevel / C3DJimmy "StartPoint" refers to. It also keeps the view
    to put the camera at ("ViewportP*" / "ViewportR*", taken from the viewport when its
    properties change in the editor), the level's music, and "StartTrigger": a C3DTriggerType
    fired when Jimmy arrives here (the opening cut scene of level1d is 'cam2').
    Reconstructed from Neutron.exe: constructor 0x4422d0, destructor 0x4424f0 (only resets the
    vtables, then ~C3DSprite; deleting thunks 0x4428e0 / 0x4424c0 / 0x4428f0). Vtables 0x4b7ab8
    (element, 55 slots, all C3DSprite's) / 0x4b7aa4 (listener) / 0x4b7654 (game object) /
    0x4b7644 (streamer: 0x414c00 / 0x414c10, shared with C3DCheckPoint, are C3DSprite's
    read_class / write_class). 0x6e0 bytes (factory): C3DSprite's 0x5e8, own members
    0x5e8..0x6d5, vtordisp 0x6d8, the streamer at 0x6dc. Derives from C3DSprite directly (RTTI),
    not from C3DSpriteType. Overrides slots 0, 7, 241 (Process), 257 (v258), 259 (v260) only.

    Members are commented with their Neutron.exe offset from the object start (element view;
    the CGameObject-family methods of the original see them 0xc8 lower).  The tree's C3DSprite
    still has Negatron's layout, so the C++ offsets differ.                                   */
#pragma once
#include "../C3DSprite.h"

class C3DStartPoint : public C3DSprite
{
	public:
	C3DStartPoint();														// 0x4422d0
	virtual ~C3DStartPoint();												// 0x4424f0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x442530 the properties
	virtual void Process(float dt);											// SLOT 241 0x442640 C3DSprite's, then fires the armed StartTrigger
	virtual void v258(void);												// SLOT 257 0x442700 properties changed: the view from the viewport
	virtual void v260(void);												// SLOT 259 0x442740 (re)load: Jimmy arriving here arms StartTrigger

	// FIELD 0x5e8 0xee
	float		m_viewport_pos[4];		// 0x5e8  "ViewportPx/Py/Pz" (w 0x5f4 = 1)
	float		m_viewport_rot[4];		// 0x5f8  "ViewportRx/Ry/Rz" (w 0x604 = 1)
	char		m_music_db[100];		// 0x608  "MusicDatabase" (= "none")
	int			m_music_index;			// 0x66c  "MusicIndex"
	char		m_start_trigger[100];	// 0x670  "StartTrigger" (= "none")
	unsigned char m_trigger_armed;		// 0x6d4  set by v260, fired (and cleared) by Process
};

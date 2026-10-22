/*  C3DMusicTrigger ('3MUS') - switches the level music: when fired (by a trigger chain, by the
    camera touching it with "TouchActivated", by ToggleObject, or at a level load whose level
    matches "RequiredLevel" / "ExactLevel" / "RemoveLevel") it has the game play music
    "MusicIndex0" of "MusicDatabase" (CJimmyGame::PlayLevelMusic). Fired by a trigger chain it
    is the end of that chain (no NextTrigger), and does nothing while a cut scene is skipped.
    Reconstructed from Neutron.exe: constructor N2:0x458d60 (factory '3MUS', 0x830 bytes),
    destructor 0x431c10 (only resets the vtables, then ~C3DTriggerType; deleting thunks 0x431be0
    / 0x432130 / 0x432140). Vtables 0x4a993c (element, 57 slots: C3DTriggerType's 56 + 56) /
    0x4a9928 (listener) / 0x4a94d8 (game object: 7, 16, 231, 260, 267, 268 overridden; 231 is
    code shared with C3DFogTrigger) / 0x4a94c8 (streamer). Own members 0x7a0..0x828, vtordisp
    0x828, the streamer at 0x82c.

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DTriggerType.h"

class C3DMusicTrigger : public C3DTriggerType
{
	public:
	C3DMusicTrigger();														// N2:0x458d60
	virtual ~C3DMusicTrigger();												// 0x431c10

	// ---- element-family slots it adds
	virtual void Trigger(void);												// ESLOT 56 N2:0x4590d0 (+0xe0) fire: play the music

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x431c50
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x431dc0 the camera fires it
	virtual unsigned char v231(CGameObject *other);							// SLOT 231 N2:0x45ce70 collides with the camera and '3JIM' only
	virtual void v260(void);												// SLOT 260 0x431d80 collisions only when TouchActivated
	virtual void v267(int level);											// SLOT 267 0x432080 level loaded: the level conditions fire it
	virtual void v268(int a2);												// SLOT 268 N2:0x459100 ToggleObject: play the music

	// FIELD 0x7a0 0x88
	char		m_music_db[100];		// 0x7a0  "MusicDatabase" (= "none")
	int			m_music_index[5];		// 0x804  "MusicIndex0" (= 0), 1..4 = -1 (not properties)
	int			m_touch_activated;		// 0x818  "TouchActivated" (= 1)
	int			m_required_level;		// 0x81c  "RequiredLevel" (= -1)
	int			m_exact_level;			// 0x820  "ExactLevel" (= -1)
	int			m_remove_level;			// 0x824  "RemoveLevel" (= -1)
};

namespace neg {
// Trigger (element slot 56) of a C3DMusicTrigger known as a CGameObject (C3DTriggerType).
void MusicTriggerTrigger(CGameObject *o);
}

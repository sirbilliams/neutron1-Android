/*  C3DMusicTrigger ('3MUS') - switches the level music: when fired (by a trigger chain, by the
    camera touching it with "TouchActivated", or at a level load whose level matches
    "RequiredLevel" / "ExactLevel" / "RemoveLevel") it stops the music of Jimmy ("JIM1": his
    channel 0xa38, database 0xa4c, the global 0x696004) and plays the first of
    "MusicIndex0".."MusicIndex4" that is not -1 from "MusicDatabase" (not "none"), looped.
    Fired by a trigger chain it is the end of that chain (no NextTrigger), and does nothing
    while a cut scene is skipped.
    Reconstructed from Neutron.exe: constructor 0x431a20 (factory '3MUS', 0x74c bytes),
    destructor 0x431c10 (only resets the vtables, then ~C3DTriggerType; deleting thunks 0x431be0
    / 0x432130 / 0x432140). Element vtable (55 slots: C3DTriggerType's 54 + Trigger); game object
    slots 7, 16, 259, 265 overridden. Own members 0x6bc..0x738, vtordisp 0x744, streamer 0x748.

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DTriggerType.h"

class C3DMusicTrigger : public C3DTriggerType
{
	public:
	C3DMusicTrigger();														// 0x431a20
	virtual ~C3DMusicTrigger();												// 0x431c10

	// ---- element-family slots it adds
	virtual void Trigger(void);												// ESLOT 54 0x431df0 (+0xd8) fire: play the music

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x431c50
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x431dc0 the camera fires it
	virtual void v260(void);												// SLOT 259 0x431d80 collisions only when TouchActivated
	virtual void v267(int level);											// SLOT 265 0x432080 level loaded: the level conditions fire it

	// FIELD 0x6bc 0x88
	char		m_music_db[100];		// 0x6bc  "MusicDatabase" (= "none")
	int			m_music_index[5];		// 0x720  "MusicIndex0".."MusicIndex4" (= 0, -1, -1, -1, -1)
	int			m_touch_activated;		// 0x734  "TouchActivated" (= 1)
	int			m_required_level;		// 0x738  "RequiredLevel"
	int			m_exact_level;			// 0x73c  "ExactLevel"
	int			m_remove_level;			// 0x740  "RemoveLevel"
};

namespace neg {
// Trigger (element slot 56) of a C3DMusicTrigger known as a CGameObject (C3DTriggerType).
void MusicTriggerTrigger(CGameObject *o);
}

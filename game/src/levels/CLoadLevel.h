/*  CLoadLevel ('LOAD') - a level exit: a sprite ("icons.omt" canvas 2) that, when Jimmy touches
    it, hands him the level to load ("LevelName", at its "StartPoint"). "RETURN" sends him back
    to the level and start point he came from. "RequiredTask" (with "RequiredLevel" as a
    minimum or "ExactLevel" as the only state) can hold the exit closed. With a "FadeType" the
    load waits for the fade ("FadeTime"). A few tagged exits move the story on as they are
    used (TagStep: LABTONEIGH, YOKIAN2, YOKIANS2, PLANTSAVED).
    Reconstructed from Neutron.exe: constructor 0x457b10 (factory 'LOAD', 0x6f4 bytes),
    destructor 0x457d60 (deleting thunks 0x457d30 / 0x4586b0 / 0x4586c0), vtables 0x4d06c0
    (element, 56 slots: C3DSprite's 53 + 53..55) / 0x4d06ac (listener) / 0x4d025c (game object
    at +0xc8: 7, 16, 257, 259 overridden) / 0x4d024c (streamer at +0x6f0). Own members
    0x5e8..0x6ec (named here by Negatron's offsets).
    port: Negatron's "LoadScreen", "Promotion" and "GoIfActive" members are kept (unset and not
    registered: Boy Genius has none) for C3DTriggerType's hand-over and port/Companion.cpp.      */
#pragma once
#include "../C3DSprite.h"

class CLoadLevel : public C3DSprite
{
	public:
	CLoadLevel();															// 0x457b10
	virtual ~CLoadLevel();													// 0x457d60

	// ---- element-family slots it adds
	virtual void CallLoadLevel(void);										// ESLOT 53 0x458370 (+0xd4) TagStep, then load LevelName now
	virtual void ev55(void);												// ESLOT 54 0x458400 (+0xd8) TagStep: the story step of a tagged exit

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x457da0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x457ec0 Jimmy: the exit
	virtual void v258(void);												// SLOT 257 0x4583f0 (jump to C3DSprite's)
	virtual void v260(void);												// SLOT 259 0x4583b0 (re)load the sprite canvas

	// FIELD 0x600 0x110
	char		m_level_name[80];		// 0x600  "LevelName": the .gam to load, "RETURN", or "none"
	char		m_start_point[80];		// 0x650  "StartPoint": the C3DStartPoint tag in that level
	char		m_required_task[80];	// 0x6a0  "RequiredTask", "none" = no condition
	int			m_required_level;		// 0x6f0  "RequiredLevel": the task's state must be at least this
	int			m_sound_index;			// 0x6f4  "SoundIndex": played when used, -1 = none
	int			m_exact_level;			// 0x6f8  "ExactLevel": ... and exactly this, -1 = any
	int			m_fade_type;			// 0x6fc  "FadeType", -1 = none
	float		m_fade_time;			// 0x700  "FadeTime"
	int			m_load_screen;			// 0x704  "LoadScreen": the loading picture
	int			m_promotion;			// 0x708  "Promotion": given to Jimmy (+0xc18)
	int			m_go_if_active;			// 0x70c  "GoIfActive": a menu page 3 (level items) item that must be on, -1 = none
};

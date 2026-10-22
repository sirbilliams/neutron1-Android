/*  CLoadLevel ('LOAD') - a level exit: a sprite that, when Jimmy touches it, hands him the level
    to load ("LevelName", at its "StartPoint") through his PlayerLoadLevel. "RETURN" sends him back
    to the level and start point he came from. "RequiredTask" (with "RequiredLevel" as a minimum
    or "ExactLevel" as the only state) can hold the exit closed; "FadeType" fades out first.
    Triggers fire it too: C3DTriggerType's NextTrigger hand-over, and element slots 53 (load its
    level) and 55 (load a level for the object that touched it). Its sprite is "icons.omt" 2.
    Reconstructed from Neutron.exe: constructor 0x457b10, destructor 0x457d60 (deleting thunks
    0x4586b0 / 0x457d30 / 0x4586c0); C3DSprite's 0x5e8 bytes, own members 0x5e8..0x6ec,
    vtordisp 0x6ec, streamer 0x6f0. The CGameObject-family part is at +0xc8.

    Members are named after their 2002 offset in the element view.                          */
#pragma once
#include "../C3DSprite.h"

class CLoadLevel : public C3DSprite
{
	public:
	CLoadLevel();															// N2:0x48e370
	virtual ~CLoadLevel();													// 0x457d60

	// ---- element-family slots it adds
	virtual void ev53(void);												// ESLOT 53 0x458370 load LevelName now (editor slot 64)
	virtual void ev54(void);												// ESLOT 54 0x458400 (+0xd8) used: the story steps of a few exits
	virtual void ev55(CGameObject *who, char *game, char *start);			// ESLOT 55 0x458510 (+0xdc) load <game> at <start> for <who>

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x457da0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x457ec0 touch starts
	virtual void v258(void);												// SLOT 258 0x4583f0 (BG 257; jump to C3DSprite's)
	virtual void v260(void);												// SLOT 260 0x4583b0 (BG 259) (re)load the sprite canvas

	char		m_level_name[80];		// 0x5e8  "LevelName": the .gam to load, "RETURN", or "none"
	char		m_start_point[80];		// 0x638  "StartPoint": the C3DStartPoint tag in that level
	char		m_required_task[80];	// 0x688  "RequiredTask", "none" = no condition
	int			m_required_level;		// 0x6d8  "RequiredLevel": the task's state must be at least this
	int			m_sound_index;			// 0x6dc  "SoundIndex": played when used, -1 = none
	int			m_exact_level;			// 0x6e0  "ExactLevel": ... and exactly this, -1 = any
	int			m_fade_type;			// 0x6e4  "FadeType", -1 = none (StartFade)
	float		m_fade_time;			// 0x6e8  "FadeTime"
};

/*  C3DTriggerType - a C3DSprite (not a C3DSpriteType) that is a link in a chain of triggers: when it fires it calls
    the object named by "NextTrigger" (CallNextTrigger), or with "DEFAULT" hands the camera back
    to the player. Base of the text, music, AI, fade and cut-scene triggers and of CPickupType.
    Reconstructed from Neutron.exe: constructor 0x4478c0, destructor 0x447a30 (only resets the
    vtables, then ~C3DSprite). Layout: C3DSprite (0x5e8), own members 0x5e8..0x6bc, then the
    streamer (vtordisp at +0x6bc, streamer at +0x6c0); element vtable 0x4bd6e4 (54 slots:
    C3DSprite's 53 + CallNextTrigger). No factory ID. (Neutron.exe's trigger has no "DefaultTo".) */
#pragma once
#include "C3DSprite.h"

class C3DTriggerType : public C3DSprite
{
	public:
	C3DTriggerType();														// 0x4478c0
	virtual ~C3DTriggerType();												// SLOT 0  0x447a30 (thunks 0x4482d0 / 0x447a00 / 0x4482e0)

	// (streamer slots 1/2 0x4482f0 / 0x448300: copies of C3DSprite's read_class / write_class)

	// ---- element-family game slots
	virtual void CallNextTrigger(const char *name);							// ESLOT 53 0x447cf0 (+0xd4)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x447c60
	virtual void v242(float t);												// SLOT 242 0x447a70 editor: PgDn shows the next trigger

#include "gen/C3DTriggerType_slots.inc"

	// FIELD 0x5e8 0xd4
	char			m_toggle_object[100];	// 0x5e8 "ToggleObject" (= "none")
	int				m_toggle;				// 0x64c "Toggle" (= -1)
	char			m_next_trigger[100];	// 0x650 "NextTrigger" (= "none"; "DEFAULT": back to the player)
	int				m_fade_type;			// 0x6b4 "FadeType" (unset by the ctor)
	float			m_fade_time;			// 0x6b8 "FadeTime" (unset by the ctor)
#include "gen/C3DTriggerType_fields.inc"
};

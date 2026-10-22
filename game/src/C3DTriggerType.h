/*  C3DTriggerType - a C3DSpriteType that is a link in a chain of triggers: when it fires it calls
    the object named by "NextTrigger" (CallNextTrigger), or with "DEFAULT" hands the camera back
    to the player. Base of the text, music, AI, fade and cut-scene triggers and of CPickupType.
    Reconstructed from Neutron.exe: constructor 0x4478c0, destructor 0x447a30 (only resets the
    vtables, then ~C3DSprite). Layout: C3DSpriteType (0x668), own members 0x668..0x7a0, size
    0x7a0 + the streamer (vtordisp at +0x7a0, streamer at +0x7a4); element vtable 0x4bd6e4
    (56 slots: C3DSprite's 55 + CallNextTrigger). No factory ID.                              */
#pragma once
#include "C3DSpriteType.h"

class C3DTriggerType : public C3DSpriteType
{
	public:
	C3DTriggerType();														// 0x4478c0
	virtual ~C3DTriggerType();												// SLOT 0  0x447a30 (thunks 0x4482d0 / 0x447a00 / 0x4482e0)

	// (streamer slots 1/2 0x4482f0 / 0x448300: copies of C3DSprite's read_class / write_class)

	// ---- element-family game slots
	virtual void CallNextTrigger(const char *name, unsigned char flag);	// ESLOT 55 N2:0x4790b0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x447c60
	virtual void v242(float t);												// SLOT 242 0x447a70 editor: PgDn shows the next trigger
	virtual void v280(void);												// SLOT 280 N2:0x479800 editor: list the trigger chain

#include "gen/C3DTriggerType_slots.inc"

	// FIELD 0x668 0x138
	char			m_toggle_object[100];	// 0x668 "ToggleObject" (= "none")
	int				m_toggle;				// 0x6cc "Toggle" (= -1)
	char			m_next_trigger[100];	// 0x6d0 "NextTrigger" (= "none"; "DEFAULT": back to the player)
	char			m_default_to[100];		// 0x734 "DefaultTo" (= "none": the object DEFAULT gives the camera to)
	int				m_fade_type;			// 0x798 "FadeType" (= -1)
	float			m_fade_time;			// 0x79c "FadeTime" (= 1.0)
#include "gen/C3DTriggerType_fields.inc"
};

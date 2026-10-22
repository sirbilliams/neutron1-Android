/*  C3DLightObj ('3LIO') - a coloured light glow: "sprites.omt" canvas 36, size 100, additive
    (opaque blend when windowed), facing the viewport like the other sprites. Properties Red /
    Green / Blue / Alpha (its colour), Activated (shown), Pulsate with Period (the alpha rises
    0 -> 1 -> 0 over Period seconds, from OffsetTime), and On/OffSoundIndex (numbered sounds
    played when a ToggleObject switches it). ToggleObject (v268): 1 on, 0 off, -1 toggle (at
    most once a second).
    Reconstructed from Neutron.exe: constructor 0x42dba0 (factory '3LIO', 0x624 bytes),
    destructor 0x42dd80 (only resets the vtables, then ~C3DSpriteType; deleting thunks 0x42e1e0
    / 0x4129b0 / 0x42e1f0). Vtables 0x4a6cd0 (element, 53 slots: C3DSprite's) / 0x4a6cbc
    (listener) / 0x4a686c (game object) / 0x4a685c (streamer). Own members 0x5e8..0x61c
    (named by their Negatron offsets), vtordisp 0x61c, the streamer at 0x620. Boy Genius: no
    pause check in Process; shown / hidden with an inline force_show / hide(true).

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DSpriteType.h"

class C3DLightObj : public C3DSpriteType
{
	public:
	C3DLightObj();															// 0x42dba0
	virtual ~C3DLightObj();													// 0x42dd80

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42ddc0 properties
	virtual void Process(float dt);											// SLOT 241 0x42df00 pulsate
	virtual void v260(void);												// SLOT 259 0x42e030 (re)load: blend, colour, shown
	virtual void v268(int a2);												// SLOT 266 0x42e0f0 ToggleObject: on / off

	// FIELD 0x668 0x34
	float		m_red;					// 0x668  "Red" (= 1)
	float		m_green;				// 0x66c  "Green" (= 1)
	float		m_blue;					// 0x670  "Blue" (= 1)
	float		m_alpha;				// 0x674  "Alpha" (= 1)
	int			m_pulsate;				// 0x678  "Pulsate" (= 0)
	int			m_activated;			// 0x67c  "Activated" (= 1): shown
	float		m_period;				// 0x680  "Period" (= 1): pulse length (s)
	float		m_time;					// 0x684  time in the pulse (= OffsetTime on load)
	float		m_pulse;				// 0x688  the pulse's alpha (0..1)
	float		m_offset_time;			// 0x68c  "OffsetTime" (= 0)
	int			m_on_sound;				// 0x690  "OnSoundIndex" (= -1: none)
	int			m_off_sound;			// 0x694  "OffSoundIndex" (= -1: none)
	float		m_toggle_time;			// 0x698  time since the last toggle (-1 toggles need 1 s)
};

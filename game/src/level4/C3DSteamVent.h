/*  C3DSteamVent ('3STE') - a steam-vent button of level 4b ("buttondown.ase" HIDOWN /
    "buttonup.ase" HIUP, texture "switch.png", radius 500) with a C3DLightCone glow above it.
    Available ("ButtonAvailable" 1), it glows in "Red/Green/Blue" (pulsing red part, the cone green);
    touched by the camera object (Jimmy) or a C3DBASEBALL while up it is pressed: 50 points, the
    DOWN animation, sound "AvailSound", the colour halved, the cone off, and it switches on the
    object named "NextButton" (another vent: made available; anything else: NextState 0).
    Unavailable (red cone, dark grey) a touch only plays "NASound". Touched while down (and
    available) it just comes up again.
    Reconstructed from Neutron.exe: constructor 0x442920 (factory '3STE', 0x748 bytes),
    destructor 0x442b20 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x442af0 /
    0x443320 / 0x443330 / 0x443340). Vtables: element 0x4b8048 (C3DAnimated's 72 + 72, 73), game
    object (+0xc0) 0x4b7ba8 (0, 7, 16, 241, 257, 259 overridden). Own members 0x6bc..0x73c,
    vtordisp 0x740.                                                                           */
#pragma once
#include "../C3DAnimated.h"

class C3DLightCone;

class C3DSteamVent : public C3DAnimated
{
	public:
	C3DSteamVent();															// 0x442920
	virtual ~C3DSteamVent();												// 0x442b20

	// ---- element-family slots
	virtual void ev72(void) {}												// ESLOT 72 0x4732a0 (empty)
	virtual void ev73(void);												// ESLOT 73 0x442ed0 made available again (shown cone, colour)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x442b70 properties, animations, texture
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x442cd0 pressed
	virtual void Process(float dt);											// SLOT 241 0x442f50 colours, the cone follows
	virtual void v258(void);												// SLOT 257 0x443100 reload: cone colour, animation, colour
	virtual void v260(void);												// SLOT 259 0x4432c0 C3DAnimated's, then v258, radius 100

	// ---- data members (object offsets)
	float				m_red;				// 0x6bc  "Red" (1)
	float				m_green;			// 0x6c0  "Green" (0.1)
	float				m_blue;				// 0x6c4  "Blue" (0.1)
	int					m_available;		// 0x6c8  "ButtonAvailable" (0)
	int					m_na_sound;			// 0x6cc  "NASound" (-1): played when touched unavailable
	int					m_avail_sound;		// 0x6d0  "AvailSound" (-1): played when pressed
	char				m_next_button[100];	// 0x6d4  "NextButton" ("none"): ObjectTag of the object it switches on
	C3DLightCone		*m_lightcone;		// 0x73c  the glow cone
};

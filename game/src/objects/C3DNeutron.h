/*  C3DNeutron ('3NEU') - a neutron: a spinning energy pickup (sprites.omt canvases 0..7 cycled
    every 0.05 s). Touched by the camera (Jimmy) it gives 5 neutron energy and 5 points, ticks
    the menu's pickup item 0 (unless the energy is full), plays sound 4 and bursts (canvases
    8..12), then hides and stops colliding. It comes back 240 s later. When the level loads
    each neutron is there with a 4 in 10 chance only.
    Reconstructed from Neutron.exe: constructor 0x4329a0 (factory '3NEU', 0x640 bytes),
    destructor 0x432cf0 (only resets the vtables, then ~C3DSpriteType; deleting thunks
    0x433030 / 0x433040). Vtables 0x4ab140 (element, 53 slots, none overridden) / 0x4ab12c
    (listener) / 0x4aacdc (game object at +0xc8: 16, 241, 257, 259 overridden) / 0x4aaccc
    (streamer). Boy Genius has no collision filter and does not save the neutrons' states.

    Members are named after their Negatron offsets (element view; the CGameObject-family
    methods of the original see them 0xc8 lower).                                            */
#pragma once
#include "../C3DSpriteType.h"

class OMediaCanvas;

class C3DNeutron : public C3DSpriteType
{
	public:
	C3DNeutron();															// 0x4329a0
	virtual ~C3DNeutron();													// 0x432cf0

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x432ef0 collected by the camera
	virtual void Process(float dt);											// SLOT 241 0x432d30 animation, burst, coming back
	virtual void v258(void);												// SLOT 257 0x432ec0 reload: first frame
	virtual void v260(void);												// SLOT 259 0x432f70 reload: there 4 times in 10, else gone

	// FIELD 0x668 0x50
	OMediaCanvas	*m_frames[13];		// 0x668  sprites.omt canvases 0..12 (0..7 spin, 8..12 burst)
	int				m_frame;			// 0x69c  current frame
	float			m_time;				// 0x6a0  time on this frame (time since gone, once gone)
	unsigned char	m_collected;		// 0x6a4  touched, bursting
	int				m_channel;			// 0x6a8  the "neutron" sound channel
	unsigned char	m_gone;				// 0x6ac  hidden until it comes back
};

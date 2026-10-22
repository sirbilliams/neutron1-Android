/*  C3DRedNeutron ('3RED') - a red neutron: a pulsing pickup (sprites.omt canvases 0..7 cycled
    every 0.05 s, scaled by the stopwatch's time scale; its size wobbles and its blue channel
    pulses). Touched by the camera (Jimmy) it is collected (Collect, slot 56): an extra life
    (g_lives), the pickup announcement, 500 points, 25 neutron energy, a red flash on Jimmy,
    menu pickup item 46, "red neutron", and its object state counts up; then its NextTrigger
    is called. It bursts (canvases 8..12) and is hidden for good (state saved: hidden on load).
    Unlike C3DNeutron it never comes back.
    Reconstructed from Neutron.exe: constructor 0x43c370 (factory '3RED', 0x80c bytes),
    destructor (only resets the vtables, then ~CPickupType; deleting thunks 0x43c6c0 / 0x43cc00
    / 0x43cc10). Vtables 0x4b1324 (element, 57 slots: CPickupType's 56 + Collect) / 0x4b1310
    (listener) / 0x4b0ec0 (game object) / 0x4b0eb0 (streamer). Own members 0x7b0..0x804,
    vtordisp 0x804, the streamer at 0x808.

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "CPickupType.h"

class OMediaCanvas;

class C3DRedNeutron : public CPickupType
{
	public:
	C3DRedNeutron();														// 0x43c370
	virtual ~C3DRedNeutron();

	// ---- element-family slots it adds
	virtual void Collect(void);												// ESLOT 56 N2:0x469710 (+0xe0) life, score, energy, sound

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x43c730 + "Radius"
	virtual void AddTouching(CGameObject *other);							// SLOT 16  N2:0x4696b0 collected by the camera
	virtual void Process(float dt);											// SLOT 240 0x43c760 pulse, animation, burst
	virtual void v258(void);												// SLOT 258 0x43c980 reload: first frame
	virtual void v260(void);												// SLOT 260 0x43cb10 first frame; hidden if already taken
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 N2:0x4698f0
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 N2:0x4698b0

	// FIELD 0x7b0 0x54
	OMediaCanvas	*m_frames[13];		// 0x7b0  sprites.omt canvases 0..12 (0..7 spin, 8..12 burst)
	int				f_7e4;				// 0x7e4  (not used)
	int				f_7e8;				// 0x7e8  (not used)
	int				m_frame;			// 0x7ec  current frame
	float			m_time;				// 0x7f0  time on this frame
	unsigned char	m_collected;		// 0x7f4  touched, bursting (saved)
	int				m_channel;			// 0x7f8  the "red neutron" sound channel
	unsigned char	m_gone;				// 0x7fc  hidden for good (saved)
	float			m_pulse;			// 0x800  time for the size / colour pulse
};

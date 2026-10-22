/*  C3DRedNeutron ('3RED') - a red neutron: a pulsing pickup (sprites.omt canvases 0..7 cycled
    every 0.05 s; its size wobbles and its blue channel pulses). Touched by the camera (Jimmy)
    it is collected (Collect, slot 54): an extra life, 200 points, 25 neutron energy, menu
    pickup item 1, sound 14, and its object state counts up; then the pickup pop-up and its
    NextTrigger. The one tagged "REDNEUTRON1" first sets the task SCENE to 90. It bursts
    (canvases 8..12) and is hidden for good (hidden on load once taken).
    Reconstructed from Neutron.exe: constructor 0x43c370 (factory '3RED', 0x728 bytes),
    destructor (only resets the vtables, then ~CPickupType; deleting thunks 0x43c6c0 / 0x43cc00
    / 0x43cc10). Vtables 0x4b1324 (element, 55 slots: CPickupType's 54 + Collect) / 0x4b1310
    (listener) / 0x4b0ec0 (game object at +0xc8) / 0x4b0eb0 (streamer at +0x724). Own members
    0x6c8..0x720. Boy Genius does not save its state beyond the object state.

    Members are named after their Negatron offsets (element view; the CGameObject-family
    methods of the original see them 0xc8 lower).                                            */
#pragma once
#include "CPickupType.h"

class OMediaCanvas;

class C3DRedNeutron : public CPickupType
{
	public:
	C3DRedNeutron();														// 0x43c370
	virtual ~C3DRedNeutron();

	// ---- element-family slots it adds
	virtual void Collect(void);												// ESLOT 54 0x43ca90 (+0xd8) life, score, energy, sound

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x43c730 + "Radius"
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x43c9b0 collected by the camera
	virtual void Process(float dt);											// SLOT 241 0x43c760 pulse, animation, burst
	virtual void v258(void);												// SLOT 257 0x43c980 reload: first frame
	virtual void v260(void);												// SLOT 259 0x43cb10 first frame; hidden if already taken

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

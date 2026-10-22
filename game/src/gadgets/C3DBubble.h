/*  C3DBubble ('3BUB') - the bubble Jimmy floats in (the bubble gadget, C3DJimmy f_a14): sprite 1
    of "RetainedSprites.omt" (240 wide, additive blend when not windowed). C3DJimmy fades it in
    (Start), it then bobs up and down (a sine of its height, the width stays) and Jimmy follows
    it; Pop fades it out and hides it. C3DJimmy's constructor makes the one bubble.
    Reconstructed from Neutron.exe: factory '3BUB', constructor 0x410840 (0x620 bytes),
    destructor 0x410a40 (only resets the vtables, then ~C3DPermanentSprite; deleting thunks
    0x410d50 / 0x410a10 / 0x410d60). Start 0x410ca0 and Pop 0x410d00 are its non-virtual
    methods; the original's C3DJimmy sets the "fade in" flag to 0 itself (inlined, 0x428060).

    Own members are named after their Neutron.exe offsets (BG C3DSprite ends at 0x5e8; ours is
    0x14 longer, the names keep the original's offsets).                                       */
#pragma once
#include "../C3DPermanentSprite.h"

class OMediaCanvas;

class C3DBubble : public C3DPermanentSprite
{
	public:
	C3DBubble();															// 0x410840
	virtual ~C3DBubble();													// 0x410a40

	// ---- CGameObject-family overrides
	virtual void Process(float dt);											// BG SLOT 241 0x410ae0 fade in / bob / fade out
	virtual void v260(void);												// BG SLOT 259 0x410a80 the canvas, blend 6 / 7

	// ---- non-virtual methods
	void Start(void);														// 0x410ca0 fade in
	void Pop(void);															// 0x410d00 fade out
	bool Busy(void) const	{ return m_floating || m_fade_in || m_fade_out; }	// (C3DJimmy's tests of +0x614..0x616)
	void FadeInOff(void)	{ m_fade_in = 0; }								// C3DJimmy 0x428060: +0x615 = 0

	// ---- data members (Neutron.exe offsets)
	OMediaCanvas		*m_canvas;		// 0x5e8  sprites 1 of "RetainedSprites.omt"
	int					f_5fc;			// 0x5fc  = 0 (not used)
	float				m_time;			// 0x600  time since Start / Pop
	int					f_604;			// 0x604  = 0 (not used)
	unsigned char		f_608;			// 0x608  = 1 (not used)
	float				m_bob;			// 0x60c  the sine part of the height
	float				m_alpha;		// 0x610  fade
	unsigned char		m_floating;		// 0x614  fully shown: bobbing
	unsigned char		m_fade_in;		// 0x615  fading in (1 s)
	unsigned char		m_fade_out;		// 0x616  fading out (1 s), then hidden
};

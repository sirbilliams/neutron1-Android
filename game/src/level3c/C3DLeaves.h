/*  C3DLeaves ('3LEA') - falling leaves: a sprite (sprites.omt canvases 0x31, 0x2e, 0x2f, 0x30,
    0x2d) that, touched by Jimmy or a baseball or after a random 0..30 s wait, plays "leaves" and
    falls for 7 s (velocity y -150 against the gravity, collisions on, the frames cycled every
    0.07 s until it lands), then goes back to its initial position and waits again.
    Reconstructed from Neutron.exe: constructor 0x42c6f0, destructor 0x42c970 (only resets the
    vtables, then ~C3DSpriteType; deleting thunks 0x42c940 / 0x42cd00 / 0x42cd10). Vtables
    0x4a5b44 (element) / 0x4a5b30 (listener) / 0x4a56e0 (game object: 0, 16, 240, 260
    overridden; 260 is code shared with C3DNeutron / C3DCheckPoint) / 0x4a56d0 (streamer).
    Own members 0x668..0x6c0, vtordisp 0x6c0, the streamer at 0x6c4 (0x6c8 bytes).

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DSpriteType.h"

class OMediaCanvas;

class C3DLeaves : public C3DSpriteType
{
	public:
	C3DLeaves();															// 0x42c6f0
	virtual ~C3DLeaves();													// 0x42c970

	// ---- CGameObject-family overrides
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x42c9b0 Jimmy / a baseball: fall
	virtual void Process(float dt);											// SLOT 240 0x42ca80 falling, waiting
	virtual void v260(void);												// SLOT 260 0x42ca60 first frame (no base call)

	// FIELD 0x668 0x58
	OMediaCanvas	*m_frames[5];		// 0x668  sprites.omt canvases 0x31, 0x2e, 0x2f, 0x30, 0x2d
	char			f_67c[0x28];		// 0x67c  (unused)
	int				m_frame;			// 0x6a4  current frame
	float			m_time;				// 0x6a8  time on this frame
	unsigned char	m_falling;			// 0x6ac
	int				m_channel;			// 0x6b0  the "leaves" sound channel
	unsigned char	m_landed;			// 0x6b4  on the ground: frames stop
	float			m_fall_time;		// 0x6b8  time left falling (7 s)
	float			m_wait;				// 0x6bc  time left until it falls by itself (0..30 s)
};

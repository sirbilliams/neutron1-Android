/*  C3DAnimatedSprite ('3ANI') - a pickup-type sprite that plays a short flip-book: "Sprite1".. 
    "Sprite9" (sprites.omt canvas ids; -1 ends the list; default 60, 61, 62) at "FPS" (10)
    frames a second, tinted "Red" / "Green" / "Blue" / "Alpha" (1). "Loop" 1 repeats it,
    0 stops on the last frame, 2 hides it at the end. "Activated" (1) starts it playing when the
    level loads (0: hidden); "InitallyVisible" (-1: as Activated) 1 shows it, 0 hides it.
    NextState (v268) 1 starts it ("OnSoundIndex"), 0 stops and hides it ("OffSoundIndex"),
    -1 toggles it, at most once a second.
    Reconstructed from Neutron.exe: constructor 0x40e880 (factory '3ANI', 0x738 bytes),
    destructor 0x40eae0 (only resets the vtables, then ~CPickupType; deleting thunks 0x40f290 /
    0x40eab0 / 0x40f2a0). Vtables 0x49085c (element, CPickupType's 54 slots) / 0x490848
    (listener) / 0x4903f8 (game object at +0xc8: 0, 7, 241, 259, 266 overridden) / 0x4903e8
    (streamer at +0x734). Own members 0x6cc..0x730 (named by their Negatron offsets).
    Boy Genius: no pause check; shown / hidden with an inline force_show / hide(true).

    Members are named after their 2002 offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../pickups/CPickupType.h"

class C3DAnimatedSprite : public CPickupType
{
	public:
	C3DAnimatedSprite();													// 0x40e880
	virtual ~C3DAnimatedSprite();											// 0x40eae0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x40eb20 properties
	virtual void Process(float dt);											// SLOT 241 0x40ed40 the flip-book
	virtual void v260(void);												// SLOT 259 0x40ef70 properties loaded: frame count, shown / hidden
	virtual void v268(int state);											// SLOT 266 0x40f160 "NextState": 1 on, 0 off, -1 toggle

	// FIELD 0x7b0 0x64
	float			f_7b0;				// 0x7b0  "Red" (1)
	float			f_7b4;				// 0x7b4  "Green" (1)
	float			f_7b8;				// 0x7b8  "Blue" (1)
	float			f_7bc;				// 0x7bc  "Alpha" (1)
	int				f_7c0;				// 0x7c0  "Activated" (1)
	float			f_7c4;				// 0x7c4  time on this frame
	int				f_7c8;				// 0x7c8  (0, unused)
	int				f_7cc;				// 0x7cc  "OnSoundIndex" (-1)
	int				f_7d0;				// 0x7d0  "OffSoundIndex" (-1)
	float			f_7d4;				// 0x7d4  time since the last toggle
	float			f_7d8;				// 0x7d8  "FPS" (10)
	int				f_7dc;				// 0x7dc  frames (9; v260: up to the first -1)
	int				f_7e0[9];			// 0x7e0  "Sprite1".."Sprite9"
	int				f_804;				// 0x804  the next frame
	int				f_808;				// 0x808  "Loop" (1)
	unsigned char	f_80c;				// 0x80c  playing
	int				f_810;				// 0x810  "InitallyVisible" (-1)
};

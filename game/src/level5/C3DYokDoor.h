/*  C3DYokDoor ('3DOR') - a Yokian door that slides up: a shape from an OMT database
    ("OmtDatabase", default "objects.omt", "OmtIndex" 0), no animations. NextState
    (v268: 0 open, 1 close, other toggles) moves it by "OpenAmount" (500) in y over "DoorSpeed"
    (5 s; reversed half-way if asked) with sound 0x3b, then 0x3a;
    open, it closes by itself after "OpenTime" (3 s). Each NextState also fires the "Next"
    object (an AI trigger or a cut-scene camera). Opened when a Yokian touches it. "ItemClosed"
    (1) is its state on reset. The same door as C3DDoorUpDown, with a database shape.
    Reconstructed from Neutron.exe (verified against Boy Genius): constructor 0x449cb0 (factory
    '3DOR', 0x7b4 bytes), destructor 0x449ec0 (deleting thunks 0x449e90 / 0x44a410 / 0x44a420 /
    0x44a430). Game object vtable (+0xc0): 0, 7, 10, 16, 17, 241, 259, 266 overridden
    (Negatron's names below: Process, v260, v268). Unlike Negatron's door: sounds by index
    (0x3b the loop, 0x3a closing), moves while paused, default collision filter, nothing in
    the savegame, no cut-scene handling. Own members 0x6bc..0x7ac, vtordisp 0x7ac, streamer
    0x7b0 (member names keep Negatron's offsets).                                            */
#pragma once
#include "../C3DAnimated.h"

class C3DYokDoor : public C3DAnimated
{
	public:
	C3DYokDoor();															// 0x449cb0
	virtual ~C3DYokDoor();													// 0x449ec0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x449f70 properties
	virtual void ResetObject(void);											// SLOT 10  0x449f10 back to ItemClosed
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x44a0d0 a Yokian: open
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x44a100 CGameObject's
	virtual void Process(float dt);											// SLOT 240 0x44a110 moving, closing by itself
	virtual void v260(void);												// SLOT 260 0x44a060 properties loaded: the database shape
	virtual void v268(int state);											// SLOT 268 0x44a2a0 "NextState": 0 open, 1 close, else toggle

	// ---- data members (object offsets)
	int					f_924;			// 0x924  "ItemClosed" (1): closed on reset
	int					f_928;			// 0x928  closed (or closing)
	char				f_92c[100];		// 0x92c  "Next" ("none")
	char				f_990[100];		// 0x990  "OmtDatabase" ("objects.omt")
	int					f_9f4;			// 0x9f4  "OmtIndex" (0)
	float				f_9f8;			// 0x9f8  moving time left
	float				f_9fc;			// 0x9fc  "DoorSpeed" (5 s for the whole way)
	float				f_a00;			// 0xa00  "OpenTime" (3 s)
	float				f_a04;			// 0xa04  time open
	int					f_a08;			// 0xa08  = 0 (unused)
	float				f_a0c;			// 0xa0c  "OpenAmount" (500)
	int					f_a10;			// 0xa10  the opening loop channel (-1: none)
};

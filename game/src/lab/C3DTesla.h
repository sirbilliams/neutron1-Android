/*  C3DTesla ('3TES') - a Tesla coil in Negatron's lab: a morph object ("tesla.ase", DEFAULT)
    whose four textures ("tesla0000..0003.png", each added onto the scene) cycle every 0.1 s
    while it is on (f_930, from "ItemActive"). On: shown, with the "electric spark" sound (3D,
    when "SoundActive"); off: hidden, silent. NextState (v268) 0 turns it off, 1 on, anything
    else toggles. Touching Jimmy while on shocks him, pushed away from the coil (his element
    slot 119). Water balloons pop on it (C3DWaterBalloon).
    Reconstructed from Neutron.exe: constructor 0x445870 (factory '3TES', 0x944 bytes),
    destructor N2:0x476140 (stops the sound; deleting thunks 0x4459a0 / 0x445f00 / 0x445f10 /
    0x445f20).

    Original layout: as C3DAnimated (element vtable 0x4baf18, 96 slots: C3DAnimated's 94 + 94,
    95; OMediaAnim vtable 0x4baec8 at +0x90; listener 0x4baf04 at +0x30; CLocalGameObject vtable
    0x4baa78 at +0xc0), own members 0x924..0x93c, vtordisp 0x93c, streamer 0x940.          */
#pragma once
#include "../C3DAnimated.h"

class OMediaStreamOperators;

class C3DTesla : public C3DAnimated
{
	public:
	C3DTesla();																// 0x445870
	virtual ~C3DTesla();													// N2:0x476140

	// ---- element-family slots
	virtual void ev94(void);												// ESLOT 94 N2:0x476670 start the spark sound (SoundActive, not playing)
	virtual void ev95(void);												// ESLOT 95 N2:0x4766d0 stop it

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x445a70 properties, animation, textures
	virtual void ResetObject(void);											// SLOT 10  0x44d4e0 (a jump to CLocalGameObject's: the start position)
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x445ba0 Jimmy: shock
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x44a100 (CGameObject's)
	virtual void Process(float dt);											// SLOT 240 N2:0x476420 texture cycle
	virtual void v260(void);												// SLOT 260 N2:0x476640 on = ItemActive, additive 0.8
	virtual void v266(void);												// SLOT 266 0x445dd0 sound on / off by the state
	virtual void v268(int state);											// SLOT 268 N2:0x4764f0 "NextState": 0 off, 1 on, else toggle
	virtual void v277(void);												// SLOT 277 0x445e70 + stop the sound
	virtual void v278(void);												// SLOT 278 N2:0x476720 + the sound again
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 N2:0x476590 + on
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 N2:0x4765c0 + on (shown / hidden)

	// ---- data members (object offsets)
	short				f_924;			// 0x924  texture shown (0..3)
	float				f_928;			// 0x928  time since the last texture change
	int					f_92c;			// 0x92c  "ItemActive" (1)
	int					f_930;			// 0x930  on (1)
	int					f_934;			// 0x934  spark sound channel (-1)
	int					f_938;			// 0x938  "SoundActive" (0)
};

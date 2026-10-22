/*  C3DTesla ('3TES') - a Tesla coil: a morph object ("tesla.ase", DEFAULT) whose four textures
    ("tesla0000..0003.png", each added onto the scene by blend 6 / 1) cycle every 0.1 s while it
    is on, drawn at alpha 0.7. On: shown, with the looping electric sound (sound index 0xb0, 3D);
    off: hidden, silent. NextState (v268) 0 turns it off, 1 on, anything else toggles. Touching
    Jimmy while on shocks him, pushed away from the coil (his element slot 88, TouchedShock).
    Base of C3DSparkWire.
    Reconstructed from Neutron.exe: constructor 0x445870 (factory '3TES', 0x6d8 bytes),
    destructor 0x4459d0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4459a0 /
    0x445f00 / 0x445f10 / 0x445f20). Vtables: element 0x4baf18 (74 slots: C3DAnimated's 72 + 72,
    73), OMediaAnim 0x4baec8, listener 0x4baf04, game object (+0xc0) 0x4baa78 (0, 7, 10, 16, 17,
    241, 259, 266, 272, 273 overridden). Own members 0x6bc..0x6d0, vtordisp 0x6d4.         */
#pragma once
#include "../C3DAnimated.h"

class C3DTesla : public C3DAnimated
{
	public:
	C3DTesla();																// 0x445870
	virtual ~C3DTesla();													// 0x4459d0

	// ---- element-family slots
	virtual void ev94(void);												// ESLOT 72 0x445e00 start the spark sound
	virtual void ev95(void);												// ESLOT 73 0x445e40 stop it

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x445a70 property, animation, textures
	virtual void ResetObject(void);											// SLOT 10  0x445a20 on = ItemActive, shown / hidden
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x445ba0 Jimmy: shock
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17  0x44a100 (CGameObject's)
	virtual void Process(float dt);											// SLOT 241 0x445ca0 texture cycle
	virtual void v260(void);												// SLOT 259 0x445dd0 sound on / off by the state
	virtual void v268(int state);											// SLOT 266 0x445d30 "NextState": 0 off, 1 on, else toggle
	virtual void v277(void);												// SLOT 272 0x445e70 + stop the sound
	virtual void v278(void);												// SLOT 273 0x445e90 + the sound again

	// ---- data members (object offsets)
	int					f_6bc;			// 0x6bc  texture shown (0..3; the constructor writes 4 bytes, the rest reads a short)
	float				f_6c0;			// 0x6c0  time since the last texture change
	int					f_6c4;			// 0x6c4  "ItemActive" (1)
	int					f_6c8;			// 0x6c8  on (1)
	int					f_6cc;			// 0x6cc  spark sound channel (-1)
};

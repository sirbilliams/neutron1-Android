	// ---- C3DJimmy part A (jimmy/C3DJimmy_a.cpp and C3DJimmy.cpp): the CGameObject-family
	// overrides and C3DPlayer's slots. CGameObject-family slots keep our (Negatron-numbered)
	// names; the comment gives Neutron.exe's slot.

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x422ac0 the .ase animations, the textures
	virtual void UnInitObject(void);										// SLOT 8   0x422a90 + close SoundEffects.omt
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x422d30 bumped into: rocket, sub, jeep, tank, enemies, buttons
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// BG SLOT 221 0x42a660 swinging: hitting a wall lets go
	virtual void Process(float dt);											// BG SLOT 241 0x424600
	virtual void v242(float t);												// BG SLOT 242 0x4235b0 stop the shrink ray; new game: forget object states / pickups
	virtual void v243(float dt);											// BG SLOT 243 0x426030 control: the shot, the jet pack, landing, the cursors
	virtual void v258(void);												// BG SLOT 257 0x423580 + become the camera
	virtual void v260(void);												// BG SLOT 259 0x423610 "PostLoad": Goddard, the jeep, gadgets by level, the start point
	virtual void v261(void);												// BG SLOT 260 0x42afc0 stop + StepSoundOff + "STOP"
	virtual void v271(void);												// BG SLOT 270 0x42ae00 gadget off, the cursor off, menu blocked
	virtual void v277(void);												// BG SLOT 272 0x425c40 pause: the music and the loops off
	virtual void v278(void);												// BG SLOT 273 0x425db0 resume: the music and the loops again

	// ---- C3DPlayer's slots
	virtual void JetpackOn(int a);											// ESLOT 78 0x429970 the gadget bar, the jet pack sound
	virtual void JetpackOff(void);											// ESLOT 80 0x429910 bar and sound off
	virtual void StepSoundOn(void);											// ESLOT 81 0x4299d0 the level's footstep loop
	virtual void StepSoundOff(void);										// ESLOT 82 0x429a00
	virtual void Bounced(void) {}											// ESLOT 85 0x42a720 (empty)
	virtual void TouchedShock(OMedia3DVector plane);						// ESLOT 88 0x42a7e0 SPLAT, pushed back off the wall
	virtual void TouchedHurt(void);											// ESLOT 89 0x42a8d0 hit: flashing, energy -5, score -5

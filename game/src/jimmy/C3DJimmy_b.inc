	// ---- C3DJimmy part B (jimmy/C3DJimmy_b.cpp): Jimmy's own element slots (Neutron.exe 93..130;
	// 107, 116, 117, 124..127 are the PDA menu's, jimmy/C3DJimmy_menu.inc).
	virtual void ActivateGadget(short g);									// ESLOT 93  0x428d50 (VR page: load that gadget's VR level)
	virtual void DeactivateGadget(void);									// ESLOT 94  0x427ff0 the gadget in use (g_4f0588) off
	virtual void MenuMark(void);											// ESLOT 95  0x4299b0 menu +0x470
	virtual void ThrowBaseball(float x, float y, float z, float w);			// ESLOT 96  0x429a30 the next baseball from v224(x, y, z)
	virtual void GadgetKeys(void);											// ESLOT 97  0x425870 the PDA / gadget keys
	virtual unsigned char GrappleSwing(void);								// ESLOT 98  0x429d00 gadget 7: swinging on the rope, the target cursor
	virtual void Slot99(void) {}											// ESLOT 99  0x472970 (empty)
	virtual void EnterWater(void);											// ESLOT 100 0x42aa70 "Calling EnterWater"
	virtual void LeaveWater(void);											// ESLOT 101 0x42aac0 "Calling LeaveWater"
	virtual void Splash(void);												// ESLOT 102 0x42ab00
	virtual void Slot103(void) {}											// ESLOT 103 0x472970 (empty)
	virtual void Shock(float energy, int points, float time);				// ESLOT 104 0x42a870 shocked: energy, score, flashing for <time>
	virtual void HeadHit(void);												// ESLOT 105 0x425170 rising into a ceiling: stopped there
	virtual void PlaceShadow(float dt);										// ESLOT 106 0x4252e0 the shadow on the floor below
	virtual void Trail(float dt);											// ESLOT 108 0x425490 remember the way (or walk it back)
	virtual void HurtFlash(float dt);										// ESLOT 109 0x4255c0 hit / shock flashing, the splat push
	virtual void ShrinkRay(float dt);										// ESLOT 110 0x426e40 no gadget, the ray: shrink what is in front
	virtual void GoddardHeals(float dt);									// ESLOT 111 0x427340 health while Goddard is there
	virtual unsigned char GrappleFire(float dt);							// ESLOT 112 0x426a70 gadget 7: fire the hook upwards / reel in
	virtual unsigned char OutOfEnergy(float dt);							// ESLOT 113 0x4269d0 "not available" (1), else 0
	virtual void PowerGadget(float dt);										// ESLOT 114 0x424e80 gadget 2: power timer, the gadget bar
	virtual void AddHealth(float d);										// ESLOT 115 0x42aa20 f_a28 += d, kept in 0..100
	virtual void Slot118(void) {}											// ESLOT 118 0x472970 (empty)
	virtual void Shrunk(CGameObject *o);									// ESLOT 119 0x42ac10 the ray shrank <o>: SHRINK, the level's reactions
	virtual void JetpackFall(void);											// ESLOT 120 0x425120 jumping: FALL
	virtual void SetStepSound(int id);										// ESLOT 121 0x42aec0 another footstep loop
	virtual void SaveCheckpoint(void);										// ESLOT 122 0x42af00 remember the level and our position
	virtual void SaveCheckpointShow(void);									// ESLOT 123 0x42af50 the same + HUD picture 19
	virtual void ProcessGadgets(float dt);									// ESLOT 128 0x424570 the gadget keys, the menu timer, the menu input
	virtual void GrappleRelease(void);										// ESLOT 129 0x429c10 let go of the rope
	virtual void ScorePopup(float dt);										// ESLOT 130 0x424d10 the "+N" score pop-up

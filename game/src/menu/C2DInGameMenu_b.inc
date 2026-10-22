// C2DInGameMenu part b: per frame (inside the class). Bodies in menu/C2DInGameMenu_b.cpp.
	// The cursor (pad), the pickup pop-up, the HUD, the yes/no box and its actions.
	virtual void Process(float dt);												// SLOT 241 0x404080
	// After a level load: the story message of the level, the fade in.
	virtual void v260();														// SLOT 259 0x4072d0
	virtual void v308();														// SLOT 308 0x407070 the story message panel
	virtual void v311(float dt);												// SLOT 311 0x403c50 the screen fade
	virtual void v312(float dt);												// SLOT 312 0x406690 the end-of-level bonus count
	virtual void v313(float dt);												// SLOT 313 0x407370 the pulse clock
	virtual void v314(int sound);												// SLOT 314 0x407410 the menu's sound (-1 stops it)
	virtual void v315();														// SLOT 315 0x407490 hide the message lines

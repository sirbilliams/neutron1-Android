// C3DJimmy: the PDA menu methods (inside the class). Bodies in jimmy/C3DJimmy_menu.cpp (agent F's
// part; the other Jimmy slots they call are hooks, jimmy/JimmyMenu.h).
	// Per frame: the HUD numbers when the PDA is closed; with it open, the pointer over the
	// items and a click on one (by page).
	virtual unsigned char MenuInput(float dt);								// ESLOT 107 0x427370
	virtual void MenuTravel(short place);									// ESLOT 116 0x4289a0 phone booth map: go to place 0..8
	virtual void OpenPhoneMenu(void);										// ESLOT 117 0x42abb0 the phone booth map (toggles)
	virtual void ToggleMainMenu(void);										// ESLOT 124 0x42ab60 the PDA's main page (Esc)
	virtual void MainMenuAction(short item);								// ESLOT 125 0x428870 an item of the main page
	virtual void ActivateActiveMenu(int page);								// ESLOT 126 0x425ef0 "Calling ActivateActiveMenu()"
	virtual void DeactivateActiveMenu(void);								// ESLOT 127 0x425b20 "Calling DeactivateActiveMenu()"
	// FIELD 0x55f 1
	unsigned char		f_55f;			// 0x55f  the PDA is open
	// FIELD 0x1e25 1
	unsigned char		f_1e25;			// 0x1e25 the click was handled (until the button is released)
	// FIELD 0x1e34 4
	float				f_1e34;			// 0x1e34 0 when the PDA opens (10 when a gadget is picked)
	// FIELD 0x1e4c 4
	int					f_1e4c;			// 0x1e4c the page to go back to (3), -1 = close

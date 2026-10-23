// C2DInGameMenu part a: construction, sprites, menu pages (inside the class). Bodies in
// menu/C2DInGameMenu_a.cpp.
	// Closes omt\screens.omt: the base's UnInitObject, then the database and its stream.
	virtual void UnInitObject();												// SLOT 8 0x402b40
	// Sprites: placement in 640x480 space (z 0, centred auto-align, free size).
	virtual void v275(OMediaCanvasElement *e, float x, float y);				// SLOT 275 0x402cc0 position
	virtual void v276(OMediaCanvasElement *e, float x, float y, int canvas_id, float unused, bool scaled);	// SLOT 276 0x402d70 canvas + position
	virtual void v277(OMediaCanvasElement *e, int canvas_id, bool scaled);		// SLOT 277 0x402e30 canvas, sized (scaled to the screen)
	virtual OMediaCanvas *v278(int canvas_id);									// SLOT 278 0x402f00 a canvas of screens.omt (unlocked)
	// The current page: hide and free it / load and show it.
	virtual void v279();														// SLOT 279 0x402f30
	virtual void v280();														// SLOT 280 0x4033f0
	virtual unsigned short v281();												// SLOT 281 0x403860 (0)
	virtual void v282(float health);											// SLOT 282 0x403980 health bar height
	virtual void v284();														// SLOT 284 0x4039e0 f_4be = 1
	virtual void v285();														// SLOT 285 0x4039f0 f_4be = 0
	virtual unsigned short v286(short item);									// SLOT 286 0x403bd0 the clicked item (f_4c8)
	virtual void v288();														// SLOT 288 0x403bf0 f_4bc = f_4c8 = 3
	// The power bar (0x4f7cb0 frame, 0x4f8028 bar): show, hide, red (empty), white (full).
	virtual void v289();														// SLOT 289 0x4036c0
	virtual void v290();														// SLOT 290 0x403700
	virtual void v291();														// SLOT 291 0x403730
	virtual void v292();														// SLOT 292 0x4037a0
	virtual void v293();														// SLOT 293 0x404de0 CreateMenuPages
	virtual void v294(MenuPage *page, short index, short rollover_id, short active_id, short x, short y);	// SLOT 294 0x406240 SetMenuItem
	virtual void v295(MenuPage *page);											// SLOT 295 0x4060d0 displayMenu
	virtual void v296(MenuPage *page);											// SLOT 296 0x406080 HideMenu
	virtual short v297(short mx, short my);										// SLOT 297 0x403a30 rollovers; the item under (mx, my), -1
	virtual short v298();														// SLOT 298 0x403a10 current page
	virtual void v299(short page);												// SLOT 299 0x403a20
	virtual void v300(int gadget);												// SLOT 300 0x405fb0 the current gadget's pictures
	virtual void v301(bool show);												// SLOT 301 0x406560 picture 0x67 (0x4f7c90)
	virtual void v302(int canvas_id, bool show, int x, int y);					// SLOT 302 0x4063e0 ShowHudPicture (0x4f8150)
	virtual void v303(bool show);												// SLOT 303 0x403420 picture 0x6b (windowed only)
	virtual void v304();														// SLOT 304 0x406650 message panel up
	virtual void v305();														// SLOT 305 0x406590 message panel down
	virtual void v306(int index, int canvas_id);								// SLOT 306 0x402b70 a story message line sprite
	virtual void v307(int index, int line, bool show);							// SLOT 307 0x402c40 place / hide message line
	virtual void v309(int page);												// SLOT 309 0x402f60 LoadMyMenu
	virtual void v310(int page);												// SLOT 310 0x4032c0 UnloadMyMenu

	// Compatibility names (port) for the Negatron callers: the same Boy Genius slots.
	void ShowGadgetBar()						{ v289(); }
	void HideGadgetBar()						{ v290(); }
	void GadgetBarEmpty()						{ v291(); }
	void GadgetBarFull()						{ v292(); }
	void ShowHudPicture(int id, bool show, int x, int y)	{ v302(id, show, x, y); }
	void v316(bool show)						{ v301(show); }
	int  GetCurrentGadget()						{ return f_4d8; }
	unsigned char IsCurrentGadget(int g)		{ return f_4d8 == g; }

	// ---- own members (0x4b4..0x51c)
	// FIELD 0x4b4 8
	OMediaDataBase		*f_4b4;				// 0x4b4  omt\screens.omt
	OMediaFileStream	*f_4b8;				// 0x4b8  its stream
	// TYPE 0x4bc short
	// TYPE 0x4c8 short
	// TYPE 0x4d4 short
	// TYPE 0x4d8 int
	// FIELD 0x4e4 4
	OMediaCanvasElement	*f_4e4;				// 0x4e4  picture 0xfd at (315, 100)
	// TYPE 0x4ec int
	// TYPE 0x4f4 float
	// TYPE 0x4f8 float
	// TYPE 0x4fc int
	// TYPE 0x500 int
	// TYPE 0x504 int
	// TYPE 0x508 int
	// TYPE 0x50c int
	// TYPE 0x510 int
	// TYPE 0x514 float
	// TYPE 0x518 float

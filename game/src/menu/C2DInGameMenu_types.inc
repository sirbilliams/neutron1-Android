// C2DInGameMenu: types shared by the parts (inside the class).

	// One item of a menu page (0x28 bytes; constructor 0x405f70).
	struct MenuItem
	{
		OMediaCanvasElement	*rollover;		// 0x00  picture rollover_id (LoadMyMenu)
		OMediaCanvasElement	*active;		// 0x04  picture active_id
		short	f_08;						// 0x08
		float	x, y;						// 0x0c  position (640x480 space)
		int		f_14;						// 0x14
		float	f_18;						// 0x18  = 1.0
		short	type;						// 0x1c  0 off, 1 shown, 2 / 3 highlight shown / hidden on hover, 4 dimmed
		short	w, h;						// 0x1e  size of the last picture LoadMyMenu made
		short	rollover_id;				// 0x22  canvas IDs in screens.omt, -1 = none
		short	active_id;					// 0x24
		short	shown;						// 0x26
		MenuItem() : rollover(0), active(0), f_08(0), x(0), y(0), f_14(0), f_18(1.0f), type(0),
					 w(0), h(0), rollover_id(-1), active_id(-1), shown(0) {}
	};
	enum { MENU_ITEMS = 29, MENU_COUNT = 7 };
	// A menu page (0x490 bytes): its background picture and 29 items.
	struct MenuPage
	{
		OMediaCanvasElement	*background;	// 0x00
		short	background_id;				// 0x04
		MenuItem items[MENU_ITEMS];			// 0x08
		MenuPage() : background(0), background_id(0) {}
	};
	enum
	{
		MENU_GADGETS = 0, MENU_PHONE = 1, MENU_PICKUPS = 2, MENU_MAIN = 3, MENU_SAVE = 4,
		MENU_OPTIONS = 5, MENU_CREDITS = 6
	};
	static MenuPage *&Page(int i) { return g_menu_pages[i]; }	// 0x4f8164[i]
	static MenuPage *GetPage(int i) { return g_menu_pages[i]; }

	// A fixed HUD picture (0x18 bytes, static in the original: 0x4f7c40, 0x4f7c98, 0x4f7cb8,
	// 0x4f7ef0[8], 0x4f8100, 0x4f8120, 0x4f8138; static initialisers 0x401010..0x4013c0).
	struct HudPic
	{
		OMediaCanvasElement	*elt;			// 0x00  made by Show
		short	canvas_id;					// 0x04  -1 = none
		short	x, y;						// 0x06
		int		src_blend, dst_blend;		// 0x0c
		OMediaDataBase	*db;				// 0x14
		HudPic() : elt(0), canvas_id(-1), x(0), y(0), src_blend(6), dst_blend(7), db(0) {}
		void Show(void);					// 0x403490 make the element if needed, show it
		void Delete(void);					// (inlined) hide and delete the element and its canvas
		OMediaCanvas *Canvas(int id);		// 0x403690
	};
	static HudPic s_gadget_pics[8];			// 0x4f7ef0  the current gadget's picture (by f_4d8)
	static HudPic s_msg_arrow;				// 0x4f7c40
	static HudPic s_pic_4f7c98;				// 0x4f7c98  deleted by the bonus count
	static HudPic s_pic_4f7cb8;				// 0x4f7cb8  (fade mode 3)
	static HudPic s_pic_4f8008;				// 0x4f8008  (fade mode 2; never given a picture)
	static HudPic s_fade_pic;				// 0x4f8100  the fade picture (fade modes 2 / 3)
	static HudPic s_loading_pic;			// 0x4f8120  ShowLoadingPicture
	static HudPic s_pic_4f8138;				// 0x4f8138  v303

	// The ten slots of the load / save page (20 bytes each at 0x4f82e4): what the headers of
	// JimmyGame1..10.tsk say; C3DJimmy fills them when the page opens.
	struct SaveSlotInfo
	{
		int		score;			// 0x00  -1 = no save in this slot
		int		f_04, f_08, f_0c;
		int		scene;			// 0x10  the story message number (the 4th int of the file)
	};

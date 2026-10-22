/*  C2DInGameMenu part a: constructor 0x401430, destructor 0x4029c0, UnInitObject (slot 8), the
    sprite helpers (275..278), the HUD bars and pictures, the menu pages (279, 280, 293..300,
    309, 310) and the story message lines (304..307). See C2DInGameMenu.h.

    Sprites are OMT canvas elements on the 2D layer (0xd0-byte OMediaCanvasElements in the
    original), placed in 640x480 space by v275 / v276. Two idioms of the original:
      show:   `if (+0x70) { +0x70 = 1; hide(false); }`  = OMT 2.5's force_show();
      delete: hide(true), delete its canvas (+ reset), delete the element  = FreeElt below.
    Constants: 0x48d40c = 1/640, 0x48d408 = 1/480, 0x48d910 = 27, 0x48d918 = 1.044 (double),
    0x48d924 = 1, 0x48d954 = 2, 0x48d400 = 50, 0x48d404 = 30.                                */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMT headers)
#include "../C2DInGameMenu.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Todo.h"
#include "OMediaCanvasElement.h"
#include "OMediaCanvas.h"
#include "OMediaDataBase.h"
#include "OMediaLayer.h"
#pragma GCC diagnostic pop

#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- statics

C2DInGameMenu::HudPic C2DInGameMenu::s_gadget_pics[8];	// 0x4f7ef0 (ten 0x18 records, 0x4010f0; eight used)
C2DInGameMenu::HudPic C2DInGameMenu::s_msg_arrow;		// 0x4f7c40
C2DInGameMenu::HudPic C2DInGameMenu::s_pic_4f7c98;		// 0x4f7c98
C2DInGameMenu::HudPic C2DInGameMenu::s_pic_4f7cb8;		// 0x4f7cb8
C2DInGameMenu::HudPic C2DInGameMenu::s_pic_4f8008;		// 0x4f8008
C2DInGameMenu::HudPic C2DInGameMenu::s_fade_pic;		// 0x4f8100
C2DInGameMenu::HudPic C2DInGameMenu::s_loading_pic;		// 0x4f8120
C2DInGameMenu::HudPic C2DInGameMenu::s_pic_4f8138;		// 0x4f8138

// --------------------------------------------------------------------------- helpers

static inline float ScaleX(void) { return (float)(short)g_screen_w * 0.0015625f; }	// 0x48d40c
static inline float ScaleY(void) { return (float)(short)g_screen_h * 0.0020833334f; }	// 0x48d408

// `new OMediaCanvasElement` linked to the 2D layer (vtable +0x34).
static OMediaCanvasElement *NewElt(void)
{
	OMediaCanvasElement *e = new OMediaCanvasElement;
	if (e) e->link_layer(g_layer_hud2d);
	return e;
}

static inline void Show(OMediaCanvasElement *e)	{ e->force_show(); }
static inline void Hide(OMediaCanvasElement *e)	{ e->hide(true); }

// The delete idiom: hide, delete the canvas (it came from the database cache) and reset the
// element, delete the element. port: the canvas is detached before its delete (OMT 2.5's
// reset() would unlock the freed canvas).
static void FreeElt(OMediaCanvasElement *&e, bool hide_first, bool with_canvas = true)
{
	if (!e) return;
	if (hide_first) e->hide(true);
	OMediaCanvas *c = e->get_canvas();
	if (with_canvas && c)
	{
		e->set_canvas(NULL);
		delete c;
		e->reset();
	}
	delete e;
	e = NULL;
}

static void SetAlpha(OMediaCanvasElement *e, float a)
{
	OMediaFARGBColor c;
	c.alpha = a; c.red = a; c.green = a; c.blue = a;	// +0x90..+0x9c all set to a
	e->set_diffuse(c);
}

// --------------------------------------------------------------------------- HudPic

// 0x403690: canvas `id` of the picture's database, its lock count taken back.
OMediaCanvas *C2DInGameMenu::HudPic::Canvas(int id)
{
	OMediaCanvas *c = LoadCanvasID(db, id);					// 0x477890
	if (c) c->db_unlock();									// lock count (+0x14) down if > 0
	return c;
}

// 0x403490
void C2DInGameMenu::HudPic::Show(void)
{
	if (elt)
	{
		elt->force_show();
		return;
	}
	if (canvas_id < 0) return;
	elt = NewElt();
	if (!elt) return;
	const float sx = ScaleX(), sy = ScaleY();
	OMediaCanvas *c = NULL;
	if (canvas_id != -1)
	{
		c = Canvas(canvas_id);
		if (c) elt->set_size((float)c->get_width() * sy, (float)c->get_height() * sx);	// sic: x size by the y scale and back
	}
	elt->set_canvas(c);
	elt->set_canvas_flags(omcanef_FreeWorldSize);
	elt->set_auto_align(omaac_Center, omaac_Center);
	elt->setz(0);
	elt->setx(sx * (float)x - (float)((short)g_screen_w / 2));
	elt->sety(((float)((short)g_screen_h / 2) - elt->get_height()) - sy * (float)y);
	elt->force_show();
	elt->set_blend((omt_BlendFunc)src_blend, (omt_BlendFunc)dst_blend);
}

void C2DInGameMenu::HudPic::Delete(void)
{
	FreeElt(elt, true);
}

// --------------------------------------------------------------------------- 0x401430

C2DInGameMenu::C2DInGameMenu()
{
	SetName((char *)"C2DINGAMEMENU");
	Log("C2DInGameMenu()");
	CLocalGameObject::InitObject();							// (direct call 0x458820)
	const bool windowed = g_windowed;						// 0x509a13
	FindDevice(2);											// 0x4783c0 the joystick
	CGameObject::v64(0.3f);									// (direct call 0x4711e0)
	m_object_id = 'NEM2';
	f_4be = 0;
	g_4f8030 = 0; g_4f8034 = 0; g_4f8038 = 0; g_4f803c = 0;
	f_4b4 = NULL;
	f_4b8 = NULL;
	f_4bf = 0;
	f_4e8 = 0;
	f_50c = 0;
	f_510 = -1;
	f_4b8 = InitFileStream("omt\\screens.omt");				// 0x476170 (through a std::string)
	f_4b4 = InitDatabase(f_4b8);							// 0x476000
	g_screens_db = f_4b4;									// port: Negatron's global, for callers not ported yet
	v299(0);
	v293();													// the menu pages
	f_4bc = 3;
	f_4c8 = 3;

	// the HUD: lives picture, health bar, power bar
	const omt_BlendFunc add = windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One;
	g_hud_lives = NULL;
	g_hud_lives = new OMediaCanvasElement;
	if (g_hud_lives) g_hud_lives->link_layer(g_layer_hud2d);
	g_hud_lives->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
	g_hud_lives->set_canvas(NULL);
	v276(g_hud_lives, 32.0f, 0.0f, 0x17, 1.0f, true);

	g_health_bar = NewElt();
	g_health_bar->set_blend(omblendfc_Src_Alpha, add);
	g_health_bar->set_canvas(NULL);
	v276(g_health_bar, 6.0f, 4.0f, 0x16, 1.0f, true);

	g_health_frame = NewElt();
	g_health_frame->set_blend(omblendfc_Src_Alpha, add);
	g_health_frame->set_canvas(NULL);
	v276(g_health_frame, 3.0f, 7.0f, 0x15, 1.0f, true);

	g_gadget_bar_frame = NewElt();
	g_gadget_bar_frame->set_blend(omblendfc_Src_Alpha, add);
	g_gadget_bar_frame->set_canvas(NULL);
	v276(g_gadget_bar_frame, 320.0f, 7.0f, 0x15, 1.0f, true);
	Hide(g_gadget_bar_frame);

	g_gadget_bar = NewElt();
	f_4d0 = -1;
	g_gadget_bar->set_blend(omblendfc_Src_Alpha, add);
	g_gadget_bar->set_canvas(NULL);
	v276(g_gadget_bar, 322.0f, 4.0f, 0x16, 1.0f, true);
	g_gadget_bar->set_size(ScaleX() * 30.0f, ScaleY() * 50.0f);	// 0x48d404, 0x48d400
	Hide(g_gadget_bar);

	g_4f8188 = 20.0f;
	for (int i = 0; i < 44; i++) g_story_lines[i] = NULL;

	// the current gadget's picture (one per gadget, bottom left)
	static const short kGadgetPics[8] = { 0x6d, 0x73, 0x6f, 0x71, 0x74, 0x72, 0x76, 0x6e };
	for (int i = 0; i < 8; i++)
	{
		HudPic &p = s_gadget_pics[i];
		p.db = f_4b4;
		p.canvas_id = kGadgetPics[i];
		p.x = 0;
		p.y = 0x19b;
		p.src_blend = 6;
		p.dst_blend = 7;
	}

	// the gadget icons (bottom left, one shown: v300)
	static const short kGadgetIcons[8] = { 0x5e, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d };
	for (int i = 0; i < 8; i++)
	{
		g_gadget_icons[i] = NewElt();
		v276(g_gadget_icons[i], 0.0f, 410.0f, kGadgetIcons[i], 1.0f, true);
		Hide(g_gadget_icons[i]);
	}
	for (int i = 0; i < 8; i++)
		g_gadget_icons[i]->set_blend(omblendfc_Src_Alpha, windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);

	s_msg_arrow.db = f_4b4;
	s_msg_arrow.canvas_id = 0x75;
	s_msg_arrow.x = 0x189;
	s_msg_arrow.y = 0x19b;
	s_msg_arrow.src_blend = 6;
	s_msg_arrow.dst_blend = 7;
	s_pic_4f7c98.db = f_4b4;
	s_pic_4f7c98.canvas_id = 0;
	s_pic_4f7c98.x = 0;
	s_pic_4f7c98.y = -1;
	s_pic_4f7c98.src_blend = 6;
	s_pic_4f7c98.dst_blend = 7;
	f_4f0 = 0;
	s_pic_4f7c98.Delete();
	f_4f4 = 0;
	f_4f8 = 2.0f;
	f_4fc = 0;
	f_500 = 0;
	f_504 = 0;
	f_4d8 = 3;
	f_4ec = -0x45;
	v305();

	g_hud_4f7c90 = NewElt();
	v276(g_hud_4f7c90, 260.0f, 1.0f, 0x67, 1.0f, true);
	g_hud_4f7c90->set_blend(omblendfc_Src_Alpha, omblendfc_One);
	Hide(g_hud_4f7c90);
	g_confirm_shown = 0;
	Log("Before IntSetScreen");								// (debug log)
	s_pic_4f8138.db = f_4b4;
	s_pic_4f8138.canvas_id = 0x6b;
	s_pic_4f8138.x = 0;
	s_pic_4f8138.y = -1;
	s_pic_4f8138.src_blend = 6;
	s_pic_4f8138.dst_blend = 1;
	Log("After IntSetScreen");

	f_4e4 = NewElt();
	f_4e4->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
	v276(f_4e4, 315.0f, 100.0f, 0xfd, 1.0f, true);
	Hide(f_4e4);

	// the yes/no boxes (one picture per question) and their two highlights
	struct Box { OMediaCanvasElement **elt; float x, y; int id; };
	const Box boxes[] = {
		{ &g_box_12, 210.0f, 210.0f, 0x18 },
		{ &g_box_4f7cfc, 285.0f, 256.0f, 0x1a },
		{ &g_box_level, 210.0f, 210.0f, 0xa9 },
		{ &g_box_quit, 210.0f, 210.0f, 0xf1 },
		{ &g_box_delete, 210.0f, 210.0f, 0x113 },
	};
	for (size_t i = 0; i < sizeof boxes / sizeof boxes[0]; i++)
	{
		*boxes[i].elt = NewElt();
		(*boxes[i].elt)->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
		v276(*boxes[i].elt, boxes[i].x, boxes[i].y, boxes[i].id, 1.0f, true);
		Hide(*boxes[i].elt);
	}
	s_loading_pic.db = f_4b4;
	s_loading_pic.canvas_id = 0xf4;
	s_loading_pic.x = 0x19a;
	s_loading_pic.y = 0x14;
	s_loading_pic.src_blend = 6;
	s_loading_pic.dst_blend = 7;
	const Box boxes2[] = {
		{ &g_box_save, 210.0f, 210.0f, 0xf5 },
		{ &g_box_load, 210.0f, 210.0f, 0x7b },
		{ &g_box_yes, 210.0f, 252.0f, 0xf2 },
		{ &g_box_no, 325.0f, 252.0f, 0xf3 },
	};
	for (size_t i = 0; i < sizeof boxes2 / sizeof boxes2[0]; i++)
	{
		*boxes2[i].elt = NewElt();
		(*boxes2[i].elt)->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
		v276(*boxes2[i].elt, boxes2[i].x, boxes2[i].y, boxes2[i].id, 1.0f, true);
		Hide(*boxes2[i].elt);
	}
	g_confirm_open = 0;
	f_4dc = 0;

	// the fade quad (full screen)
	g_fade_quad = NewElt();
	v276(g_fade_quad, 0.0f, -1.0f, 0xfc, 1.0f, true);
	g_fade_quad->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
	Hide(g_fade_quad);
	s_fade_pic.db = f_4b4;
	s_fade_pic.y = 0;
	s_fade_pic.canvas_id = 0xf7;
	s_fade_pic.x = 0;
	s_fade_pic.src_blend = 6;
	s_fade_pic.dst_blend = 7;
	s_pic_4f7cb8.db = f_4b4;
	s_pic_4f7cb8.canvas_id = 0x14f;
	s_pic_4f7cb8.x = 0;
	s_pic_4f7cb8.y = 200;
	s_pic_4f7cb8.src_blend = 6;
	s_pic_4f7cb8.dst_blend = 7;

	// the story message lines (44 pictures, v306)
	static const short kLines[44] = {
		0x9c, 0x78, 0x79, 0x7a, 0xa8, 0x9d, 0x9e, 0xa0, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xc4, 0xb8,
		0xb9, 0xda, 0xfa, 0xf9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc3, 0xb7, 0xde, 0xdd, 0xdc,
		0xcc, 0xc6, 0xc9, 0xca, 0xe0, 0xdf, 0xcb, 0xc5, 0xe6, 0xe2, 0xe3, 0xe4, 0xe5, 0xe1 };
	v306(0, kLines[0]);
	Hide(g_story_lines[0]);
	for (int i = 1; i < 44; i++) v306(i, kLines[i]);
	for (int i = 0; i < 44; i++) g_story_flags[i] = 0;

	v107(0);												// (BG's slot 108 setter: not saved in a .gam)
	v36(0);
	v52(0);
	f_4c4 = 0;
	f_4c0 = 0;
	g_menu_active = 0;
	f_4cc = 0;
	f_4e0 = 0;
	g_hud_on = 1;
	g_fade_time = 1.0f;
	f_508 = 0;
	f_514 = 0;
	f_518 = 0;
	f_594 = 0; f_598 = 0; f_59c = 0; f_5b0 = NULL; f_4ce = 0;	// port: the Negatron members
}

// --------------------------------------------------------------------------- 0x4029c0

C2DInGameMenu::~C2DInGameMenu()
{
	OMediaCanvasElement **elts[] = {
		&g_hud_lives, &g_health_bar, &g_health_frame, &g_gadget_bar_frame, &g_gadget_bar,
		&g_box_level, &g_box_quit, &g_box_save, &g_box_load, &g_box_delete, &g_box_yes, &g_box_no,
	};
	for (size_t i = 0; i < sizeof elts / sizeof elts[0]; i++)
		if (*elts[i]) delete *elts[i];						// (the pointers are not cleared)
	if (s_gadget_pics[0].elt) delete s_gadget_pics[0].elt;	// sic: 0x4f7ef0 only, the first gadget's picture
	for (int i = 0; i < 44; i++)
		if (g_story_lines[i]) delete g_story_lines[i];
	v279();
	// port: the pointers above are cleared so a second menu (a new C3DJimmy) does not see them
	for (size_t i = 0; i < sizeof elts / sizeof elts[0]; i++) *elts[i] = NULL;
	s_gadget_pics[0].elt = NULL;
	for (int i = 0; i < 44; i++) g_story_lines[i] = NULL;
}

// --------------------------------------------------------------------------- slot 8

void C2DInGameMenu::UnInitObject()										// 0x402b40
{
	CLocalGameObject::UnInitObject();
	if (f_4b4) UnInitDatabase(f_4b4);						// 0x476080
	if (f_4b8) UnInitFileStream(f_4b8);						// 0x476290
}

// --------------------------------------------------------------------------- sprites

void C2DInGameMenu::v275(OMediaCanvasElement *e, float x, float y)		// 0x402cc0
{
	const float sx = ScaleX(), sy = ScaleY();
	if (!e) return;
	e->set_canvas_flags(omcanef_FreeWorldSize);
	e->set_auto_align(omaac_Center, omaac_Center);
	e->setz(0);
	e->setx(sx * x - (float)((short)g_screen_w / 2));
	e->sety(((float)((short)g_screen_h / 2) - e->get_height()) - sy * y);
}

void C2DInGameMenu::v276(OMediaCanvasElement *e, float x, float y, int canvas_id, float unused, bool scaled)	// 0x402d70
{
	(void)unused;
	const float sx = ScaleX(), sy = ScaleY();
	if (!e) return;
	v277(e, canvas_id, scaled);
	e->set_canvas_flags(omcanef_FreeWorldSize);
	e->set_auto_align(omaac_Center, omaac_Center);
	e->setz(0);
	e->setx(sx * x - (float)((short)g_screen_w / 2));
	e->sety(((float)((short)g_screen_h / 2) - e->get_height()) - sy * y);
}

void C2DInGameMenu::v277(OMediaCanvasElement *e, int canvas_id, bool scaled)	// 0x402e30
{
	const float sx = ScaleX(), sy = ScaleY();
	if (canvas_id == -1)
	{
		// the original passes its `scaled` argument as the canvas (every caller with -1 passes 0)
		e->set_canvas(NULL);
		return;
	}
	OMediaCanvas *c = v278(canvas_id);
	if (!c)
	{
		e->set_canvas(NULL);
		return;
	}
	if (scaled) e->set_size((float)c->get_width() * sx, (float)c->get_height() * sy);
	else e->set_size((float)c->get_width(), (float)c->get_height());
	e->set_canvas(c);
}

OMediaCanvas *C2DInGameMenu::v278(int canvas_id)						// 0x402f00
{
	OMediaCanvas *c = LoadCanvasID(f_4b4, canvas_id);		// 0x477890
	if (c) c->db_unlock();
	return c;
}

// --------------------------------------------------------------------------- the current page

void C2DInGameMenu::v279()												// 0x402f30
{
	v296(Page(f_4d4));
	v310(f_4d4);
}

void C2DInGameMenu::v280()												// 0x4033f0
{
	v309(f_4d4);
	v295(Page(f_4d4));
}

unsigned short C2DInGameMenu::v281()									// 0x403860
{
	return 0;
}

void C2DInGameMenu::v282(float health)									// 0x403980 the health bar
{
	if (!g_health_bar) return;
	const float sy = ScaleY();
	g_health_bar->set_size(ScaleX() * 27.0f, sy * health * (float)1.044);	// 0x48d910, 0x48d918
}

void C2DInGameMenu::v284()												// 0x4039e0
{
	f_4be = 1;
}

void C2DInGameMenu::v285()												// 0x4039f0
{
	if (f_4be) f_4be = 0;
}

unsigned short C2DInGameMenu::v286(short item)							// 0x403bd0
{
	f_4c8 = item;
	return 1;
}

void C2DInGameMenu::v288()												// 0x403bf0
{
	f_4bc = 3;
	f_4c8 = 3;
}

// --------------------------------------------------------------------------- the power bar

void C2DInGameMenu::v289()												// 0x4036c0
{
	if (g_gadget_bar_frame) Show(g_gadget_bar_frame);
	if (g_gadget_bar) Show(g_gadget_bar);
}

void C2DInGameMenu::v290()												// 0x403700
{
	if (g_gadget_bar_frame) Hide(g_gadget_bar_frame);
	if (g_gadget_bar) Hide(g_gadget_bar);
}

void C2DInGameMenu::v291()												// 0x403730 red
{
	if (!g_gadget_bar) return;
	OMediaFARGBColor c;
	c.alpha = 1.0f; c.red = 0.2f; c.green = 0.2f; c.blue = 1.0f;	// +0x90..0x9c = 1, 0.2, 0.2, 1
	g_gadget_bar->set_diffuse(c);
	Show(g_gadget_bar);
}

void C2DInGameMenu::v292()												// 0x4037a0 white
{
	if (!g_gadget_bar) return;
	SetAlpha(g_gadget_bar, 1.0f);
}

// --------------------------------------------------------------------------- 0x404de0 the pages

// One page: allocated (items constructed), background `bg_id`, the background element made
// here only when one exists (never: it is NULL), placed at (bx, by).
static C2DInGameMenu::MenuPage *NewPage(C2DInGameMenu *m, short bg_id, float bx, float by)
{
	C2DInGameMenu::MenuPage *p = new C2DInGameMenu::MenuPage;	// 0x490 bytes
	p->background = NULL;
	p->background_id = bg_id;
	if (p->background)										// (dead: the pointer was just cleared)
	{
		p->background->link_layer(g_layer_hud2d);
		m->v276(p->background, bx, by, p->background_id, 1.0f, true);
	}
	return p;
}

void C2DInGameMenu::v293()
{
	// 0: gadgets (eight in a 2x4 grid, three more)
	MenuPage *p = NewPage(this, 0x14, 0.0f, -1.0f);
	Page(MENU_GADGETS) = p;
	v294(p, 0, 0x0b, 0x34, 0x0c, 0xb6);
	v294(p, 1, 0x0c, 0x2e, 0x9e, 0xb6);
	v294(p, 2, 0x0d, 0x2f, 0x0c, 0x147);
	v294(p, 3, 0x0e, 0x30, 0x9d, 0x147);
	v294(p, 4, 0x0f, 0x31, 0x156, 0xb6);
	v294(p, 5, 0x10, 0x32, 0x1e7, 0xb6);
	v294(p, 6, 0x11, 0x33, 0x156, 0x147);
	v294(p, 7, 0x12, 0x2d, 0x1e7, 0x147);
	v294(p, 8, 0x54, 0x55, 0x1e7, 0x14);
	v294(p, 9, 0xb4, 0xb3, 0x1e7, 0x5f);
	v294(p, 10, 0xb6, 0xb5, 0x0c, 0x5f);

	// 1: the phone booth map (nine places, four more)
	p = NewPage(this, 0x1a, 0.0f, -1.0f);
	Page(MENU_PHONE) = p;
	v294(p, 0, 0x2c, 0x1b, 0x45, 0x14);
	v294(p, 1, 0x1c, 0x1d, 0xcb, 0x14);
	v294(p, 2, 0x1e, 0x1f, 0x152, 0x14);
	v294(p, 3, 0x20, 0x21, 0x45, 0x7f);
	v294(p, 4, 0x22, 0x23, 0xcb, 0x7f);
	v294(p, 5, 0x24, 0x25, 0x152, 0x7f);
	v294(p, 6, 0x26, 0x27, 0x45, 0xea);
	v294(p, 7, 0x28, 0x29, 0xcb, 0xea);
	v294(p, 8, 0x2a, 0x2b, 0x152, 0xea);
	v294(p, 9, 0x150, -1, 0x1d6, 0x22);
	v294(p, 10, 0x151, -1, 0x45, 0x154);
	v294(p, 11, 0x153, -1, 0xcb, 0x154);
	v294(p, 12, 0x152, -1, 0x152, 0x154);
	MenuSetItem(1, 9, 1);
	MenuSetItem(1, 10, 1);
	MenuSetItem(1, 11, 1);
	MenuSetItem(1, 12, 1);

	// 6: credits
	p = NewPage(this, 0xf6, 0.0f, 0.0f);
	Page(MENU_CREDITS) = p;
	v294(p, 1, 0x112, -1, 0, 0x1b0);
	MenuSetItem(6, 1, 1);

	// 2: pickups (29 icons in rows of six)
	p = NewPage(this, 0x35, 0.0f, 0.0f);
	Page(MENU_PICKUPS) = p;
	static const short kPickups[29] = {
		0xf0, 0x53, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42,
		0x50, 0x43, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x51 };
	static const short kCols[6] = { 0x37, 0x91, 0xeb, 0x145, 0x19f, 0x1f9 };
	static const short kRows[5] = { 0x27, 0x81, 0xdb, 0x135, 399 };
	for (int i = 0; i < 29; i++)
		v294(p, (short)i, -1, kPickups[i], kCols[i % 6], kRows[i / 6]);

	// 3: the PDA main page
	p = NewPage(this, 0xd9, 200.0f, 200.0f);
	Page(MENU_MAIN) = p;
	v294(p, 1, 0xce, -1, 0, 0x79);
	v294(p, 2, 0xd5, -1, 0xd5, 0x7d);
	v294(p, 3, 0xd7, -1, 0x1f0, 0x8a);
	v294(p, 4, 0xd8, -1, 0x8a, 0xef);
	v294(p, 5, 0xd6, -1, 199, 2);
	v294(p, 6, 0xcd, -1, 0x24, 0x0e);
	v294(p, 7, 0xd1, -1, 0x178, 0x13a);
	v294(p, 8, 0xd2, -1, 0x178, 0x13a);
	v294(p, 9, 0xd3, -1, 0x178, 0x13a);
	v294(p, 10, 0xd4, -1, 0x178, 0x13a);
	v294(p, 11, 0xd0, -1, 0x178, 0x13a);
	v294(p, 12, 0xe7, -1, 0, 0x154);
	v294(p, 13, 0xe8, -1, 0x1d8, 0x13c);
	v294(p, 14, 0xed, -1, 0x178, 0x13a);
	v294(p, 15, 0xee, -1, 0x178, 0x13a);
	v294(p, 16, 0xf8, -1, 0x178, 0x13a);
	for (int i = 1; i <= 13; i++) MenuSetItem(3, (short)i, 1);

	// 4: load / save (five buttons, ten slots)
	p = NewPage(this, 0xef, 0.0f, -1.0f);
	Page(MENU_SAVE) = p;
	v294(p, 1, 0x85, -1, 0, 0x198);
	v294(p, 2, 0x86, -1, 0x215, 0x198);
	v294(p, 3, 0x89, -1, 0x71, 0x198);
	v294(p, 4, 0x8e, -1, 0xfd, 0x198);
	v294(p, 5, 0x82, -1, 0x189, 0x198);
	static const short kSlots[10] = { 0x81, 0x87, 0x88, 0x84, 0x8b, 0x8c, 0x8d, 0x90, 0x91, 0x80 };
	static const short kSlotY[10] = { 0x5b, 0x79, 0x97, 0xb5, 0xd3, 0xf1, 0x10f, 0x12e, 0x14c, 0x16a };
	for (int i = 0; i < 10; i++)
		v294(p, (short)(6 + i), kSlots[i], -1, 0xae, kSlotY[i]);
	for (int i = 6; i <= 15; i++) MenuSetItem(4, (short)i, 1);
	for (int i = 1; i <= 5; i++) MenuSetItem(4, (short)i, 1);

	// 5: options (the two volumes)
	p = NewPage(this, 0xec, 0.0f, -1.0f);
	Page(MENU_OPTIONS) = p;
	v294(p, 1, 0xea, -1, 0x90, 0x5f);
	v294(p, 2, 0xe9, -1, 0x90, 0xff);
	v294(p, 3, 0xeb, -1, 0, 0x1a3);
	v294(p, 4, 0x10e, -1, 0x1a3, 0x5f);
	v294(p, 5, 0x10f, -1, 0x1a3, 0xff);
	for (int i = 1; i <= 5; i++) MenuSetItem(5, (short)i, 1);

	v309(2);
	v310(2);
	v296(Page(0));
	v296(Page(1));
	v296(Page(2));
	v296(Page(3));
	v296(Page(4));
	v296(Page(5));
	v296(Page(6));
}

// 0x406240: item `index` of `page`: its two pictures (made later by LoadMyMenu) and position.
void C2DInGameMenu::v294(MenuPage *page, short index, short rollover_id, short active_id, short x, short y)
{
	if (index < 0 || index >= MENU_ITEMS) return;
	MenuItem &it = page->items[index];
	it.active = NULL;
	it.active_id = active_id;
	it.type = 0;
	if (it.active)											// (dead: just cleared)
	{
		it.active->link_layer(g_layer_hud2d);
		if (active_id != -1) v276(it.active, (float)x, (float)y, active_id, 1.0f, true);
	}
	if (!g_windowed)
	{
		it.x = (float)x;
		it.y = (float)y;
	}
	else
	{
		it.x = (float)x + 1.0f;								// 0x48d924
		it.y = (float)y + 2.0f;								// 0x48d954
	}
	it.rollover = NULL;
	it.rollover_id = rollover_id;
	if (it.rollover)										// (dead)
	{
		it.rollover->link_layer(g_layer_hud2d);
		v276(it.rollover, (float)x, (float)y, rollover_id, 1.0f, true);
	}
}

// 0x4060d0 "displayMenu"
void C2DInGameMenu::v295(MenuPage *page)
{
	Log("starting  displayMenu");
	if (page->background) Show(page->background);
	Log("In displayMenu");
	for (int i = 0; i < MENU_ITEMS; i++)
	{
		if (page == Page(MENU_PICKUPS) && g_pickup_counts[i * 2] != 0)
		{
			const MenuItem &it = page->items[i];
			DrawText((short)(int)it.x, (short)(int)it.y, 'MED', "%d", g_pickup_counts[i * 2]);	// 0x468660
		}
		const MenuItem &it = page->items[i];
		if (it.type != 0 && it.type != 4)
		{
			if (it.rollover) Hide(it.rollover);
			if (it.active) Show(it.active);
		}
	}
	Log("Exiting  displayMenu");
}

void C2DInGameMenu::v296(MenuPage *page)								// 0x406080
{
	if (page->background) Hide(page->background);
	for (int i = 0; i < MENU_ITEMS; i++)
	{
		if (page->items[i].rollover) Hide(page->items[i].rollover);
		if (page->items[i].active) Hide(page->items[i].active);
	}
}

// 0x403a30: the rollovers of the current page for the pointer at (mx, my); the item under it.
short C2DInGameMenu::v297(short mx, short my)
{
	short hit = -1;
	for (short i = 0; i < MENU_ITEMS; i++)
	{
		MenuItem &it = Page(f_4d4)->items[i];
		const short t = it.type;
		OMediaCanvasElement *r = it.rollover;
		if (t == 0 || t == 4 || !r) continue;
		const short ix = (short)(int)it.x;					// _ftol
		const short iy = (short)(int)it.y;
		bool inside = false;
		if (ix < mx && !(r->get_width() + (float)ix <= (float)mx) &&
			iy < my && (float)my < (float)iy + r->get_height())
			inside = true;
		if (inside)
		{
			if (t == 3) Hide(r);
			else Show(r);
			hit = i;
			continue;
		}
		if (t == 2) Show(r);
		else Hide(r);
	}
	return hit;
}

short C2DInGameMenu::v298()												// 0x403a10
{
	return f_4d4;
}

void C2DInGameMenu::v299(short page)									// 0x403a20
{
	f_4d4 = page;
}

// 0x405fb0: the current gadget becomes `gadget`: the old one's picture freed and its icon
// hidden, the new icon shown when its item on page 0 is available.
void C2DInGameMenu::v300(int gadget)
{
	s_gadget_pics[f_4d8].Delete();
	Hide(g_gadget_icons[f_4d8]);
	if (Page(0)->items[gadget].type == 1)
	{
		if (gadget < 0)
		{
			Show(g_gadget_icons[3]);						// (0x4f7ce0; unreachable with an index < 0 above)
			f_4d8 = gadget;
			return;
		}
		Show(g_gadget_icons[gadget]);
	}
	f_4d8 = gadget;
}

void C2DInGameMenu::v301(bool show)										// 0x406560
{
	if (!show) Hide(g_hud_4f7c90);
	else Show(g_hud_4f7c90);
}

// 0x4063e0 "ShowHudPicture": picture `canvas_id` at (x, y), or none.
void C2DInGameMenu::v302(int canvas_id, bool show, int x, int y)
{
	if (show)
	{
		if (g_hud_picture) FreeElt(g_hud_picture, false);
		g_hud_picture = NewElt();
		v276(g_hud_picture, (float)x, (float)y, canvas_id, 1.0f, true);
		Show(g_hud_picture);
		g_confirm_shown = 1;
		return;
	}
	FreeElt(g_hud_picture, true);
	g_confirm_shown = 0;
}

void C2DInGameMenu::v303(bool show)										// 0x403420
{
	if (!show) s_pic_4f8138.Delete();
	else if (!g_windowed) s_pic_4f8138.Show();
}

void C2DInGameMenu::v304()												// 0x406650
{
	if (f_4ec != 0) return;
	s_gadget_pics[f_4d8].Show();
	s_msg_arrow.Show();
	f_4ec = -0x45;
}

void C2DInGameMenu::v305()												// 0x406590
{
	if (f_4ec != -0x45) return;
	s_gadget_pics[f_4d8].Delete();
	s_msg_arrow.Delete();
	f_4ec = 0;
}

void C2DInGameMenu::v306(int index, int canvas_id)						// 0x402b70
{
	OMediaCanvasElement *e = new OMediaCanvasElement;
	g_story_lines[index] = e;
	e->link_layer(g_layer_hud2d);
	e->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
	v276(e, 0.0f, 0.0f, canvas_id, 1.0f, true);
	Hide(e);
}

void C2DInGameMenu::v307(int index, int line, bool show)				// 0x402c40
{
	OMediaCanvasElement *e = g_story_lines[index];
	if (!show) Hide(e);
	else Show(e);
	v275(e, 384.0f, (float)(f_4ec + 0x1c0 + line * 15));
}

// --------------------------------------------------------------------------- 0x402f60 LoadMyMenu

void C2DInGameMenu::v309(int page)
{
	MenuPage *p = Page(page);
	Log("LoadMyMenu: CurrMenu = %d", page);
	if (!p->background)
	{
		p->background = new OMediaCanvasElement;
		if (p->background) p->background->link_layer(g_layer_hud2d);
		if (g_windowed) v276(p->background, 0.0f, 0.0f, p->background_id, 1.0f, true);
		else v276(p->background, 0.0f, -1.0f, p->background_id, 1.0f, true);
	}
	for (int i = 0; i < MENU_ITEMS; i++)
	{
		MenuItem &it = p->items[i];
		Log("Item #%d", i);
		if (!it.active && it.active_id != -1)
		{
			Log("Item #%d is ActiveItem IsActive:%d", i, (int)it.type);
			it.active = new OMediaCanvasElement;
			if (it.active)
			{
				it.active->link_layer(g_layer_hud2d);
				v276(it.active, (float)(int)it.x, (float)(int)it.y, it.active_id, 1.0f, true);
				it.w = (short)(int)it.active->get_width();
				it.h = (short)(int)it.active->get_height();
				if (it.type == 0 || it.type == 4) Hide(it.active);
				else if (it.type == 1) Show(it.active);
			}
		}
		if (!it.rollover && it.rollover_id != -1)
		{
			Log("Item #%d is Rollover IsActive:%d", i, (int)it.type);
			it.rollover = new OMediaCanvasElement;
			if (it.rollover)
			{
				it.rollover->link_layer(g_layer_hud2d);
				v276(it.rollover, (float)(int)it.x, (float)(int)it.y, it.rollover_id, 1.0f, true);
				it.w = (short)(int)it.rollover->get_width();
				it.h = (short)(int)it.rollover->get_height();
				if (it.type == 0 || it.type == 4) Hide(it.rollover);
				else if (it.type == 1) Show(it.rollover);
			}
		}
	}
}

// --------------------------------------------------------------------------- 0x4032c0 UnloadMyMenu

void C2DInGameMenu::v310(int page)
{
	Log("Enter UnloadMyMenu:%d", page);
	MenuPage *p = Page(page);
	if (page == MENU_PICKUPS) return;						// the pickups page keeps its pictures
	FreeElt(p->background, false);
	Log("Not a pickups_menu, deallocating all");
	for (int i = 0; i < MENU_ITEMS; i++)
	{
		MenuItem &it = p->items[i];
		if (it.active)
		{
			Log("Deallocaing ActiveItem %d", i);
			FreeElt(it.active, false);
		}
		if (it.rollover)
		{
			Log("Deallocaing Rollover %d", i);
			// its canvas only when it is not the active picture's too
			FreeElt(it.rollover, false, it.active_id != it.rollover_id);
		}
	}
	Log("Leave UnloadMyMenu");
}

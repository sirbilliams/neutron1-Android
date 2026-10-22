/*  C3DDino - see C3DDino.h. Reconstructed from Neutron.exe N2:0x42b7a0..0x4174f0. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DDino.h"
// TODO(BG): #include "../C3DAIGranny.h" (Negatron-only class, deleted)
// TODO(BG): #include "../C3DASEObj.h" (Negatron-only class, deleted)
#include "../C3DJimmy.h"
// TODO(BG): #include "../C3DRadar.h" (Negatron-only class, deleted)
#include "../C3DTriggerType.h"
#include "../objects/C3DSoundEffect.h"
#include "../CanvasWrapper.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../Sound.h"
#include "../gen/globals.h"
#include "OMediaStreamOperators.h"
#include "OMediaViewPort.h"
#pragma GCC diagnostic pop
#include <list>
#include <string.h>
#include <strings.h>
#include <string>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// 0x402cc0 (C2DInGameMenu::PositionSprite, `this` unused): place a sprite at (x, y) in 640x480
// space, centred, its top at y (as in C3DHerman.cpp).
static void PositionSprite(OMediaCanvasElementWrapper *e, float x, float y)
{
	const float sx = (float)(short)g_screen_w * 0.0015625f;	// 1/640
	const float sy = (float)(short)g_screen_h * 0.0020833334f;	// 1/480
	if (!e) return;
	e->set_canvas_flags(omcanef_FreeWorldSize);
	e->set_auto_align(omaac_Center, omaac_Center);
	const float px = sx * x - (float)((short)g_screen_w / 2);
	const float py = ((float)((short)g_screen_h / 2) - e->get_height()) - sy * y;
	e->place(px, py, 0);
}

// The game's random number: 0..1000 (the LCG at g_random_seed).
static int Random1000(void)
{
	unsigned int *seed = g_random_seed;
	*seed = *seed * 0x19660d + 0x7fff;
	return (int)(((*seed >> 16) * 1001) >> 16) & 0xffff;
}

// The shock effect and the shadow follow us: same position, same angles.
static void FollowUs(CGameObject *o, CGameObject *us)
{
	float p[4], a[4];
	us->v195(p);											// +0x30c
	o->v196(p[0], p[1], p[2], p[3]);						// +0x310
	us->v201(a);											// +0x324
	o->v202(a[0], a[1], a[2], a[3]);						// +0x328
}

// --------------------------------------------------------------------------- construction

C3DDino::C3DDino()											// N2:0x42b7a0
{
	// (f_1b50, f_1c54: InterfaceElt constructors N2:0x480a10)
	SetName((char *)"C3DDINO");
	Log("C3DDino");
	f_6d0 = 1;												// Granny
	f_649 = 0;												// ev69 loads the animations later
	f_1d74 = 0;
	Log("InitObject()");									// 0x4173c0 inlined
	C3DAI::InitObject();									// 0x407ee0
	RegisterProperty("EnemyType", &f_1d68, PROP_LONG, 0);	// +0x3fc
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// 0x407fa0
	C3DAI::ev72(m_anim_run, (char *)"WALK");
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	C3DAI::ev72(m_anim_walk, (char *)"WALK");
	f_1b50.db = g_screens_db;								// N2:0x545e50
	f_1b50.canvas_id = 411;
	f_1b50.x = 0;
	f_1b50.y = 0;
	f_1b50.src_blend = omblendfc_Src_Alpha;
	f_1b50.dest_blend = omblendfc_Inv_Src_Alpha;
	f_1b50.home_x = 0;
	f_1b50.home_y = 0;
	f_1b50.f_34 = 1;
	f_1c54.src_blend = omblendfc_Src_Alpha;
	f_1c54.db = g_screens_db;
	f_1c54.canvas_id = 462;
	f_1c54.x = 0;
	f_1c54.y = 0;
	f_1c54.dest_blend = omblendfc_Inv_Src_Alpha;
	f_1c54.home_x = 0;
	f_1c54.home_y = 0;
	f_1c54.f_34 = 1;
	C3DObject::InitPhysics();								// 0x4623d0
	m_keep_fleeing = 0.5f;
	C3DObject::SetObjectID('3DIN');							// 0x462220
	m_hover = 0;
	m_speed = 600.0f;											// speed
	f_1d58 = 1.0f;											// health
	f_1d5c = 50.0f;
	f_1d60 = 0;
	CGameObject::v42(1);									// 0x470fd0 (direct calls)
	CGameObject::v94(2);
	f_1d64 = 0;
	f_1d68 = 0;
	m_check_stuck = 0;
	f_1d6c = -1;
	f_1d7c = 0;
	m_show_on_radar = 1;									// CLocalGameObject +0x4c0
	// (then a call to the empty 0x472970)
}

C3DDino::~C3DDino()											// N2:0x42ba50
{
	if (f_1d6c != -1)
	{
		StopGameSound(f_1d6c, false);						// 0x458a00
		f_1d6c = -1;
	}
	f_1b50.Hide();											// N2:0x4826f0
	f_1c54.Hide();
	// (then the empty InterfaceElt destructors and ~C3DAI)
}

namespace neg {
CGameObject *New_C3DDino(void) { return new C3DDino; }		// '3DIN' (0x1d88 bytes)
}

void C3DDino::InitObject(void)								// 0x4173c0
{
	Log("InitObject()");
	C3DAI::InitObject();									// 0x407ee0
	RegisterProperty("EnemyType", &f_1d68, PROP_LONG, 0);
}

// --------------------------------------------------------------------------- the model

void C3DDino::ev69(void)									// N2:0x42bb30
{
	if (f_649) return;
	f_649 = 1;
	const bool rex = f_1d68 == 0 || f_1d68 == 3;
	if (rex)
	{
		InitModel(std::string("trexbase.grn"));				// +0x144
		CreateAnim((char *)"WALK", (char *)"trexwalk.grn");	// +0xe0
		CreateAnim((char *)"TALK", (char *)"trextalk.grn");
		CreateAnim((char *)"FALL", (char *)"trexfall.grn");
		CreateAnim((char *)"STOP", (char *)"trexstop.grn");
		CreateAnim((char *)"SIZE", (char *)"trexsize.grn");
		CreateAnim((char *)"PLAY", (char *)"trexplay.grn");
		CreateAnim((char *)"PICKUP", (char *)"trexpickup.grn");
	}
	else
	{
		InitModel(std::string("raptorbase.grn"));
		CreateAnim((char *)"WALK", (char *)"raptorrun.grn");
		CreateAnim((char *)"TALK", (char *)"raptorattack.grn");
		CreateAnim((char *)"FALL", (char *)"raptordeath.grn");
		CreateAnim((char *)"STOP", (char *)"raptorstop.grn");
		CreateAnim((char *)"SIZE", (char *)"raptorsize.grn");
	}
	// TODO(BG): the shock effect (N2 C3DASEObj) and shadow (N2 C3DAIGranny) are Negatron classes:
	// none for now (f_1d70 / f_1d74 stay NULL, as when windowed).
	f_1d78 = 0;
	ev58((char *)"SIZE", 0);								// +0xe8
	set_shape(get_shape());									// element +0xb0, +0xac
	v67(100.0f);											// radius
	v64(0.01f);
	g_N2_545f48 = 0;											// none fallen yet
}

void C3DDino::ev58(char *name, unsigned char loop)			// N2:0x42d140
{
	if (strcasecmp(name, "WALK") == 0)						// 0x487c20
	{
		if (f_1d74) f_1d74->ev58((char *)"WALK", 1);
		C3DAnimated::ev58((char *)"WALK", 1);				// (always looped)
		return;
	}
	if (strcasecmp(name, "STOP") == 0)
	{
		if (f_1d74) f_1d74->ev58((char *)"STOP", 1);
		C3DAnimated::ev58((char *)"STOP", 1);
		return;
	}
	if (strcasecmp(name, "TALK") == 0)
	{
		if (f_1d74) f_1d74->ev58((char *)"STOP", 1);
		C3DAnimated::ev58((char *)"TALK", 1);
		return;
	}
	C3DAnimated::ev58(name, loop);
	if (f_1d74) f_1d74->ev58((char *)"STOP", 1);
}

void C3DDino::v260(void)									// N2:0x42cf30
{
	C3DAI::v260();											// 0x409480
	InitAnim();												// element +0x110
	ev70(1);												// +0x118
	set_shape(get_shape());									// +0xb0, +0xac
	if (f_1d6c == -1)
	{
		const char *walk = NULL;
		if (f_1d68 == 0)
		{
			const float r = (float)Random1000() * 0.001f;	// 0x48e5d8
			if (r < 0.33f) walk = "dinowalk";				// N2:0x4d7f98
			else if (r < 0.66f) walk = "dinowalk2";			// 0x4a3118
			else walk = "dinowalk3";
		}
		else if (f_1d68 == 1)
			walk = "raptorwalk";
		if (walk) f_1d6c = PlayGameSound3DNoDoppler(this, -1, std::string(walk), true);	// 0x458a40
	}
	// Count the dinosaurs and pterosaurs of the level.
	g_N2_545f4c = 0;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && ((*i)->m_object_id == '3DIN' || (*i)->m_object_id == '3PTE')) g_N2_545f4c++;
	if (f_1d70) f_1d70->HideAgain();						// element +0xdc
	if (f_1d74)
	{
		f_1d74->n2_ev92(2, 0);									// element +0x170 blend
		f_1d74->n2_ev91(1);									// +0x16c second pass
	}
}

unsigned char C3DDino::v231(CGameObject *other)				// N2:0x42d250
{
	return g_camera == other || other->m_object_id == '3DIN' || other->m_object_id == '3WBL';
}

// --------------------------------------------------------------------------- hits

void C3DDino::AddTouching(CGameObject *other)				// N2:0x42c4e0 (does not call the base's)
{
	if (!other->NameContains((char *)"C3DWATERBALLOON")) return;	// +0x18
	if (!(f_1d58 > 0))
	{
		m_hover = 1;
		v136(0);											// +0x220
		return;
	}
	const int d = g_difficulty;								// N2:0x53b5b0
	if (f_1d68 == 0 || f_1d68 == 3)
	{
		if (d == 0) f_1d58 -= 0.15f;						// N2:0x4d7f7c
		else if (d == 1) f_1d58 -= 0.1f;					// 0x48d920
		else if (d == 2) f_1d58 -= 0.05f;					// 0x495334
	}
	else
	{
		if (d == 0) f_1d58 -= 0.26f;						// N2:0x4d7f80
		else if (d == 1) f_1d58 -= 0.16f;					// N2:0x4d7f84
		else if (d == 2) f_1d58 -= 0.1f;
	}
	f_1d78 = 0.3f;											// shock flash
	C3DSprite *ball = dynamic_cast<C3DSprite *>(other);		// (other - 0xc8)
	if (ball) ball->hide(true);								// element +0x58 (port: checked)
	PlayGameSound(-1, std::string("SPLASH"), false);		// 0x40acc0, 0x458b40
	PlayGameSound(-1, std::string("balloon pop"), false);
	other->v132(0);											// +0x210
	other->v136(0);											// +0x220
	PlayGameSound(-1, std::string("vrzap"), false);
	f_1d60 = 8.0f;											// show the bar
	if (f_1d58 > 0) return;
	// Out of health.
	f_1b50.Hide();											// N2:0x4826f0
	f_1c54.Hide();
	AddScore(100);											// g_gametype N2:0x482b50
	ev73(1);												// element +0x1ac: wait
	if (strcasecmp(m_tag, "SPORKEATER") == 0)
	{
		CGameObject *o = FindObjectByTag("C3DSPORKY");
		C3DAI *ai = dynamic_cast<C3DAI *>(o);				// (o - 0xc0)
		if (ai) ai->ev73(1);
	}
	if (strcasecmp(m_tag, "DINO6") == 0)
	{
		CGameObject *o = FindObjectByTag("C3DCARL");
		C3DAI *ai = dynamic_cast<C3DAI *>(o);
		if (ai) ai->ev73(1);
	}
	ev58((char *)"FALL", 0);
	f_1d64 = 8.0f;
	if (f_1d68 == 3)
	{
		CGameObject *o = FindObjectByTag("FOWLYELLS");
		C3DSoundEffect *fx = dynamic_cast<C3DSoundEffect *>(o);	// (its +0x5b0 / +0x5ac)
		if (fx && fx->m_channel != -1)
		{
			StopGameSound(fx->m_channel, false);			// 0x458a00
			fx->m_is_ambient = 0;
		}
		o = FindObjectByTag("DINOCAR");
		C3DAnimated *car = dynamic_cast<C3DAnimated *>(o);	// (o - 0xc0)
		if (car) car->ev58((char *)"ANIM1", 0);
	}
	if (f_1d6c == -1) return;
	StopGameSound(f_1d6c, false);
	f_1d6c = -1;
	if (f_1d68 != 0 && f_1d68 != 3)
		PlayGameSound(-1, std::string("dinodown"), false);
	else
		PlayGameSound(-1, std::string("dinodown2"), false);
}

// --------------------------------------------------------------------------- every frame

void C3DDino::Process(float dt)								// N2:0x42c850
{
	C3DAI::Process(dt);										// N2:0x411240
	if (f_1d78 > 0)
	{
		f_1d78 = f_1d78 - dt;
		if (f_1d78 <= 0)
		{
			f_1d78 = 0;
			if (f_1d70) f_1d70->HideAgain();				// element +0xdc
		}
	}
	if (f_1d64 > 0)
	{
		const float t = f_1d64 - dt;
		f_1d64 = t;
		if (t < 6.0f && t > 1.0f)							// N2:0x4d7f90, 0x48d924
		{
			const float pct = (t - 1.0f) * 0.2f;			// N2:0x4d7f8c (debug "Percentage=%f")
			if (pct < 0) HideAgain();						// (never: t > 1)
		}
		else if (t < 1.0f)
		{
			if (f_1d70) f_1d70->HideAgain();
			f_1d64 = 0;
			HideAgain();									// element +0xdc: gone
			v136(0);										// +0x220
			v132(0);										// +0x210
			if (f_1d74) f_1d74->HideAgain();
			g_N2_545f48++;
			if (g_N2_545f48 >= 9)
			{
				CGameObject *t2 = FindObjectByTag("KILLDINOS");
				if (t2 && t2->NameContains((char *)"C3DTRIGGERTYPE"))
				{
					C3DJimmy *j = Jimmy();					// g_jimmy - 0xc0 (not checked)
					(void)j;	// TODO(BG): N2 hid C3DJimmy +0xd40 (C3DRadar) here
					C3DTriggerType *trig = dynamic_cast<C3DTriggerType *>(t2);	// (t2 - 0xc8)
					if (trig) trig->CallNextTrigger("KILLDINOS");	// element +0xdc
				}
			}
		}
		else if (f_1d74)									// t >= 6: the shadow follows the fall
		{
			FollowUs(f_1d74, this);
			if (!f_1d7c)
			{
				f_1d7c = 1;
				f_1d74->n2_ev92(2, 0);							// element +0x170
				f_1d74->n2_ev91(1);							// +0x16c
			}
		}
	}
	if (!(f_1d58 > 0)) return;
	if (f_1d74)
	{
		FollowUs(f_1d74, this);
		if (!f_1d7c)
		{
			f_1d7c = 1;
			f_1d74->n2_ev92(2, 0);
			f_1d74->n2_ev91(1);
		}
	}
	if (f_1d60 > 0)
	{
		f_1d60 = f_1d60 - dt;
		OMedia3DVector head = v224(0, 275.0f, 200.0f);		// +0x380
		float sx, sy;
		if (v281(g_viewport, head.xyzw(), &sx, &sy))		// +0x464: on screen
		{
			if (f_1d78 > 0 && f_1d70)
			{
				f_1d70->ShowAgain();						// element +0xd8
				f_1d70->n2_ev72(0.1f);							// +0x120 additive
			}
			if (f_1d70) FollowUs(f_1d70, this);
			f_1b50.Show();									// N2:0x482520
			PositionSprite(f_1b50.canvas_elt, (sx + 320.0f) - 26.0f, (240.0f - sy) - 12.0f);	// 0x402cc0
			f_1c54.Show();
			PositionSprite(f_1c54.canvas_elt, (sx + 320.0f) - 24.0f, (242.0f - sy) - 12.0f);
			f_1c54.Scale(f_1d5c * f_1d58, 11.0f);			// N2:0x482390
			OMediaCanvasElementWrapper *bar = f_1c54.canvas_elt;
			if (f_1d58 > 0.34f)								// N2:0x4d7f88
				bar->set_diffuse(OMediaFARGBColor(1.0f, 0, 1.0f, 0));	// 2002 +0x90..: green
			else
				bar->set_diffuse(OMediaFARGBColor(1.0f, 1.0f, 0, 0));	// red
			return;
		}
		if (f_1d70) f_1d70->HideAgain();
	}
	else if (f_1d70)
		f_1d70->HideAgain();
	f_1b50.Hide();											// N2:0x4826f0
	f_1c54.Hide();
}

// --------------------------------------------------------------------------- saving

void C3DDino::WriteState(OMediaStreamOperators &s)			// N2:0x42d590
{
	C3DAI::WriteState(s);									// N2:0x415760
	s << f_1d58;
	s << f_1d64;
	s << GetAnim();											// element +0xf4
	s << g_N2_545f48;										// (MSVC long: 32 bits)
}

void C3DDino::ReadState(OMediaStreamOperators &s)			// N2:0x42d430
{
	C3DAI::ReadState(s);									// N2:0x4155e0
	// (a debug print "TEST" when out of health)
	s >> f_1d58;
	s >> f_1d64;
	char anim[64];
	s >> anim;
	s >> g_N2_545f48;
	if (f_1d58 > 0)
	{
		ShowAgain();										// element +0xd8
		v136(1);
		v132(1);
		if (f_1d74) f_1d74->ShowAgain();
	}
	else
	{
		HideAgain();										// element +0xdc
		v136(0);
		v132(0);
		f_1b50.Hide();
		f_1c54.Hide();
		if (f_1d70) f_1d70->HideAgain();
		if (f_1d74) f_1d74->HideAgain();
	}
	if (anim[0]) ev58(anim, 1);								// +0xe8
}

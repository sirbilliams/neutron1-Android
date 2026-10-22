/*  C3DJimmy part B: Jimmy's own element slots (Neutron.exe 93..130 but the PDA menu's): the
    gadgets, water, shocks and hits, the shadow, the trail, the shrink ray, the score pop-up.
    See C3DJimmy.h.                                                                              */
#include "JimmyBG.h"
#include "../C3DAI.h"

using namespace neg;
using namespace jimmybg;

// The PDA menu's slots (agent F's, jimmy/C3DJimmy_menu.cpp), called from here.
static void MenuOpen(C3DJimmy *j)				{ j->ToggleMainMenu(); }			// element +0x1f0 (slot 124)
static void MenuActivate(C3DJimmy *j, int p)	{ j->ActivateActiveMenu(p); }		// element +0x1f8 (slot 126)
static void MenuDeactivate(C3DJimmy *j)		{ j->DeactivateActiveMenu(); }		// element +0x1fc (slot 127)

// The hooks the menu methods call (jimmy/JimmyMenu.h; weak defaults in C3DJimmy_menu.cpp).
namespace neg {
void JimmyAnimEnded(C3DJimmy *j)				{ j->ev67(); }					// slot 65
void JimmyStopLoopSound(C3DJimmy *j)			{ j->StepSoundOff(); }			// slot 82
void JimmySelectGadget(C3DJimmy *j, short g)	{ j->ActivateGadget(g); }		// slot 93
void JimmyLeaveMenu(C3DJimmy *j)				{ j->DeactivateGadget(); }		// slot 94
short JimmyMove7e8(C3DJimmy *j)				{ return j->f_7e8; }
void JimmyHideA10(C3DJimmy *j)					{ if (j->f_a10) Hide(j->f_a10); }
}

// The bubble (C3DBubble, Jimmy's f_a14: its Start / Pop and the flags at +0x614..0x616).
static void BubblePop(C3DBubble *b)			{ b->Pop(); }				// 0x410d00
static void BubbleStart(C3DBubble *b)			{ b->Start(); }				// 0x410ca0
static bool BubbleBusy(C3DBubble *b)			{ return b->Busy(); }		// +0x614 floating / +0x616 fading out / +0x615 fading in
static void BubbleOff(C3DBubble *b)			{ b->FadeInOff(); }			// 0x428060: +0x615 = 0

// An enemy the shrink ray can shrink (CGameObject +0x898 of the enemy: a C3DAI flag of
// Neutron.exe that our Negatron-shaped C3DAI does not have).
static bool Shrinkable(CGameObject *o)		{ (void)o; return true; }	// TODO(BG): C3DAI CGameObject +0x898

static void LoadVR(C3DJimmy *j, const char *game)
{
	// the PDA's VR page was open: that gadget's VR level, from the phone booth
	SetItemType(0, 9, 1);									// 0x4038c0
	MenuHudPicture(j->f_a18, 0x9b, true, 0, -1);			// +0x4b8
	// (debug log "MENTEMP Loading VR0n %s  %s")
	j->PlayerLoadLevel((char *)game, (char *)"PHONEBOOTH", j->f_955, j->f_724);	// element +0x168
}

// --------------------------------------------------------------------------- the gadgets

void C3DJimmy::ActivateGadget(short g)						// 0x428d50
{
	// (debug log "CAll in AMI %d")
	if (f_1e59) return;
	if (f_a18 && g < 9) MenuSelectGadget(f_a18, g);			// +0x4b0
	if (g_camera && f_a18 && !g_4f8181 && g != 3 && g != -1) f_1e34 = 10.0f;
	switch (g)
	{
		case 0:												// the bubble
			if (f_a14)
			{
				if (ItemType(0, 9) == 2) { LoadVR(this, "vr01.gam"); break; }
				if (g_4f83d4 <= 0 && !f_9f8) { g_4f0588 = -1; return; }
				if (BubbleBusy(f_a14)) return;
				v67(160.0f);								// +0x110
				v64(0.415f);								// +0x100
				g_4f0588 = 0;
				f_7c4 = 2;
				f_6c4 = 1350.0f;
				BubbleStart(f_a14);							// 0x410ca0
				if (f_a40 == -1) f_a40 = PlayGameSoundIndex(-1, 2, true);
				StepSoundOff();								// element +0x148
			}
			break;
		case 1:												// the rocket ship
			if (f_ab0)
			{
				if (f_a2c) f_a2c->SetSize(150, 50);			// 0x43f870
				Show(f_ab0);
				GO(f_ab0)->v52(1);							// +0xd0
				GO(f_ab0)->v132(1);							// +0x214
				OMedia3DVector p = Pos(this);
				GO(f_ab0)->v197(p.x, p.y, p.z);				// +0x318
				OMedia3DVector r = Rot(this);
				GO(f_ab0)->v203(r.x, r.y, r.z);				// +0x330
				g_gametype->SetCamera(GO(f_ab0));			// +0x118
				f_ab0->f_9f4 = this;						// BG +0x740: the rider
				v120(1);									// +0x1e4
				if (f_a1c)
				{
					f_a1c->v120(1);
					f_a1c->v132(0);
					f_a1c->f_1b60 = 1;						// BG +0x9a4: riding along
					f_a1c->ev58((char *)"ROCK", 1);			// element +0xe0 (0x4eca5c)
					f_a1c->v136(0);							// +0x224
					if (f_a30)
					{
						GO(f_a30)->v120(1);
						GO(f_a30)->v136(0);
					}
				}
				ev58((char *)"DRIVE", 1);
				StepSoundOff();
				g_4f0588 = 1;
			}
			if (ItemType(0, 9) == 2) LoadVR(this, "vr02.gam");
			break;
		case 2:												// power
			if (g_4f83d4 > 0 || f_9f8)
			{
				if (f_a1c)
				{
					f_a1c->HideAgain();
					f_a1c->v52(0);
					f_a1c->v132(0);
				}
				g_4f0588 = 2;
				f_1e1c = 0;
				f_55d = 1;									// (CGameObject +0x49d) ghost
				f_1e20 = 0;
				MenuGadgetBarEmpty(f_a18);					// +0x48c
				MenuGadgetBarShow(f_a18);					// +0x484
			}
			else g_4f0588 = -1;
			if (ItemType(0, 9) == 2) LoadVR(this, "vr03.gam");
			break;
		case -1: case 3:
			if (ItemType(0, 9) == 2) LoadVR(this, "vr04.gam");
			break;
		case 4:												// Goddard on the scooter
			if (f_a1c)
			{
				Hide(f_a1c->f_1b50);						// BG +0x994: his shadow
				if (f_a1c->v51())							// +0xcc
				{
					g_4f0588 = 4;
					OMedia3DVector p = Pos(GO(f_a1c));
					GO(f_a30)->v90(p.x, p.y, p.z);			// +0x16c
					float a[4];
					f_a1c->v201(a);
					GO(f_a30)->v92(a[0], a[1], a[2]);		// +0x174
					f_a30->ev106();							// its element +0x174 (0x421800): shown with the wheels
					GO(f_a30)->v52(1);
					GO(f_a30)->v132(1);
					f_a1c->HideAgain();
					f_a1c->v52(0);
					f_a1c->v132(0);
					g_4f0588 = 4;
				}
			}
			if (ItemType(0, 9) == 2) LoadVR(this, "vr05.gam");
			break;
		case 5:												// Goddard
			g_4f0588 = 5;
			if (f_a1c)
			{
				v261();										// +0x410
				g_gametype->SetCamera(GO(f_a1c));
				f_a1c->ev147(1);							// his element +0x188
				MenuGoddardMode(f_a18, true);				// +0x4bc
			}
			if (ItemType(0, 9) == 2) LoadVR(this, "vr06.gam");
			break;
		case 6:												// the baseball launcher
			g_4f0588 = 6;
			f_7c8.y += 30.0f;
			Show(MenuShotSprite(f_a18));
			if (f_a1c)
			{
				f_a1c->HideAgain();
				f_a1c->v52(0);
				f_a1c->v132(0);
			}
			if (ItemType(0, 9) == 2) LoadVR(this, "vr07.gam");
			break;
		case 7:												// the grappling hook
			if (f_a1c)
			{
				f_a1c->HideAgain();
				f_a1c->v52(0);
				f_a1c->v132(0);
			}
			f_7c8.set(0, 50.0f, -350.0f);					// the camera further back
			f_7d8.set(0, 150.0f, 80.0f);
			g_4f0588 = 7;
			if (ItemType(0, 9) == 2) LoadVR(this, "vr08.gam");
			break;
		case 8:
			f_1e24 = 1;
			f_1e25 = 1;
			break;
	}
	// (debug log)
}

void C3DJimmy::DeactivateGadget(void)						// 0x427ff0
{
	if (f_a18)
	{
		MenuSelectGadget(f_a18, 3);							// +0x4b0
		MenuGadgetTimerOff(f_a18);							// +0x4c4
	}
	switch ((unsigned short)g_4f0588)
	{
		case 0:												// the bubble: pops
			if (!f_a14) return;
			f_a20 = 0.5f;
			f_7c4 = 0;
			f_6c4 = 650.0f;
			BubblePop(f_a14);								// 0x410d00
			BubbleOff(f_a14);
			if (f_a40 != -1)
			{
				StopGameSound(f_a40, false);
				f_a40 = -1;
			}
			v67(60.0f);
			v64(1.265f);
			g_4f0588 = -1;
			return;
		case 1:												// out of the rocket ship
			// (debug log "Deactivating Rocket")
			Hide(f_ab0);
			GO(f_ab0)->v52(0);
			if (f_a2c) f_a2c->SetSize(70, 30);				// 0x43f870
			GO(f_ab0)->v132(0);
			GO(f_ab0)->v120(1);
			f_ab0->ev108(0);								// its element +0x128 (BG): engine sound off
			if (CGameObject *flame = f_ab0->f_1230) Hide(Elem(flame));	// BG +0xf7c
			g_gametype->SetCamera(static_cast<CGameObject *>(this));
			v120(0);
			if (f_a1c)
			{
				f_a1c->v120(0);
				f_a1c->f_1b60 = 0;
				f_a1c->v136(1);
				f_a1c->v132(1);
				if (!g_windowed && f_a1c->f_1b50) f_a1c->f_1b50->ShowAgain();	// his shadow
				if (f_a30) GO(f_a30)->v120(0);
			}
			if (f_ab0 && f_ab0->f_9f4)
			{
				g_gametype->SetCamera(static_cast<CGameObject *>(this));
				ev58((char *)"RUN", 1);						// (0x4eca64)
				f_ab0->f_9f4 = NULL;
				if (f_a1c) f_a1c->f_1b60 = 0;
			}
			v157(0, 0, 0);									// +0x278
			v176(0, 0, 0);									// +0x2c4
			ev58((char *)"FALL", 1);
			f_7c4 = 1;
			if (f_a18)
			{
				MenuGadgetsOff(f_a18);						// +0x480
				g_4f0588 = -1;
				return;
			}
			break;
		case 2:												// power over
			f_1e1c = 7.0f;
			ev65(0, 6, 7);						// element +0xfc (BG C3DAnimated slot 63)
			f_55d = 1;
			f_1e20 = 0;
			MenuGadgetBarHide(f_a18);						// +0x488
			if (f_a1c && !f_a1c->f_1bb4)					// BG +0x9e8
			{
				f_a1c->ShowAgain();
				f_a1c->v52(1);
				f_a1c->v132(1);
			}
			ev71(0);										// element +0x114 (BG slot 69: alpha off)
			g_4f0588 = -1;
			return;
		case 4:												// Goddard off the scooter
			if (f_a1c)
			{
				if (!g_windowed && f_a1c->f_1b50) f_a1c->f_1b50->ShowAgain();
				if (!f_a1c->v51())							// +0xcc
				{
					MenuShowHud(f_a18, false);				// +0x4b4
					f_a20 = 0.5f;
					OMedia3DVector p = Pos(GO(f_a30));
					f_a1c->v90(p.x, p.y, p.z);				// +0x16c
					float a[4];
					GO(f_a30)->v201(a);
					f_a1c->v92(a[0], a[1], a[2]);			// +0x174
					f_a1c->ShowAgain();
					f_a1c->v52(1);
					f_a1c->v132(1);
					g_4eefc8 = -1.0f;						// 0x42ae90
					g_4eefcc = -1.0f;
					Unlink(GO(f_a30));						// +0x3b0
					f_a30->HideAgain();						// its element +0xd8 (slot 54: hide)
					GO(f_a30)->v52(0);
					GO(f_a30)->v132(0);
					NEG_TODO(0x467f70, "C3DVehicle slot 89: delete its +0x664 object");	// its element +0x164
					if (f_a30->f_a78 != -1)					// BG +0x690: its engine
					{
						StopGameSound(f_a30->f_a78, false);
						f_a30->f_a78 = -1;
					}
					if (f_a30->f_a84)						// BG +0x69c: Jimmy was driving
					{
						f_1e25 = 0;
						g_4f8182 = 0;
						g_gametype->SetCamera(static_cast<CGameObject *>(this));
						OMedia3DVector q = Pos(this);
						v197(q.x, q.y + 100.0f, q.z);		// +0x318 (0x49fad4)
						v157(0, 0, 0);
						v176(0, 0, 0);
						ev58((char *)"RUN", 1);
						if (!g_windowed) Show(f_a2c);
						f_a30->f_a84 = NULL;
						g_4f0588 = -1;
						return;
					}
				}
			}
			break;
		case 5:												// Goddard: back to Jimmy
			if (f_a1c)
			{
				MenuGoddardMode(f_a18, false);				// +0x4bc
				f_a1c->ev147(0);							// his element +0x188
				g_4f0588 = -1;
				return;
			}
			break;
		case 6:												// the launcher away: camera back
			f_84c = 0;
			if (f_a1c)
			{
				f_a1c->ShowAgain();
				f_a1c->v52(1);
				f_a1c->v132(1);
				if (f_a1c->f_1b50) f_a1c->f_1b50->ShowAgain();
			}
			ResetCamera();									// element +0x170
			Hide(MenuShotSprite(f_a18));					// menu +0x4e4
			break;
		case 7:												// the hook away
			if (f_a1c && !f_a1c->f_1bb4)
			{
				f_a1c->ShowAgain();
				f_a1c->v52(1);
				f_a1c->v132(1);
			}
			GrappleRelease();								// element +0x204
			ResetCamera();									// element +0x170
			g_4f8210 = 0;
			f_864 = 0;
			Hide(f_b14);
			break;
	}
	g_4f0588 = -1;
}

void C3DJimmy::MenuMark(void)								// 0x4299b0
{
	if (f_a18) jimmybg::MenuMark(f_a18);					// +0x470
}

void C3DJimmy::ThrowBaseball(float x, float y, float z, float pitch)	// 0x429a30
{
	OMedia3DVector p = v224(x, y, z);						// +0x384
	C3DBaseball *ball = f_ab4[f_b04];
	if (!ball) return;
	Show(ball);
	GO(ball)->v136(1);										// +0x224
	GO(ball)->v132(1);										// +0x214
	GO(ball)->v196(p.x, p.y, p.z, p.w);						// +0x314
	OMedia3DVector d = v226(0, pitch * (1.0f / 45.0f), 1.0f);	// +0x38c (0x49fabc)
	const float inv = 1.0f / d.quick_magnitude();
	GO(ball)->v176(d.x * inv * 1800.0f, d.y * inv * 1800.0f, d.z * inv * 1800.0f);	// +0x2c4
	if (++f_b04 >= 20) f_b04 = 0;
}

// The gadget keys (each acts once per press, f_1e00): Esc / M / button 4 the PDA; P, C + Shift,
// Tab, Space, button 2, H, F1, button 9, I, button 3: the gadget and menu pages.
void C3DJimmy::GadgetKeys(void)								// 0x425870
{
	C2DInGameMenu *m = f_a18;
	if (!((!m || g_4ec494) && f_1e6c == 0)) return;
	if (g_keys[0x1b] || g_keys[0x4d] || g_joy_button4)		// Esc, M, button 4
	{
		if (!f_1e00)
		{
			f_1e00 = 1;
			if (!g_4f8182) MenuOpen(this);					// element +0x1f0
		}
		return;
	}
	if (g_keys[0x50] || (g_keys[0x43] && g_keys[0x10]))		// P, C + Shift
	{
		if (!f_1e00)
		{
			f_1e00 = 1;
			if (m)
			{
				if (g_4f8181) { MenuDeactivate(this); return; }	// element +0x1fc
				if (g_4f0588 != -1)
				{
					DeactivateGadget();						// element +0x178
					g_4f0588 = -1;
					MenuGadgetsOff(f_a18);					// +0x480
					return;
				}
				DeactivateGadget();
				f_1e4c = -1;
				MenuActivate(this, 0);						// element +0x1f8
			}
		}
		return;
	}
	if (g_keys[0x09] || g_keys[0x20] || g_joy_button2)		// Tab, Space, button 2
	{
		if (!f_1e00)
		{
			f_1e00 = 1;
			if (m && !g_4f8181)
			{
				if (f_1e34 > 0)
				{
					MenuGadgetTimerOff(m);					// +0x4c4
					f_1e34 = 0;
					return;
				}
				f_1e34 = 20.0f;
			}
		}
		return;
	}
	if (g_keys[0x48] || g_keys[0x70] || g_joy_button9)		// H, F1, button 9
	{
		if (!f_1e00)
		{
			f_1e00 = 1;
			if (m && !g_509834 && !g_50993a && !g_joy_button1 && !g_joy_button0 && g_4ec494)
			{
				if (g_4f8181)
				{
					if (g_4f0588 == 5) MenuGoddardMode(m, true);
					MenuDeactivate(this);
					return;
				}
				f_1e4c = -1;
				if (g_4f0588 == 5) MenuGoddardMode(m, false);
				MenuActivate(this, 2);
				f_1e24 = 0;
				g_4f8182 = 1;
			}
		}
		return;
	}
	if (g_keys[0x49] || g_joy_button3 || f_1e24)			// I, button 3
	{
		if (!f_1e00)
		{
			f_1e00 = 1;
			if (m && !g_509834 && !g_50993a && !g_joy_button1 && !g_joy_button0 && g_4ec494)
			{
				if (g_4f8181)
				{
					if (g_4f0588 == 5) MenuGoddardMode(m, true);
					MenuDeactivate(this);
					return;
				}
				f_1e4c = -1;
				if (g_4f0588 == 5) MenuGoddardMode(m, false);
				MenuActivate(this, 2);
				f_1e24 = 0;
				g_4f8182 = 1;
			}
		}
		return;
	}
	f_1e00 = 0;
}

unsigned char C3DJimmy::GrappleSwing(void)					// 0x429d00
{
	if (g_4f0588 != 7) return 0;
	NEG_TODO(0x429d00, "C3DJimmy slot 98: the grappling hook swing (gadget 7)");
	return 0;
}

// --------------------------------------------------------------------------- water, hits

void C3DJimmy::EnterWater(void)								// 0x42aa70
{
	// (debug log "Calling EnterWater")
	if (!f_1e01 && f_1e08 == -1)
	{
		PlayGameSoundIndex(-1, 0x27, false);
		f_1e08 = PlayGameSoundIndex(-1, 0x33, true);
	}
}

void C3DJimmy::LeaveWater(void)								// 0x42aac0
{
	// (debug log "Calling LeaveWater")
	if (f_1e08 != -1)
	{
		StopGameSound(f_1e08, false);
		f_1e08 = -1;
	}
}

void C3DJimmy::Splash(void)									// 0x42ab00
{
	PlayGameSoundIndex(-1, 0x27, false);
	if (g_4f0588 != 0) PlayGameSoundIndex(-1, 0xbe, false);
}

void C3DJimmy::Shock(float energy, int points, float time)	// 0x42a870
{
	if (f_1e03 || g_4f0588 == 0) return;					// (not in the bubble)
	f_1e03 = 1;
	f_1de8 = 0;
	f_1e2c = time;
	AddEnergy(energy);										// 0x42a920
	ScoreAdd(points);										// 0x42adc0
	JetpackFall();											// element +0x1e0
}

void C3DJimmy::HeadHit(void)								// 0x425170
{
	OMedia3DVector p = Pos(this);
	OMedia3DVector up(p.x, p.y + 150.0f, p.z, p.w);			// 0x4a3e34
	if (!PhysicsLineBlocked(p, up, false)) return;			// 0x47c210
	if (Vel(this).y <= 0) return;
	float vy = (float)(-Vel(this).y * 0.6);					// bounced back down
	if (vy > -85.0f) vy = -85.0f;
	OMedia3DVector v = Vel(this);
	v176(v.x, vy, v.z);										// +0x2c4
	if (g_4f0588 != 0)
	{
		if (f_1e5c > 5.0f) PlayGameSoundIndex(-1, 0x2f, false);
		f_1e5c = 0;
	}
	JetpackFall();											// element +0x1e0
}

void C3DJimmy::PlaceShadow(float dt)						// 0x4252e0
{
	(void)dt;
	if (g_4f8210) return;									// on the rope
	OMedia3DVector p = Pos(this);
	OMedia3DVector down(p.x, p.y - 2500.0f, p.z, p.w);		// 0x4a0ce8
	if (!PhysicsClosestHit(p, down, false)) return;			// 0x47c6e0
	const OMedia3DVector hit = g_phys_ray.end;				// 0x5cfc60
	if (!f_a2c) return;
	const float h = (float)(f_a2c->f_602 / 2 - 2);			// half its height (BG +0x5ea)
	OMedia3DVector q = Pos(this);
	CGameObject *sh = static_cast<CGameObject *>(f_a2c);
	sh->v197(q.x, hit.y + h, q.z);							// +0x318
	f_86c = Dist(Pos(sh), Pos(this));
}

void C3DJimmy::Trail(float dt)								// 0x425490
{
	f_1ddc += dt;
	if (!f_851)
	{
		if (f_1ddc > 0.3f)									// 0x49534c
		{
			if (++f_1de0 > 299) f_1de0 = 0;
			OMedia3DVector p = Pos(this);
			f_b1c[f_1de0].set(p.x, p.y, p.z, p.w);			// (0xb1c + n * 0x10)
			f_1ddc -= 0.3f;
		}
	}
	else if (f_1ddc > 0.1f)									// 0x495334: walking the trail back
	{
		f_1ddc -= 0.1f;
		if (--f_1de0 > 0)
		{
			const OMedia3DPoint &q = f_b1c[f_1de0];
			v196(q.x, q.y, q.z, q.w);						// +0x314
			return;
		}
		f_851 = 0;
	}
}

void C3DJimmy::HurtFlash(float dt)							// 0x4255c0
{
	OMedia3DMaterial *mat = f_5d4[0];						// TODO(BG): BG +0x5c0 (C3DAnimated -0x14): the material
	if (!f_1ded)
	{
		if (f_1e60 > 0) f_1e60 -= dt;
		if (f_1e03)
		{
			// shocked: red flashing for f_1e2c s
			f_1de8 = dt + f_1de8;
			double k;
			if (f_1de8 <= f_1e2c) k = (sin(f_1de8 * 20.0f) + 1.0) * 0.5;	// TODO(BG): 0x492a58 / 0x48e5d0 (+1, * 0.5)
			else
			{
				k = 1.0;
				f_1e03 = 0;
			}
			if (mat) mat->set_diffuse(OMediaFARGBColor(1.0f, 1.0f, (float)k, (float)k));
		}
		if (f_1dec)
		{
			// hit: blue / green flashing for 3 s
			const double t = dt + f_1de8;
			f_1de8 = (float)t;
			double k;
			if (t <= 3.0) k = (sin(t * 20.0f) + 1.0) * 0.5;	// TODO(BG): 0x49d5e8
			else
			{
				k = 1.0;
				f_1dec = 0;
			}
			if (mat) mat->set_diffuse(OMediaFARGBColor(1.0f, (float)k, 1.0f, (float)k));
		}
		return;
	}
	// splat: the shock texture flashing, then pushed back off the wall
	f_1de8 += dt;
	v261();													// +0x410
	const double s = sin(f_1de8 * 60.0f);					// 0x49d5b4
	ApplyTexture(f_590, (s + 1.0) * 0.5 <= 0.5 ? 1 : 0);	// element +0xf4: jimyshock / jimycarl (BG +0x57c)
	if (f_1de8 > 1.0f)
	{
		ApplyTexture(f_590, 0);
		f_1de8 = 0;
		f_1e60 = 0.9f;
		f_1ded = 0;
		f_1df0.y = 100.0f;
		f_1df0.x = f_1df0.x * 300.0f;						// 0x4a3e38 TODO(BG): check
		f_1df0.z = f_1df0.z * 300.0f;
		v175(f_1df0.x, f_1df0.y, f_1df0.z, f_1df0.w);		// +0x2c0
		if (g_camera == static_cast<CGameObject *>(this))
		{
			if (v133()) ev58((char *)"STOP", 1);
			else ev58((char *)"FALL", 1);
		}
	}
}

// --------------------------------------------------------------------------- the shrink ray

void C3DJimmy::ShrinkRay(float dt)							// 0x426e40
{
	if (ItemType(0, 3) != 1 || g_4f0588 != -1) return;
	if (f_a10)
	{
		Show(f_a10);
		OMedia3DVector p = Pos(this);
		GO(f_a10)->v197(p.x, p.y, p.z);						// +0x318
		OMedia3DVector r = Rot(this);
		GO(f_a10)->v203(r.x, r.y, r.z);						// +0x330
	}
	AddEnergy(-dt);											// 0x42a920
	for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
	{
		CGameObject *o = *it;
		if (!o || !o->f_13e || !o->NameContains((char *)"C3DENEMY") || !Shrinkable(o)) continue;
		OMedia3DVector a = Pos(o), b = Pos(this);
		OMedia3DVector d(a.x - b.x, a.y - b.y, a.z - b.z, 0);
		if (!(b.y - 150.0f < a.y)) continue;
		if (!(a.y < b.y + 60.0f)) continue;
		const float dist = sqrtf(d.z * d.z + d.y * d.y + d.x * d.x);
		if (!(dist < 1500.0f)) continue;
		omt_Angle ax, ay;
		d.angles(ax, ay);
		const float deg = (float)(short)ay * kAngleToDeg;
		if (!(deg > Rot(this).y - 5.0f)) continue;			// within 5 degrees of our heading
		if (!(deg < Rot(this).y + 5.0f)) continue;
		C3DAI *ai = dynamic_cast<C3DAI *>(o);
		if (ai && ai->m_state_locked) continue;						// BG +0x970: already shrunk
		OMedia3DVector from = Pos(this);
		from.y += 150.0f;
		if (PhysicsLineBlocked(from, v224(0, 150.0f, dist), false)) continue;	// 0x47c210
		Shrunk(o);											// element +0x1dc
		if (o->NameContains((char *)"C3DHUMPHREY"))
		{
			if (C3DAnimated *h = dynamic_cast<C3DAnimated *>(o)) h->ev58((char *)"RUNSHRUNK", 1);	// (and its +0x760 name)
		}
		PlayGameSound3DNoDoppler(this, -1, std::string("zap"), false);	// 0x458a40
		if (f_a1c && f_a1c->v133())
		{
			f_a1c->ev87(o, 5.0f);							// element +0x15c: Goddard barks at it
			if (!IsChannelPlaying(f_a1c->f_1b58))			// BG +0x99c
				f_a1c->f_1b58 = PlayGameSound(-1, std::string("goddard bark"), false);	// 0x458b40
		}
	}
}

void C3DJimmy::Shrunk(CGameObject *o)						// 0x42ac10
{
	// (debug log)
	C3DAnimated *e = dynamic_cast<C3DAnimated *>(o);
	C3DAI *ai = dynamic_cast<C3DAI *>(o);
	if (e) e->ev58((char *)"SHRINK", 0);					// its element +0xe0
	if (ai)
	{
		ai->ev73(5);										// its element +0x124
		*reinterpret_cast<float *>(&ai->m_974) = 30.0f;		// BG +0x974: shrunk for 30 s
		ai->m_state_locked = 1;										// BG +0x970
	}
	if (strcasecmp(o->m_tag, "LIBBYPLANT") == 0)
	{
		// the plant that holds Libby: she is free
		CGameObject *libby = FindObjectByTag("C3DLIBBY");	// 0x474070
		C3DAI *l = libby ? dynamic_cast<C3DAI *>(libby) : NULL;
		if (l)
		{
			ScoreAdd(100);
			l->ev73(0);
			l->m_speed = 160.0f;								// BG +0x6c4: speed
			l->ev72(l->m_anim_run, (char *)"WALK");				// her element +0x120: the animation names (BG +0x820 / +0x898)
			l->ev72(l->m_anim_walk, (char *)"WALK");
			if (GetTaskState("SCENE") == 0xfa) SetTaskState("SCENE", 0xff);
		}
		return;
	}
	if (strcasecmp(o->m_tag, "DINO") == 0)					// (0x4ee208)
	{
		if (GetTaskState("DINO") < 10)
		{
			SetTaskState("DINO", 10);
			ScoreAdd(300);
		}
		return;
	}
	ScoreAdd(10);
	if (ai) ai->m_keep_fleeing = 1.0f;								// BG +0x94c (C3DAI): keeps fleeing
}

void C3DJimmy::GoddardHeals(float dt)						// 0x427340
{
	if (f_a1c) AddHealth(dt * 0.1f);						// element +0x1cc (0x4a3e60) TODO(BG): check the constant
}

unsigned char C3DJimmy::GrappleFire(float dt)				// 0x426a70
{
	(void)dt;
	if (g_4f0588 != 7) return 0;
	NEG_TODO(0x426a70, "C3DJimmy slot 112: the grappling hook fire / reel (gadget 7)");
	return 1;
}

unsigned char C3DJimmy::OutOfEnergy(float dt)				// 0x4269d0
{
	(void)dt;
	if (g_4f83d4 > 0 || f_9f8) return 0;
	if (f_a34 != -1)
	{
		StopGameSound(f_a34, false);
		f_a34 = -1;
	}
	if (f_a48 == -1) f_a48 = PlayGameSound(-1, std::string("not available"), true);	// 0x458b40
	Hide(f_a10);
	return 1;
}

// Gadget 2 (power): 7 s of it, then it recharges (the bar fills over 7 s); while it is on, the
// bar empties and the colour flashes in the last 3 s.
void C3DJimmy::PowerGadget(float dt)						// 0x424e80
{
	if (!g_camera)
	{
		MenuGadgetBarHide(f_a18);							// +0x488
		return;
	}
	if (g_4f0588 != 2) return;
	MenuGadgetBarShow(f_a18);								// +0x484
	if (KeyFire() && !KeyJumpGadget())						// 0x42a7a0, 0x42a730
	{
		if (g_4f8182) return;
		if (f_1e1c == 0 && f_1e20 == 7.0f)
		{
			if (g_4f83d4 <= 5.0f && !f_9f8) return;			// 0x48d954 TODO(BG): check
			AddEnergy(-2.0f);
			f_1e1c = 7.0f;
			ev65(0, 6, 1);					// element +0xfc (BG C3DAnimated slot 63)
			ev71(0.7f);									// element +0x114 (BG slot 69)
			f_55d = 0;
			f_1e20 = 0;
		}
	}
	if (f_1e1c == 0 && (g_4f83d4 > 0 || f_9f8))
	{
		const float t = dt + f_1e20;
		f_1e20 = t;
		if (f_a18) SetEnergyBar(t * (1.0f / 7.0f) * 100.0f);	// 0x4a3e30 TODO(BG): check
		if (f_1e20 >= 7.0f)									// 0x48e5b8
		{
			MenuGadgetBarFull(f_a18);						// +0x490
			f_1e20 = 7.0f;
		}
	}
	if (f_1e1c > 0)
	{
		const float t = f_1e1c - dt;
		f_1e1c = t;
		float v = t * (1.0f / 7.0f) * 100.0f;
		if (v <= 0) v = 0;
		if (f_a18) SetEnergyBar(v);
		if (f_1e1c > 0)
		{
			if (f_1e1c <= 3.0f)
			{
				if (Random() & 1) ev65(0, 6, 7);	// flashing
				else ev65(0, 6, 1);
			}
		}
		else
		{
			f_1e1c = 0;
			ev65(0, 6, 7);
			ev71(0);
			f_55d = 1;
			MenuGadgetBarEmpty(f_a18);						// +0x48c
		}
	}
}

void C3DJimmy::AddHealth(float d)							// 0x42aa20
{
	f_a28 += d;
	if (f_a28 > 100.0f) f_a28 = 100.0f;
	if (f_a28 < 0) f_a28 = 0;
}

void C3DJimmy::JetpackFall(void)							// 0x425120
{
	if (f_7c4 != 1) return;
	f_7e8 = 0;
	f_7c4 = 1;
	ev58((char *)"FALL", 1);
	if (f_7f8) f_7f8->f_928 = 0;
	JetpackOff();											// element +0x140
}

void C3DJimmy::SetStepSound(int id)							// 0x42aec0
{
	if (f_a44 == -1) return;
	StopGameSound(f_a44, false);
	f_a44 = PlayGameSoundIndex(-1, id, true);
}

void C3DJimmy::SaveCheckpoint(void)							// 0x42af00
{
	f_1e48 = g_gametype->m_object_id;							// +0x490
	OMedia3DVector p = Pos(this);
	f_1e38.set(p.x, p.y, p.z, p.w);
}

void C3DJimmy::SaveCheckpointShow(void)						// 0x42af50
{
	SaveCheckpoint();
	MenuHudPicture(f_a18, 0x13, true, 200, 0x12);			// +0x4b8
}

void C3DJimmy::ProcessGadgets(float dt)						// 0x424570
{
	if (g_4f8198) return;									// 0x407060: paused
	if (g_camera) GadgetKeys();								// element +0x184
	if (f_1e34 > 0)
	{
		f_1e34 -= dt;
		if (f_1e34 <= 0)
		{
			MenuGadgetTimerOff(f_a18);						// +0x4c4
			MenuInput(dt);									// element +0x1ac
			return;
		}
		MenuGadgetTimerOn(f_a18);							// +0x4c0
	}
	MenuInput(dt);
}

void C3DJimmy::GrappleRelease(void)							// 0x429c10
{
	if (!g_4f8210) return;
	g_4f8210 = 0;
	f_864 = 0;
	f_854.x = 1.4f;
	f_854.y = 1.0f;
	f_854.z = 1.4f;
	f_7c8.set(0, 50.0f, -350.0f);
	f_7d8.set(0, 130.0f, 100.0f);
	g_4f82bc *= 0.5f;										// 0x4a3e64 TODO(BG): check
	g_4f82c0 *= 0.5f;
	v176(0, 0, 0);
	ev58((char *)"FALL", 1);
	if (f_1e04 != -1)
	{
		StopGameSound(f_1e04, false);
		f_1e04 = -1;
	}
	Hide(f_b0c);
}

void C3DJimmy::ScorePopup(float dt)							// 0x424d10
{
	if (g_score_popup_time <= 0) return;					// 0x4f83c8
	g_score_popup_time -= dt;
	if (g_score_popup_time <= 0) g_score_popup_time = 0;
	if (g_camera && !g_4f8198)
	{
		if (g_score_popup_time > 0.2)
		{
			DrawText(0x12c, 0xd2, 'GIB', "%d", g_score_popup);	// 0x468660
			return;
		}
		const double t = 0.2 - g_score_popup_time;			// flies away to the score
		DrawText((short)(int)(t * 1500.0 + 300.0), (short)(int)(t * 1000.0 + 210.0), 'GIB', "%d", g_score_popup);
		return;
	}
	if (g_score_popup_time > 0.2)
	{
		DrawText(0x1c2, 0xd2, 'GIB', "%d", g_score_popup);
		return;
	}
	const double t = 0.2 - g_score_popup_time;
	DrawText((short)(int)(t * 400.0 + 450.0), (short)(int)(t * 1000.0 + 210.0), 'GIB', "%d", g_score_popup);
}

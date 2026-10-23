/*  C3DJimmy part A: the CGameObject-family overrides (touching, Process, the control slot, pause /
    resume) and C3DPlayer's slots. See C3DJimmy.h.                                               */
#include "JimmyBG.h"
#include "../C3DAI.h"
#include "../yokian/C3DYokian.h"
#include "../pickups/C3DPickupItem.h"
#include "../TestHooks.h"
#include "../levels/CLevel04Games.h"

using namespace neg;
using namespace jimmybg;

// 0x403c10: the screen fades (kind <k>, over <t> s, at least 0.1).
static void FadeOut(int k, float t)
{
	g_4f8190 = k;
	if (t < 0.1f) t = 0.1f;
	g_4ec490 = t;
	g_4f818c = t;
	g_4ec494 = 0;
}

// 0x4073b0(0): the HUD picture at 0x4f8120 (hide, release its canvas, delete) away.
static void HudBlink(bool on)
{
	ShowLoadingPicture(on);									// 0x4073b0 (menu/C2DInGameMenu_b.cpp)
}

// --------------------------------------------------------------------------- touching

// An enemy that caught Jimmy (Yokians, Humphrey, Darwin, the plants): Jimmy is shocked (energy
// -10, score -10, 5 s), plays HIT; the enemy flees (AI state 5) <flee> units for 5 s.
static void CaughtBy(C3DJimmy *j, C3DAI *o, float flee)
{
	j->Shock(-10.0f, -10, 5.0f);							// element +0x1a0
	j->PlayAnim((char *)"HIT");								// element +0x150 (0x4ef2e4)
	o->ev73(5);											// its element +0x124
	o->m_flee_dist = flee;										// BG +0x940
}

void C3DJimmy::AddTouching(CGameObject *other)				// 0x422d30
{
	// (debug log "Enter Jimmt::Touched %s")
	CGameObject::AddTouching(other);						// 0x470a90
	C3DAI *ai = dynamic_cast<C3DAI *>(other);
	if (other->NameContains((char *)"C3DPENDULUM"))			// vtable +0x18
	{
		TouchedHurt();										// element +0x164
		return;
	}
	if (other->NameContains((char *)"C3DROCKETSHIP"))
	{
		if (!f_ab0)
		{
			f_ab0 = dynamic_cast<C3DRocketShip *>(other);
			other->f_00b = 0;								// (element 0xcb)
			g_gametype->SetCamera(other);					// +0x118
			if (f_ab0) f_ab0->f_9f4 = this;					// BG +0x740: the rider
			ev58((char *)"DRIVE", 1);						// element +0xe0
			StepSoundOff();									// element +0x148
			g_4f0588 = 1;
			if (f_a18)
			{
				SetItemType(0, 1, 1);
				ActivateGadget(1);							// element +0x174
			}
		}
		return;
	}
	if (other->NameContains((char *)"C3DSUB"))
	{
		// port: BG +0x1de4 (the sub ridden) has no member here (no C3DSub yet); it is set and used
		// only by this branch (the sub's +0x740 = the rider).
		NEG_TODO(0x422e60, "C3DJimmy touched a C3DSUB: get in (no C3DSub class yet)");
		return;
	}
	if (other->NameContains((char *)"C3DJEEP"))
	{
		JetpackFall();										// element +0x1e0
		g_gametype->SetCamera(other);
		if (C3DJeep *jeep = dynamic_cast<C3DJeep *>(other)) jeep->f_a84 = this;	// BG +0x69c: the driver
		Hide(f_a2c);										// the shadow
		Link(GO(f_a30));									// +0x3a8
		ev58((char *)"SCOOT", 1);
		StepSoundOff();
		return;
	}
	if (other->NameContains((char *)"C3DTANK"))
	{
		g_gametype->SetCamera(other);
		NEG_TODO(0x423030, "C3DJimmy touched a C3DTANK: the tank's +0x69c = Jimmy (no C3DTank yet)");
		ev58((char *)"FLY", 1);								// (0x4eca60)
		return;
	}
	if (other->NameContains((char *)"C3DBASEBALL"))
	{
		other->v132(0);										// +0x214
		other->v136(0);										// +0x224
		if (C3DObject *e = Elem(other)) e->hide(true);		// element +0x58
		return;
	}
	if (other->NameContains((char *)"C3DMISSILE"))
	{
		if (f_1e03) return;
		Shock(-10.0f, -10, 5.0f);
		v261();												// +0x410
		StepSoundOff();
		PlayGameSoundIndex(-1, 0x2a, false);
		return;
	}
	const bool plant = other->NameContains((char *)"C3DGIRLEATINGPLANT");
	if (plant || other->NameContains((char *)"C3DDARWIN"))
	{
		if (!ai || ai->m_state_locked || f_1e03) return;				// BG +0x970: its state locked
		if (strcasecmp(other->m_tag, "LIBBYPLANT") == 0) return;
		CaughtBy(this, ai, 500.0f);
		ai->m_flee_time = 5.0f;									// BG +0x96c
		v261();
		StepSoundOff();
		PlayGameSoundIndex(-1, 0x2a, false);
		return;
	}
	if (other->NameContains((char *)"C3DYOKIANSPY") || other->NameContains((char *)"C3DYOKIANGUARD") ||
		other->NameContains((char *)"C3DYOKIANSOLDIER"))
	{
		if (f_7c4 == 2 || !ai || ai->m_state_locked || f_1e03 || ai->m_state != 0) return;	// BG +0x6c8: attacking
		CaughtBy(this, ai, 1500.0f);
		ai->m_flee_time = 5.0f;
		v261();
		StepSoundOff();
		PlayGameSoundIndex(-1, 0x2a, false);
		C3DYokian *y = dynamic_cast<C3DYokian *>(other);
		if (y && y->m_loop_sound != -1)						// BG +0x9b4
		{
			StopGameSound(y->m_loop_sound, false);
			y->m_loop_sound = -1;
		}
		return;
	}
	if (other->NameContains((char *)"C3DHUMPHREY"))
	{
		if (f_7c4 == 2 || !ai || ai->m_state_locked || f_1e03 || ai->m_state != 0) return;
		CaughtBy(this, ai, 1500.0f);
		ai->m_flee_time = 5.0f;
		v261();
		StepSoundOff();
		PlayGameSoundIndex(-1, 0x2a, false);
		return;
	}
	if (other->NameContains((char *)"C3DBUTTON"))
	{
		if (strcasecmp(other->m_tag, "REACTOR") == 0)
		{
			f_1e51 = 1;
			if (g_gametype->m_object_id == 'LV4B') static_cast<CLevel04BGame *>(g_gametype)->m_sweeping = 1;	// 0x42325c (+0x5bc)
		}
		return;
	}
	if (other->NameContains((char *)"C3DAITRIGGER"))
	{
		// QUIRK: compares Jimmy's own tag (edi + 0x3a0), not the trigger's
		if (strcasecmp(m_tag, "DEACTIVATE") == 0) DeactivateGadget();	// element +0x178
		return;
	}
	if (other->NameContains((char *)"C3DLOADLEVEL") && g_4eefd0 > 0)
	{
		// leaving the level stops its timer
		g_4eefd0 = -1.0f;
		MenuShowHud(f_a18, false);							// +0x4b4
	}
	// (debug log "Leave Jimmy::Touch")
}

// --------------------------------------------------------------------------- Process / control

void C3DJimmy::Process(float dt)							// 0x424600
{
	TestPosLog(this, dt);									// port: NEG_TEST_POSLOG / NEG_TEST_GOTO
	g_4f8408 += dt;
	if (!g_camera)
	{
		// not the camera (a cut scene): the cursors and the shrink ray off
		Hide(Elem(f_1e54));
		MenuGadgetTimerOff(f_a18);							// +0x4c4
		f_1e34 = 0;
		if (f_a34 != -1)
		{
			StopChannel(f_a34, false);						// 0x47d7a0
			f_a34 = -1;
			Hide(f_a10);
		}
		if (f_a3c != -1)
		{
			StopChannel(f_a3c, false);
			f_a3c = -1;
		}
	}
	if (v133())												// on the floor (+0x218)
	{
		if (g_4f0588 == 0)
		{
			v152(160.0f);									// +0x264
			ev58((char *)"STOP", 1);						// element +0xe0
		}
		else v152(60.0f);
	}
	if (f_1e50) DrawText(0x14, 0xf0, 'DEM', "%d", GetTaskState("SCENE"));	// (debug: the SCENE task)
	if (f_a18 && g_4f8181)
	{
		// the PDA is open
		if (MenuPageShown(f_a18) != 2) return;				// +0x4a8
		MenuHideSelected(f_a18);							// +0x460
		return;
	}
	ScorePopup(dt);											// element +0x208
	if (f_1e6c > 0)
	{
		f_1e6c -= dt;
		if (f_1e6c <= 0)
		{
			// out of energy: the level starts again
			f_1e6c = 0;
			PauseAnim(0);										// element +0x11c
			if (f_a1c) f_a1c->PauseAnim(0);
			DeactivateGadget();								// element +0x178
			DrawText(0x3c, 0x39, 'GIB', "%6.0d", g_score);	// (0x4ec714)
			neg::RequestTask("RestartLevel.tsk");				// 0x460e70
			if (f_a18) MenuGadgetBarHide(f_a18);			// +0x488
			return;
		}
	}
	if (g_4f83d4 <= 0)
	{
		const int id = g_gametype->m_object_id;
		if (((id >= 'LEV6' && id <= 'LEV7') || id == 'LV4B' || (id >= 'LV5A' && id <= 'LV5B') || id == 'LV6A') &&
			f_1e6c == 0)
		{
			StopAllChannels();								// 0x47d850
			FadeOut(4, 6.0f);								// 0x403c10
			FreezeGame(true);								// g_gametype +0x168
			PauseAnim(1);
			if (f_a1c) f_a1c->PauseAnim(1);
			f_1e6c = 10.8f;
			PlayGameSoundIndex(-1, 0xe5, false);
		}
	}
	f_1e5c += dt;
	if (g_509a12)
	{
		// (debug: the number of objects shown)
		int n = 0;
		for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
			if ((*i)->v135()) n++;							// +0x220
		DrawText(0x123, 100, 'TIME', "%.4d", n);
	}
	if (f_1e64 && f_1e68 > 0)
	{
		f_1e68 -= dt;
		if (f_1e68 <= 0) f_1e68 = 0;
	}
	if (g_4eefc8 >= 0)
	{
		// a stopwatch counting up (minutes, seconds, hundredths)
		g_4eefc8 += dt;
		MenuShowHud(f_a18, true);
		const int m = (int)(g_4eefc8 * (1.0f / 60.0f));
		const int s = (int)((float)(m * 60) - g_4eefc8);
		const int h = (int)((g_4eefc8 - (float)(int)g_4eefc8) * 100.0f);
		DrawText(0x10f, 0xf, 'TIME', "%.2d", m);
		DrawText(0x133, 0xf, 'TIME', "%.2d", s);
		DrawText(0x157, 0xf, 'TIME', "%.2d", h);
		if (g_4eefcc > 0) DrawText(0x217, 0x1e, 'TIME', "%3d", (int)g_4eefcc);
	}
	if (g_4eefd0 >= 0)
	{
		// a countdown: the alarm at 0
		if (g_camera && !f_1e51)
		{
			g_4eefd0 -= dt;
			if (g_4eefd0 < 0) PlayGameSoundIndex(-1, 0x5b, false);
		}
		const int m = (int)(g_4eefd0 * (1.0f / 60.0f));
		const int s = (int)((float)(m * 60) - g_4eefd0);
		const int h = (int)((g_4eefd0 - (float)(int)g_4eefd0) * 100.0f);
		DrawText(0x10f, 0xf, 'TIME', "%.2d", m);
		DrawText(0x133, 0xf, 'TIME', "%.2d", s);
		DrawText(0x157, 0xf, 'TIME', "%.2d", h);
	}
	else if (g_gametype->m_object_id == 'LV4B')
	{
		g_4eefd0 -= dt;
		if (g_4eefd0 < -8.0f) neg::RequestTask("RestartLevel.tsk");	// 8 s after the time ran out
	}
	if (Vel(this).y > 0) HeadHit();							// element +0x1a4: rising
	PlaceShadow(dt);										// element +0x1a8
	if (!v133() && f_1e01) LeaveWater();					// element +0x194
	C3DPlayer::Process(dt);									// 0x437940
	PowerGadget(dt);										// element +0x1c8
	if (v133())
	{
		OMedia3DVector p = Pos(this);
		f_1e0c.set(p.x, p.y, p.z, p.w);
	}
	HurtFlash(dt);											// element +0x1b4
	Trail(dt);												// element +0x1b0
	if (f_a1c && f_a1c->f_1b5c == 4 && f_a1c->m_time > 9.0f)	// Goddard: BG +0x9a0 side, +0x6bc time there
	{
		f_a1c->m_time = 0;
		if (f_a1c->f_1b5e == 1) { f_a1c->ev95(2); return; }	// BG +0x9a2, element +0x17c (Goddard ev95)
		if (f_a1c->f_1b5e == 3) f_a1c->ev95(0);
	}
}

void C3DJimmy::v243(float dt)								// 0x426030
{
	if (g_4f8198) return;									// 0x407060: paused
	if (!GameIsRunning() || !g_camera || g_4f8430 || f_1ded) return;
	C3DPlayer::v243(dt);									// 0x438a60
	if (GrappleSwing(dt)) return;								// element +0x188
	if (!v133())
	{
		// in the air: the jet pack starts by itself when falling fast (with it in the menu)
		if (f_880 && !g_4f8210 && g_4f0588 != 0)
		{
			if (Vel(this).y < -900.0f)
			{
				if (ItemType(0, 3) == 1)
				{
					if (f_7f8) f_7f8->f_928 = 3.0f;			// its burn time
					PlayAnim((char *)"FLY");
					if (f_a3c == -1) f_a3c = PlayGameSoundIndex(-1, 1, true);
					f_1e64 = 1;
					f_1e68 = 5.0f;
					ev58((char *)"FLY", 1);
					f_7e8 = 5;
					v178(0, 100.0f, 0);						// +0x2cc
					f_7c4 = 1;
				}
				else if (Vel(this).y < -1100.0f) f_1e58 = 1;	// a long fall
			}
			else if (f_1e64 && f_1e68 == 0)
			{
				f_1e64 = 0;
				JetpackOff();								// element +0x140
				f_7e8 = 0;
				ev58((char *)"FALL", 1);
				if (f_7f8) f_7f8->f_928 = 0;
			}
		}
	}
	else if (f_1e58)
	{
		// landed after a long fall: hurt
		f_1e58 = 0;
		PlayGameSoundIndex(-1, 0xc0, false);
		Shock(-10.0f, -10, 5.0f);							// element +0x1a0
	}
	if (f_7c4 == 2)
	{
		// in the bubble: it uses energy; it follows Jimmy, towards the camera
		AddEnergy(dt * -0.5f);								// 0x42a920
		if (g_4f83d4 <= 0) DeactivateGadget();
		OMedia3DVector p = Pos(this);
		OMedia3DVector d(p.x - g_viewport->getx(), (p.y + 100.0f) - g_viewport->gety(), p.z - g_viewport->getz(), 0);
		const float inv = 1.0f / d.quick_magnitude();
		d.x *= inv; d.y *= inv; d.z *= inv;
		if (f_a14)
		{
			OMedia3DVector q = Pos(this);
			GO(f_a14)->v197(q.x - d.x * 80.0f, (q.y + 50.0f) - d.y * 80.0f, q.z - d.z * 80.0f);	// +0x318
		}
	}
	GoddardHeals(dt);										// element +0x1bc
	// the point cursor over the nearest pickup
	CGameObject *pick = FindNearest(this, '3PIC');			// 0x458e40
	C3DAnimated *cur = dynamic_cast<C3DAnimated *>(f_1e54);
	if (!pick) Hide(cur);
	else
	{
		const float d = Dist(Pos(pick), Pos(this));
		if (d >= 850.0f) Hide(cur);
		else if (dynamic_cast<C3DPickupItem *>(pick) && static_cast<C3DPickupItem *>(pick)->f_760)	// its BG +0x6a0 (CGameObject part)
		{
			if (cur)
			{
				cur->ev71(1.0f - d * 0.0011764705f);		// element +0x114: fading with the distance
				OMedia3DVector q = Pos(pick);
				GO(cur)->v196(q.x, q.y, q.z, q.w);
				GO(cur)->AddRotation(0, dt * -100.0f, 0);	// +0x334
				Show(cur);
			}
		}
	}
	if (!KeyFire() && !KeyPressed(0x0d))					// 0x42a7a0, 0x46a3e0(Enter)
	{
		// the fire keys up: the shot over
		Hide(MenuShotSprite(f_a18));						// menu +0x4e4
		f_1e25 = 0;
		if (f_1e26)
		{
			PlayGameSoundIndex(-1, 10, false);
			StepSoundOff();
			ThrowBaseball(0, 80.0f, 50.0f, f_1e28);			// element +0x180
			f_b08 = 0;
			f_1e26 = 0;
			AddEnergy(-1.0f);
		}
		if (f_a34 != -1)
		{
			StopGameSound(f_a34, false);
			f_a34 = -1;
		}
		if (f_a48 != -1)
		{
			StopGameSound(f_a48, false);
			f_a48 = -1;
		}
		if (f_6c8 < 10.0f && strcasecmp(GetAnim(), "HISHOOT") == 0) StepSoundOff();	// element +0xe8
		Hide(f_a10);
		return;
	}
	if (f_1e25 || g_4f8182 || g_4f0588 == 0) return;
	f_6c8 = 0;
	if (GrappleFire(dt)) return;							// element +0x1c0
	if (OutOfEnergy(dt)) return;							// element +0x1c4
	if (g_4f0588 == 4) return;
	if (v133())
	{
		// (debug log "MEBTEMP calling SHOOT")
		ev58((char *)"SHOOT", 1);
		StepSoundOff();
		Hide(MenuShotSprite(f_a18));						// 0x426737: menu +0x4e4 hide(true) for every gadget
		if (g_4f0588 == 6)
		{
			// the baseball launcher: aim with the camera height, throw when the key is let go
			DrawText(0x11e, (short)(int)((45.0f - f_1e28) * 4.3 + 100.0), 'TIME', "% .2d", (int)f_1e28);	// the aim, beside its bar
			Show(MenuShotSprite(f_a18));					// 0x4267a1: force_show, only the baseball launcher's aim bar
			if (!f_1e26)
			{
				f_1e26 = 1;
				StepSoundOff();
			}
			else
			{
				if (f_84c > 45.0f) f_84c = 45.0f;
				if (f_84c < 0) f_84c = 0;
				OMedia3DVector p = Pos(this);
				OMedia3DVector a = v224(0, f_84c + 80.0f, 45.0f);
				OMedia3DVector d(a.x - p.x, a.y - (p.y + 80.0f), a.z - p.z, 0);
				omt_Angle ax, ay;
				d.angles(ax, ay);							// (the angles are not used)
				f_1e28 = f_84c;
			}
			goto shoot_end;
		}
		if (g_4f0588 == 2 || ItemType(0, 3) != 1) goto not_available;
		if (f_a34 != -1) goto shoot_end;
	}
	else
	{
		if (g_4f0588 == 2 || ItemType(0, 3) != 1) goto not_available;
		if (f_a34 != -1)
		{
			if (strcasecmp(ev66(), "FALL") != 0) goto shoot_end;
			if (f_a34 != -1)
			{
				StopGameSound(f_a34, false);
				f_a34 = -1;
			}
			Hide(f_a10);
			return;
		}
		if (strcasecmp(ev66(), "FALL") == 0) return;
	}
	// the shrink ray's sound
	f_a34 = PlayGameSoundIndex(-1, 0, true);
	StepSoundOff();
	goto shoot_end;
not_available:
	PlayGameSoundIndex(-1, 0x75, false);
	f_1e25 = 1;
shoot_end:
	if (g_4f0588 != 4) ShrinkRay(dt);						// element +0x1b8
}

void C3DJimmy::v242(float t)								// 0x4235b0
{
	(void)t;
	if (f_a34 != -1)
	{
		StopGameSound(f_a34, false);						// 0x458a00
		f_a34 = -1;
		StopAllChannels();									// 0x47d850
	}
	if (g_50988a)
	{
		// a new game: forget the object states and the pickups
		memset(g_4f8438, 0, 0x1068 * sizeof(int));
		for (int i = 0; i < 29; i++) ClearItem(2, (short)i);	// 0x403910
	}
}

void C3DJimmy::v258(void)									// 0x423580
{
	C3DPlayer::v258();										// (C3DFlyingObject's 0x41afb0 in the original)
	g_gametype->SetCamera(static_cast<CGameObject *>(this));	// +0x118
}

void C3DJimmy::v261(void)									// 0x42afc0
{
	C3DPlayer::v261();										// 0x43a750
	StepSoundOff();											// element +0x148
	PlayAnim((char *)"STOP");								// element +0x150
}

void C3DJimmy::v271(void)									// 0x42ae00
{
	DeactivateGadget();										// element +0x178
	f_1e25 = 1;
	g_4f8182 = 1;
}

void C3DJimmy::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x42a660
{
	if (g_4f0588 == 7 && g_4f8210)
	{
		// swinging on the rope: a wall hit fast lets go
		OMedia3DVector plane(a, b, c, d);					// (0x409f60: a copy)
		C3DPlayer::v220(a, b, c, d, poly);					// 0x43aa40
		const float sz = g_4f82c0 * g_4f82c0, sx = g_4f82bc * g_4f82bc;
		g_4f82bc = 0;
		g_4f82c0 = 0;
		if (sqrtf(sx + sz) > 60.0f)							// 0x49d5b4
		{
			GrappleRelease();								// element +0x204
			f_1e25 = 1;
		}
		return;
	}
	C3DPlayer::v220(a, b, c, d, poly);
}

void C3DJimmy::v277(void)									// 0x425c40
{
	C3DAnimated::v277();									// 0x40e770
	if (strcasecmp(f_a4c, "none") != 0 && g_696004)
	{
		StopChannel(f_a38, false);							// 0x47d7a0
		if (strcasecmp(f_a4c, "none") != 0) CloseDatabase(f_a4c);	// 0x46aef0
		g_696004 = NULL;
		f_a38 = -1;
	}
	if (f_1e08 != -1) StopGameSound(f_1e08, false);			// (the channels are kept for v278)
	HudBlink(false);										// 0x4073b0(0)
	if (f_a48 != -1) StopGameSound(f_a48, false);
	if (f_1e04 != -1) StopGameSound(f_1e04, false);
	if (f_a34 != -1)
	{
		StopGameSound(f_a34, false);
		f_a34 = -1;
		Hide(f_a10);
		f_1e25 = 1;
	}
	if (f_a40 != -1) StopGameSound(f_a40, false);
	if (f_a3c != -1) StopGameSound(f_a3c, false);
	if (f_a44 != -1) StopGameSound(f_a44, false);
	if (f_a38 != -1) StopGameSound(f_a38, false);
	if (f_ab0 && f_ab0->f_123c != -1) StopGameSound(f_ab0->f_123c, false);	// BG +0xf88: its engine
}

void C3DJimmy::v278(void)									// 0x425db0
{
	C3DAnimated::v278();									// 0x40e790
	if (strcasecmp(f_a4c, "none") != 0)
	{
		g_696004 = GetDatabase(f_a4c);						// 0x46a910
		if (g_696004) f_a38 = PlayMusic(g_696004, -1, 0, true);	// 0x47d710
	}
	if (f_1e08 != -1) f_1e08 = PlayGameSoundIndex(-1, 0x33, true);	// 0x425e0b
	if (f_a48 != -1) f_a48 = PlayGameSound(-1, std::string("not available"), true);	// 0x458b40
	if (f_1e04 != -1) f_1e04 = PlayGameSoundIndex(-1, 0x22, true);
	if (f_a40 != -1) f_a40 = PlayGameSoundIndex(-1, 2, true);
	if (f_a3c != -1) f_a3c = PlayGameSoundIndex(-1, 1, true);
	if (f_a44 != -1) f_a44 = PlayGameSoundIndex(-1, static_cast<CJimmyGame *>(g_gametype)->m_surface_sound, true);	// the level's footsteps (+0x54c)
	if (f_ab0 && f_ab0->f_123c != -1)
	{
		f_ab0->f_123c = -1;
		f_ab0->ev108(2);									// its element +0x128: the engine sound
	}
}

// --------------------------------------------------------------------------- C3DPlayer's slots

void C3DJimmy::JetpackOn(int a)								// 0x429970
{
	C3DPlayer::JetpackOn(a);								// (the empty 0x4732a0)
	MenuGadgetBarShow(f_a18);								// +0x484
	if (f_a3c == -1) f_a3c = PlayGameSoundIndex(-1, 1, true);
}

void C3DJimmy::JetpackOff(void)								// 0x429910
{
	f_1e64 = 0;
	f_1e68 = 0;
	if (g_4f0588 != 2) MenuGadgetBarHide(f_a18);			// +0x488
	if (f_a3c != -1)
	{
		StopGameSound(f_a3c, false);
		f_a3c = -1;
	}
}

void C3DJimmy::StepSoundOn(void)							// 0x4299d0
{
	if (f_a44 == -1) f_a44 = PlayGameSoundIndex(-1, static_cast<CJimmyGame *>(g_gametype)->m_surface_sound, true);	// the level's footsteps (+0x54c)
}

void C3DJimmy::StepSoundOff(void)							// 0x429a00
{
	if (f_a44 != -1)
	{
		StopGameSound(f_a44, false);
		f_a44 = -1;
	}
}

void C3DJimmy::TouchedShock(OMedia3DVector plane)			// 0x42a7e0
{
	if (f_1e60 > 0 || f_1ded) return;
	PlayAnim((char *)"SPLAT");
	f_1ded = 1;
	f_1de8 = 0;
	f_1df0 = plane;
	f_6c8 = 0;
	AddEnergy(-5.0f);										// 0x42a920
	ScoreAdd(-5);											// 0x42adc0
	PlayGameSoundIndex(-1, 0x25, false);
}

void C3DJimmy::TouchedHurt(void)							// 0x42a8d0
{
	if (f_1dec || g_4f0588 == 0) return;					// (not in the bubble)
	f_1dec = 1;
	f_1de8 = 0;
	AddEnergy(-5.0f);
	ScoreAdd(-5);
	JetpackFall();											// element +0x1e0
}

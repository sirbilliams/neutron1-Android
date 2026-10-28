/*  C3DJimmy - see C3DJimmy.h. This file: construction, destruction, the factory entry,
    InitObject / UnInitObject and the level start (PostLoad). The other slots are in
    jimmy/C3DJimmy_a.cpp (CGameObject family, C3DPlayer's slots) and _b.cpp (Jimmy's own).       
    Reconstructed from Neutron.exe  */
#include "jimmy/JimmyBG.h"
#include "objects/C3DStartPoint.h"
#include "JimmySlots.h"

using namespace neg;
using namespace jimmybg;

// Factory entries of the helper classes (gen/factory.cpp: weak TODO stubs until they exist).
namespace neg {
void JimmyAddEnergy(float d);
CGameObject *New_C3DRocketShip(void);
CGameObject *New_C3DShrinkRay(void);
CGameObject *New_C3DBubble(void);
CGameObject *New_C3DBaseball(void);
CGameObject *New_C3DShadow(void);
CGameObject *New_C3DGoddard(void);
CGameObject *New_C3DJeep(void);
}

// The core's hooks (JimmySlots.h: CJimmyGame, CLoadLevel): Jimmy's element slots.
static C3DJimmy *J(CGameObject *jim)	{ return jim ? dynamic_cast<C3DJimmy *>(jim) : NULL; }
void neg::JimmyElementSlot88(CGameObject *jim, float a, float b, float c, float d)	{ if (C3DJimmy *j = J(jim)) j->TouchedShock(OMedia3DVector(a, b, c, d)); }
void neg::JimmyElementSlot89(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->TouchedHurt(); }
void neg::JimmyElementSlot94(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->DeactivateGadget(); }
void neg::JimmyElementSlot95(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->MenuMark(); }
void neg::JimmyElementSlot99(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->Slot99(); }
void neg::JimmyElementSlot100(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->EnterWater(); }
void neg::JimmyElementSlot101(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->LeaveWater(); }
void neg::JimmyElementSlot102(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->Splash(); }
void neg::JimmyElementSlot103(CGameObject *jim)	{ if (C3DJimmy *j = J(jim)) j->Slot103(); }
void neg::JimmyElementSlot104(CGameObject *jim, float a, int b, float c)	{ if (C3DJimmy *j = J(jim)) j->Shock(a, b, c); }
void neg::JimmyElementSlot121(CGameObject *jim, int sound_set)	{ if (C3DJimmy *j = J(jim)) j->SetStepSound(sound_set); }
void neg::JimmyElementSlot128(CGameObject *jim, float dt)	{ if (C3DJimmy *j = J(jim)) j->ProcessGadgets(dt); }
unsigned char &neg::JimmyByte(CGameObject *jim, int off)
{
	static unsigned char none = 0;
	C3DJimmy *j = J(jim);
	if (!j) return none;
	switch (off)
	{
		case 0x1e01: return j->f_1e01;
		case 0x1e02: return j->f_1e02;
		case 0x1e03: return j->f_1e03;
	}
	return none;
}

void neg::JimmyAddEnergy(float d)	{ AddEnergy(d); }		// 0x42a920 (for C3DJetpackFire)

C3DJimmy *neg::Jimmy(void)
{
	return g_jimmy ? dynamic_cast<C3DJimmy *>(g_jimmy) : NULL;
}

// --------------------------------------------------------------------------- construction

C3DJimmy::C3DJimmy()										// 0x422160
{
	for (int i = 0; i < 300; i++) f_b1c[i].set(0, 0, 0);	// (w = 1)
	f_1df0 = OMedia3DVector(0, 0, 0, 0);
	f_1e0c.set(0, 0, 0);
	f_1e38.set(0, 0, 0);
	SetName((char *)"C3DJIMMY");
	Log("C3DJimmy()");
	C3DJimmy::InitObject();									// 0x422ac0 (called directly)
	SetObjectID('3JIM');									// element +0xc4
	f_9fc = 0;
	g_4f0588 = -1;											// no gadget
	f_a34 = -1;
	f_b18 = new CTaskList;									// 0x45f790 (0x4b8 bytes)
	f_a10 = dynamic_cast<C3DAnimated *>(New_C3DShrinkRay());	// 0x43fe20 (0x6d0 bytes)
	Hide(f_a10);
	f_a14 = dynamic_cast<C3DBubble *>(New_C3DBubble());							// 0x410840 (0x620 bytes)
	Hide(f_a14);
	f_a18 = new C2DInGameMenu;								// 0x401430 (0x51c bytes)
	g_menu = f_a18;											// port: the menu is also reached through g_menu
	if (f_a18)
	{
		SetItemType(0, 9, 1);								// 0x4038c0
		SetItemType(0, 10, 1);
		SetItemType(1, 0, 1);
		for (short i = 2; i <= 8; i++) ClearItem(1, i);		// 0x403910
	}
	f_ab0 = dynamic_cast<C3DRocketShip *>(New_C3DRocketShip());	// 0x43d840 (0xf9c bytes)
	if (f_ab0)
	{
		Hide(f_ab0);
		GO(f_ab0)->v52(0);									// +0xd0
		GO(f_ab0)->v132(0);									// +0x214
		GO(f_ab0)->v120(1);									// +0x1e4
		GO(f_ab0)->v107(0);									// +0x1b0
		GO(f_ab0)->v36(0);									// +0x90
	}
	f_a2c = dynamic_cast<C3DShadow *>(New_C3DShadow());	// 0x43f640 (0x5f4 bytes)
	f_b14 = new C3DTargetCursor;							// 0x4453b0 (0x5f4 bytes)
	Hide(f_b14);
	C3DPointCursor *pc = new C3DPointCursor;				// 0x43bc70 (0x6c4 bytes)
	f_1e54 = static_cast<CGameObject *>(pc);
	Hide(pc);
	for (int i = 0; i < 20; i++)
	{
		f_ab4[i] = dynamic_cast<C3DBaseball *>(New_C3DBaseball());					// 0x40fc50 (0x61c bytes)
		if (f_ab4[i])
		{
			Hide(f_ab4[i]);
			GO(f_ab4[i])->v52(0);
			GO(f_ab4[i])->v132(0);
		}
	}
	f_b04 = 0;
	f_a1c = NULL;
	f_a30 = NULL;
	f_a0c = 0;
	v36(0);													// +0x90
	v107(0);												// +0x1b0
	f_a20 = 0;
	f_a26 = -1;
	f_a24 = 4;
	g_4f83d4 = 20.0f;										// energy
	f_a28 = 50.0f;											// health
	g_4f8434 = 0;
	f_a38 = -1;
	f_a40 = -1;
	f_a3c = -1;
	f_a34 = -1;
	f_a44 = -1;
	f_a48 = -1;
	strcpy(f_a4c, "none");
	f_b08 = 0;
	g_4f83cc = 0;
	f_009 = 1;												// (CGameObject +0x9: kept over level loads)
	g_4f8210 = 0;											// the grappling hook: not hooked
	g_4f82cc = 0;
	g_4f8214 = 0;											// the first rope anchor (0x4f8214, x y z)
	g_4f8218[0] = 0;
	g_4f821c = 0;
	g_4f82b8 = 0;
	g_4f82b4 = 0;
	g_4f82bc = 0;
	g_4f82c0 = 0;
	g_4f82c4 = 0;
	f_1e1c = 0;
	f_1e20 = 0;
	f_1e04 = -1;
	f_1e08 = -1;
	f_1e25 = 1;
	f_1e24 = 0;
	f_b0c = new C3DGraplingHook;							// 0x41eeb0 (0x6c4 bytes)
	{
		// the rope's material: diffuse (1, 0.3, 0.3, 1) (element +0x5c0, its first material +0x4c)
		OMedia3DMaterial *m = f_b0c->f_5d4[0];
		if (m) m->set_diffuse(OMediaFARGBColor(1.0f, 1.0f, 0.3f, 0.3f));	// (BG +0x5c0 = our f_5d4, C3DAnimated -0x14)
	}
	f_1ddc = 0;
	f_1de0 = 0;
	f_1de8 = 0;
	f_1dec = 0;
	f_1ded = 0;
	f_1e00 = 0;
	f_1e01 = 0;
	f_1e02 = 0;
	f_1e03 = 0;
	g_4f83d0 = 0;
	g_score_popup_time = 0;									// 0x4f83c8
	f_1e26 = 0;
	f_1e28 = 0;
	f_84c = 10.0f;
	g_score_popup = 0;										// 0x4f83c4
	g_4eefcc = -1.0f;
	g_4eefc8 = -1.0f;
	g_4eefd0 = -1.0f;
	f_1e2c = 5.0f;
	f_1e30 = 0;
	f_1e34 = 0;
	f_880 = 1;
	f_1e60 = 0;
	g_4eefd4 = 6;
	g_4f83d8 = static_cast<CGameObject *>(this);
	f_1e4c = -1;
	f_1e50 = 0;
	f_1e51 = 0;
	f_1e58 = 0;
	f_42c = 1;												// (element 0x4ec = CGameObject +0x42c: Physics reads it as f_42c)
	f_1e59 = 0;
	f_1e5c = 0;
	f_1e64 = 0;
	f_1e68 = 0;
	f_1e6c = 0;
	f_1e70 = 0;
	g_4f83dc = 0;
	// port: not set by the original (left as the allocation had them)
	f_1e48 = 0;
	f_55d = 0;
	f_55f = 0;
}

C3DJimmy::~C3DJimmy()										// 0x4229e0
{
	Log("~C3DJimmy()");
	// ("MEBTEMP deleting jimmy!!!", debug log; the helpers are not deleted)
}

namespace neg {
CGameObject *New_C3DJimmy(void)	{ return new C3DJimmy; }	// factory '3JIM' (0x1e7c bytes)
}

// --------------------------------------------------------------------------- InitObject / UnInitObject

void C3DJimmy::InitObject(void)								// 0x422ac0
{
	Log("C3DJimmy::InitObject()");
	C3DPlayer::InitObject();								// 0x437830
	InitAnim();												// element +0x108
	CreateAnim((char *)"HISHOOT", (char *)"jimshoot.ase");	// element +0xd8
	CreateAnim((char *)"HIDRIVE", (char *)"jimdrive.ase");
	CreateAnim((char *)"HIFLY", (char *)"jimfly.ase");
	CreateAnim((char *)"HIRUN", (char *)"jimrun.ase");
	CreateAnim((char *)"HILEFT", (char *)"jimleft.ase");
	CreateAnim((char *)"HIRIGHT", (char *)"jimright.ase");
	CreateAnim((char *)"HITALK", (char *)"jimtalk.ase");
	CreateAnim((char *)"HIFALL", (char *)"jimfall.ase");
	CreateAnim((char *)"HIFENCE", (char *)"jimfence.ase");
	CreateAnim((char *)"HISPLAT", (char *)"jimsplat.ase");
	CreateAnim((char *)"HIHIT", (char *)"jimhit.ase");
	CreateAnim((char *)"HISWING", (char *)"jimswing.ase");
	CreateAnim((char *)"HILADDER", (char *)"jimladder.ase");
	CreateAnim((char *)"HIJUMP", (char *)"jimjump.ase");
	CreateAnim((char *)"HISCRATCH", (char *)"jimscratch.ase");
	CreateAnim((char *)"HIBUTTONS", (char *)"jimButtons.ase");
	CreateAnim((char *)"HIPLAY", (char *)"jimheadshrink.ase");
	CreateAnim((char *)"HIBACK", (char *)"jimbackpedal.ase");
	CreateAnim((char *)"HISCOOT", (char *)"jimscooter.ase");
	CreateAnim((char *)"HISCOOTSTOP", (char *)"Jimscooterstop.ase");
	CreateAnim((char *)"HISTOP", (char *)"jimstop.ase");
	CreateTexture((char *)"jimycarl.png", 0);				// element +0xf0
	CreateTexture((char *)"jimyshock.png", 1);
	ApplyTexture(f_590, 0);									// element +0xf4 (BG +0x57c: the morph base shape)
	set_shape(f_590);										// element +0xac
	set_morph_flags(ommorf_InterpolateVertices | ommorf_InterpolateVNormals);	// element +0xb8 = 3
	f_594 = 0;												// (element short 0x580: C3DAnimated's detail HI)
	ev58((char *)"RUN", 1);									// element +0xe0 (0x4eca64 "RUN")
	v197(0, 600.0f, 100.0f);								// +0x318
	OMediaElement::place(0, 600.0f, 100.0f);				// element +0x44..0x4c
}

void C3DJimmy::UnInitObject(void)							// 0x422a90
{
	C3DAnimated::UnInitObject();							// 0x40d3a0
	if (g_active_sound_db)
	{
		CloseDatabase("SoundEffects.omt");					// 0x46aef0
		g_active_sound_db = NULL;
		g_4f8434 = 0;
	}
}

// --------------------------------------------------------------------------- the level start

// 0x406f50: the level's countdown of <n> (g_4f8194; g_4f7d00[n] = on; the other 44 cleared).
static void LevelTimer(int n, unsigned char on)
{
	if (n <= 0) return;
	for (int i = 0; i < 0x2c; i++) g_4f7d00[i] = 0;
	g_4f8194 = n;
	g_4f8188 = 0;
	g_4f7d00[n] = on;
}

// "PostLoad" (BG slot 259): after the level's objects exist. Sounds, the start point (its position,
// angles, camera and music), Goddard and the jeep (made the first time), the gadgets the level
// allows (menu page 0 items 0..7), the restart task file, the camera.
void C3DJimmy::v260(void)									// 0x423610
{
	// (debug log "Calling PostLoad for C3DJimmy()")
	f_1e51 = 0;
	g_4f8434 = 0;
	g_active_sound_db = GetDatabase("SoundEffects.omt");	// 0x46a910
	strcpy(f_a4c, "none");
	set_shape(f_590);										// element +0xac (BG +0x57c)
	v67(60.0f);												// +0x110 (radius)
	v64(1.265f);											// +0x100
	f_9f8 = 0;
	g_4f6b3c = 0.07f;										// sound falloff
	g_4f8430 = 0;
	f_55f = 0;
	f_1e03 = 0;
	f_1ded = 0;
	f_1dec = 0;
	FreezeGame(false);										// g_gametype +0x168 (0)
	PauseAnim(0);												// element +0x11c: animation not paused
	g_4f6b34 = g_695f80;									// music volume
	if (f_a1c) f_a1c->PauseAnim(0);
	if (g_active_sound_db) g_4f8434 = 1;
	if (g_4f0588 == 5) DeactivateGadget();					// element +0x178
	if (f_a34 != -1)
	{
		StopGameSound(f_a34, false);						// 0x458a00
		f_a34 = -1;
	}
	if (f_a48 != -1)
	{
		StopGameSound(f_a48, false);
		f_a48 = -1;
	}
	f_7e8 = 0;
	f_a30 = NULL;
	if (f_a18 && MenuMarked(f_a18)) MenuUnmark(f_a18);		// +0x4be, +0x474
	C3DAnimated::v260();									// 0x418950 (jmp to C3DAnimated's)

	// the start point: "StartPoint", or the one PlayerLoadLevel asked for (f_774)
	if (strcasecmp(f_774, "none") != 0) strcpy(f_724, f_774);
	strcpy(f_774, "none");
	if (strcasecmp(f_724, g_4f81a8) != 0)
	{
		for (std::list<CGameObject *>::iterator it = g_objects.begin(); it != g_objects.end(); ++it)
		{
			CGameObject *sp = *it;
			if (!sp || !sp->NameContains((char *)"C3DSTARTPOINT")) continue;
			if (strcasecmp(f_724, sp->m_tag) != 0) continue;	// +0x3a0
			strcpy(f_a4c, "none");
			InitPhysics();										// +0x2c
			if (f_7f8)
			{
				f_7f8->link(g_world);							// element +0x2c
				f_7f8->link_layer(g_layer_world);				// element +0x34
				f_7f8->InitPhysics();
			}
			float sp4[4];
			sp->v88(sp4);										// +0x164: its (loaded) position
			OMedia3DVector p(sp4[0], sp4[1], sp4[2], sp4[3]);
			f_1e0c.set(p.x, p.y, p.z, p.w);
			v196(p.x, p.y, p.z, p.w);							// +0x314
			v90(p.x, p.y, p.z);									// +0x16c
			float a[4];
			sp->v91(a);											// +0x170: its angles
			v92(a[0], a[1], a[2]);								// +0x174
			if (f_a1c) f_a1c->v89(p.x, p.y, p.z, *(unsigned int *)&p.w);	// +0x168
			OMediaElement::place(p.x, p.y, p.z);				// element +0x44..0x4c
			if (f_9a5)
			{
				// coming back (PlayerLoadLevel kept the position): where he was
				f_9a5 = 0;
				v90(f_9a8.x, f_9a8.y, f_9a8.z);
				OMediaElement::place(f_9a8.x, f_9a8.y, f_9a8.z);
				if (f_a1c) f_a1c->v89(f_9a8.x, f_9a8.y, f_9a8.z, *(unsigned int *)&f_9a8.w);
			}
			if (g_gametype) g_gametype->SetCamera(static_cast<CGameObject *>(this));	// +0x118
			C3DStartPoint *stp = dynamic_cast<C3DStartPoint *>(sp);
			if (stp) g_viewport->place(stp->m_viewport_pos[0], stp->m_viewport_pos[1], stp->m_viewport_pos[2]);	// its view (+0x520)
			StopChannel(g_4f6b30, true);						// 0x47d7a0
			CloseDatabase(f_a4c);								// 0x46aef0
			CloseDatabase("music.omt");
			f_a38 = -1;
			const char *music = stp ? stp->m_music_db : "none";	// +0x540 "MusicDatabase"
			if (strcasecmp(music, "none") != 0)
			{
				strcpy(f_a4c, music);
				g_696004 = GetDatabase(f_a4c);
				if (g_696004 && f_a38 == -1) f_a38 = PlayMusic(g_696004, -1, 0, true);	// 0x47d710
			}
		}
	}
	OMedia3DVector cam = v224(0, 200.0f, -300.0f);			// +0x384
	g_viewport->place(cam.x, cam.y, cam.z);

	// Goddard and the jeep
	if (!f_a1c)
	{
		f_a1c = static_cast<C3DGoddard *>(New_C3DGoddard());	// 0x41c810 (0x9f4 bytes)
		// (debug log of the allocation)
	}
	else f_a1c->v92(0, 0, 0);								// +0x174 (CGameObject part)
	if (!f_a30)
	{
		f_a30 = static_cast<C3DJeep *>(New_C3DJeep());		// 0x4211a0 (0x6ac bytes)
		if (f_a30) f_a30->ev141(73.0f, -30.0f, 3.0f, 3.0f, 1.0f, 1, 1);	// element +0x168 "SetupCar4"
	}
	if (f_a30)
	{
		f_a30->HideAgain();									// C3DVehicle slot 54 (+0xd8, 0x464f00): hide with the wheels (not C3DObject's)
		GO(f_a30)->v52(0);									// +0xd0
		GO(f_a30)->v46(0);									// +0xb8
	}
	g_4eefc8 = -1.0f;
	g_4eefd0 = -1.0f;
	MenuShowHud(f_a18, false);								// +0x4b4

	// the gadgets: all usable again, then what the level allows
	ItemOff(0, 7, 0); ItemOff(0, 3, 0); ItemOff(0, 0, 0); ItemOff(0, 5, 0);
	ItemOff(0, 1, 0); ItemOff(0, 6, 0); ItemOff(0, 2, 0); ItemOff(0, 4, 0);
	const int id = g_gametype->m_object_id;					// +0x490
	if (id >= 'VR01' && id <= 'VR08')
	{
		f_9f8 = 1;
		ItemOff(0, 7, 1); ItemOff(0, 3, 1); ItemOff(0, 0, 1); ItemOff(0, 5, 1);
		ItemOff(0, 1, 1); ItemOff(0, 6, 1); ItemOff(0, 2, 1); ItemOff(0, 4, 1);
		g_4f8188 = 20.0f;
		MenuClearVR(f_a18);									// +0x4ec
	}
	switch (id)
	{
		case 'LEV7':
			DeactivateGadget();
			ActivateGadget(2);
			LevelTimer(12, 1);								// 0x406f50
			ItemOff(0, 7, 1); ItemOff(0, 3, 1); ItemOff(0, 5, 1); ItemOff(0, 4, 1); ItemOff(0, 6, 1); ItemOff(0, 1, 1);
			WriteTaskFile("RestartLevel.tsk");				// 0x45fee0
			break;
		case 'LEV8':
			DeactivateGadget();
			ActivateGadget(2);
			break;
		case 'LV3C':
			ItemOff(0, 4, 1);
			// fall through
		case 'LEV3': case 'LV3A': case 'LV3B': case 'LV3D':
			ItemOff(0, 7, 1); ItemOff(0, 3, 1); ItemOff(0, 0, 1); ItemOff(0, 1, 1); ItemOff(0, 5, 1);
			break;
		case 'LV4B':
			DeactivateGadget();
			ItemOff(0, 5, 1); ItemOff(0, 4, 1); ItemOff(0, 1, 1); ItemOff(0, 2, 1);
			ActivateGadget(7);
			g_4eefd0 = 900.0f;								// 0x42aeb0: the level's time limit
			MenuShowHud(f_a18, true);
			WriteTaskFile("RestartLevel.tsk");
			break;
		case 'LV1A': case 'LV1B': case 'LV1C': case 'LV2A': case 'LV4A':
			ItemOff(0, 1, 1); ItemOff(0, 7, 1);
			break;
		case 'LV5A':
			ItemOff(0, 7, 1); ItemOff(0, 0, 1); ItemOff(0, 5, 1); ItemOff(0, 6, 1); ItemOff(0, 4, 1);
			DeactivateGadget();
			WriteTaskFile("RestartLevel.tsk");
			g_4f6b3c = 0.02f;
			ActivateGadget(1);
			break;
		case 'LV5B':
			ItemOff(0, 7, 1); ItemOff(0, 0, 1); ItemOff(0, 5, 1); ItemOff(0, 6, 1); ItemOff(0, 4, 1);
			WriteTaskFile("RestartLevel.tsk");
			SetItemType(0, 3, 1);
			DeactivateGadget();
			break;
		case 'LEV6': case 'LV6A':
			DeactivateGadget();
			ActivateGadget(2);
			LevelTimer(42, 1);
			ItemOff(0, 7, 1); ItemOff(0, 3, 1); ItemOff(0, 5, 1); ItemOff(0, 4, 1); ItemOff(0, 6, 1); ItemOff(0, 1, 1);
			WriteTaskFile("RestartLevel.tsk");
			break;
		case 'VR01': DeactivateGadget(); ItemOff(0, 0, 0); break;
		case 'VR02': DeactivateGadget(); ItemOff(0, 1, 0); break;
		case 'VR03': DeactivateGadget(); ItemOff(0, 2, 0); ActivateGadget(2); break;
		case 'VR04': DeactivateGadget(); ItemOff(0, 3, 0); ActivateGadget(3); break;
		case 'VR05': ItemOff(0, 4, 0); DeactivateGadget(); break;
		case 'VR06': DeactivateGadget(); ItemOff(0, 5, 0); break;
		case 'VR07': DeactivateGadget(); ItemOff(0, 6, 0); break;
		case 'VR08': DeactivateGadget(); ItemOff(0, 7, 0); ActivateGadget(7); break;
	}
	if ((id >= 'LEV6' && id <= 'LEV7') || id == 'LV4B' || (id >= 'LV5A' && id <= 'LV5B') || id == 'LV6A')
		if (g_4f83d4 < 20.0f) g_4f83d4 = 20.0f;				// 0x495324: some energy to start with

	g_gametype->SetCamera(static_cast<CGameObject *>(this));	// +0x118
	v261();													// +0x410
	if (f_a1c)
	{
		OMedia3DVector p = v224(0, 0, -150.0f);
		f_a1c->v89(p.x, p.y, p.z, *(unsigned int *)&p.w);	// Goddard behind Jimmy (+0x168)
	}
	if (f_ab0)
	{
		// the rocket ship: shown with its rider, else hidden
		if (!f_ab0->f_9f4) Hide(f_ab0);						// BG +0x740
		else Show(f_ab0);
	}
	MenuHudPicture(f_a18, 0, false, 0, 0);					// +0x4b8 (0, 0, 0, 0)
}

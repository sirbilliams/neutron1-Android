/*  CJimmyGame - see CJimmyGame.h. Reconstructed from Neutron.exe 0x44d2b0..0x44ef20, with the
    texture-scroll helpers 0x45f4c0 / N2:0x4970e0 / 0x45f610 that only it uses.

    The original reaches C3DJimmy's members (music state +0x4c14.., surface flags +0x4ca6..) and
    many C3DJimmy / C3DPlayerObject slots at fixed offsets; here they go through the small helper
    functions below (a few are still TODOs), named after what the original does with them.    */

#include "SaveGame.h"
#include "Physics.h"
#include "CJimmyGame.h"
#include "JimmySlots.h"
#include "C3DObject.h"
#include "C3DJimmy.h"
// TODO(BG): #include "C3DRadar.h" (Negatron-only class, deleted)
#include "C3DRocketShip.h"
#include "C2DInGameMenu.h"
#include "CanvasWrapper.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "Sound.h"
#include "DatabaseCache.h"
#include "gen/globals.h"
#include "OMediaLayer.h"
#include "OMediaDataBase.h"
#include "OMedia3DShape.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include "OMedia3DShapeElement.h"
#include "OMediaCanvas.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include "OMediaStreamOperators.h"
#include "OMediaError.h"
#include <math.h>
#include <string.h>
#include <strings.h>

namespace neg {
std::string					g_queued_sfx;
}
using namespace neg;

// --------------------------------------------------------------------------- not reconstructed yet

// Sound: Sound.h (PlayGameSound 0x458b40, StopGameSound 0x458a00, StopChannel 0x47d7a0,
// StopAllChannels 0x47d850, UpdateSounds N2:0x4b8a30, PlayMusic 0x47d710)
static OMediaDataBase *LoadMusic(const char *omt_name)	{ return GetDatabase(omt_name); }	// 0x46a910
// 0x46aef0: close the cached database opened from `name` (object database cache 0x50996c)
static void CloseDatabaseByName(const char *name)	{ neg::CloseDatabase(name); }	// DatabaseCache.cpp

// Tasks (CTaskList, 0x4fc5fc): SaveGame.cpp
static void SetTaskValue(const char *task, int v)	{ neg::SetTaskState(task, v); }		// 0x45f990
static int  GetTaskValue(const char *task)		{ return neg::GetTaskState(task); }		// 0x45fea0
static void LoadTask(const char *name, int a)	{ neg::LoadTask(name, a != 0); }		// N2:0x498af0

// Granny (RAD): granny.dll import N2:0x4ca464 GrannyAdvanceTime, game/granny/granny_compat.h.
#include "granny_compat.h"


// The player's HUD object (C3DJimmy +0xd40, minus 200 to its element; NULL if none) and its
// hide (C3DRadar N2:0x468aa0).
static OMediaElement *PlayerHud(void)			{ return NULL; }	// TODO(BG): N2:0x483b9d C3DJimmy +0xd40 C3DRadar (Negatron only)
static void HudHide(OMediaElement *hud)			{ (void)hud; }	// TODO(BG): C3DRadar N2:0x468aa0

// g_camera's element slot 105 (+0x1a4): called once when the health has run out. Every 3D object's
// slot 105 is N2:0x465360 (C3DPlayerObject::ev105: back to the autosave + a comment), the other game
// objects' is 0x471830 (returns a member, nothing done). N2:0x465360 uses only globals, so for a camera
// that is not a C3DPlayerObject (the hover cycle, a cut-scene object) it runs on Jimmy.
static void CameraPlayerDead(CGameObject *camera)
{
	if (!dynamic_cast<C3DObject *>(camera)) return;			// 0x471830
	C3DPlayer *p = dynamic_cast<C3DPlayer *>(camera);	// port: N2 C3DPlayerObject
	if (!p) p = Jimmy();									// port: the same code (N2:0x465360) through Jimmy
	(void)p;												// TODO(BG): Negatron's C3DPlayerObject::ev105 (autosave); Neutron.exe's C3DPlayer has no such slot
}

// The player's level-music members ((g_player - 0xc0) +0x4c14 channel, +0x4c18 a value given to
// PlayMusic, +0x4c28 the music .omt name): C3DJimmy f_4c14 / f_4c18 / f_4c28.
struct PlayerMusicState
{
	int &channel; int &param; char *name;
	PlayerMusicState(int &c, int &p, char *n) : channel(c), param(p), name(n) {}	// C++98: no brace init
};
static PlayerMusicState PlayerMusic(void)
{
	C3DJimmy *j = g_player ? dynamic_cast<C3DJimmy *>(g_player) : NULL;
	static int param0 = 0;									// TODO(BG): Neutron.exe's Jimmy keeps no music parameter
	if (j) return PlayerMusicState(j->f_a38, param0, j->f_a4c);	// Neutron.exe: +0xa38 channel, +0xa4c name
	static int channel = -1, param = 0;						// port: the original reads through NULL
	static char name[100] = "none";
	return PlayerMusicState(channel, param, name);
}

// C3DJimmy members and slots TouchSurface uses (on the player's element; the original reads them at
// fixed offsets of whatever the element is, here only a C3DJimmy has them).
static C3DJimmy *AsJimmy(OMediaElement *p)					{ return p ? dynamic_cast<C3DJimmy *>(p) : NULL; }
static bool PlayerFlag(OMediaElement *p, int off)
{
	C3DJimmy *j = AsJimmy(p);
	if (!j) return false;
	switch (off)
	{
		case 0x4ca6: return j->f_1e01 != 0;									// in water (Neutron.exe +0x1e01)
		case 0x4ca7: return j->f_1e02 != 0;									// (+0x1e02)
		case 0x4cb8: return j->f_1e03 != 0;									// shocked (+0x1e03)
	}
	return false;
}
static void SetPlayerFlag(OMediaElement *p, int off, bool v)
{
	C3DJimmy *j = AsJimmy(p);
	if (!j) return;
	switch (off)
	{
		case 0x4ca6: j->f_1e01 = v; break;
		case 0x4ca7: j->f_1e02 = v; break;
		case 0x4db4: break;													// TODO(BG): Negatron-only flag
	}
}
static void SetPlayerFloat(OMediaElement *p, int off, float v)	{ (void)p; (void)off; (void)v; }	// TODO(BG): Negatron's +0xc08
static void PlayerSlot(OMediaElement *p, int slot)
{
	C3DJimmy *j = AsJimmy(p);
	if (!j) return;
	switch (slot)
	{
		case 120: j->JetpackFall(); break;										// +0x1e0 0x425120
		case 127: break;										// +0x1fc (empty)
		case 128: j->EnterWater(); break;										// +0x200 N2:0x44bf20 EnterWater
		case 129: j->LeaveWater(); break;										// +0x204 N2:0x44bfa0 LeaveWater
		case 130: j->Splash(); break;										// +0x208 N2:0x44bfe0
		case 131: break;										// +0x20c (empty)
	}
}
static void PlayerSlot119(OMediaElement *p, float a, float b, float c, float d)	// +0x1dc N2:0x44bc40
{
	if (C3DJimmy *j = AsJimmy(p)) j->TouchedShock(OMedia3DVector(a, b, c, d));
}
static void PlayerHurt(OMediaElement *p, float a, int b, float c, bool d)	// slot 134 (+0x218) N2:0x44bd90
{
	if (C3DJimmy *j = AsJimmy(p)) { (void)d; j->Shock(a, b, c); }
}
static void SetPlayerSurfaceSound(OMediaElement *p, int set)	{ if (C3DJimmy *j = AsJimmy(p)) j->SetStepSound(set); }	// +0x264 N2:0x44c080
// A '3ROC' C3DRocketShip (the original's ID test is 'COR3' in memory, 0x33524f43): its rider
// (+0x934, element 0x9f4) and its hit state (+0x1138 / +0x113c / +0x1178 / +0x1188).
static OMediaElement *CorCarried(CGameObject *o)
{
	C3DRocketShip *r = dynamic_cast<C3DRocketShip *>(o);
	return r ? static_cast<OMediaElement *>(r->f_9f4) : NULL;
}
static void CorStartHit(CGameObject *o)
{
	C3DRocketShip *r = dynamic_cast<C3DRocketShip *>(o);
	if (!r) return;
	r->f_1238 = 5.0f;
	r->f_11f8 = 1;
	r->f_1248 = 0;
	r->f_11fc = 0;
}

// --------------------------------------------------------------------------- small helpers

// 0x46a3e0: was the key pressed? (clears it)
static bool KeyPressed(int vk)
{
	if (g_keys[vk]) { g_keys[vk] = 0; return true; }
	return false;
}


// C2DInGameMenu N2:0x40ed30 (this unused): show or hide the sprite g_N2_5446ec (the pause picture).
static void ShowPauseSprite(bool show)
{
	if (!g_N2_5446ec) return;								// port: Boy Genius never makes it
	if (show) g_N2_5446ec->force_show();
	else g_N2_5446ec->hide(true);
}

// The random number of the original (0x19660d LCG, 0..1000) as a fraction: < 0.5 picks the first sound.
static bool RandomHalf(void)
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	return (double)((float)r * 0.001f) < 0.5;			// 0x48e5d8, 0x48e5d0
}

// 2002 layer flag 0x800 (fog on) is OMT 2.5's fog_enabled. (2.5's enable_fog() always sets it.)
struct LayerFogAccess : OMediaLayer { static bool OMediaLayer::*Enabled(void) { return &LayerFogAccess::fog_enabled; } };
static void SetLayerFog(OMediaLayer *l, bool on)	{ l->*LayerFogAccess::Enabled() = on; }

// A material's diffuse r, g, b; its alpha is kept (the original copies it onto itself).
static void SetDiffuseRGB(OMedia3DMaterial *m, float r, float g, float b)
{
	m->set_diffuse_r(r);
	m->set_diffuse_g(g);
	m->set_diffuse_b(b);
}

// The game's paths are DOS-style and relative.
static std::string PortPath(const char *dos)
{
	std::string s(dos);
	if (s.compare(0, 2, ".\\") == 0) s.erase(0, 2);
	for (size_t i = 0; i < s.size(); i++) if (s[i] == '\\') s[i] = '/';
	return s;
}

static bool FileExists(const char *dos_path)	// OMediaFilePath + OMediaFileStream::fileexists, as the original
{
	try
	{
		OMediaFilePath path(PortPath(dos_path));
		OMediaFileStream fs;
		fs.setpath(&path);
		bool e = fs.fileexists();
		fs.close();
		return e;
	}
	catch (OMediaError &) { return false; }
}

// name[i], lower case ("" past the end, as the original's string helpers give)
static int NameChar(const std::string &name, size_t i)
{
	return tolower(i < name.size() ? (unsigned char)name[i] : 0);
}

// --------------------------------------------------------------------------- texture scrolls (0x497xxx)

// The texture-scroll list 0x45f4c0 / N2:0x4970e0 / 0x45f610 is in SaveGame.cpp (neg::ClearTexScrolls,
// UpdateTexScrolls, AddTexScroll), with the other free functions of that address range.

// --------------------------------------------------------------------------- construction

CJimmyGame::CJimmyGame()													// 0x44d2b0
{
	// nothing of its own: the members are set by InitGame
}

CJimmyGame::~CJimmyGame()													// 0x44d390
{
}

// --------------------------------------------------------------------------- CLocalGameObject-family slots

void CJimmyGame::ResetObject(void)											// 0x44d4e0
{
	CLocalGameObject::ResetObject();
}

void CJimmyGame::Process(float dt)											// 0x44df80
{
	neg::UpdateTexScrolls(dt);												// 0x45f510
	CGameType::Process(dt);
	UpdateSounds();														// 0x47d8f0
	if (m_sky && m_sky_rotates) m_sky->AddRotation(0.0f, dt, 0.0f);		// sky +0xc0, vtable +0x334
	UpdateInput();															// 0x478530: the joystick poll
	if (g_jimmy) neg::JimmyElementSlot128(g_jimmy, dt);					// element +0x200
}

// N2:0x484640: per frame while the editor is editing (CGameObject::v4): no fog, grey clear colour,
// a far clip plane far away.
void CJimmyGame::v242(float t)
{
	CGameObject::v242(t);													// 0x4732a0 (empty)
	if (g_granny) GrannyAdvanceTime(g_granny, (double)t);
	SetLayerFog(g_layer_world, false);										// 0x476910(layer, 0x800)
	g_layer_clear->set_clear_color(OMediaFARGBColor(0.0f, 0.5f, 0.5f, 0.5f));	// 2002 +0x10c..0x118 = r, g, b, a
	g_layer_clear->set_fog_color(OMediaFARGBColor(0.0f, 0.5f, 0.5f, 0.5f));	// 2002 +0xc8..0xd4
	g_layer_world->set_far_clip(200000.0f);									// 2002 +0xc0
}

// 0x44df30 (BG slot 259): after a level load (CEditor ResetLevel).
void CJimmyGame::v260(void)
{
	// (debug log)
	m_pickup_index = 0;
}

void CJimmyGame::v277(void)													// N2:0x485e80
{
	if (m_stopwatch_channel != -1) StopGameSound(m_stopwatch_channel, false);			// (the channel is kept)
}

void CJimmyGame::v278(void)													// N2:0x485ea0
{
	if (m_stopwatch_channel != -1) m_stopwatch_channel = PlayGameSound(-1, "stopwatch", 1);
}

// N2:0x486170. A missing player (g_player NULL) makes the original write through address 0x4c18.
void CJimmyGame::WriteState(OMediaStreamOperators &s)
{
	int level_items = (int)g_N2_545efc, f00 = g_N2_545f00, f04 = g_N2_545f04, f08 = g_N2_545f08;
	CLocalGameObject::WriteState(s);
	s << m_fog_start << m_fog_end << m_far_clip;
	s << m_fog_restore[0] << m_fog_restore[1] << m_fog_restore[2];
	s << m_fog_color[0] << m_fog_color[1] << m_fog_color[2];				// (alpha is not saved)
	PlayerMusicState pm = PlayerMusic();
	s << pm.param;
	s << pm.name;
	for (int i = 0; i < 10; i++) s << g_power_timers[i];
	s << g_difficulty;
	s << level_items << f00 << f04 << f08;
}

void CJimmyGame::ReadState(OMediaStreamOperators &s)						// N2:0x485ef0
{
	int level_items = 0, f00 = 0, f04 = 0, f08 = 0;
	CLocalGameObject::ReadState(s);
	s >> m_fog_start >> m_fog_end >> m_far_clip;
	s >> m_fog_restore[0] >> m_fog_restore[1] >> m_fog_restore[2];
	s >> m_fog_color[0] >> m_fog_color[1] >> m_fog_color[2];
	SetFog(m_fog_color[0], m_fog_color[1], m_fog_color[2], m_fog_color[3],
		   m_fog_restore[0], m_fog_restore[1], m_fog_restore[2], true);		// element +0x194
	PlayerMusicState pm = PlayerMusic();
	s >> pm.param;
	s >> pm.name;
	if (strcasecmp(pm.name, "none") != 0) PlayLevelMusic(pm.name, pm.param);	// element +0x198
	for (int i = 0; i < 10; i++) s >> g_power_timers[i];
	s >> g_difficulty;
	if (g_power_timers[6] <= 0.0f)
	{
		StopwatchEnd();														// element +0x1ac
		g_enemy_time_scale = 1.0f;
	}
	else g_enemy_time_scale = 0.25f;
	RestoreTimerEffects();													// element +0x1b0
	s >> level_items >> f00 >> f04 >> f08;
	if (g_menu)
	{
		g_N2_545f04 = f04;
		g_N2_545f00 = f00;
		g_N2_545f08 = f08;
		SetLevelItems(level_items);											// N2:0x40a860
	}
}

// --------------------------------------------------------------------------- element-family overrides

void CJimmyGame::InitGame(void)												// 0x44d3d0
{
	Log("InitGame()");
	f_564 = 0;																// +0x550
	m_reactor_touched = 0;												// port: BG's InitGame does not touch 0x5bc (the allocator zeroes it?); read only by the level 4 classes
	CGameType::InitGame();
	neg::ClearTexScrolls();													// 0x45f4c0
	g_gravity.x = 0.0f;														// 0x5cfc00 (Physics.cpp)
	g_gravity.y = -800.0f;
	g_gravity.z = 0.0f;
	f_554 = 1.0f;															// +0x540
	f_558 = 1.0f;															// +0x544
	g_layer_world->add_flags(omlayerf_SortElementBack2Front);				// 0x476930(layer 0x509a3c, 0x20)
	v107(0);																// vtable +0x1b0
	m_surface_sound_next = 5;												// +0x54c
	m_surface_sound = 5;													// +0x548
	m_pickup_index_base = 100;												// +0x5a8
	v128(0);																// vtable +0x204
	m_sky = NULL;															// +0x5b0
	m_sky_rotates = 1;														// +0x5b4
	m_stopwatch_channel = -1;												// port: Negatron's stopwatch member (no such sound in Boy Genius)
	// (debug log)
}

void CJimmyGame::UnInitGame(void)											// 0x44d4a0
{
	Log("UnInitGame()");
	CloseDatabaseByName("SoundEffects.omt");								// 0x46aef0
	g_active_sound_db = NULL;
	CGameType::UnInitGame();
}

// 0x44e7b0: the path is first resolved by FileIsThere (omt\<path>, else c:\omt\<path>).
void CJimmyGame::InitTerrainDatabase(std::string path)
{
	char buf[256];
	strcpy(buf, path.c_str());
	// "Gona call Fileisthere" (debug log)
	FileIsThere(buf, "omt", "omt");											// element +0x164
	// "called Fileisthere"
	CGameType::InitTerrainDatabase(std::string(buf));						// (the same code, inline in the original)
}

// The three terrain builders (0x44d4f0, 0x44d810, 0x44dbf0) differ in small ways; they are one
// function here with the differences spelled out.
enum TerrainMode { TERRAIN_LIT, TERRAIN_UNLIT, TERRAIN_TINTED };

static void BuildTerrain(CJimmyGame *game, TerrainMode mode, float r, float g, float b)
{
	if (!g_terrain_db) return;
	for (short i = 0; ShapeExists(g_terrain_db, 'E3ds', i); i++)
	{
		OMedia3DShape *shape = Load3DShape(g_terrain_db, i);
		std::string name;
		shape->get_chunk_name(name);
		if (name.find("3d") == 0) continue;								// 0x4f1ae8 "3d"
		g_5099ac = 0;
		CGameObject *o = CreateObject('3TER');
		C3DObject *t = o ? dynamic_cast<C3DObject *>(o) : NULL;			// (a C3DTerrain)
		t->InitObject();													// (no NULL test in the original)
		t->set_shape(shape);												// element +0xac
		const bool nc = NameChar(name, 0) == 'n' && NameChar(name, 1) == 'c';	// "nc...": no collision
		if (mode == TERRAIN_LIT)
		{
			if (!nc)
			{
				t->InitPhysics();											// element +0x2c
				MakeShapeUnLit(shape);										// 0x477550
			}
		}
		else
		{
			if (!nc) t->InitPhysics();
			MakeShapeUnLit(shape);
		}
		if (!g_windowed) t->ev51();											// element +0xcc (0x509a13)
		else if (!g_5099ac) MakeShapeUnLit(shape);
		omt_PolygonList *polys = shape->get_polygons();
		for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); p++)
		{
			game->SetupTextureScroll(&*p);									// element +0x160
			OMedia3DMaterial *m = (*p).get_material();
			if (!m) continue;
			if (mode == TERRAIN_TINTED) SetDiffuseRGB(m, r, g, b);			// (+0x4c..+0x54; alpha kept)
			if (nc) m->set_flags(m->get_flags() | ommatf_SecondPass);		// "nc...": drawn in the second pass
		}
	}
	game->SetFogColor();													// element +0xb0
}

// Quirk: unlike CGameType's, these do not set g_terrain.
void CJimmyGame::CreateTerrain(void)		{ BuildTerrain(this, TERRAIN_LIT, 0.0f, 0.0f, 0.0f); }		// 0x44d4f0
void CJimmyGame::CreateTerrainUnLit(void)	{ BuildTerrain(this, TERRAIN_UNLIT, 0.0f, 0.0f, 0.0f); }	// 0x44d810

void CJimmyGame::CreateTerrainTinted(float r, float g, float b, float a)	// 0x44dbf0 (a unused)
{
	(void)a;
	BuildTerrain(this, TERRAIN_TINTED, r, g, b);
}


void CJimmyGame::OnActivate(void)											// N2:0x482a80
{
	Log("GetFocus()");
	// "Gettting focus here!!!" (debug log)
	g_has_focus = 1;
	m_refog = 1;
	if (g_game_mode == 6) ShowPauseSprite(false);
}

// N2:0x482ad0. Quirk: the world layer's near and far clip go into the first two values SetFog is
// given again on the next focus (fog start / end), not the fog's own range.
void CJimmyGame::OnDeactivate(void)
{
	Log("LoseFocus()");
	g_has_focus = 0;
	ShowPauseSprite(false);
	// "Losing focus here!!!" (debug log)
	m_fog_restore[1] = g_layer_world->get_far_clip();						// 2002 +0xc0
	m_fog_restore[0] = g_layer_world->get_near_clip();						// 2002 +0xc4
}

// 0x44e030 (BG slot 76): an object stands on a polygon with material `mat` (texture `tex`). The
// material's chunk ID picks the effect. Only Jimmy ('3JIM') and the rocket ship ('3ROC') react:
// 100..109 and 110..119 are two kinds of surface Jimmy is told about (entering, each frame,
// leaving), 120..129 push him (element slot 88), 185..189 hurt him (or the rocket's pilot), and
// the ID picks the footstep sound set he is given.
void CJimmyGame::TouchSurface(CGameObject *o, OMedia3DMaterial *mat, OMediaCanvas *tex, float dt)
{
	(void)dt;
	if (!o || !tex) return;
	const int id = (int)mat->get_chunk_ID();								// (material +0xc -> +8; no NULL test of mat)
	if (o->m_object_id == '3JIM')
	{
		if (id >= 110 && id <= 119)
		{
			if (!JimmyByte(o, 0x1e02)) { JimmyElementSlot102(o); JimmyByte(o, 0x1e02) = 1; }	// element +0x198
			JimmyElementSlot89(o);											// +0x164
		}
		else if (JimmyByte(o, 0x1e02)) { JimmyElementSlot103(o); JimmyByte(o, 0x1e02) = 0; }	// +0x19c
		if (id > 184 && id < 190 && o->v133() && !JimmyByte(o, 0x1e03))		// vtable +0x218: on the floor
		{
			JimmyElementSlot104(o, -5.0f, -5, 5.0f);						// +0x1a0
			PlayGameSoundIndex(-1, RandomHalf() ? 0xbf : 0xbd, false);		// 0x458980
		}
		if (id >= 100 && id <= 109)
		{
			if (!JimmyByte(o, 0x1e01)) { JimmyElementSlot100(o); JimmyByte(o, 0x1e01) = 1; }	// +0x190
			JimmyElementSlot99(o);											// +0x18c
		}
		else if (JimmyByte(o, 0x1e01)) { JimmyElementSlot101(o); JimmyByte(o, 0x1e01) = 0; }	// +0x194
		if (id > 119 && id < 130 && o->v133()) JimmyElementSlot88(o, 0.0f, 1.0f, 0.0f, 0.0f);	// +0x160

		int set;
		if (id >= 185 && id <= 189) set = 0x23;
		else if (id >= 110 && id <= 119) set = 0x23;
		else if (id >= 50 && id <= 55) set = 0x7c;
		else if (id >= 56 && id <= 59) set = 0x7e;
		else if (id >= 130 && id <= 135) set = 0x7b;
		else if (id >= 136 && id <= 139) set = 0x80;
		else if (id >= 140 && id <= 149) set = 0x80;
		else if (id >= 150 && id <= 159) set = 0x7f;
		else if (id >= 70 && id <= 79) set = 0x81;
		else if (id == 90) set = 0x7d;
		else set = 5;
		m_surface_sound_next = set;
		if (m_surface_sound != m_surface_sound_next) JimmyElementSlot121(o, m_surface_sound_next);	// +0x1e4
		m_surface_sound = m_surface_sound_next;
		return;
	}
	if (o->m_object_id != '3ROC') return;
	if (id < 185 || id > 189 || !o->v133()) return;
	CGameObject *pilot = CorCarried(o) ? dynamic_cast<CGameObject *>(CorCarried(o)) : NULL;	// rocket +0x740
	if (!pilot || JimmyByte(pilot, 0x1e03)) return;
	JimmyElementSlot104(pilot, -5.0f, -5, 5.0f);							// its element +0x1a0
	CorStartHit(o);															// +0xf84 = 5, +0xf44 = 1, +0xf90 = 0, +0xf48 = 0
	PlayGameSoundIndex(-1, RandomHalf() ? 0xbf : 0xbd, false);
}

// --------------------------------------------------------------------------- CJimmyGame's own slots

int CJimmyGame::NextPickupIndex(void)										// 0x44dff0
{
	m_pickup_index++;
	if (m_pickup_index > 100)
		ErrorLog("ERROR: PickupIndexCounter > 100  %d + %d", m_pickup_index_base, m_pickup_index);
	return m_pickup_index_base + m_pickup_index;
}

// 0x44e560: `name` becomes "<dir>\<name>" when that file exists, else "c:\<cd_dir>\<name>".
bool CJimmyGame::FileIsThere(char *name, const char *dir, const char *cd_dir)
{
	// "Inside fileisthere" (debug log)
	char local[256];
	strcpy(local, dir);
	strcat(local, "\\");													// 0x4f1b18
	strcat(local, name);
	if (!FileExists(local))
	{
		char cd[256];
		strcpy(cd, "c:\\");													// 0x4f3ed4
		strcat(cd, cd_dir);
		strcat(cd, "\\");
		strcat(cd, name);
		strcpy(name, cd);
	}
	else strcpy(name, local);
	return true;
}

// 0x44e450 (BG slot 90): stop (or let go on) the game: GameIsRunning() is false while set.
void CJimmyGame::StopGame(char stop)
{
	g_5099ab = stop;
	g_4f641c = 0;
}

// 0x44df50: the stage of the level's story from task "SCENE": 0 below 250, 1 below 320, else 2.
char CJimmyGame::GetSceneStage(void)
{
	const int v = GetTaskValue("SCENE");
	if (v < 250) return 0;
	return (char)((319 < v) + 1);
}

// 0x44e470 (BG slot 88): textures 800..869 scroll, ten at a time in one of seven directions.
bool CJimmyGame::SetupTextureScroll(OMedia3DPolygon *p)						// 0x44e470
{
	OMedia3DMaterial *m = p->get_material();
	if (!m || !m->get_texture()) return false;
	const int id = (int)m->get_texture()->get_chunk_ID();
	if (id < 800 || id > 869) return false;
	if (id < 810) neg::AddTexScroll(p, 0.0f, 0.2f);						// 0x45f610
	else if (id < 820) neg::AddTexScroll(p, 0.2f, 0.2f);
	else if (id < 830) neg::AddTexScroll(p, 0.2f, -0.2f);
	else if (id < 840) neg::AddTexScroll(p, 0.0f, -0.2f);
	else if (id < 850) neg::AddTexScroll(p, -0.2f, -0.2f);
	else if (id < 860) neg::AddTexScroll(p, -0.2f, 0.0f);
	else neg::AddTexScroll(p, -0.2f, 0.2f);
	return true;
}

// 0x44db30: diffuse r, g, b of every material of the terrain database's shapes.
void CJimmyGame::SetTerrainDBColor(float r, float g, float b, float a)
{
	(void)a;
	if (!g_terrain_db) return;
	for (short i = 0; ShapeExists(g_terrain_db, 'E3ds', i); i++)
	{
		OMedia3DShape *shape = Load3DShape(g_terrain_db, i);
		omt_PolygonList *polys = shape->get_polygons();
		for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); p++)
			if ((*p).get_material()) SetDiffuseRGB((*p).get_material(), r, g, b);
	}
}

// N2:0x4835b0 / N2:0x4836a0: diffuse r, g, b of the shapes of every '3TER' object.
static void TintTerrainObjects(float r, float g, float b)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		CGameObject *o = *i;
		if (!o || o->m_object_id != '3TER') continue;
		OMedia3DShapeElement *e = dynamic_cast<OMedia3DShapeElement *>(o);	// o - 0xc0
		omt_PolygonList *polys = e->get_shape()->get_polygons();			// element +0xb0 (no NULL test)
		for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); p++)
			if ((*p).get_material()) SetDiffuseRGB((*p).get_material(), r, g, b);
	}
}

void CJimmyGame::SetTerrainColor(float r, float g, float b, float a)		// N2:0x4835b0 (a unused)
{
	(void)a;
	TintTerrainObjects(r, g, b);
}

void CJimmyGame::SetTerrainGrey(float v)									// N2:0x4836a0
{
	TintTerrainObjects(v, v, v);
}

// N2:0x483c50: count the power-up timers down; when one runs out its effect ends (RefreshObjects).
// Quirk: timer [1] (N2:0x54a428) is not counted down.
void CJimmyGame::UpdateTimers(float dt)
{
	if (IsPaused()) return;													// element +0x114
	float *t = g_power_timers;
	if (t[3] > 0.0f && (t[3] -= dt) <= 0.0f)
	{
		t[3] = 0.0f;
		HudHide(g_player_hud);												// N2:0x468aa0
		RefreshObjects();
	}
	if (t[0] > 0.0f && (t[0] -= dt) <= 0.0f) { t[0] = 0.0f; RefreshObjects(); }
	if (t[2] > 0.0f && (t[2] -= dt) <= 0.0f) { t[2] = 0.0f; RefreshObjects(); }
	if (t[4] > 0.0f)
	{
		// x-ray: the terrain flashes green / yellow while m_xray_time runs, then goes back to grey-green
		if (m_xray_time > 0.0f)
		{
			m_xray_time -= dt;
			if (m_xray_flash > 0.0f)
			{
				m_xray_flash -= dt;
				if (m_xray_flash < 0.0f)
				{
					SetTerrainColor(0.1f, 1.0f, 0.1f, 1.0f);				// element +0x180
					m_xray_flash = -0.2f;
				}
			}
			else
			{
				m_xray_flash += dt;
				if (m_xray_flash > 0.0f)
				{
					SetTerrainColor(1.0f, 1.0f, 0.1f, 1.0f);
					m_xray_flash = 0.2f;
				}
			}
			if (m_xray_time <= 0.0f)
			{
				m_xray_time = 0.0f;
				SetTerrainColor(0.4f, 1.0f, 0.4f, 1.0f);
			}
		}
		if ((t[4] -= dt) <= 0.0f)
		{
			t[4] = 0.0f;
			SetTerrainColor(1.0f, 1.0f, 1.0f, 1.0f);
			RefreshObjects();
		}
	}
	if (t[5] > 0.0f && (t[5] -= dt) <= 0.0f) { t[5] = 0.0f; RefreshObjects(); }
	if (t[7] > 0.0f && (t[7] -= dt) <= 0.0f) { t[7] = 0.0f; RefreshObjects(); }
	if (t[8] > 0.0f && (t[8] -= dt) <= 0.0f) { t[8] = 0.0f; RefreshObjects(); }
	if (t[9] > 0.0f && (t[9] -= dt) <= 0.0f) { t[9] = 0.0f; RefreshObjects(); }
	// stopwatch: ticks while it runs
	if (t[6] > 0.0f)
	{
		t[6] -= dt;
		if (m_stopwatch_channel == -1) m_stopwatch_channel = PlayGameSound(-1, "stopwatch", 1);
		if (t[6] <= 0.0f)
		{
			StopwatchEnd();													// element +0x1ac
			return;
		}
	}
	else if (m_stopwatch_channel != -1)
	{
		StopGameSound(m_stopwatch_channel, false);
		m_stopwatch_channel = -1;
	}
}

// N2:0x485570: promote when the rank has gone up (task "PROMOTION" = 10 x rank, score box moves).
bool CJimmyGame::CheckPromotion(void)
{
	const int rank = GetRank();												// element +0x18c
	if (rank <= g_promotion_rank) return false;
	static const int box_y[10] = { 0, 0x182, 0x151, 0x122, 0xf1, 0xc1, 0x91, 0x61, 0x30, 1 };
	int value;
	switch (rank)
	{
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
			SetTaskValue("PROMOTION", rank * 10);							// 0x45f990
			g_promotion_rank = rank;
			g_score_box_y = box_y[rank];
			return true;
		case 10:
		case 11:
			g_score_box2_on = 1;
			value = 100;
			break;
		default:
			value = 120;
			break;
	}
	SetTaskValue("PROMOTION", value);
	g_promotion_rank = rank;
	return true;
}

int CJimmyGame::GetRank(void)												// N2:0x4854e0
{
	if (g_score < 3000) return 0;
	if (g_score < 6000) return 1;
	if (g_score < 9000) return 2;
	if (g_score < 12000) return 3;
	if (g_score < 15000) return 4;
	if (g_score < 18000) return 5;
	if (g_score < 21000) return 6;
	if (g_score < 25000) return 7;
	if (g_score < 30000) return 8;
	return 34999 < g_score ? 10 : 9;
}

void CJimmyGame::RefreshObjects(void)										// N2:0x483c20: slot 271 of every object
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		if (*i) static_cast<CLocalGameObject *>(*i)->v271();				// vtable +0x43c
}

// N2:0x484520: fog colour and range, the clear colour (both layers), the far clip plane.
void CJimmyGame::SetFog(float r, float g, float b, float a, float start, float end, float far_clip, bool on)
{
	m_fog_start = start;
	m_fog_end = end;
	m_far_clip = far_clip;
	m_fog_restore[0] = start;
	m_fog_restore[1] = end;
	m_fog_restore[2] = far_clip;
	const OMediaFARGBColor c(a, r, g, b);
	g_layer_clear->set_fog_color(c);										// 2002 +0xc8
	g_layer_clear->set_clear_color(c);										// 2002 +0x10c
	g_layer_world->set_far_clip(m_far_clip);								// 2002 +0xc0
	g_layer_world->set_clear_color(c);
	g_layer_world->set_fog_color(c);
	g_layer_world->set_fog_range(m_fog_start, m_fog_end);					// 2002 +0xd8 / +0xdc
	SetLayerFog(g_layer_world, true);										// 2002 flags |= 0x800
	m_fog_color[0] = r;
	m_fog_color[1] = g;
	m_fog_color[2] = b;
	m_fog_color[3] = a;
	if (!on) SetLayerFog(g_layer_world, false);								// 0x476910(layer, 0x800)
}

// N2:0x485d00: stop the level music, then load and play `name` ("none": no music).
void CJimmyGame::PlayLevelMusic(const char *name, int a)
{
	PlayerMusicState pm = PlayerMusic();									// (g_player - 0xc0)->+0x4c14..
	if (g_level_music)
	{
		StopChannel(pm.channel, 0);											// 0x47d7a0
		if (strcasecmp(pm.name, "none") != 0)
		{
			CloseDatabaseByName(pm.name);									// 0x46aef0
			pm.channel = -1;
		}
		g_level_music = NULL;
	}
	if (strcasecmp(name, "none") != 0)
	{
		if (pm.name != name) strcpy(pm.name, name);						// (name may be Jimmy's own +0x4c28)
		g_level_music = LoadMusic(name);									// 0x46a910
		if (g_level_music && pm.channel == -1 && a != -1)
		{
			pm.param = a;
			pm.channel = PlayMusic(g_level_music, -1, a, true);				// 0x47d710
		}
	}
}

// N2:0x485b80. Quirk: the channel is forgotten even when the music database stays open.
void CJimmyGame::StopLevelMusic(void)
{
	if (!g_level_music) return;
	PlayerMusicState pm = PlayerMusic();
	StopChannel(pm.channel, 0);
	pm.channel = -1;
	if (strcasecmp(pm.name, "none") != 0)
	{
		CloseDatabaseByName(pm.name);
		strcpy(pm.name, "none");
	}
	g_level_music = NULL;
}

// N2:0x485c60: as StopLevelMusic, but the name stays (so ReadState / PlayLevelMusic can restart it).
void CJimmyGame::StopLevelMusicKeepName(void)
{
	if (!g_level_music) return;
	PlayerMusicState pm = PlayerMusic();
	StopChannel(pm.channel, 0);
	pm.channel = -1;
	if (strcasecmp(pm.name, "none") != 0) CloseDatabaseByName(pm.name);
	g_level_music = NULL;
}

void CJimmyGame::StartGadgetEffect(const char *gadget)						// N2:0x483570
{
	if (strcasecmp("XRAY", gadget) == 0)									// N2:0x539608
	{
		m_xray_time = 3.0f;
		m_xray_flash = 0.2f;
	}
}

// N2:0x485b40: a running average of the frame rate (2% per frame). The x87 keeps the sum in a
// register; double does the same here.
float CJimmyGame::SmoothFrameRate(float dt)
{
	g_frame_rate_now = 1.0f / dt;
	const double sum = (double)g_frame_rate_acc + g_frame_rate_now;
	g_frame_rate = (float)(sum * 0.02f);									// 0x4d69f4
	g_frame_rate_acc = (float)(sum - g_frame_rate);
	return g_frame_rate;
}

// N2:0x4841f0: the stopwatch has run out: everything runs at normal speed again.
void CJimmyGame::StopwatchEnd(void)
{
	if (m_stopwatch_channel != -1)
	{
		StopGameSound(m_stopwatch_channel, false);
		m_stopwatch_channel = -1;
	}
	g_power_timers[6] = 0.0f;
	RefreshObjects();
	g_enemy_time_scale = 1.0f;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		if (*i) (*i)->f_430 = 1.0f;											// each object's time scale
}

// N2:0x484110: after a savegame was read, end the effects whose timers are not running.
void CJimmyGame::RestoreTimerEffects(void)
{
	if (g_power_timers[3] <= 0.0f)
	{
		g_power_timers[3] = 0.0f;
		HudHide(g_player_hud);
	}
	if (g_power_timers[4] <= 0.0f)
	{
		g_power_timers[4] = 0.0f;
		SetTerrainColor(1.0f, 1.0f, 1.0f, 1.0f);							// element +0x180
	}
	if (g_power_timers[6] <= 0.0f && m_stopwatch_channel != -1)
	{
		StopGameSound(m_stopwatch_channel, false);
		m_stopwatch_channel = -1;
	}
	RefreshObjects();														// element +0x190
}

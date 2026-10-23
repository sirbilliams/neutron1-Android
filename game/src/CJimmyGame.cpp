/*  CJimmyGame - see CJimmyGame.h. Reconstructed from Neutron.exe 0x44d2b0..0x44ef20, with the
    texture-scroll helpers 0x45f4c0 / 0x45f510 / 0x45f610 that only it uses.

    The original reaches C3DJimmy's surface flags (+0x1e01..) and element slots at fixed offsets;
    here they go through JimmySlots.h and the small helpers below.                            */

#include "SaveGame.h"
#include "Physics.h"
#include "CJimmyGame.h"
#include "JimmySlots.h"
#include "C3DObject.h"
#include "C3DJimmy.h"
#include "C3DRocketShip.h"
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
#include "OMediaError.h"
#include <math.h>
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- outside helpers

// Sound: Sound.h (UpdateSounds 0x47d8f0, PlayGameSoundIndex 0x458980)
// 0x46aef0: close the cached database opened from `name` (object database cache 0x50996c)
static void CloseDatabaseByName(const char *name)	{ neg::CloseDatabase(name); }	// DatabaseCache.cpp

// Tasks (CTaskList, 0x4fc5fc): SaveGame.cpp
static int  GetTaskValue(const char *task)		{ return neg::GetTaskState(task); }		// 0x45fea0

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

// The random number of the original (0x19660d LCG, 0..1000) as a fraction: < 0.5 picks the first sound.
static bool RandomHalf(void)
{
	*g_random_seed = *g_random_seed * 0x19660d + 0x7fff;
	const int r = (int)(((*g_random_seed >> 16) * 1001) >> 16) & 0xffff;
	return (double)((float)r * 0.001f) < 0.5;			// 0x48e5d8, 0x48e5d0
}

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


// The texture-scroll list 0x45f4c0 / 0x45f510 / 0x45f610 is in SaveGame.cpp (neg::ClearTexScrolls,
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
	if (m_sky && m_sky_rotates) m_sky->AddRotation(0.0f, dt, 0.0f);		// +0x524 / +0x528 (-0x8c): sky +0xc0, vtable +0x334
	UpdateInput();															// 0x478530: the joystick poll
	if (g_jimmy) neg::JimmyElementSlot128(g_jimmy, dt);					// element +0x200
}

// 0x44df30: after a level load (CEditor ResetLevel).
void CJimmyGame::v260(void)
{
	// (debug log)
	m_pickup_index = 0;
}

// --------------------------------------------------------------------------- element-family overrides

void CJimmyGame::InitGame(void)												// 0x44d3d0
{
	Log("InitGame()");
	f_550 = 0;																// +0x550
	CGameType::InitGame();
	neg::ClearTexScrolls();													// 0x45f4c0
	g_gravity.x = 0.0f;														// 0x5cfc00 (Physics.cpp)
	g_gravity.y = -800.0f;
	g_gravity.z = 0.0f;
	f_540 = 1.0f;															// +0x540
	f_544 = 1.0f;															// +0x544
	g_layer_world->add_flags(omlayerf_SortElementBack2Front);				// 0x476930(layer 0x509a3c, 0x20)
	v107(0);																// vtable +0x1b0
	m_surface_sound_next = 5;												// +0x54c
	m_surface_sound = 5;													// +0x548
	m_pickup_index_base = 100;												// +0x5a8
	v128(0);																// vtable +0x204
	m_sky = NULL;															// +0x5b0
	m_sky_rotates = 1;														// +0x5b4
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


// 0x44e030: an object stands on a polygon with material `mat` (texture `tex`). The
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

// 0x44e450: stop (or let go on) the game: GameIsRunning() is false while set.
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

// 0x44e470: textures 800..869 scroll, ten at a time in one of seven directions.
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

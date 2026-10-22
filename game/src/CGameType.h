/*  CGameType - the base of every level's game type: CDefaultGame (main menu), CJimmyGame and the
    CLevelXXGame classes. It owns the level-wide databases (effects, objects, songs, sounds, terrain,
    vehicles: one .omt file each), the fog colour, pause and help state, the active camera and the
    per-player input devices.
    Reconstructed from Neutron.exe: constructor 0x4745a0, destructor 0x4746e0 ("~CGameType()").

    The object is both an OMT element and a game object (classes.json):
        +0x000  OMediaElement           element vtable 0x4d6890 (87 slots)
        +0x030  OMediaListener          vtable 0x4d687c
        +0x08c  CLocalGameObject        vtable 0x4d642c (286 slots)
        +0x558  OMediaClassStreamer     shared virtual base, vtable 0x4d641c
    It has no members of its own (0x554..0x558 is never used). The constructor links it into
    g_world and makes it the running game type (neg::g_gametype); the CGameObject constructor puts
    it in g_objects like every game object.

    Per frame: OMT calls update_logic on it (it is linked into the world), which only forwards to
    OMediaElement. The game's own update comes from CViewPort::render_viewport (0x47e4f0), which
    calls v4(dt) on every object in g_objects (0x473740) -> Process(dt) below (CGameObject 0x470280).

    Element-family slots 43..86 are the game-type interface the rest of the game calls through
    neg::g_gametype (vtable +0xac..+0x158). Unnamed ones keep ev<N>.                           */
#pragma once
#include "OMediaElement.h"
#include "CLocalGameObject.h"
#include <string>

class OMediaDataBase;
class OMediaFileStream;
class OMediaElement;
class OMedia3DShape;
class OMedia3DMaterial;
class OMediaCanvas;

class CGameType : public OMediaElement, public CLocalGameObject
{
	public:
	CGameType();															// 0x4745a0
	virtual ~CGameType();													// 0x4746e0 (deleting: 0x475d80 / 0x475d90 / 0x4746b0)

	// ---- OMT element slots
	virtual void update_logic(float millisecs_elapsed);						// ESLOT 15  0x4747a0

	// ---- CLocalGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4749a0
	virtual void UnInitObject(void);										// SLOT 8   0x4749e0
	virtual void UnInitDatabases(void);										// SLOT 13  0x474a80
	virtual void Process(float dt);											// SLOT 240 0x475a70

	// ---- element-family game slots (vtable offset = slot * 4)
	virtual void SetID(int id);												// ESLOT 43 0x474790  +0xac
	virtual void SetFogColor(void);											// ESLOT 44 0x4747b0  +0xb0
	virtual void InitGame(void);											// ESLOT 45 0x474a10  +0xb4
	virtual void UnInitGame(void);											// ESLOT 46 0x474a50  +0xb8
	virtual void StartSong(void);											// ESLOT 47 0x474af0  +0xbc
	virtual void ev48(void) {}												// ESLOT 48 0x472970 (empty)
	virtual void InitEffectsDatabase(std::string path);						// ESLOT 49 0x474b60  +0xc4
	virtual void UnInitEffectsDatabase(void);								// ESLOT 50 0x474cc0  +0xc8
	virtual void InitObjectDatabase(std::string path);						// ESLOT 51 0x474d00  +0xcc
	virtual void UnInitObjectDatabase(void);								// ESLOT 52 0x474e60  +0xd0
	virtual void InitSongDatabase(std::string path);						// ESLOT 53 0x474ea0  +0xd4
	virtual void UnInitSongDatabase(void);									// ESLOT 54 0x475000  +0xd8
	virtual void InitSoundDatabase(std::string path);						// ESLOT 55 0x475040  +0xdc
	virtual void UnInitSoundDatabase(void);									// ESLOT 56 0x4751c0  +0xe0
	virtual void InitTerrainDatabase(std::string path);						// ESLOT 57 0x475210  +0xe4
	virtual void UnInitTerrainDatabase(void);								// ESLOT 58 0x475370  +0xe8
	virtual void InitVehicleDatabase(std::string path);						// ESLOT 59 0x4753b0  +0xec
	virtual void UnInitVehicleDatabase(void);								// ESLOT 60 0x475510  +0xf0
	virtual void CreateTerrain(void);										// ESLOT 61 0x475550  +0xf4
	virtual void CreateTerrainUnLit(void);									// ESLOT 62 0x4757a0  +0xf8
	virtual void SetTerrainFlatShading(void);								// ESLOT 63 0x475b10  +0xfc
	virtual void SetTerrainGouraudShading(void);							// ESLOT 64 0x475b60  +0x100
	virtual void SetTerrainLit(void);										// ESLOT 65 0x475bb0  +0x104
	virtual void SetTerrainUnLit(void);										// ESLOT 66 0x475c50  +0x108
	virtual void SetTerrainVertexLit(void);									// ESLOT 67 0x475c00  +0x10c
	virtual void PauseGame(bool pause);										// ESLOT 68 0x475a00  +0x110 (only logs; port: + Negatron's flag)
	virtual bool IsPaused(void);											// port: Negatron's ESLOT 69 N2:0x4b0a40 (not in Neutron.exe)
	virtual void ev70(void);												// ESLOT 70 0x475a30 (BG slot 69, +0x114) g_5099a9 = 1
	virtual void OnActivate(void) {}										// ESLOT 71 0x40db10  +0x11c (empty) omsg_Activate
	virtual void OnDeactivate(void) {}										// ESLOT 72 0x40db10  +0x120 (empty) omsg_DeActivate
	virtual void SetCamera(CGameObject *camera);							// ESLOT 73 0x475a40  +0x124
	virtual void SetInputDevice(short player, unsigned int device);			// ESLOT 74 0x475a50  +0x128
	virtual void PlaySong(void);											// ESLOT 75 0x475ac0  +0x12c
	virtual void ev76(void) {}												// ESLOT 76 0x472970  +0x130 (empty; called from Process)
	// a checkpoint polygon of the terrain (material "cstart" = 0, "ccheck1".."ccheck9" = 1..9; PhysicsAddTerrain)
	virtual void ev77(int checkpoint, OMedia3DPolygon *poly, OMedia3DShape *shape)	// ESLOT 77 0x4735e0 +0x134 (empty)
		{ (void)checkpoint; (void)poly; (void)shape; }
	// ESLOT 78 0x475cc0 +0x138 (empty here): C3DObject::Process (0x462520 area) reports the material
	// (and its texture) of the polygon an object stands on; CJimmyGame N2:0x484740 reacts to it.
	virtual void TouchSurface(CGameObject *o, OMedia3DMaterial *mat, OMediaCanvas *tex, float dt)
		{ (void)o; (void)mat; (void)tex; (void)dt; }
	// PhysicsAddTerrain asks for each terrain polygon: true keeps it out of the collision grid
	virtual unsigned char ev79(OMedia3DShape *shape, OMedia3DPolygon *poly, OMedia3DVector center)	// ESLOT 79 0x475cd0 +0x13c (returns 0)
		{ (void)shape; (void)poly; (void)center; return 0; }
	virtual void ToggleHelp(void);											// ESLOT 80 0x475ce0  +0x140
	virtual void ShowHelp(void) {}											// ESLOT 81 0x472970  +0x144 (empty)
	virtual void HideHelp(void) {}											// ESLOT 82 0x472970  +0x148 (empty)
	virtual int  TouchStart(CGameObject *o, CGameObject *other)	{ (void)o; (void)other; return -1; }	// ESLOT 83 0x475d40 +0x14c
	virtual int  TouchEnd(CGameObject *o, CGameObject *other)	{ (void)o; (void)other; return -1; }	// ESLOT 84 0x475d40 +0x150
	virtual int  Touch154(CGameObject *o, CGameObject *other)	{ (void)o; (void)other; return -1; }	// ESLOT 85 0x475d40 +0x154
	virtual void ev86(void) {}												// ESLOT 86 0x472970  +0x158 (empty)
};

// --------------------------------------------------------------------------- globals of the game type
namespace neg {
extern CGameObject		*g_camera;				// 0x5099e4  active camera (SetCamera); gets v243(dt) each frame
extern unsigned int		 g_song_id;				// 0x5099a4  chunk id of the level song (StartSong)
extern bool				 g_game_paused;			// 0x5099ad  PauseGame / IsPaused
extern bool				 g_help_shown;			// 0x5099ee  ToggleHelp
extern OMediaElement	*g_terrain;				// 0x5099e8  the last '3TER' object CreateTerrain made (its element)
extern unsigned int		 g_input_device[4];		// 0x4f6420  per player, 'KEYB' initially (SetInputDevice)
extern OMediaDataBase	*g_active_sound_db;		// 0x696008  sound-effect database the sound code plays from

extern OMediaDataBase	*g_effects_db;			// 0x5099b0
extern OMediaDataBase	*g_object_db;			// 0x5099b4
extern OMediaDataBase	*g_song_db;				// 0x5099b8
extern OMediaDataBase	*g_sound_db;			// 0x5099bc
extern OMediaDataBase	*g_terrain_db;			// 0x5099c0
extern OMediaDataBase	*g_vehicle_db;			// 0x5099c4
extern OMediaFileStream	*g_effects_stream;		// 0x5099c8
extern OMediaFileStream	*g_object_stream;		// 0x5099cc
extern OMediaFileStream	*g_song_stream;			// 0x5099d0
extern OMediaFileStream	*g_sound_stream;		// 0x5099d4
extern OMediaFileStream	*g_terrain_stream;		// 0x5099d8
extern OMediaFileStream	*g_vehicle_stream;		// 0x5099dc

// Shape helpers next to the game type (CGameType.cpp); CJimmyGame's terrain code uses them too.
bool ShapeExists(OMediaDataBase *db, unsigned int type, int id);		// 0x477c10 (type ignored: always '3DSh')
OMedia3DShape *Load3DShape(OMediaDataBase *db, int id);				// 0x477ba0
void MakeShapeUnLit(OMedia3DShape *shape);								// 0x477550
}

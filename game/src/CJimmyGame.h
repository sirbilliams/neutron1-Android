/*  CJimmyGame - the game type every level's game type derives from (CLevel01Game ... CLevelVR02).
    It adds to CGameType the logic all levels share: the terrain with its per-texture effects
    (scrolling and wobbling textures, unlit "nc" / additive "nc1" "nc2" pieces),, surface
    effects when Jimmy walks on special textures, the sky's turn and the pickup index counter.
    Reconstructed from Neutron.exe: constructor 0x44d2b0, destructor 0x44d390 (both empty: all
    the work is CGameType's), deleting thunks 0x44eef0 / 0x44d360 / 0x44ef00.

    Layout (classes.json), the same as CGameType's plus own members:
        +0x000  OMediaElement           element vtable 0x4c30e8 (92 slots: CGameType's 85 + 85..91)
        +0x030  OMediaListener          vtable 0x4c30d4
        +0x08c  CLocalGameObject        vtable 0x4c2c84 (275 slots)
        +0x540  own members (below); the level game types are 0x5c0 bytes
        +0x5c0  OMediaClassStreamer     shared virtual base, vtable 0x4c2c74
    The factory has no ID for CJimmyGame itself: only the CLevelXXGame classes are created.
    Negatron's CJimmyGame adds much more (power-up timers, level music, focus, savegame state,
    SetFog, ranks and promotions, slots 94..108): none of it exists in Neutron.exe. The levels set
    their fog inline (WorldFog).

    Members are named after their offset from the object start (the element-family view; a
    CLocalGameObject-family method of the original sees them 0x8c lower).                     */
#pragma once
#include "CGameType.h"
#include <list>
#include <string>

class C3DObject;
class OMedia3DPolygon;
class OMedia3DMaterial;
class OMediaCanvas;

class CJimmyGame : public CGameType
{
	public:
	CJimmyGame();															// 0x44d2b0
	virtual ~CJimmyGame();													// 0x44d390

	// ---- CLocalGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10  0x44d4e0 (jmp to CLocalGameObject's)
	virtual void Process(float dt);											// SLOT 241 0x44df80
	virtual void v260(void);												// SLOT 259 0x44df30 after a level load: pickup index 0

	// ---- element-family overrides of CGameType
	virtual void InitGame(void);											// ESLOT 45 0x44d3d0
	virtual void UnInitGame(void);											// ESLOT 46 0x44d4a0
	virtual void InitTerrainDatabase(std::string path);						// ESLOT 57 0x44e7b0
	virtual void CreateTerrain(void);										// ESLOT 61 0x44d4f0
	virtual void CreateTerrainUnLit(void);									// ESLOT 62 0x44d810
	virtual void TouchSurface(CGameObject *o, OMedia3DMaterial *mat, OMediaCanvas *tex, float dt);	// ESLOT 76 0x44e030

	// ---- element-family slots CJimmyGame adds (vtable offset = slot * 4)
	virtual int  NextPickupIndex(void);										// ESLOT 85  0x44dff0 +0x154
	virtual void CreateTerrainTinted(float r, float g, float b, float a);	// ESLOT 86  0x44dbf0 +0x158
	virtual char GetSceneStage(void);										// ESLOT 87  0x44df50 +0x15c
	virtual bool SetupTextureScroll(OMedia3DPolygon *p);					// ESLOT 88  0x44e470 +0x160
	virtual void SetTerrainDBColor(float r, float g, float b, float a);		// ESLOT 89  0x44db30 +0x164
	virtual void StopGame(char stop);										// ESLOT 90  0x44e450 +0x168 GameIsRunning() false while set
	virtual bool FileIsThere(char *name, const char *dir, const char *cd_dir);	// ESLOT 91 0x44e560 +0x16c

	// ---- members (not initialised by the constructor; InitGame sets them)
	float		f_540;					// 0x540  1.0 (InitGame; not read here)
	float		f_544;					// 0x544  1.0 (InitGame; not read here)
	int			m_surface_sound;		// 0x548  footstep sound set last given to Jimmy (his element slot 121)
	int			m_surface_sound_next;	// 0x54c  set from the texture under Jimmy by TouchSurface
	int			f_550;					// 0x550  0 (InitGame; not read here)
	// (0x554..0x5a8 is not used by CJimmyGame)
	int			m_pickup_index_base;	// 0x5a8  100
	int			m_pickup_index;			// 0x5ac  NextPickupIndex counter, reset by v260
	C3DObject	*m_sky;					// 0x5b0  the level's '3SKY' (the levels create it)
	unsigned char m_sky_rotates;		// 0x5b4  Process turns the sky about y by dt
	// 0x5bc: CLevel04BGame's m_sweeping (C3DJimmy sets it when Jimmy touches the REACTOR button)
};

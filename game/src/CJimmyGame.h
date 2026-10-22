/*  CJimmyGame - the game type every level's game type derives from (CLevel01Game ... CLevelVR02).
    It adds to CGameType the logic all levels share: the terrain with its per-texture effects
    (scrolling and wobbling textures, unlit "nc" / additive "nc1" "nc2" pieces), the level's fog
    and clear colour, the power-up timers (stopwatch, x-ray...), surface effects when Jimmy walks
    on special textures, the score rank and its promotions, the level music, pausing, focus
    changes and the per-level save state.
    Reconstructed from Neutron.exe: constructor 0x44d2b0, destructor 0x44d390 (both empty: all
    the work is CGameType's), deleting thunks 0x44eef0 / 0x44d360 / 0x44ef00.

    Layout (classes.json), the same as CGameType's plus own members:
        +0x000  OMediaElement           element vtable 0x4c30e8 (109 slots: CGameType's 87 + 87..108)
        +0x030  OMediaListener          vtable 0x4c30d4
        +0x08c  CLocalGameObject        vtable 0x4c2c84 (286 slots)
        +0x554  own members (below), up to the vtordisp at +0x684
        +0x688  OMediaClassStreamer     shared virtual base, vtable 0x4c2c74
    The factory has no ID for CJimmyGame itself: only the CLevelXXGame classes are created.

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
class OMediaStreamOperators;

class CJimmyGame : public CGameType
{
	public:
	CJimmyGame();															// 0x44d2b0
	virtual ~CJimmyGame();													// 0x44d390

	// ---- CLocalGameObject-family overrides
	virtual void ResetObject(void);											// SLOT 10  0x44d4e0 (jmp to CLocalGameObject's)
	virtual void Process(float dt);											// SLOT 240 0x44df80
	virtual void v242(float t);												// SLOT 242 N2:0x484640 per frame while the editor is editing
	virtual void v260(void);												// SLOT 260 0x44df30 (BG 259) after a level load: pickup index 0
	virtual void v277(void);												// SLOT 277 N2:0x485e80 PauseAll(true): stop the stopwatch tick
	virtual void v278(void);												// SLOT 278 N2:0x485ea0 PauseAll(false): restart it
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 N2:0x486170 savegame
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 N2:0x485ef0 savegame

	// ---- element-family overrides of CGameType
	virtual void InitGame(void);											// ESLOT 45 0x44d3d0
	virtual void UnInitGame(void);											// ESLOT 46 0x44d4a0
	virtual void InitTerrainDatabase(std::string path);						// ESLOT 57 0x44e7b0
	virtual void CreateTerrain(void);										// ESLOT 61 0x44d4f0
	virtual void CreateTerrainUnLit(void);									// ESLOT 62 0x44d810
	virtual void PauseGame(bool pause);										// ESLOT 68 N2:0x485410
	virtual void OnActivate(void);											// ESLOT 71 N2:0x482a80 "GetFocus()"
	virtual void OnDeactivate(void);										// ESLOT 72 N2:0x482ad0 "LoseFocus()"
	virtual void TouchSurface(CGameObject *o, OMedia3DMaterial *mat, OMediaCanvas *tex, float dt);	// ESLOT 78 0x44e030 (BG slot 76)

	// ---- element-family slots CJimmyGame adds (vtable offset = slot * 4)
	virtual void ev87(void) {}												// ESLOT 87  0x472970 +0x15c (empty)
	virtual int  NextPickupIndex(void);										// ESLOT 88  0x44dff0 +0x160
	virtual bool FileIsThere(char *name, const char *dir, const char *cd_dir);	// ESLOT 89 0x44e560 +0x164
	virtual void CreateTerrainTinted(float r, float g, float b, float a);	// ESLOT 90  0x44dbf0 +0x168
	virtual char GetSceneStage(void);										// ESLOT 91  0x44df50 +0x16c
	virtual void StopGame(char stop);										// ESLOT     0x44e450 (BG slot 90) GameIsRunning() false while set
	virtual bool SetupTextureScroll(OMedia3DPolygon *p);					// ESLOT 92  0x44e470 +0x170 (BG slot 88)
	virtual void SetTerrainDBColor(float r, float g, float b, float a);		// ESLOT 93  0x44db30 +0x174
	virtual void SetTerrainGrey(float v);									// ESLOT 94  N2:0x4836a0 +0x178
	virtual void UpdateTimers(float dt);									// ESLOT 95  N2:0x483c50 +0x17c
	virtual void SetTerrainColor(float r, float g, float b, float a);		// ESLOT 96  N2:0x4835b0 +0x180
	virtual void ev97(float dt) { (void)dt; }								// ESLOT 97  0x4732a0 +0x184 (empty; per frame, for the levels)
	virtual bool CheckPromotion(void);										// ESLOT 98  N2:0x485570 +0x188
	virtual int  GetRank(void);												// ESLOT 99  N2:0x4854e0 +0x18c
	virtual void RefreshObjects(void);										// ESLOT 100 N2:0x483c20 +0x190
	virtual void SetFog(float r, float g, float b, float a, float start, float end, float far_clip, bool on);	// ESLOT 101 N2:0x484520 +0x194
	virtual void PlayLevelMusic(const char *name, int a);					// ESLOT 102 N2:0x485d00 +0x198
	virtual void StopLevelMusic(void);										// ESLOT 103 N2:0x485b80 +0x19c
	virtual void StopLevelMusicKeepName(void);								// ESLOT 104 N2:0x485c60 +0x1a0
	virtual void StartGadgetEffect(const char *gadget);						// ESLOT 105 N2:0x483570 +0x1a4
	virtual float SmoothFrameRate(float dt);								// ESLOT 106 N2:0x485b40 +0x1a8
	virtual void StopwatchEnd(void);										// ESLOT 107 N2:0x4841f0 +0x1ac
	virtual void RestoreTimerEffects(void);									// ESLOT 108 N2:0x484110 +0x1b0

	// ---- members (not initialised by the constructor; InitGame sets them)
	// (Neutron.exe offsets: Negatron's - 0x10)
	// FIELD 0x554 4
	float		f_554;					// BG 0x540  1.0 (InitGame; not read here)
	// FIELD 0x558 4
	float		f_558;					// BG 0x544  1.0 (InitGame; not read here)
	// FIELD 0x55c 8
	int			m_surface_sound;		// BG 0x548  footstep sound set last given to Jimmy (his element slot 121)
	int			m_surface_sound_next;	// BG 0x54c  set from the texture under Jimmy by TouchSurface
	// FIELD 0x564 4
	int			f_564;					// BG 0x550  0 (InitGame)
	// (0x568..0x5b8 is not used by CJimmyGame)
	// FIELD 0x5b8 8
	int			m_pickup_index_base;	// 0x5b8  100
	int			m_pickup_index;			// 0x5bc  NextPickupIndex counter, reset by v260
	// FIELD 0x5c0 8
	C3DObject	*m_sky;					// 0x5c0  the level's '3SKY' (the levels create it)
	unsigned char m_sky_rotates;		// 0x5c4  Process turns the sky about y by dt
	// FIELD 0x5c8 4
	int			f_5c8;					// 0x5c8  0 (InitGame)
	// FIELD 0x5cc 24
	float		m_fog_restore[3];		// 0x5cc  start, end, far given back to SetFog when the window gets the focus again
	float		m_fog_start;			// 0x5d8
	float		m_fog_end;				// 0x5dc
	float		m_far_clip;				// 0x5e0
	// FIELD 0x5e4 16
	float		m_fog_color[4];			// 0x5e4  r, g, b, a (also the clear colour)
	// FIELD 0x5f4 1
	unsigned char m_refog;				// 0x5f4  set by OnActivate: Process calls SetFog again
	// (0x5f5..0x660 is not used by CJimmyGame)
	// FIELD 0x660 12
	float		m_xray_time;			// 0x660  terrain flashing while the x-ray gadget runs (StartGadgetEffect "XRAY")
	float		m_xray_flash;			// 0x664  >0 counting down: green next; <=0 counting up: yellow next
	int			m_stopwatch_channel;	// 0x668  "stopwatch" tick sound, -1 when not playing
	// FIELD 0x66c 24
	float		f_66c;					// 0x66c  5.0 (InitGame; not read here)
	float		f_670;					// 0x670  20000.0
	float		f_674;					// 0x674  20000.0
	float		f_678;					// 0x678  15.0
	int			m_no_texture_scroll;	// 0x67c  0: Process runs the texture scrolls
	float		f_680;					// 0x680  0.7
};

// --------------------------------------------------------------------------- globals CJimmyGame owns
namespace neg {
extern std::string					g_queued_sfx;			// N2:0x54a4a8  played by Process once channel g_queued_sfx_after is free (set by N2:0x48f310)
}

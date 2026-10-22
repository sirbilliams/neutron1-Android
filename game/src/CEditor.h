/*  CEditor - the level loader/saver and the developer level editor (one instance, g_editor).
    Reconstructed from Neutron.exe: constructor 0x46b160, destructor 0x46baa0 (deleting thunks
    0x46ba70 via the listener vtable, 0x46faf0 via the streamer vtable), 0x82c bytes.

    vtables: 0x4d5ea0 element (86 slots, game slots from 43), 0x4d5e8c listener at +0x30,
    0x4d5e7c streamer (virtual base, at +0x828). No CGameObject base.

    What it does in the shipped game (the editor UI is never switched on there - nothing sets
    m_editing - but every frame goes through it):
      * update_logic (slot 15) runs UpdateGame every frame: the start-up splashes 0 and 1 (3 s
        each, Escape skips), then RequestLevel("level1d.gam"); when RequestLevel's countdown runs
        out or LoadLevelNow was called, m_load_pending is set and CheckLoadLevel deletes the
        level's objects and calls LoadGame(m_level_name, -1).
      * LoadGame reads gam\<name>: the game type's 4cc, the object count, then per object its 4cc
        and its properties (CGameObject::ReadProperties); objects come from neg::CreateObject.
        Then ResetLevel resets every object.
      * SaveGame writes the same format. LoadLevelNow / RequestLevel (slots 64 / 69) are how the
        rest of the game asks for a level change (C3DPlayer's PlayerLoadLevel, CLoadLevel, the
        fades of C2DInGameMenu set m_load_pending directly).
      * OpenEditorDatabase / DrawSplash (slots 70/75, vtable +0x118 / +0x12c) are called by the
        game-type factory N2:0x480230 after it creates a new game type: open omt\splash.omt and
        show splash canvas 3.

    Element-family layout: offsets < 0x8c are OMediaElement (use the OMT accessors); own members
    are named after their offset in the 2002 object.                                          */
#pragma once
#include "OMediaElement.h"
#include "Globals.h"

class OMediaStringField;
class OMediaCanvas;
class OMediaCanvasFont;
class OMediaCanvasElement;
class OMediaDataBase;
class OMediaFileStream;
class OMediaPickRequest;
class OMedia3DShape;
class OMedia3DPolygon;
class OMedia3DMaterial;
class CGameObject;

class CEditor : public OMediaElement
{
	public:
	CEditor();																// 0x46b160 (new CEditor(1) in omt_main)
	virtual ~CEditor();														// 0x46baa0

	// ---- OMT element slots
	virtual void update_logic(float millisecs_elapsed);						// SLOT 15 0x46bbf0

	// ---- game slots (element vtable 0x4d5ea0)
	virtual void InitObject(void);											// SLOT 43 0x46bc40
	virtual void UnInitObject(void);										// SLOT 44 0x46bc90
	virtual void InitEditor(void);											// SLOT 45 0x46bcb0
	virtual void UnInitEditor(void);										// SLOT 46 0x46bdc0
	virtual void InitGrid(void);											// SLOT 47 0x46be80
	virtual void UnInitGrid(void);											// SLOT 48 0x46bea0
	virtual void UpdateGame(float dt);										// SLOT 49 N2:0x4a5c70
	virtual void UpdateEditor(float dt);									// SLOT 50 0x46c070
	virtual void ViewportClicked(OMediaPickRequest *req);					// SLOT 51 0x46d270 (omsg_ViewPortClicked)
	virtual void ev52(void);												// SLOT 52 0x46da80
	virtual void ClickList(int x, int y);									// SLOT 53 0x46c8d0
	virtual void ClickMenu(int x, int y);									// SLOT 54 0x46e980
	virtual bool ClickPropertyField(int x, int y);							// SLOT 55 0x46cf30
	virtual void HighlightSelection(bool on);								// SLOT 56 0x46d630
	virtual void SetMenuSelection(const char *entry);						// SLOT 57 0x46d4f0
	virtual void HideList(bool hide);										// SLOT 58 0x46c880
	virtual void ApplyPropertyEdit(void);									// SLOT 59 0x46daa0
	virtual void ShowProperties(bool on);									// SLOT 60 0x46d7c0
	virtual bool ToggleOption(int index);									// SLOT 61 0x46cd10
	virtual void SaveGame(char *name);										// SLOT 62 0x46dd50
	virtual void LoadGame(char *name, int level_id);						// SLOT 63 0x46e170
	virtual void CheckLoadLevel(void);										// SLOT 64 0x46dff0
	virtual void MakeBackupName(char *name);								// SLOT 65 0x46e7b0
	virtual void ListObjects(void);											// SLOT 66 N2:0x4a8c70
	virtual CGameObject *FindObject(int id);								// SLOT 67 0x46e6e0
	virtual void LoadLevelNow(char *name);									// SLOT 64 0x46df70 (BG numbering) m_load_pending + name
	virtual void RequestLevel(char *name);									// SLOT 69 0x46dfb0 (BG numbering) m_load_countdown = 2 + name
	void RequestLevelDelayed(char *name, float delay);						// port: Negatron's N2:0x4a83b0 (SaveGame.cpp)
	virtual void OpenEditorDatabase(void);									// SLOT 70 0x46f760 (vtable +0x118)
	virtual bool IsSingletonLoaded(int id);									// SLOT 71 0x46e6a0
	virtual void ShowObjectList(void);										// SLOT 72 N2:0x4a9d20
	virtual void ShowCatalogue(void);										// SLOT 73 0x46f250
	virtual void ResetLevel(void);											// SLOT 74 0x46e570
	virtual void DrawSplash(int canvas_id);									// SLOT 75 0x46f830 (vtable +0x12c)
	virtual void PlaceSprite(OMediaCanvasElement *e, float x, float y, int canvas_id, float unused, bool scaled);	// SLOT 76 N2:0x4aa500
	virtual void SetSpriteCanvas(OMediaCanvasElement *e, int canvas_id, bool scaled);	// SLOT 77 N2:0x4aa5c0
	virtual OMediaCanvas *GetCanvas(int canvas_id);							// SLOT 78 0x403690
	virtual void PlaceSplash(OMediaCanvasElement *e, int unused1, int unused2, int canvas_id);	// SLOT 79 N2:0x4aa3e0
	virtual void ShowPanel(bool on);										// SLOT 80 N2:0x4aa7e0
	virtual int  CountObjects(void);										// SLOT 81 N2:0x4a9c80
	virtual int  CountCollisionObjects(void);								// SLOT 82 N2:0x4a9ce0
	virtual int  CountSavedObjects(void);									// SLOT 83 N2:0x4a9ca0
	virtual void ResetLevelObjects(void);									// SLOT 84 0x473680
	virtual void PasteObject(CGameObject *src);								// SLOT 85 N2:0x4aa6c0

	// ---- helpers (inline in the original)
	void ClearSelection(void);			// zeroes m_sel_element/object/material/polygon and m_pick

	// ---- members (offsets in the 2002 object)
	unsigned char		m_editing;				// 0x8c  editor UI active (neg::EditorIsEditing)
	float				m_cam_save[4];			// 0x90  viewport position kept over InitEditor (w = 1)
	float				f_a0[4];				// 0xa0  (w = 1; only cleared)
	OMediaElement		*m_sel_element;			// 0xb0  selected object's element
	CGameObject			*m_sel_object;			// 0xb4  selected object
	OMedia3DMaterial	*m_sel_material;		// 0xb8  material taken off the picked polygon
	OMedia3DPolygon		*m_sel_polygon;			// 0xbc  picked polygon ("Select Poly")
	OMedia3DShape		*m_sel_shape;			// 0xc0  shape of the picked element
	OMediaPickRequest	*m_pick;				// 0xc4  pick request of the selection
	int					f_c8[13];				// 0xc8  cleared, unused
	OMediaStringField	*m_menu_fields[100];	// 0xfc  right-hand menu (g_editor_menu entries)
	OMediaStringField	*m_list_fields[100];	// 0x28c list: catalogue, objects, levels, toggles, databases
	OMediaStringField	*m_edit_name;			// 0x41c property being edited (name field)
	OMediaStringField	*m_edit_value;			// 0x420 property being edited (value field)
	OMediaStringField	*m_prop_names[100];		// 0x424 property names of the selected object
	OMediaStringField	*m_prop_values[100];	// 0x5b4 property values
	int					m_mode;					// 0x744 0 select, 1 create, 2 select poly, 4 toggles, 5 object list, 6 load game, 7 databases
	float				m_move_speed;			// 0x748 = 100
	unsigned char		m_load_pending;			// 0x74c set: CheckLoadLevel loads m_level_name (cleared by the new level)
	char				m_level_name[0x53];		// 0x74d level to load next
	int					f_7a0;					// 0x7a0 = -1
	char				m_game_name[0x54];		// 0x7a4 last .gam loaded (Save Game writes it back)
	unsigned char		f_7f8;					// 0x7f8 (set by C3DPlayer / C3DTriggerType)
	unsigned char		f_7f9;					// 0x7f9
	short				m_load_countdown;		// 0x7fa frames until m_load_pending (-1 = off)
	float				m_load_delay;			// port: seconds until m_load_pending (Negatron's RequestLevelDelayed)
	int					m_list_offset;			// 0x800 first list entry shown (scroll by 20)
	unsigned char		m_key_latch;			// 0x804
	float				m_start_timer;			// 0x804 time in the start-up state
	unsigned char		m_splash_key;			// 0x80d Escape held (skips one splash per press)
	int					m_start_state;			// 0x808 0 wait, 1 splash 0 shown, 2 splash 1 shown + level1d requested, 3 running
	unsigned char		f_810;					// 0x810
	unsigned char		f_811;					// 0x811
	unsigned char		m_panel_open;			// 0x812 list panel shown (arrow keys scroll instead of fly)
	int					m_label_mode;			// 0x814 object labels: 0 off, 1 near (1000), 2 all
	int					m_list_filter;			// 0x818 object list: 0 all, 1 3D, 2 cutscene, 3 pickups
	CGameObject			*m_copied;				// 0x81c Ctrl-C
	unsigned char		m_paste_latch;			// 0x820 Ctrl-V
};

// --- globals owned by the editor
namespace neg {
extern OMediaCanvas				*g_editor_font_canvas;	// 0x509978 png\fontsmall.png
extern OMediaCanvasFont			*g_editor_font;			// 0x50997c
extern unsigned char			 g_new_game;			// N2:0x56af08 set when level1d.gam (a new game) is loaded
extern OMediaCanvasElement		*g_splash;				// 0x509984
extern OMediaDataBase			*g_editor_db;			// 0x509988 omt\splash.omt
extern OMediaFileStream			*g_editor_stream;		// 0x50998c
extern OMediaCanvasElement		*g_editor_menu_panel;	// N2:0x56af1c canvas 5
extern OMediaCanvasElement		*g_editor_list_panel;	// N2:0x56af20 canvas 6
extern OMediaCanvasElement		*g_editor_props_panel;	// N2:0x56af24 canvas 7
}

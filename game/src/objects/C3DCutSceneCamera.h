/*  C3DCutSceneCamera ('3CAM') - one shot of a cut scene: when fired (by a trigger chain, a
    friend's talk through slot 23, or a C3DStartPoint) it hands the camera over
    ("PlayerControlled": an object to follow, or "NULL": nobody), stops the level music, plays
    clip "SoundIndex" of "SoundDatabase", points the camera at "CameraTarget" (starting
    "TargetActAnim" on it) and, when the clip ends or the player skips, fires "NextTrigger".
    While a shot runs every object is frozen (CLocalGameObject f_4ab, BG +0x49f).
    "CameraType" 2 keeps the camera where the sprite is; otherwise the camera rides on the target
    ("TargOffset", zooming from "InitialDist" by "ZoomSpeed" within MinDist..MaxDist), or with
    "ViewFromCamera" looks at the target from the sprite's direction.
    Reconstructed from Neutron.exe: constructor 0x415a00, destructor 0x415d30 (only resets the
    vtables, then ~C3DTriggerType; deleting thunks 0x416a80 / 0x415d00 / 0x416a90..0x416ab0).
    Vtables: element 56 slots (C3DTriggerType's 54 + 54, 55), game object at +0xc8 (0, 7, 23,
    241 own). 0x9cc bytes: own members 0x6bc..0x9c4, vtordisp 0x9c4, streamer 0x9c8.

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DTriggerType.h"
#include "OMedia3DVector.h"

class C3DCutSceneCamera : public C3DTriggerType
{
	public:
	C3DCutSceneCamera();													// 0x415a00
	virtual ~C3DCutSceneCamera();											// 0x415d30

	// ---- element-family slots it adds
	virtual void TriggerCamera(void);										// ESLOT 54 0x4165a0 (+0xd8) fire: take the view, start the clip
	virtual void SetView(OMedia3DVector pos, OMedia3DVector look);			// ESLOT 55 0x431670 (+0xdc) place the viewport (shared with 3MCA)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x415d70
	virtual void v23(CGameObject *o) { (void)o; TriggerCamera(); }			// SLOT 23  0x416a30 activated: fire
	virtual void Process(float dt);											// SLOT 241 0x415f90 the shot runs until its clip ends

	// port: the camera placement of Process (inline in the original)
	void UpdateView(float t);

	char		m_camera_target[100];	// 0x6bc  "CameraTarget" (= "none")
	char		f_720[100];				// 0x720  (not used)
	char		m_sound_db[100];		// 0x784  "SoundDatabase" (= "none")
	char		m_face_object[100];		// 0x7e8  "FaceObject" (= "none"; not used by the class)
	char		m_target_act_anim[100];	// 0x84c  "TargetActAnim" (= "none")
	char		m_target_deact_anim[100];	// 0x8b0  "TargetDeactAnim" (= "none")
	char		m_player_controlled[100];	// 0x914  "PlayerControlled" ("none": keep; "NULL": nobody; else that object)
	int			m_sound_index;			// 0x978  "SoundIndex" (= -1)
	int			m_channel;				// 0x97c  the clip's sound channel (= -1)
	unsigned char m_active;				// 0x980  the shot is running
	CGameObject	*m_target;				// 0x984  the CameraTarget object
	float		m_targ_offset[4];		// 0x988  "TargOffsetX/Y/Z" (= 0, 100, 200; Z becomes the zoomed distance), w 0x994
	int			m_view_from_camera;		// 0x998  "ViewFromCamera" (= 1)
	float		m_look_voffset;			// 0x99c  "LookVoffset" (= 100)
	int			f_9a0;					// 0x9a0  (not used)
	int			m_camera_type;			// 0x9a4  "CameraType" (= 0; 2: the camera stays at the sprite)
	float		m_zoom_speed;			// 0x9a8  "ZoomSpeed" (= 15)
	float		m_max_dist;				// 0x9ac  "MaxDist" (= 600)
	float		m_min_dist;				// 0x9b0  "MinDist" (= 200)
	float		m_initial_dist;			// 0x9b4  "InitialDist" (= 300)
	float		m_time;					// 0x9b8  time into the shot
	int			m_loop_act_anim;		// 0x9bc  "LoopActAnim" (= -1)
	int			m_deactivate_inv;		// 0x9c0  "DeactivateInv" (= 1)
};

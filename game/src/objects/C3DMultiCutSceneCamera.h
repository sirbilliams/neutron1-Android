/*  C3DMultiCutSceneCamera ('3MCA') - a cut scene of up to eight shots (the talks of the story
    scenes). Shot i points the camera at "CameraTarget<i>" (starting "TargetAnim<i>" on it), plays
    clip "SoundIndex<i>" of "SoundDatabase" and places the camera by "CameraType<i>" (an offset in
    the target's frame: 0 fixed, 1..4 zooming in), looking at the target raised by
    "LookatVOffset<i>" - 60. When a clip ends the target gets "TargetDeactAnim" and the next shot
    starts; after the last (or a skip) the camera goes to "PlayerControlled" ("NULL": nobody),
    the C3DAI held by the scene is released, and "NextTrigger" fires.
    Reconstructed from Neutron.exe: constructor 0x4308d0, destructor 0x430b00 (only resets the
    vtables, then ~C3DTriggerType; deleting thunks 0x4319e0 / 0x430ad0 / 0x4319f0). Vtables
    0x4a93c8 (element, 58 slots: C3DTriggerType's 54 + 54..57) / 0x4a8f64 (game object, at
    +0xc8) / 0x4a8f54 (streamer). 0xec4 bytes: C3DTriggerType's, own members 0x6bc..0xebc,
    vtordisp 0xebc, the streamer at 0xec0.

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DTriggerType.h"
#include "OMedia3DVector.h"

class C3DMultiCutSceneCamera : public C3DTriggerType
{
	public:
	enum { kShots = 8 };

	C3DMultiCutSceneCamera();												// 0x4308d0
	virtual ~C3DMultiCutSceneCamera();										// 0x430b00

	// ---- element-family slots it adds
	virtual void TriggerCamera(void);										// ESLOT 54 0x431280 (+0xd8) fire: shot 0
	virtual void SetView(OMedia3DVector pos, OMedia3DVector look);			// ESLOT 55 0x431670 (+0xdc) place the viewport
	virtual bool NextShot(void);											// ESLOT 56 0x431750 (+0xe0) "GetNextSoundChannel": start shot m_shot
	virtual void EndCutScene(void);											// ESLOT 57 0x4311f0 (+0xe4) hand the camera on, fire NextTrigger

	// port: what C3DTriggerType (still Negatron-shaped) and the editor call
	void TriggerCallback(CGameObject *caller, unsigned char flag);			// = TriggerCamera
	void StartPreview(void) {}												// (Negatron's editor preview; Boy Genius has none)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x430b40
	virtual void v23(CGameObject *o) { (void)o; TriggerCamera(); }			// SLOT 23  0x416a30 activated (a friend's talk): fire
	virtual void Process(float dt);											// SLOT 241 0x430da0 the shots run until the last clip ends

	char		m_sound_db[100];				// 0x6bc  "SoundDatabase" (= "none")
	char		m_target_deact_anim[100];		// 0x720  "TargetDeactAnim" (= "STOP")
	char		m_camera_target[kShots][100];	// 0x784  "CameraTarget<i>" (= "none")
	char		m_target_anim[kShots][100];		// 0xaa4  "TargetAnim<i>" (= "TALK")
	float		m_lookat_voffset[kShots];		// 0xdc4  "LookatVOffset<i>" (= 100)
	int			m_sound_index[kShots];			// 0xde4  "SoundIndex<i>" (= -1: no clip, the scene ends)
	int			m_camera_type[kShots];			// 0xe04  "CameraType<i>" (= 0; 0..4)
	char		m_player_controlled[100];		// 0xe24  "PlayerControlled" (= "NULL": the camera goes to nobody)
	int			m_channel;						// 0xe88  the clip's sound channel (= -1)
	unsigned char m_active;						// 0xe8c  a shot is running
	CGameObject	*m_target;						// 0xe90  the shot's CameraTarget object
	float		m_cam_offset[3];				// 0xe94  the camera point in the target's frame
	int			f_ea0;							// 0xea0  = 0 (not used)
	int			m_shot;							// 0xea4  the next shot
	float		m_time;							// 0xea8  time into the shot
	float		m_lookat;						// 0xeac  the running shot's LookatVOffset
	int			m_cur_camera_type;				// 0xeb0  the running shot's CameraType
	CGameObject	*m_held_ai;						// 0xeb4  the C3DAI the scene holds (state 9), released at the end
	int			m_deactivate_inv;				// 0xeb8  "DeactivateInv" (= 1): Jimmy puts his gadget away
};

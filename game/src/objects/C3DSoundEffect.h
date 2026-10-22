/*  C3DSoundEffect ('3SOU') - a sound trigger, drawn (in the editor) as "icons.omt" canvas 8,
    size 50. It plays the numbered sound SoundIndex once the task TaskName has reached
    RequiredLevel:
    * IsAmbient 1: looped from where it stands (3D) while the sound effects database is open.
    * IsAmbient 0: when the camera object touches it, up to TimesToTrigger times (-1: no limit;
      the touches count even while the task is not there yet).
    Reconstructed from Neutron.exe: constructor 0x440e20 (factory '3SOU', 0x608 bytes),
    destructor 0x440fe0 (only resets the vtables, then ~C3DSpriteType; deleting thunks
    0x440fb0 / 0x441220 / 0x441230). Vtables 0x4b5e14 (element, 53 slots: C3DSprite's) /
    0x4b5e00 (listener) / 0x4b59b0 (game object at +0xc8: 0, 7, 16, 241, 259 overridden) /
    0x4b59a0 (streamer at +0x604). Own members 0x5e8..0x600, vtordisp 0x600.
    Boy Genius's is much simpler than Negatron's (which played sounds by name, with voices,
    camera quakes, pause / resume and a saved trigger count, shared with C3DHelpSound).

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower).                          */
#pragma once
#include "../C3DSpriteType.h"

class C3DSoundEffect : public C3DSpriteType
{
	public:
	C3DSoundEffect();														// 0x440e20
	virtual ~C3DSoundEffect();												// 0x440fe0

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x441020 properties
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x4410b0 the camera touched it
	virtual void Process(float dt);											// SLOT 241 0x441130 the ambient loop
	virtual void v260(void);												// SLOT 259 0x4411e0 (a jump to C3DSpriteType's)

	int			m_sound_index;			// 0x5e8  "SoundIndex" (= -1: none)
	int			m_times_to_trigger;		// 0x5ec  "TimesToTrigger" (= -1: no limit)
	int			m_times_triggered;		// 0x5f0  (= 0)
	int			m_is_ambient;			// 0x5f4  "IsAmbient" (= 0)
	int			m_channel;				// 0x5f8  the ambient loop's channel (= -1)
	int			m_required_level;		// 0x5fc  "RequiredLevel" (= 0): TaskName's state needed
};

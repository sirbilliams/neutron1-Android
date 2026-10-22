/*  C3DButton ('3BUT') - a push button: a morph object with two animations UP / DOWN (created
    as HIUP / HIDOWN from "Up.ase" / "Down.ase", default "buttonup.ase" / "buttondown.ase";
    "...ship.ase" in LEV6 / LV6A) and one texture ("UpDown.Png", "switch.png"). Its colour
    ("Red" 1, "Green" 0.1, "Blue" 0.1) is dimmed to half while it is not available, and its
    red pulses (0.5 + 0.5 x the global pulse; green 0.2, blue 0.3) while it plays UP.
    Touched by the camera object (Jimmy) or a baseball while available and not just pressed:
    if it is up (the animation name is "HIUP" - see the note in AddTouching) it plays
    "AvailSound" (an index, -1 none), goes DOWN (dark grey), tells the object tagged
    "ActivateButton" NextState("Toggle") and, if that is another button, makes it available and
    itself not; in LV4B the button tagged REACTOR also gives 1000 points and sets the task
    REACTOR to 10 (once). Otherwise it plays UP, restores its colour and toggles the same
    object. 4 s after a press it resets (ResetObject: back to "ButtonAvailable", UP or DOWN).
    Radius 300 (150 after a reset).
    Reconstructed from Neutron.exe (Boy Genius): constructor 0x4115c0 (factory '3BUT', 0x87c
    bytes), destructor 0x411860 (only resets the vtables, then ~C3DAnimated; deleting thunks
    0x411830 / 0x411f50 / 0x411f60 / 0x411f70). Boy Genius's button is simpler than Negatron's:
    no task requirement, sound by index, no ResetTime / Radius / AvailSoundName properties, no
    state in the savegame.

    Original layout: as C3DAnimated (element vtable 73 slots: C3DAnimated's 72 + 72 (the empty
    0x4732a0); OMediaAnim at +0x90; listener at +0x30; CLocalGameObject vtable at +0xc0: 0, 7,
    10, 16, 241, 257 overridden), own members 0x6bc..0x874, vtordisp 0x874, streamer 0x878.
    Member names are their offsets from the object start.                                     */
#pragma once
#include "../C3DAnimated.h"

class C3DButton : public C3DAnimated
{
	public:
	C3DButton();															// 0x4115c0
	virtual ~C3DButton();													// 0x411860

	// ---- element-family slots
	virtual void ev72_bg(int unused) { (void)unused; }						// ESLOT 72 0x4732a0 (empty, one stack argument)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4119d0 properties, animations, texture
	virtual void ResetObject(void);											// SLOT 10  0x4118b0 available again (or not): UP / DOWN
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x411bb0 the camera or a baseball: press
	virtual void Process(float dt);											// SLOT 241 0x411de0 held in place, reset timer, pulse
	virtual void v258(void);												// SLOT 257 0x411ef0 (BG numbering) radius 300

	// ---- data members (object offsets)
	float				f_6bc;			// 0x6bc  "Red" (1)
	float				f_6c0;			// 0x6c0  "Green" (0.1)
	float				f_6c4;			// 0x6c4  "Blue" (0.1)
	int					f_6c8;			// 0x6c8  available (0)
	int					f_6cc;			// 0x6cc  "ButtonAvailable" (0)
	int					f_6d0;			// 0x6d0  "NASound" (-1; not used)
	int					f_6d4;			// 0x6d4  "AvailSound" (-1: none): sound index played on a press
	char				f_6d8[100];		// 0x6d8  "ActivateButton" ("none"): tag of the object pressed
	char				f_73c[100];		// 0x73c  "Down.ase"
	char				f_7a0[100];		// 0x7a0  "Up.ase"
	char				f_804[100];		// 0x804  "UpDown.Png" ("switch.png")
	int					f_868;			// 0x868  "NewTaskState" (0; not used)
	int					f_86c;			// 0x86c  "Toggle" (-1): the state passed on
	float				f_870;			// 0x870  reset timer (0)
};

/*  C3DButton ('3BUT') - a push button: a morph object with two animations UP / DOWN ("Up.ase" /
    "Down.ase", default "buttonup.ase" / "buttondown.ase"; "...ship.ase" in LEV6 / LV6A) and one
    texture ("UpDown.Png", "switch.png"). Its colour ("Red", "Green", "Blue"; -1 each: leave the
    material alone) pulses while it is up and available (f_930, from "ButtonAvailable").
    Pressed (ev95; touched by the camera object or a water balloon once the task "TaskName" has
    reached RequiredLevel, or told so by NextState): when up and available and not timed out, it
    plays "AvailSoundName" (or "not available"), goes DOWN (dark), tells the object tagged
    "ActivateButton" NextState("Toggle") and, if that is another button, makes it available and
    itself not. Pressed while down it goes back UP and toggles the same object. "ResetTime" (4 s)
    after a press it resets (ResetObject: back to ButtonAvailable, UP or DOWN).
    Reconstructed from Neutron.exe: constructor 0x4115c0 (factory '3BUT', 0xb50 bytes),
    destructor 0x411860 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x411830 /
    0x411f50 / 0x411f60 / 0x411f70).

    Original layout: as C3DAnimated (element vtable 0x494000, 96 slots: C3DAnimated's 94 + 94,
    95; OMediaAnim vtable 0x493fb0 at +0x90; listener 0x493fec at +0x30; CLocalGameObject vtable
    0x493b60 at +0xc0), own members 0x924..0xb48, vtordisp 0xb48, streamer 0xb4c.          */
#pragma once
#include "../C3DAnimated.h"

class OMediaStreamOperators;

class C3DButton : public C3DAnimated
{
	public:
	C3DButton();															// 0x4115c0
	virtual ~C3DButton();													// 0x411860

	// ---- element-family slots
	virtual void ev69(void);												// ESLOT 69 N2:0x424320 load the animations and the texture (once)
	virtual void ev94(int unused) { (void)unused; }							// ESLOT 94 0x4732a0 (empty, one stack argument)
	virtual void ev95(void);												// ESLOT 95 N2:0x423c10 pressed

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4119d0 properties
	virtual void ResetObject(void);											// SLOT 10  0x4118b0 available again (or not): UP / DOWN
	virtual void AddTouching(CGameObject *other);							// SLOT 16  N2:0x47d9a0 the camera or a water balloon: press
	virtual void Process(float dt);											// SLOT 240 0x411de0 reset timer, pulse
	virtual void v258(void);												// SLOT 258 0x411ef0 radius 300
	virtual void v260(void);												// SLOT 260 N2:0x4242f0 radius "Radius"
	virtual void v268(int state);											// SLOT 268 N2:0x47da00 "NextState": press (state unused)
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 N2:0x424410 + reset timer, animation
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 N2:0x4243c0 + reset timer, animation

	// ---- data members (object offsets)
	float				f_924;			// 0x924  "Red" (1)
	float				f_928;			// 0x928  "Green" (0.1)
	float				f_92c;			// 0x92c  "Blue" (0.1)
	int					f_930;			// 0x930  available (0)
	int					f_934;			// 0x934  "ButtonAvailable" (0)
	int					f_938;			// 0x938  "NASound" (-1; not used)
	int					f_93c;			// 0x93c  "AvailSound" (-1: no "not available" sound)
	char				f_940[100];		// 0x940  "ActivateButton" ("none"): tag of the object pressed
	char				f_9a4[100];		// 0x9a4  "Down.ase"
	char				f_a08[100];		// 0xa08  "Up.ase"
	char				f_a6c[100];		// 0xa6c  "UpDown.Png" ("switch.png")
	char				f_ad0[100];		// 0xad0  "AvailSoundName" ("none")
	int					f_b34;			// 0xb34  "NewTaskState" (0; not used)
	int					f_b38;			// 0xb38  "Toggle" (-1): the state passed on
	float				f_b3c;			// 0xb3c  reset timer (0)
	float				f_b40;			// 0xb40  "ResetTime" (4)
	float				f_b44;			// 0xb44  "Radius" (150)

	private:
	bool HasColour(void) const { return f_924 != -1.0f || f_928 != -1.0f || f_92c != -1.0f; }	// (bit compares in the original)
};

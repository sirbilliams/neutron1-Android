/*  C3DFriends - the base of Jimmy's friends and the other talking characters of the story
    (C3DSheen, C3DLibby, C3DCarl, C3DCindy, C3DNick, C3DBenny, C3DHugh, C3DJudy, ...): a C3DAI
    that does not hover, is held where it stands while Jimmy or Goddard touch it, with a blob
    shadow on the floor under it (a C3DShadow, 90 x 30, not made in windowed mode) and five
    TalkState / TalkTrigger property pairs: ev95 finds the trigger to activate for the current
    task state, ev80 activates it (once arrived and facing the target). No factory ID of its own.
    Reconstructed from Neutron.exe: constructor 0x41b490, destructor 0x41b740 (only resets the
    vtables, then ~C3DAI; deleting thunks 0x41b710 / 0x41bc70 / 0x41bc80 / 0x41bc90).

    Original layout (classes.json): as C3DAI (element vtable 98 slots: C3DAI's 95 + 95..97;
    CLocalGameObject at +0xc0, 275 slots), own members 0x994..0xba8, vtordisp 0xba8, streamer
    0xbac. Derived classes put theirs from 0xba8 on. The streamer vtable's read_class /
    write_class (0x410820 / 0x410830) are jumps to OMedia3DShapeElement's.

    Shared compiled code: several subclass overrides are one function in the exe, used by
    several classes (MSVC folded identical code). They are the protected helpers below
    (Friend*), each named after what it does with its address; a subclass override that is such
    a copy just calls the helper (e.g. C3DFowl::InitObject() { FriendInitObjectAnim(); }).
    (Some are Negatron's and still to be checked against Neutron.exe: those carry N2: addresses.)

    Element-family slots are `ev<n>` (`// ESLOT n`, Neutron.exe's numbering); members are named
    by what they hold, with their Neutron.exe object offset.                                 */
#pragma once
#include "../C3DAI.h"
#include "OMedia3DVector.h"

class C3DShadow;

class C3DFriends : public C3DAI
{
	public:
	C3DFriends();															// 0x41b490
	virtual ~C3DFriends();													// 0x41b740

	// ---- element-family slots
	virtual void ev79(void);												// ESLOT 79 0x41e580 (a copy of C3DAI's: state 10 wanders on)
	virtual void ev80(void);												// ESLOT 80 0x41b7b0 activate ev95's trigger: ev96, count it, state 9, face round
	virtual void ev90(void);												// ESLOT 90 0x41bc20 state 3
	virtual CGameObject *ev95(void);										// ESLOT 95 0x41bb20 "GetActivatedObject": TalkTrigger<i> whose TalkState<i> is the task state (or -2)
	virtual void ev96(void);												// ESLOT 96 0x41b810 before activating: 0x42adc0(100), stop
	virtual void ev97(int unused) { (void)unused; }						// ESLOT 97 0x4732a0 (empty, one stack argument)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x41b840 TalkState0..4 / TalkTrigger0..4
	virtual void ResetObject(void);											// SLOT 10  0x41b790
	virtual void Process(float dt);											// SLOT 241 0x41b950 timer, ev80 while attacking, the shadow

	protected:
	// ---- shared compiled copies (see above)
	void FriendInitObjectAnim(void);										// 0x415090 SLOT 7: C3DFriends::InitObject, then InitAnim
	void FriendInitObjectAIAnim(void);										// 0x418290 SLOT 7: C3DAI::InitObject (skips C3DFriends'!), then InitAnim
	void FriendPropsLoadedShow(void);										// N2:0x417110 SLOT 260: C3DAI::v260, then ev70(1)
	void FriendNoAnimIdle(void);											// N2:0x4369f0 ESLOT 60: ev58("IDLE", 1)
	void FriendTaskStateRewrite(void);										// 0x419aa0 SLOT 266: SetTaskState(TaskName, GetTaskState(TaskName))
	void FriendLevelScene(int level);										// N2:0x436900 SLOT 267: GetTaskState("SCENE") (unused), C3DAnimated::v267
	void FriendLevelSceneShow(int level);									// 0x4418e0 SLOT 267: C3DAnimated::v267, GetTaskState("SCENE") (unused), ev70(1)
	void FriendTouchScene(void);											// N2:0x44fa60 ESLOT 145: GetTaskState("SCENE") (unused)
	void FriendChangeState(int old_mode, int new_mode, const char *anim);	// N2:0x436920 (Fowl "TALK"), N2:0x427a40 (Cindy "IDLE"),
																			// N2:0x45b120 (Numey "SCHEME") SLOT 270: a cut scene interrupts STOP with <anim>

	public:
	// ---- data members (Neutron.exe object offsets)
	int					m_talk_state[5];		// 0x994  "TalkState0".."TalkState4" (0, -1, -1, -1, -1)
	char				m_talk_trigger[5][100];	// 0x9a8  "TalkTrigger0".."TalkTrigger4" ("none")
	short				m_talks;				// 0xb9c  triggers activated (ev80; 0 by ResetObject)
	float				m_timer;				// 0xba0  100 (ResetObject), += dt; 0 when ev80 activates
	C3DShadow			*m_shadow;				// 0xba4  blob shadow (90 x 30; NULL in windowed mode; the original keeps its CGameObject part)
	OMedia3DVector		f_1d60;					// N2 0x1d60 Negatron's shadow offset (TODO(BG): Boy Genius puts the shadow on the floor)
};

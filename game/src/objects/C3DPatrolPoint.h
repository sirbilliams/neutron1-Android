/*  C3DPatrolPoint ('3PAT') - a waypoint of an AI character's patrol ("icons.omt" canvas 6,
    size 50): the AI (C3DAI states 2/3) walks to it, plays "WaitAnim" for "WaitTime" there,
    then goes on to "NextPatrolPoint" ("WAIT" / "SCAN" in place of a tag: stand / scan).
    Reconstructed from Neutron.exe: constructor 0x434b30 (factory '3PAT', 0x7ec bytes),
    destructor 0x434da0 (deleting thunks 0x434f20 / 0x434d70 / 0x434f30). A C3DSprite (element
    vtable: all C3DSprite's slots), own members 0x5e8..0x7e4, vtordisp 0x7e4, streamer 0x7e8.
    (Negatron's C3DPatrolPoint added a "PatrolType" 1 network with visible-point lists, path
    finding for C3DAI, "GroupID", "PatrolSpeed" and "AttachObj": Boy Genius has none of them.)

    Members are named after their offset from the object start (element view; the
    CGameObject-family methods of the original see them 0xc8 lower). C3DAI reads them.       */
#pragma once
#include "../C3DSprite.h"

class C3DPatrolPoint : public C3DSprite
{
	public:
	C3DPatrolPoint();														// 0x434b30
	virtual ~C3DPatrolPoint();												// 0x434da0

	virtual void InitObject(void);											// SLOT 7   0x434de0
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x434ea0 (logs "PatrolPoint %s hit")

	// FIELD 0x5e8 0x1fc
	char		m_call_object_tag[100];	// 0x5e8  "CallObjectTag" (= "none")
	char		m_activate_anim[100];	// 0x64c  "ActivateAnim" (= "none")
	char		m_next_patrol_point[100];	// 0x6b0  "NextPatrolPoint" (= "none"; "WAIT", "SCAN")
	char		m_sound_db[100];		// 0x714  "SoundDatabase" (= "none")
	char		m_wait_anim[100];		// 0x778  "WaitAnim" (= "none")
	int			m_sound_index;			// 0x7dc  "SoundIndex" (= -1)
	float		m_wait_time;			// 0x7e0  "WaitTime" (= 0)
};

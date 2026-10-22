/*  CTaskList - the game object that owns the story task states ("MOM", "SCENE", "DINO" ...) and
    loads the requested task (save) files. Reconstructed from Neutron.exe: vtable 0x4d1c7c (276
    slots: CLocalGameObject's 275 + slot 275), constructor 0x45f790, destructor 0x45f8e0 (slot 0
    0x45f8c0 is the compiler's deleting destructor around it).

    Made by C3DJimmy's constructor and kept by Jimmy; it is not in the object factory and never
    saved to a .gam file. The task states themselves are global (neg::g_tasks, SaveGame.h): the
    constructor appends the twelve tasks of the game, objects follow them through their
    "TaskName" property (CLocalGameObject::m_task_name; BG slot 265 sets the state, slot 264
    reacts to it).

    Per frame (Process) the first frame after a level load pushes the task states to every
    object (v275), and a requested task file (RequestTask 0x460e70) is loaded (TaskGameLoad).  */
#pragma once
#include "CLocalGameObject.h"

class CTaskList : public CLocalGameObject
{
	public:
	CTaskList();															// 0x45f790
	virtual ~CTaskList();													// SLOT 0  0x45f8c0 / 0x45f8e0

	virtual void Process(float dt);											// SLOT 241 0x460ea0
	virtual void v242(float t);												// SLOT 242 0x460e40
	virtual void v260();													// SLOT 259 0x4601a0 (level reset: apply the tasks again)
	// New in CTaskList (vtable +0x44c): every object gets the state of its task (slot 265's
	// field directly, 0 when its task is unknown), then its slot 264.
	virtual void v275();													// SLOT 275 0x4601c0
	void v286() { v275(); }													// port: the Negatron name

	// FIELD 0x4b4 2
	unsigned char		f_4b4;				// 0x4b4  0 (never read)
	unsigned char		f_4b5;				// 0x4b5  the tasks were applied since the level loaded

#include "gen/CTaskList_slots.inc"
#include "gen/CTaskList_fields.inc"
};

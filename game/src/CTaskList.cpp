/*  CTaskList - see CTaskList.h. Reconstructed from Neutron.exe; each function names its
    original address.                                                                        */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "CTaskList.h"
#include "C2DInGameMenu.h"
#include "CEditor.h"
#include "SaveGame.h"
#include "Game.h"
#include "Globals.h"
#include "gen/globals.h"
#pragma GCC diagnostic pop

#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- 0x45f790

CTaskList::CTaskList()
{
	SetName((char *)"CTASKLIST");
	// The game's tasks and their first states (AddItem 0x45f8f0). Appended to the global list
	// each time a CTaskList is made.
	AddTask("MOM", 0);
	AddTask("LIBBY", 0);
	AddTask("BENNY", 0);
	AddTask("SCENE", 30);
	AddTask("DINO", 0);
	AddTask("CLONE", 0);
	AddTask("KITTY1", 0);
	AddTask("KITTY2", 0);
	AddTask("KITTY3", 0);
	AddTask("HYDRANT1", 0);
	AddTask("HYDRANT2", 0);
	AddTask("REACTOR", 0);
	v36(0);
	v107(0);											// (BG's slot 108 setter: not written to a .gam)
	f_4b5 = 0;
	g_task_requested = 0;
	strcpy(g_task_request, g_empty_string);
	f_4b4 = 0;
}

// 0x45f8e0: nothing of its own (the tasks stay in g_tasks)
CTaskList::~CTaskList()
{
}

// --------------------------------------------------------------------------- slots

void CTaskList::Process(float dt)						// 0x460ea0
{
	CGameObject::Process(dt);							// (CGameObject's, 0x473240, not CLocalGameObject's)
	if (!g_editor->m_editing)
	{
		if (!f_4b5)
		{
			f_4b5 = 1;
			v275();
		}
	}
	else
		f_4b5 = 0;
	if (g_task_requested)
	{
		TaskGameLoad(g_task_request);					// 0x4603f0
		g_task_requested = 0;
	}
}

void CTaskList::v242(float t)							// 0x460e40
{
	CGameObject::v242(t);								// (0x4732a0, empty)
	if (g_editor->m_editing) f_4b5 = 0;
}

void CTaskList::v260()									// 0x4601a0
{
	f_4b5 = 0;
}

void CTaskList::v275()									// 0x4601c0
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		if (!*i) continue;
		CLocalGameObject *o = static_cast<CLocalGameObject *>(*i);
		o->f_4a0 = GetTaskState(o->m_task_name);		// 0x45fea0 (slot 265's field, set directly)
		o->v266();										// slot 264 (+0x420)
	}
}

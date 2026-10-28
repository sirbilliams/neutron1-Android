/*  CLocalGameObject - hand-written parts. See CLocalGameObject.h.
    Reconstructed from Neutron.exe (0x4586f0..0x458f90); every slot of the vtable 0x4d07ac was checked.
    Slots 8 and 257 (0x458810, 0x458940) are in gen/CLocalGameObject_gen.cpp. */
#include "CLocalGameObject.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "OMediaMemStream.h"
#include "OMediaDataBase.h"
#include "OMediaStreamOperators.h"
#include "OMediaViewPort.h"
#include "OMediaMatrix.h"
#include "OMediaRect.h"
#include <string.h>

using namespace neg;

static bool s_all_paused = false;							// 0x4f8435

CLocalGameObject::CLocalGameObject()						// 0x4586f0
{
	f_4a9 = 1;
	strcpy(m_task_name, "scene");
	f_4a0 = 0;
	f_4a4 = 0;
	f_4a8 = 1;
	f_4aa = 0;
	f_4ab = 0;
	f_4ac = 0;
	m_debug = 0;
	m_db = NULL;
	m_db_stream = NULL;
	CGameObject::v26(1);
	f_4bc = 0;
}

CLocalGameObject::~CLocalGameObject() {}					// 0x4587e0

void CLocalGameObject::InitObject(void)						// 0x458820
{
	Log("InitObject()");
	CGameObject::InitObject();
	RegisterProperty("TaskName",    m_task_name,      PROP_ALPHA, 0);
	RegisterProperty("Debug",       &m_debug,         PROP_LONG, 0);
	// (Negatron also registers "ShowOnRadar" here; Neutron.exe does not)
}

void CLocalGameObject::ResetObject(void)					// 0x4587f0
{
	CGameObject::ResetObject();
	v26(1);													// vtable +0x68
}

void CLocalGameObject::InitPhysics(void)					// 0x458910: only once
{
	if (!f_4aa)
	{
		CGameObject::InitPhysics();
		f_4aa = 1;
	}
}

void CLocalGameObject::UnInitDatabases(void)				// 0x458c50
{
	if (f_4bc) CloseObjectDatabase();						// vtable +0x430 = CloseObjectDatabase (tail call)
}

unsigned char CLocalGameObject::v133(void)					// 0x458960 (BG slot 134): on the floor, or +0x4a0
{
	if (f_4ac) return 1;
	return f_13f;
}

void CLocalGameObject::AddRotation(float x, float y, float z)	// 0x458f90
{
	if (GameIsRunning()) CGameObject::AddRotation(x, y, z);
}

void CLocalGameObject::Process(float dt)					// 0x458930
{
	CGameObject::Process(dt);
}

void CLocalGameObject::v265(float a, float b, float c, float d)	// 0x4588d0
{
	if (v117()) v221(a, b, c, d);							// vtable +0x1d4 / +0x374
}

void CLocalGameObject::v267(int state)						// 0x458880
{
	f_4a0 = state;
}

void CLocalGameObject::DebugLog(const char *fmt, ...)		// 0x458c30: only when the "Debug" property is set
{
	(void)fmt;
	if (m_debug) { /* writes to the (empty in release) debug log */ }
}

void CLocalGameObject::CloseObjectDatabase(void)			// 0x458c70
{
	// "Calling CloseObjectDatabase for %s"
	if (m_db)
	{
		UnInitDatabase(m_db);								// 0x476080
		m_db = NULL;
	}
	if (m_db_stream)
	{
		delete m_db_stream;
		m_db_stream = NULL;
	}
}

void CLocalGameObject::OpenObjectDatabase(void)				// 0x458d00: an in-memory database
{
	if (!m_db_stream) m_db_stream = new OMediaMemStream;
	if (!m_db) m_db = new OMediaDataBase(m_db_stream);
}

int CLocalGameObject::CountObjectsWithID(int id)			// 0x458e10
{
	int n = 0;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		if (*i && (*i)->m_object_id == id) n++;
	return n;
}

void CLocalGameObject::PauseAll(bool pause)					// 0x458db0
{
	if (pause == s_all_paused) return;
	s_all_paused = pause;
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		if (!*i) continue;
		CLocalGameObject *o = (CLocalGameObject *)*i;		// every listed object has these slots
		if (pause) o->v277(); else o->v278();				// vtable +0x454 / +0x458
	}
}

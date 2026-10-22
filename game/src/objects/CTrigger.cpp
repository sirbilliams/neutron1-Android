/*  CTrigger - see CTrigger.h. Reconstructed from Neutron.exe 0x47dcf0..0x47e0e0. */
#include "CTrigger.h"
#include "../Globals.h"
#include <math.h>

using namespace neg;

CTrigger::CTrigger()										// 0x47dcf0
{
	// (C3DLight's constructor has run C3DLight::InitObject once already)
	SetName((char *)"CTRIGGER");							// 0x4702d0
	m_radius = 10.0f;
	m_entries.clear();										// (inlined: the new list is emptied)
	C3DLight::InitObject();									// 0x461bb0 (called directly, again)
	C3DLight::SetObjectID('TRIG');							// 0x461ba0
}

CTrigger::~CTrigger()										// 0x47de60
{
	for (std::list<Entry *>::iterator i = m_entries.begin(); i != m_entries.end(); ++i)
		if (*i) delete *i;									// 0x4789a0
	m_entries.clear();
}

namespace neg {
CGameObject *New_CTrigger(void) { return new CTrigger; }				// 'TRIG' (0x600 bytes)
}

// --------------------------------------------------------------------------- element-family slots

void CTrigger::AddObject(CGameObject *obj)					// 0x47df30
{
	Entry *e = new Entry;									// 0x478990 (8)
	e->obj = obj;
	e->inside = 0;
	m_entries.push_back(e);
}

// --------------------------------------------------------------------------- CGameObject-family slots

void CTrigger::v21(CGameObject *other)						// 0x47e060
{
	CGameObject::v21(other);								// 0x470e60
}

void CTrigger::Process(float dt)							// 0x47dfa0 (C3DLight's Process is not called)
{
	(void)dt;
	for (std::list<Entry *>::iterator i = m_entries.begin(); i != m_entries.end(); ++i)
	{
		Entry *e = *i;
		float p[4], q[4];
		v195(p);											// +0x310 position (BG slot 196)
		e->obj->v195(q);
		const float dx = p[0] - q[0], dy = p[1] - q[1], dz = p[2] - q[2];
		if (m_radius <= sqrtf(dx * dx + dy * dy + dz * dz))
		{
			if (e->inside)
			{
				v22((unsigned int)0 /* port: BG v22(obj), empty in CGameObject */);	// +0x58
				e->inside = 0;
			}
		}
		else if (!e->inside)
		{
			v21(e->obj);									// +0x54
			e->inside = 1;
		}
	}
}

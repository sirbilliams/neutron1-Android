/*  CTrigger ('TRIG') - a C3DLight that watches a list of objects: each frame it checks every
    watched object's distance from itself against its radius (10, set by the constructor; not a
    property) and calls its own v21(obj) when one comes inside, v22(obj) when it goes out again.
    CTrigger's v21 is CGameObject's (obj->v23(this)); v22 is the empty CGameObject one, so a
    plain CTrigger only tells the object that enters. Objects are added with AddObject (element
    slot 45); the list owns its entries. Its Process does not call C3DLight's (the light's
    position and colour are not updated), and ResetObject is the empty function.
    CTriggerTimer ('TRIT', 0x47e0e0) derives from it.
    Reconstructed from Neutron.exe: constructor 0x47dcf0 (factory 'TRIG', 0x600 bytes),
    destructor 0x47de60 (frees the entries, then ~C3DLight 0x461af0; deleting thunks 0x47de30 /
    0x47e0a0 / 0x47e0b0). Vtables: element 0x4d6e90 (46 slots: C3DLight's 45 + 45 AddObject),
    listener 0x4d6e7c (+0x30), game object 0x4d6a2c (+0x104: 0, 10, 21, 241 overridden),
    streamer 0x4d6a1c (OMediaLight's read_class / write_class). Own members 0x5e8..0x5f8
    (radius, then the std::list: allocator byte 0x5ec, head 0x5f0, size 0x5f4), vtordisp 0x5f8,
    streamer 0x5fc.                                                                           */
#pragma once
#include <list>
#include "../level3a/C3DLight.h"

class CTrigger : public C3DLight
{
	public:
	// An entry of the watch list (8 bytes, operator new 0x478990)
	struct Entry
	{
		unsigned char	inside;			// +0  the object is within the radius
		CGameObject		*obj;			// +4
	};

	CTrigger();																// 0x47dcf0
	virtual ~CTrigger();													// 0x47de60

	// ---- element-family slots
	virtual void AddObject(CGameObject *obj);								// ESLOT 45 0x47df30 watch obj (outside)

	// ---- CGameObject-family overrides
	virtual void ResetObject(void) {}										// SLOT 10  0x472970 (the empty function)
	virtual void v21(CGameObject *other);									// SLOT 21  0x47e060 CGameObject's
	virtual void Process(float dt);											// SLOT 241 0x47dfa0 (BG numbering) enter / leave

	float					m_radius;		// 0x5e8  (10)
	// FIELD 0x5ec 12
	std::list<Entry *>		m_entries;		// 0x5ec  the watched objects
};

/*  C3DMissile (no factory ID, object ID '3MIS') - a turret's missile ("missile.ase" DEFAULT,
    "missile.png", additive-blended texture, translucent 0.6, radius 40): the C3DYokTurret
    places, aims and launches it (flying flag set, 1000 units / s along its facing). While
    flying it is held level against gravity and, after 0.5 s, can collide again. Touching the
    rocket ship, Jimmy or the jeep (or any wall or ceiling) it stops, hides and switches its
    collision off (element slot 72); the touched object is remembered.
    Reconstructed from Neutron.exe: constructor 0x42f500 (0x6d4 bytes; no factory entry),
    destructor 0x42f650 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x42f620 /
    0x42f9a0 / 0x42f9b0 / 0x42f9c0). Element vtable: 72 is new; game object vtable (+0xc0): 0, 7,
    16, 220, 221, 223, 241 overridden. Own members 0x6c0..0x6c8, vtordisp 0x6cc, streamer
    0x6d0.                                                                                   */
#pragma once
#include "../C3DAnimated.h"

class C3DMissile : public C3DAnimated
{
	public:
	C3DMissile();															// 0x42f500
	virtual ~C3DMissile();													// 0x42f650

	// ---- element-family slots
	virtual void ev72(void);												// ESLOT 72 0x42f900 stop: hidden, no collision

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x42f6a0 model, texture, blend, alpha
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x42f800 hit the rocket / Jimmy / jeep
	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// SLOT 220 0x42f8a0 ceiling: stop
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x42f860 wall / floor: stop
	virtual void v222(void);												// SLOT 223 0x42f8e0 stop
	virtual void Process(float dt);											// SLOT 241 0x42f760 flying: level, collide after 0.5 s

	// ---- data members (object offsets)
	unsigned char		m_flying;		// 0x6c0  launched (0)
	unsigned char		m_hit;			// 0x6c1  stopped (0)
	float				m_timer;		// 0x6c4  flight time, restarted every 0.5 s (0)
	CGameObject			*m_target;		// 0x6c8  what it hit (0)
};

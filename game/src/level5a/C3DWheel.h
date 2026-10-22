/*  C3DWheel ('3WHE') - a wheel: a bare C3DObject (no collision with anything, v231 0; touching
    does nothing; its element is not moved by the object, v247 / v248 empty) whose shape is a
    chunk of the vehicle database (ev56). Its physics are set up once, with no gravity and no
    rotating to a destination (f_005, f_00a cleared).
    Reconstructed from Neutron.exe: constructor N2:0x4a0920 (factory '3WHE', 0x598 bytes; it repeats
    InitObject inline), destructor 0x468180 (only resets the vtables, then ~C3DObject; deleting
    thunks 0x468340 / 0x468330 / 0x468150 / 0x468350). Vtables: element 0x4d5d5c (57 slots:
    C3DObject's 56 + 56), OMediaAnim 0x4d5d0c, listener 0x4d5d48, game object (+0xc0) 0x4d58bc
    (0, 7, 11, 16, 231, 240, 247, 248 overridden), streamer 0x4d58ac. Own member 0x58c, vtordisp
    0x590, streamer 0x594.                                                                    */
#pragma once
#include "../C3DObject.h"

class C3DWheel : public C3DObject
{
	public:
	C3DWheel();																// N2:0x4a0920
	virtual ~C3DWheel();													// 0x468180

	// ---- element-family slots
	virtual void ev56(int id);												// ESLOT 56 0x468270 shape = vehicle database chunk <id>

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x468220 ObjectID '3WHE'
	virtual void InitPhysics(void);											// SLOT 11  0x4682a0 "InitPhysics()" once
	virtual void AddTouching(CGameObject *other) { (void)other; }			// SLOT 16  0x4732a0 (empty)
	virtual unsigned char v231(CGameObject *other) { (void)other; return 0; }	// SLOT 231 0x4682f0 collides with nothing
	virtual void Process(float dt);											// SLOT 240 0x4682e0 (C3DObject's)
	virtual void v247(void) {}												// SLOT 247 0x472970 (empty)
	virtual void v248(void) {}												// SLOT 248 0x472970 (empty)

	float				f_58c;			// 0x58c  0; spin, degrees (C3DVehicle ev132)
};

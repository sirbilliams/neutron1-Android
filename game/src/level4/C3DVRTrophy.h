/*  C3DVRTrophy ('3TRO') - the trophy at the end of a Virtual Reality course: a spinning morph
    object ("trophy.ase" / "trophy.png", loaded the first time ev69 runs) that turns 90 degrees
    per second about y. When the camera object (Jimmy) touches it, it hides, gives 300 points,
    plays sound 0xe6 and switches its collision off (slot 136).
    Reconstructed from Neutron.exe: constructor 0x448c60 (factory '3TRO', 0x6c8 bytes),
    destructor 0x448dc0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x448d90 /
    0x448fb0 / 0x448fc0 / 0x448fd0). Vtables: element 0x4be3f4 (listener at +0x30; C3DAnimated's 72 slots, 67
    overridden), game object (+0xc0) (0, 7, 10, 16, 241 overridden). Own member 0x6bc.   */
#pragma once
#include "../C3DAnimated.h"

class C3DVRTrophy : public C3DAnimated
{
	public:
	C3DVRTrophy();															// 0x448c60
	virtual ~C3DVRTrophy();													// 0x448dc0

	virtual void ev69(void);												// ESLOT 67 0x448e10 load the animation and texture (once)

	virtual void InitObject(void);											// SLOT 7   0x448e70 radius 20
	virtual void ResetObject(void);											// SLOT 10  0x448ef0 shown again
	virtual void AddTouching(CGameObject *other);							// SLOT 16  0x448f20 the camera: collected
	virtual void Process(float dt);											// SLOT 241 0x448ec0 spin

	// ---- data members (object offsets)
	int					f_6bc;			// 0x6bc  (0) the baseball hits C3DMovingTarget has counted
};

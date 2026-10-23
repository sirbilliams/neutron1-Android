/*  C3DNeuCar2 ('3NC2') - one of the cars on Retroville's streets (Level5c): a C3DAICar with the
    morph model "car.ase" and the texture "cargreen.png" (green), radius 300, speed 20 until its patrol
    points or the C3DAICar timer give it another. It plays "DEFAULT" (looped).
    Reconstructed from Neutron.exe: constructor 0x432730 (factory '3NC2', 0x9b8 bytes), InitObject 0x4328b0, destructor 0x432860 (vtable resets only, then ~C3DAICar; deleting
    thunks 0x432830 / 0x432970 / 0x432980 / 0x432990).

    Original layout: as C3DAICar (element vtable 0x4aab44, 144 slots, all inherited), no own members.
*/
#pragma once
#include "C3DAICar.h"

class C3DNeuCar2 : public C3DAICar
{
	public:
	C3DNeuCar2();																// 0x432730
	virtual ~C3DNeuCar2();

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x4328b0 the model
};

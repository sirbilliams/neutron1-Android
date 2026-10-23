/*  C3DNeuCar ('3NCA') - one of the cars on Retroville's streets (Level5c): a C3DAICar with the
    morph model "car.ase" and the texture "car.png", radius 300, speed 20 until its patrol
    points or the C3DAICar timer give it another. It plays "DEFAULT" (looped).
    Reconstructed from Neutron.exe: constructor 0x4324c0 (factory '3NCA', 0x9b8 bytes), InitObject 0x432640, destructor 0x4325f0 (vtable resets only, then ~C3DAICar; deleting
    thunks 0x4325c0 / 0x432700 / 0x432710 / 0x432720).

    Original layout: as C3DAICar (element vtable 0x4aa50c, 144 slots, all inherited), no own members.
*/
#pragma once
#include "C3DAICar.h"

class C3DNeuCar : public C3DAICar
{
	public:
	C3DNeuCar();																// 0x4324c0
	virtual ~C3DNeuCar();

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x432640 the model
};

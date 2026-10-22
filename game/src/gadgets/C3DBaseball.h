/*  C3DBaseball ('3BAS') - a baseball of Jimmy's baseball launcher: a small sprite (2..5 of
    "retainedsprites.omt", 25 units) that C3DJimmy throws (ThrowBaseball: a pool of twenty,
    f_ab4) and that bounces off the floor with a damped vertical speed. Enemies recognise it
    by its name (C3DAI's hit test "C3DBASEBALL").
    Reconstructed from Neutron.exe: factory '3BAS', constructor 0x40fc50 (0x61c bytes),
    destructor 0x40fea0 (only resets the vtables, then ~C3DPermanentSprite; deleting thunks
    0x410050 / 0x40fe70 / 0x410060). Vtable: slots 220, 221, 223, 241 and 259 are its own.

    Own members are named after their Neutron.exe offsets (BG C3DSprite ends at 0x5e8; ours is
    0x14 longer, the names keep the original's offsets).                                       */
#pragma once
#include "../C3DPermanentSprite.h"

class OMediaCanvas;

class C3DBaseball : public C3DPermanentSprite
{
	public:
	C3DBaseball();															// 0x40fc50
	virtual ~C3DBaseball();													// 0x40fea0

	// ---- CGameObject-family overrides
	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// BG SLOT 220 0x40ff50 ceiling (just the base)
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// BG SLOT 221 0x40fee0 wall / floor (just the base)
	virtual void v222(void);												// BG SLOT 223 0x40ff80 landed: bounce
	virtual void v260(void);												// BG SLOT 259 0x410000 sprite, physics, radius 15

	// ---- data members (Neutron.exe offsets)
	int					f_5f4;			// 0x5f4  = 0 (not used)
	OMediaCanvas		*f_5f8;			// 0x5f8  retainedsprites.omt canvas 2
	OMediaCanvas		*f_5fc;			// 0x5fc  canvas 3
	OMediaCanvas		*f_600;			// 0x600  canvas 4
	OMediaCanvas		*f_604;			// 0x604  canvas 5
	float				f_608;			// 0x608  = 1
	int					f_60c;			// 0x60c  = 0
	unsigned short		f_610;			// 0x610  = 0
};

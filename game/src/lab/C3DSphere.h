/*  C3DSphere ('3SPH') - a plain sphere: OMT makes the shape (8 rings, 12 rays) and gives it one
    material, coloured (InitSphere(argb, size)) or textured from a canvas file
    (InitSphere(file, size)). The physics' debug markers use it (0x47cc60), and levels can
    place it. Its twin is C3DCube ('3CUB', not reconstructed), which shares UnInitObject.
    Reconstructed from Neutron.exe: constructor 0x463970 (factory '3SPH', 0x598 bytes),
    destructor 0x463a90 (deleting thunks 0x463a60 / 0x463ee0 / 0x463ef0 / 0x463f00).

    Original layout (classes.json): as C3DObject (element vtable 0x4d467c, 58 slots: C3DObject's
    56 + 56, 57; OMediaAnim at +0x90, vtable 0x4d462c; listener +0x30, vtable 0x4d4668;
    CLocalGameObject at +0xc0, vtable 0x4d41dc), own member 0x58c, vtordisp 0x590,
    streamer 0x594.                                                                          */
#pragma once
#include <string>
#include "../C3DObject.h"
#include "OMedia3DVector.h"

class OMedia3DMaterial;

class C3DSphere : public C3DObject
{
	public:
	C3DSphere();															// 0x463970
	virtual ~C3DSphere();													// 0x463a90

	// ---- element-family slots
	virtual void InitSphere(OMedia3DVector argb, float size);				// ESLOT 56 0x463dc0 "InitSphere(argb,%f)": coloured (the material's ambient)
	virtual void InitSphere(std::string file, float size);					// ESLOT 57 0x463bd0 "InitSphere(\"%s\",%f)": textured ("" = plain lit material)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x463b30 an empty shape
	virtual void UnInitObject(void);										// SLOT 8  0x461580 (= C3DCube's) shape and material freed

	// ---- data members (object offsets)
	OMedia3DMaterial	*f_58c;			// 0x58c  the material InitSphere made (not the textured one)
};

/*  C3DCube ('3CUB') - a plain cube, the twin of C3DSphere: OMT makes the shape (make_cube)
    and gives it one material: coloured (slot 55 InitCube(argb, size)), from an object database
    material (slot 54) or textured from a canvas file (slot 56 InitCube(file, size)). The School
    places two (level2a); C3DCamera (the vehicles' camera probe) is one and the physics debug
    markers use them.
    Reconstructed from Neutron.exe: constructor 0x461320 (factory '3CUB', 0x584 bytes),
    destructor 0x461440 (deleting thunks 0x461970 / 0x461960 / 0x461410 / 0x461980).

    Original layout: as C3DObject (element vtable 0x4d2b20-like, 57 slots: C3DObject's 54 +
    54..56; OMediaAnim at +0x90; listener +0x30; CLocalGameObject at +0xc0: 7, 8 overridden),
    own member 0x578, vtordisp 0x57c, streamer 0x580.                                        */
#pragma once
#include <string>
#include "../C3DObject.h"
#include "OMedia3DVector.h"

class OMedia3DMaterial;
class OMediaDataBase;

class C3DCube : public C3DObject
{
	public:
	C3DCube();																// 0x461320
	virtual ~C3DCube();														// 0x461440

	// ---- element-family slots
	virtual void ev54(OMediaDataBase *db, int id, float size);				// ESLOT 54 0x4618e0 material <id> of db
	virtual void InitCube(OMedia3DVector argb, float size);					// ESLOT 55 0x4617f0 "InitCube(argb,%f)": coloured (the material's ambient)
	virtual void InitCube(std::string file, float size);					// ESLOT 56 0x461600 "InitCube(\"%s\",%f)": textured

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x4614e0 an empty shape
	virtual void UnInitObject(void);										// SLOT 8  0x461580 shape and material freed

	// ---- data members (object offsets)
	OMedia3DMaterial	*f_58c;			// 0x578  the material InitCube(argb) made
};

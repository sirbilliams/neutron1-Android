/*  C3DCube ('3CUB') - a plain cube, the twin of C3DSphere: OMT makes the shape (make_cube) and
    gives it one material: coloured (ev57 InitCube(argb, size)), from an object database material
    (ev56) or textured from a canvas file (ev58 InitCube(file, size), built polygon by polygon;
    not reconstructed). ev59 sets the materials' alpha. No level places one; C3DCamera (the
    vehicles' camera probe) is one.
    Reconstructed from Neutron.exe: constructor 0x461320 (factory '3CUB', 0x598 bytes),
    destructor 0x461440 (deleting thunks 0x461970 / 0x461960 / 0x461410 / 0x461980).

    Original layout: as C3DObject (element vtable 0x4d2b20, 60 slots: C3DObject's 56 + 56..59;
    OMediaAnim at +0x90, vtable 0x4d2ad0; listener +0x30, vtable 0x4d2b0c; CLocalGameObject at
    +0xc0, vtable 0x4d2680: 7, 8 overridden), own member 0x58c, vtordisp 0x590, streamer 0x594. */
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
	virtual void ev56(OMediaDataBase *db, int id, float size);				// ESLOT 56 0x4618e0 material <id> of db
	virtual void InitCube(OMedia3DVector argb, float size);					// ESLOT 57 0x4617f0 "InitCube(argb,%f)": coloured (the material's ambient)
	virtual void InitCube(std::string file, float size);					// ESLOT 58 N2:0x4999a0 "InitCube(\"%s\",%f)": textured (TODO)
	virtual void ev59(float alpha);											// ESLOT 59 N2:0x49a0f0 the materials' alpha (<= 0: opaque)

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x4614e0 an empty shape
	virtual void UnInitObject(void);										// SLOT 8  0x461580 shape and material freed

	// ---- data members (object offsets)
	OMedia3DMaterial	*f_58c;			// 0x58c  the material InitCube(argb) made
};

/*  C3DObject - the base of every 3D game object: an OMT morph-animated shape element that is
    also a CLocalGameObject. Reconstructed from Neutron.exe: constructor 0x462040, destructor
    0x462170. Original layout: OMedia3DMorphAnim at +0 (element vtable, 56 slots: OMT's 49 +
    game slots 49..55), its OMediaAnim at +0x90, CLocalGameObject at +0xc0, own members from
    +0x588, size 0x590 + the virtual-base streamer.

    This is also the template for the other multi-base classes: element-family game slots are
    `ev<n>` marked `// ESLOT n`, CGameObject-family slots keep their names and `// SLOT n`.  */
#pragma once
#include "OMedia3DMorphAnim.h"
#include "CLocalGameObject.h"

class C3DObject : public OMedia3DMorphAnim, public CLocalGameObject
{
	public:
	C3DObject();															// 0x462040
	virtual ~C3DObject();													// SLOT 0  0x462170

	// ---- OMT element virtuals overridden
	virtual void set_shape(OMedia3DShape *shape);							// element slot 43  0x462230

	// ---- element-family game slots
	virtual void SetObjectID(int id);										// ESLOT 49 0x462220
	virtual void FitToShape(void);											// ESLOT 50 0x462270
	virtual void ev51(void);												// ESLOT 51 0x4626f0 texture filtering on
	virtual void ev52(void);												// ESLOT 52 0x462750 texture filtering off
	virtual void ev53(void);												// ESLOT 53 0x4627b0 v64(|shape min y| / radius)
	// port: Neutron.exe's element vtable ends at slot 53 (54 slots): there is no ShowAgain/HideAgain.
	// These two are Negatron's, kept as plain methods (no slot) for the callers still to be ported:
	// Physics.cpp HideMarkers, CLevel03Games, C3DRocketShip, C3DVehicle/C3DJeep/C3DAnimated (f_589).
	void ShowAgain(void);
	void HideAgain(void);

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x462340
	virtual void UnInitObject(void);										// SLOT 8  0x462390
	virtual void InitPhysics(void);											// SLOT 11 0x4623d0
	virtual void UnInitPhysics(void);										// SLOT 12 0x462480
	virtual void Process(float dt);											// SLOT 241 0x462520 floor surface + tilt
	virtual void v243(float dt);											// SLOT 243 0x462650 viewport behind the object (camera)
	virtual void v247(void);												// SLOT 247 0x4624b0 SyncElementPosition
	void SyncElementPosition(void) { v247(); }
	virtual void v248(void);												// SLOT 248 0x4624f0 (= CGameObject's)
	virtual void v249(short x, short y, short z);							// SLOT 249 0x462500 SetElementAngles
	void SetElementAngles(short x, short y, short z) { v249(x, y, z); }

#include "gen/C3DObject_slots.inc"
#include "gen/C3DObject_fields.inc"
	unsigned char	f_574;													// 0x574: set to 0 by the constructor, never read
	unsigned char	f_589;													// port: Negatron's "shown" flag (1), see ShowAgain
};

namespace neg {
// 0x476ff0: the angles (degrees) that tilt an object onto a floor of normal n (C3DObject.cpp).
void FloorNormalToAngles(const OMedia3DVector &n, float *ax, float *ay, float *az);
}

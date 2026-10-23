/*  C3DCamera - the vehicles' camera probe: a hidden C3DCube (coloured, size 75) kept at the
    viewport. C3DVehicle / C3DCar ev110 place it there and, with the camera collision on
    (g_5099aa), slide it against the terrain (PhysicsMoveAgainstTerrain) to keep the camera out
    of walls. The first vehicle constructed makes the one instance (g_4fc69c); its destructor
    clears the pointer. Its wall and ceiling hits are ignored.
    Reconstructed from Neutron.exe: constructor 0x4610e0 (no factory ID, 0x598 bytes),
    destructor 0x461240 (deleting thunks 0x4612e0 / 0x4612d0 / 0x461210 / 0x4612f0).
    Original layout: as C3DCube (vtables 0x4d2580 / 0x4d256c / 0x4d2530, CLocalGameObject
    0x4d20e0: 219, 220 overridden, streamer 0x4d20d0).                                        */
#pragma once
#include "C3DCube.h"

class C3DCamera : public C3DCube
{
	public:
	C3DCamera();															// 0x4610e0
	virtual ~C3DCamera();													// 0x461240

	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// SLOT 219 0x461290 (nothing)
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);		// SLOT 220 0x461290 (nothing)
};

namespace neg {
C3DCamera *CameraProbe(void);			// g_4fc69c (NULL until a vehicle makes it)
}

/*  C3DSky ('3SKY') - the sky dome of a level: a shape from a sky database ("omt\wintersky.omt",
    ...), drawn in the clear layer (g_layer_clear, which has its own view matrix, so the sky
    stays around the camera). The level's InitGame creates it, loads it (ev58), places it at the
    origin and sets its radius; CJimmyGame turns it a little each frame when m_sky_rotates.

    Reconstructed from Neutron.exe: constructor 0x463110, destructor 0x463240 (thunks 0x463950 /
    0x463940 / 0x463210 / 0x463960). Same layout as C3DObject (0x590 bytes + the streamer,
    factory size 0x594), no members of its own. The sky database is kept in globals (one sky at
    a time): 0x4fc6b4 the file stream, 0x4fc6b0 the database, 0x4fc6b8 the "alt" flag.

    Element vtable 0x4d40dc: C3DObject's 56 slots plus 56..58 (below). CGameObject vtable
    0x4d3c3c overrides slots 0, 7, 8 and 11 (InitPhysics: the shared empty 0x472970).          */
#pragma once
#include "C3DObject.h"
#include <string>

class C3DSky : public C3DObject
{
	public:
	C3DSky();																// 0x463110
	virtual ~C3DSky();														// SLOT 0  0x463240

	// ---- element-family game slots
	virtual void ev56(std::string file);									// ESLOT 56 0x463370 +0xe0 open the sky database
	void OpenSkyDatabase(std::string file) { ev56(file); }
	virtual void ev57(void);												// ESLOT 57 0x4634d0 +0xe4 close the sky database
	void CloseSkyDatabase(void) { ev57(); }
	virtual void ev58(std::string file, bool alt);							// ESLOT 58 0x463500 +0xe8 load the sky
	void LoadSky(std::string file, bool alt) { ev58(file, alt); }

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7  0x4632e0
	virtual void UnInitObject(void);										// SLOT 8  0x463330
	virtual void InitPhysics(void) {}										// SLOT 11 0x472970 (empty)
};

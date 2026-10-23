/*  C3DTransRepl ('3TRA') - the transporter / replicator ray of level 5b: a C3DAnimated that plays
    the animation "HIRAY" from the ASE file ("ASEFile", "doorfowl.ase" by default) with the
    texture "PNGFile" ("doorfowl.png"), looping RAY; no collision, radius 40. It copies the
    C3DSwingDoor's start (hence its log strings) but has no behaviour of its own.
    Reconstructed from Neutron.exe: constructor 0x4466c0 (factory '3TRA', 0x78c bytes),
    destructor 0x4468e0 (only resets the vtables, then ~C3DAnimated; deleting thunks 0x4468b0 /
    0x4469c0 / 0x4469d0 / 0x4469e0). Game object vtable (+0xc0): 0, 7 overridden. Own members
    0x6bc / 0x720 (100-byte file names), vtordisp 0x784, streamer 0x788.                   */
#pragma once
#include "../C3DAnimated.h"

class C3DTransRepl : public C3DAnimated
{
	public:
	C3DTransRepl();															// 0x4466c0
	virtual ~C3DTransRepl();												// 0x4468e0

	virtual void InitObject(void);											// SLOT 7   0x446930 "ASEFile" / "PNGFile"

	// ---- data members (object offsets)
	char				m_ase_file[100];	// 0x6bc  "ASEFile" ("doorfowl.ase")
	char				m_png_file[100];	// 0x720  "PNGFile" ("doorfowl.png")
};

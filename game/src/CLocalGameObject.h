/*  CLocalGameObject - a CGameObject that lives in this game (as opposed to a networked one): adds
    a task name, debug flag, radar flag and an optional private object database.
    Reconstructed from Neutron.exe: vtable 0x4d07ac (275 slots), constructor 0x4586f0.      */
#pragma once
#include "CGameObject.h"

class OMediaDataBase;
class OMediaStream;
class OMediaViewPort;
class OMediaMatrix_4x4;

class CLocalGameObject : public CGameObject
{
	public:
	CLocalGameObject();														// 0x4586f0
	virtual ~CLocalGameObject();											// SLOT 0  0x4587c0

	virtual void InitObject(void);											// SLOT 7  0x458820
	virtual void ResetObject(void);											// SLOT 10 0x4587f0
	virtual void InitPhysics(void);											// SLOT 11 0x458910
	virtual void UnInitDatabases(void);										// SLOT 13 0x458c50
	virtual unsigned char v133(void);										// SLOT 134 0x458960
	virtual void AddRotation(float x, float y, float z);					// SLOT 205 0x458f90
	virtual void Process(float dt);											// SLOT 241 0x458930
	virtual void v265(float a, float b, float c, float d);					// SLOT 263 0x4588d0
	virtual void v267(int state);											// SLOT 265 0x458880 the task state
	virtual void DebugLog(const char *fmt, ...);							// SLOT 267 0x458c30
	virtual void CloseObjectDatabase(void);									// SLOT 268 0x458c70
	virtual void OpenObjectDatabase(void);									// SLOT 269 0x458d00
	virtual int  CountObjectsWithID(int id);								// SLOT 271 0x458e10
	virtual void PauseAll(bool pause);										// SLOT 274 0x458db0
	// Slots Neutron.exe gives CLocalGameObject that the base does nothing with (a bare ret, 0x472970 /
	// 0x4732a0); the gen tool does not list them because it sees them as inherited. The names are
	// the port's (Negatron's numbering).
	virtual void v266(void) {}												// SLOT 264 0x472970 (Humphrey, friends, pickups, rocket: task state written back)
	virtual void v268(int state) { (void)state; }							// SLOT 266 0x4732a0 "NextState" (ToggleObject)
	virtual void v275(void) {}												// SLOT 270 0x472970 (Jimmy's: cut scene camera, jeep)
	virtual void v277(void) {}												// SLOT 272 0x472970 pause
	virtual void v278(void) {}												// SLOT 273 0x472970 unpause
	// Negatron-only slots (Neutron.exe's table is 275 slots long; these are the 2002 editor and
	// game-mode hooks other classes of the port still use):
	virtual void v269(float a2) { (void)a2; }								// N2SLOT 269
	virtual void v270(int prev, int mode) { (void)prev; (void)mode; }		// N2SLOT 270 game mode change
	virtual void v271(void) {}												// N2SLOT 271 refresh (CJimmyGame::RefreshObjects)
	virtual void v280(void) {}												// N2SLOT 280 editor
	virtual void v283(unsigned int a2) { (void)a2; }						// N2SLOT 283 editor
	virtual unsigned char v281(OMediaViewPort *vp, float *pos, float *sx, float *sy);	// N2SLOT 281 N2:0x48f900 project pos (x, y, z) to the screen (centre-relative, y up); 0: off screen
	virtual void v282(OMediaMatrix_4x4 &m, float *w, float *h) { (void)m; (void)w; (void)h; }	// N2SLOT 282 0x4735e0 (empty) projection matrix for v281 (C3DSpriteType, C3DAnimated: layer's)
	virtual void WriteState(OMediaStreamOperators &s);						// N2SLOT 284 N2:0x48fbb0
	virtual void ReadState(OMediaStreamOperators &s);						// N2SLOT 285 N2:0x48fbf0

#include "gen/CLocalGameObject_slots.inc"

	// Members, at Neutron.exe's offsets (constructor 0x4586f0). The names are the ones the rest of
	// the port uses (Negatron's offsets, +0xc).
	// FIELD 0x430 100
	char				m_task_name[100];	// 0x430  "TaskName" property ("scene")
	// FIELD 0x494 4
	int					f_4a0;				// 0x494  the task state (v267 writes it)
	// FIELD 0x498 4
	unsigned int		f_4a4;				// 0x498  0
	// FIELD 0x49c 1
	unsigned char		f_4a8;				// 0x49c  1
	// FIELD 0x49d 1
	unsigned char		f_4a9;				// 0x49d  1 ("visible")
	// FIELD 0x49e 1
	unsigned char		f_4aa;				// 0x49e  0 (physics initialised)
	// FIELD 0x49f 1
	unsigned char		f_4ab;				// 0x49f  0 (frozen)
	// FIELD 0x4a0 1
	unsigned char		f_4ac;				// 0x4a0  0 (on the floor: v134)
	// FIELD 0x4a4 4
	int					m_debug;			// 0x4a4  "Debug" property
	// FIELD 0x4a8 8
	OMediaDataBase		*m_db;				// 0x4a8
	OMediaStream		*m_db_stream;		// 0x4ac
	// FIELD 0x4b0 1
	unsigned char		f_4bc;				// 0x4b0  0 (the private database is open)
	// (Negatron-only, not in Neutron.exe's object: "ShowOnRadar" and the byte at +0x4c4)
	int					m_show_on_radar;
	unsigned char		f_4c4;
#include "gen/CLocalGameObject_fields.inc"
};

/*  CLocalGameObject - a CGameObject that lives in this game (as opposed to a networked one): adds
    a task name, debug flag, radar flag and an optional private object database.
    Reconstructed from Neutron.exe: vtable 0x4d07ac (286 slots), constructor 0x4586f0.      */
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
	virtual unsigned char v133(void);										// SLOT 134 N2:0x48f010
	virtual void AddRotation(float x, float y, float z);					// SLOT 205 0x458f90
	virtual void Process(float dt);											// SLOT 241 0x458930
	virtual void v265(float a, float b, float c, float d);					// SLOT 263 0x4588d0
	virtual void DebugLog(const char *fmt, ...);							// SLOT 267 0x458c30
	virtual void CloseObjectDatabase(void);									// SLOT 268 0x458c70
	virtual void OpenObjectDatabase(void);									// SLOT 269 0x458d00
	virtual int  CountObjectsWithID(int id);								// SLOT 271 0x458e10
	virtual void PauseAll(bool pause);										// SLOT 274 0x458db0
	virtual unsigned char v281(OMediaViewPort *vp, float *pos, float *sx, float *sy);	// N2SLOT 281 N2:0x48f900 project pos (x, y, z) to the screen (centre-relative, y up); 0: off screen
	virtual void v282(OMediaMatrix_4x4 &m, float *w, float *h) { (void)m; (void)w; (void)h; }	// N2SLOT 282 0x4735e0 (empty) projection matrix for v281 (C3DSpriteType, C3DAnimated: layer's)
	virtual void WriteState(OMediaStreamOperators &s);						// N2SLOT 284 N2:0x48fbb0
	virtual void ReadState(OMediaStreamOperators &s);						// N2SLOT 285 N2:0x48fbf0

#include "gen/CLocalGameObject_slots.inc"

	// FIELD 0x43c 100
	char				m_task_name[100];	// 0x43c  "TaskName" property
	// FIELD 0x4b0 4
	int					m_debug;			// 0x4b0  "Debug" property
	// FIELD 0x4b4 8
	OMediaDataBase		*m_db;				// 0x4b4
	OMediaStream		*m_db_stream;		// 0x4b8
	// FIELD 0x4c0 4
	int					m_show_on_radar;	// 0x4c0  "ShowOnRadar" property
#include "gen/CLocalGameObject_fields.inc"
};

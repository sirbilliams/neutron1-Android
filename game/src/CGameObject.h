/*  CGameObject - the base of every game object (characters, pickups, triggers, menus).
    Verification incomplete; vtable 0x4d5fec (265 slots), constructor 0x46fbe0,
    destructor 0x470060.

    Verification incomplete (2026-10-27): handwritten bodies were compared function by
    function with raw/CGameObject.c; constructor/destructor and ambiguous decompilation
    were also checked in Neutron.exe disassembly. Port changes are identified below.
    Remaining full-class verification addresses (generated bodies, including TODO stubs;
    0x472630 is missing from the generated interface). Do not certify the class until these
    are checked/fixed. The brief prohibits editing gen/* in this task:
    0x470520, 0x470a00, 0x470a20, 0x470a40, 0x470c50, 0x470e70, 0x470e80, 0x470ea0,
    0x470eb0, 0x470ed0, 0x470ee0, 0x470f00, 0x470f10, 0x470f30, 0x470f40, 0x470f60,
    0x470f70, 0x470f90, 0x470fa0, 0x470fc0, 0x470fd0, 0x470ff0, 0x471000, 0x471020,
    0x471030, 0x471050, 0x471060, 0x471080, 0x471090, 0x4710b0, 0x4710c0, 0x4710e0,
    0x4710f0, 0x471110, 0x471120, 0x471140, 0x471150, 0x471170, 0x471180, 0x4711a0,
    0x4711b0, 0x4711d0, 0x4711e0, 0x471200, 0x471210, 0x471230, 0x471240, 0x471260,
    0x471270, 0x471290, 0x4712b0, 0x4712e0, 0x471300, 0x471330, 0x471350, 0x471380,
    0x4713a0, 0x4713d0, 0x471400, 0x471440, 0x471470, 0x4714b0, 0x4714e0, 0x471520,
    0x471550, 0x471590, 0x4715c0, 0x471600, 0x471630, 0x471680, 0x4716a0, 0x4716d0,
    0x471720, 0x471730, 0x471770, 0x471780, 0x4717d0, 0x4717e0, 0x471800, 0x471810,
    0x471830, 0x471840, 0x471860, 0x471870, 0x471880, 0x4718a0, 0x4718b0, 0x4718c0,
    0x4718d0, 0x4718e0, 0x4718f0, 0x471900, 0x471910, 0x471920, 0x471930, 0x471940,
    0x471950, 0x471960, 0x471970, 0x471980, 0x471990, 0x4719a0, 0x4719b0, 0x4719c0,
    0x4719d0, 0x4719e0, 0x4719f0, 0x471a00, 0x471a20, 0x471a30, 0x471a40, 0x471a50,
    0x471a60, 0x471a70, 0x471a80, 0x471a90, 0x471aa0, 0x471ab0, 0x471ac0, 0x471ad0,
    0x471ae0, 0x471af0, 0x471b00, 0x471b10, 0x471b20, 0x471b30, 0x471b40, 0x471b50,
    0x471b60, 0x471b70, 0x471ba0, 0x471bd0, 0x471c00, 0x471c40, 0x471c60, 0x471c90,
    0x471cc0, 0x471cf0, 0x471d30, 0x471d50, 0x471d80, 0x471db0, 0x471de0, 0x471e20,
    0x471e40, 0x471e70, 0x471ea0, 0x471ed0, 0x471f10, 0x471f40, 0x471f70, 0x471fa0,
    0x471fe0, 0x472000, 0x472030, 0x472060, 0x472090, 0x4720c0, 0x4720f0, 0x472120,
    0x472150, 0x472180, 0x4721b0, 0x4721e0, 0x472210, 0x472240, 0x472270, 0x4722a0,
    0x4722d0, 0x472300, 0x472330, 0x472360, 0x472390, 0x4723d0, 0x4723f0, 0x472420,
    0x472450, 0x472590, 0x4725a0, 0x4725d0, 0x4725e0, 0x4725f0, 0x472600, 0x472610,
    0x472620, 0x472630, 0x472640, 0x472650, 0x472660, 0x472970, 0x472ad0, 0x472c10,
    0x472c50, 0x472d80, 0x472ee0, 0x472f30, 0x473030, 0x4730e0, 0x4731d0, 0x473200,
    0x4732a0, 0x4735e0, 0x4737b0, 0x473840, 0x473860, 0x4738f0.

    Most of the class is a large set of attributes, each kept twice: the value the object was
    created/loaded with (f_00c..f_038 and friends) and the current value (f_134...), with a getter
    and a setter slot for each. Those slots are generated from the binary by
    tools/decomp/gen_class.py into gen/CGameObject_*.{inc,cpp}; the rest is written here by hand.

    Properties: InitObject() registers named fields ("PositionX", "ObjectID", ...) that the level
    files (.gam) and savegames read and write by name (ReadProperties / WriteProperties).

    Members named f_<offset> keep the original byte offset as their name until their meaning is
    known; runs of adjacent fields keep their original order (the property loader copies 4 bytes
    into 1-byte fields, exactly as the original did).                                         */
#pragma once
#include <cstdlib>
#include <new>
#include <list>
#include "OMediaClassStreamer.h"
#include "Globals.h"
#include "OMedia3DVector.h"
#include "Sound.h"				// StopObjectSounds, IsChannelPlaying

class OMediaStreamOperators;
class OMedia3DShape;
class OMedia3DPolygon;
class CGameObject;

// A registered property (0x4c bytes): see RegisterProperty 0x473fa0.
struct CProperty
{
	int		flags;				// +0x00
	int		type;				// +0x04  PROP_*
	char	name[64];			// +0x08
	void	*data;				// +0x48  the field it edits
};

enum { PROP_NONE = 0, PROP_ALPHA = 1, PROP_BOOL = 2, PROP_FLOAT = 3, PROP_SPECIFIC = 4, PROP_5 = 5, PROP_LONG = 6 };

class CGameObject : public virtual OMediaClassStreamer		// virtual, as in the original (vbptr at +4)
{
	public:
	CGameObject();															// 0x46fbe0
	virtual ~CGameObject();													// SLOT 0  0x46ffe0 / 0x470060

	// port: game objects start zeroed. The original allocates them with the CRT's operator new
	// (through omt_dll_allocate), which doesn't clear memory, and several constructors leave
	// members unset (e.g. the initial velocity CGameObject +0x7c.. that ResetObject copies).
	// On Windows a fresh heap block was in practice zero; here it is not, so clear it.
	static void *operator new(size_t size)	{ void *p = calloc(1, size); if (!p) throw std::bad_alloc(); return p; }
	static void operator delete(void *p)	{ free(p); }

	// ---- properties and identity (hand-written)
	virtual void ReadProperties(OMediaStreamOperators &s);					// SLOT 1  0x473b30
	virtual void WriteProperties(OMediaStreamOperators &s);				// SLOT 2  0x473960
	virtual void v4(float t);												// SLOT 4  0x470280
	virtual void SetName(char *tag);										// SLOT 5  0x4702d0
	virtual bool NameContains(char *s);										// SLOT 6  0x470380
	virtual void InitObject(void);											// SLOT 7  0x4703b0
	virtual void UnInitObject(void);										// SLOT 8  0x470500 (log; not in the decompilation's function list)
	virtual void ResetObject(void);											// SLOT 10 0x470540
	virtual unsigned char v133(void);										// SLOT 134 0x471a10 on the floor (f_13f) (not in the function list)
	// touches, from PhysicsCollideObjects: the argument is the other object
	virtual void v3(CGameObject *other);									// SLOT 3  0x470270 (= AddTouching)
	virtual bool IsTouching(CGameObject *other);							// SLOT 15 0x470a60
	virtual void AddTouching(CGameObject *other);							// SLOT 16 0x470a90 touch starts
	virtual void RemoveTouching(CGameObject *other);						// SLOT 17 0x470b20 touch ends
	virtual void v18(CGameObject *other);									// SLOT 18 0x470bc0 still touching
	virtual void Collide(CGameObject *other, float dist);					// SLOT 19 0x470bf0 spheres overlap (dist between centres)
	virtual void v21(CGameObject *other);									// SLOT 21 0x470e60
	virtual void AddRotation(float x, float y, float z);					// SLOT 205 0x472480
	// terrain contact at the end of a sweep (PhysicsMoveAgainstTerrain): the polygon's normal
	virtual void v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly);	// SLOT 220 0x472690 ceiling
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 221 0x458890 wall/floor
	virtual OMedia3DVector v224(float x, float y, float z);				// SLOT 225 0x472980 local point -> world (rotation v201, position)
	virtual OMedia3DVector v226(float x, float y, float z);				// SLOT 227 0x472b10 local direction -> world (rotation v201 only, w 0)
	virtual unsigned char v231(CGameObject *other);							// SLOT 232 0x472f80 may collide with other (1)
	// floor polygon/shape found by PhysicsSnapToGround (initial value f_10c/f_114, current f_234/f_23c)
	virtual OMedia3DPolygon *v95(void);										// SLOT 96  0x471740
	virtual void v96(OMedia3DPolygon *poly);								// SLOT 97  0x471750
	virtual OMedia3DShape *v99(void);										// SLOT 100  0x4717a0
	virtual void v100(OMedia3DShape *shape);								// SLOT 101 0x4717b0
	virtual OMedia3DPolygon *v205(void);									// SLOT 206 0x472570
	virtual void v206(OMedia3DPolygon *poly);								// SLOT 207 0x472580
	virtual OMedia3DShape *v209(void);										// SLOT 210 0x4725b0
	virtual void v210(OMedia3DShape *shape);								// SLOT 211 0x4725c0
	virtual void Link(CGameObject *other);									// SLOT 234 0x472f90
	virtual void Unlink(CGameObject *other);								// SLOT 236 0x473040
	virtual void DisableCollisionID(int id);								// SLOT 237 0x4730f0
	virtual void EnableCollisionID(int id);									// SLOT 238 0x473170
	virtual void Process(float dt);											// SLOT 241 0x473240
	virtual void CheckSounds(float dt);										// SLOT 245 0x4732b0
	virtual void RotateToDest(float dt);									// SLOT 246 0x473320
	virtual void Log(const char *fmt, ...);									// SLOT 254 0x473910
	virtual void RegisterProperty(const char *name, void *data, int type, int flags);	// SLOT 255 0x473fa0
	virtual CProperty *FindProperty(const char *name);						// SLOT 256 0x473eb0
	virtual void DeleteAllProperties(void);									// SLOT 258 0x474480
	virtual void v248(void);												// SLOT 248 0x473540 element angles from the rotation (v249)
	virtual void v262(CGameObject *other, float dist);						// SLOT 261 0x4740c0 pushed by a touching object
	virtual void v263(CGameObject *other, OMedia3DVector pos, float r_other, float dist);	// SLOT 262 0x474140 the push
	virtual void v221(float a, float b, float c, float d);					// SLOT 222 0x472750 (surface reflection)
	virtual void v23(CGameObject *o) { (void)o; }							// SLOT 23 0x4732a0 (empty)
	virtual void v24(CGameObject *o) { (void)o; }							// SLOT 24 0x4732a0 (empty)
	virtual void v242(float t) { (void)t; }									// SLOT 242 0x4732a0 (empty)

	// ---- generated slots (accessors, forwarders) and the slots still to write
#include "gen/CGameObject_slots.inc"

	// ---- data members
	// FIELD 0x128 12
	std::list<void *>			f_128;			// 0x128  (element type not known yet)
	// FIELD 0x250 12
	std::list<CGameObject *>	m_linked;		// 0x250  objects linked both ways (Link/Unlink)
	// FIELD 0x25c 12
	std::list<CGameObject *>	m_touching;		// 0x25c  objects currently touching
	// FIELD 0x268 12
	std::list<CProperty *>		m_properties;	// 0x268
	// FIELD 0x2a0 256
	char						m_name[256];	// 0x2a0  class name, then "_<tag>" (SetName)
	// FIELD 0x3a0 100
	char						m_tag[100];		// 0x3a0  "ObjectTag" property
	// FIELD 0x404 4
	int							m_object_id;	// 0x404  "ObjectID" property (4cc, e.g. '3JIM')
	// FIELD 0x420 8
	CGameObject					*m_debug_cube;	// 0x420  created on demand by v251 ('3CUB')
	CGameObject					*m_debug_sphere;// 0x424  created on demand by v253 ('3SPH')
	// FIELD 0x428 4
	int							m_num_properties;	// 0x428

	// TYPE 0x414 OMedia3DShape *
	// TYPE 0x10c OMedia3DPolygon *
	// TYPE 0x114 OMedia3DShape *
	// TYPE 0x234 OMedia3DPolygon *
	// TYPE 0x23c OMedia3DShape *
	// TYPE 0x294 float
	// TYPE 0x298 float
#include "gen/CGameObject_fields.inc"
	// port: two members of Negatron's CGameObject that Neutron.exe's (0x42c bytes) has not
	// (no FIELD tag: the generator never sees them): the time scale granny animations and
	// SaveGame.cpp still read (1.0), and the savegame object number (SaveGame.cpp).
	float		f_430;
	unsigned int f_438;
};

// The global object list (0x50999c head / 0x5099a0 count).
extern std::list<CGameObject *> g_objects;

namespace neg {
void ErrorLog(const char *fmt, ...);										// port: Boy Genius's "ERROR: ..." debug-log calls (the empty 0x472970) are printed
CGameObject *CreateObject(int id);											// 0x459cb0
void PhysicsRemoveObject(CGameObject *o);									// 0x478fa0
}

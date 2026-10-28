/*  CGameObject - hand-written parts. See CGameObject.h.
    The accessor slots are in gen/CGameObject_gen.cpp.
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
*/

#include "CGameObject.h"
#include "Game.h"
#include "Physics.h"
#include "Todo.h"
#include "OMediaStreamOperators.h"
#include <SDL.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <string>
#include "Bridge.h"
#include "gen/globals.h"
#include "OMediaTrigo.h"
#include "OMedia3DVector.h"

std::list<CGameObject *> g_objects;					// 0x50999c

namespace neg {
}
using namespace neg;

// --------------------------------------------------------------------------- 0x46fbe0

CGameObject::CGameObject()
{
	f_048 = 0; f_058 = 0; f_068 = 0; f_078 = 0; f_088 = 0;
	f_098 = 1.0f; f_0a8 = 1.0f; f_0b8 = 1.0f; f_0c8 = 1.0f; f_0d8 = 1.0f; f_0e8 = 1.0f; f_0f8 = 1.0f; f_108 = 1.0f;
	f_170 = 0; f_180 = 0; f_190 = 0; f_1a0 = 0; f_1b0 = 0;
	f_1c0 = 1.0f; f_1d0 = 1.0f; f_1e0 = 1.0f; f_1f0 = 1.0f; f_200 = 1.0f; f_210 = 1.0f; f_220 = 1.0f; f_230 = 1.0f;
	f_280 = 1.0f; f_290 = 1.0f;
	// port: initialize identity storage, as with the zeroed object allocation.
	m_name[0] = 0; m_tag[0] = 0; m_object_id = 0;

	strcpy(m_name, "CGAMEOBJECT");
	Log("CGameObject()");

	// The executable calls the initial setters, which also update current values.
	v26(0); v30(1); v32(1); v34(0); v50(1);
	v36(1); v38(0); v40(1); v42(1); v44(1);
	v46(1); v48(0); v52(1); v54(0);
	v56(0.5f); v58(100.0f); v60(0.2f); v62(0.9f);
	v64(1.0f); v66(1.0f); v67(0); v69(0);
	v71(0, 0, 0); v73(0, 0, 0); v75(0, 0, 0); v77(0, 0, 0);
	v79(0, 0, 0); v81(0, 0, 0); v83(0, 0, 0); v85(0, 0, 0);
	v87(0, 0, 0); v90(0, 0, 0); v92(0, 0, 0);
	v94(1);
	v96(0); v98(0); v100(0); v102(0); v104(0); v106(0);
	v28(1); v107(1);
	CGameObject::ResetObject();
	f_294 = 0;
	f_29c = 1.0f;
	f_408 = -1; f_40c = -1; f_410 = -1;
	f_414 = NULL; f_418 = 0;
	g_objects.push_back(this);
	f_41c = 0;
	f_004 = 0;
	m_num_properties = 0;
	m_debug_cube = NULL; m_debug_sphere = NULL;
	f_007 = 1; f_008 = 0; f_009 = 0;
	f_42c = 0;
	// port: compatibility fields used by the Negatron animation/savegame code.
	f_430 = 1.0f;
	f_438 = 0;
}

// --------------------------------------------------------------------------- 0x470060

CGameObject::~CGameObject()
{
	Log("~CGameObject()");
	StopObjectSounds(this);									// 0x47dc20
	// port: express the original slot 218 deferred deletion flag directly;
	// CEditor's deletion pass performs the deletion on its next pass.
	if (m_debug_cube) m_debug_cube->f_006 = 1;
	if (m_debug_sphere) m_debug_sphere->f_006 = 1;
	DeleteAllProperties();
	g_objects.remove(this);
	PhysicsRemoveObject(this);								// 0x478fa0
}

// --------------------------------------------------------------------------- properties

void CGameObject::RegisterProperty(const char *name, void *data, int type, int flags)	// 0x473fa0
{
	CProperty *p = (CProperty *)malloc(sizeof(CProperty));
	if (!p) { ErrorLog("ERROR: Unable to allocate variable %s in AddProperty", name); return; }
	m_num_properties++;
	p->type = type;
	p->flags = flags;
	strcpy(p->name, name);
	p->data = data;
	m_properties.push_back(p);
}

CProperty *CGameObject::FindProperty(const char *name)		// 0x473eb0: equal length, case-insensitive match
{
	for (std::list<CProperty *>::iterator i = m_properties.begin(); i != m_properties.end(); i++)
		if (strlen((*i)->name) == strlen(name) && strcasecmp((*i)->name, name) == 0)	// _stricmp
			return *i;
	return NULL;
}

void CGameObject::DeleteAllProperties(void)					// 0x474480
{
	Log("DeleteAllProperties()");
	while (!m_properties.empty())
	{
		CProperty *p = m_properties.front();
		m_properties.remove(p);
		free(p);
	}
}

// Every value property is 4 bytes in the file and is copied as 4 bytes, even into a bool:
// the original wrote/read `int` through the field's address, so a bool property carries the
// three bytes that follow it. Kept as is - the level files depend on it.
static int read4(OMediaStreamOperators &s, void *dst, bool as_float)
{
	if (as_float) { float f; s >> f; memcpy(dst, &f, 4); }
	else { int v; s >> v; memcpy(dst, &v, 4); }
	int bits; memcpy(&bits, dst, 4);
	return bits;
}

void CGameObject::ReadProperties(OMediaStreamOperators &s)	// 0x473b30
{
	int count; s >> count;
	if (count != m_num_properties)
		;	// "WARNING: Num properties in file does not match current NumProperties" (debug log)
	unsigned int sum = 0; // port: defined 32-bit checksum wraparound.
	for (int n = 0; n < count; n++)
	{
		char name[64]; s >> name;
		CProperty *p = FindProperty(name);
		int type, size;
		if (p)
		{
			s >> type; p->type = type;
			switch (type)
			{
				case PROP_NONE: s >> size; break;
				case PROP_ALPHA:
					s >> size; s >> (char *)p->data;
					if (size != (int)strlen((char *)p->data))
						ErrorLog("ERROR: Invalid EDIT_ALPHA writing variable %s in object %s", name, m_name);
					break;
				case PROP_BOOL:  s >> size; sum += read4(s, p->data, false); break;
				case PROP_FLOAT: s >> size; sum += read4(s, p->data, true); break;
				case PROP_SPECIFIC: s >> size; sum += read4(s, p->data, false); break;
				case PROP_5: s >> size; break;
				case PROP_LONG: s >> size; sum += read4(s, p->data, false); break;
				default: s >> size; ErrorLog("ERROR: invalid type reading variable %s in object %s", name, m_name);
			}
		}
		else
		{
			// "WARNING: Could not find variable %s in object %s" (debug log); skip the value
			s >> type; s >> size;
			char tmp[132]; int v; float f;
			switch (type)
			{
				case PROP_NONE: if (size) ErrorLog("ERROR: bad EDIT_NONE for %s during load of %s", name, m_name); break;
				case PROP_ALPHA:
					s >> tmp;
					if (size != (int)strlen(tmp)) ErrorLog("ERROR: Invalid EDIT_ALPHA for %s during load of %s", name, m_name);
					break;
				case PROP_BOOL:
					if (size != 4) ErrorLog("ERROR: bad EDIT_BOOL for %s during load of %s", name, m_name);
					s >> v; sum += v; break;
				case PROP_FLOAT:
					if (size != 4) ErrorLog("ERROR: bad EDIT_FLOAT for %s during load of %s", name, m_name);
					s >> f; memcpy(&v, &f, 4); sum += v; break;
				case PROP_SPECIFIC:
					if (size != 4) ErrorLog("ERROR: bad EDIT_SPECIFIC for %s during load of %s", name, m_name);
					s >> v; sum += v; break;
				case PROP_5: break;
				case PROP_LONG:
					if (size != 4) ErrorLog("ERROR: bad EDIT_LONG for %s during load of %s", name, m_name);
					s >> v; sum += v; break;
				default: ErrorLog("ERROR: invalid type reading variable %s in object %s", name, m_name);
			}
		}
	}
	int file_sum; s >> file_sum;
	if ((unsigned int)file_sum != sum) ErrorLog("ERROR: Cheat warning: Invalid FILE checksum");
}

void CGameObject::WriteProperties(OMediaStreamOperators &s)	// 0x473960
{
	unsigned int sum = 0; // port: defined 32-bit checksum wraparound.
	for (std::list<CProperty *>::iterator i = m_properties.begin(); i != m_properties.end(); i++)
	{
		CProperty *p = *i;
		s << p->name;
		s << p->type;
		s.getsize();							// (result unused in the original)
		int size, v;
		switch (p->type)
		{
			case PROP_NONE: case PROP_5: size = 0; s << size; break;
			case PROP_ALPHA: size = strlen((char *)p->data); s << size; s << (char *)p->data; break;
			case PROP_BOOL: case PROP_SPECIFIC: case PROP_LONG:
				size = 4; s << size; memcpy(&v, p->data, 4); s << v; sum += v; break;
			case PROP_FLOAT:
			{
				size = 4; s << size; float f; memcpy(&f, p->data, 4); s << f; memcpy(&v, p->data, 4); sum += v; break;
			}
			default: size = 0; s << size; ErrorLog("ERROR: invalid type writing variable %s", p->name);
		}
	}
	s << sum;
}

// --------------------------------------------------------------------------- identity

// port: The original upper-cases its argument in place (_strupr). Callers pass string literals, which
// MSVC 6 placed in writable memory; here they are read-only, so the upper-casing works on a copy.
// The only difference: the caller's string is not modified.
void CGameObject::SetName(char *tag_in)						// 0x4702d0
{
	std::string tag(tag_in);
	for (std::string::size_type i = 0; i < tag.size(); ++i)
		tag[i] = (char)toupper((unsigned char)tag[i]);	// _strupr
	strcat(m_name, "_");								// 0x4f5fdc
	strcat(m_name, tag.c_str());
	strcpy(m_tag, tag.c_str());
}

bool CGameObject::NameContains(char *s_in)					// 0x470380 (same note as SetName)
{
	std::string s(s_in);
	for (std::string::size_type i = 0; i < s.size(); ++i)
		s[i] = (char)toupper((unsigned char)s[i]);
	return strstr(m_name, s.c_str()) != NULL;
}

void CGameObject::Log(const char *fmt, ...)					// 0x473910 (writes "+%s->%s" to the debug log)
{
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	// port: bound the original vsprintf to the size of its stack buffer.
	vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);
	// 0x472970 consumes no output, but Log still increments this 16-bit counter.
	unsigned short calls;
	memcpy(&calls, &neg::g_data[0x509af0 - neg::DATA_BASE], sizeof calls);
	calls = (unsigned short)(calls + 1);
	memcpy(&neg::g_data[0x509af0 - neg::DATA_BASE], &calls, sizeof calls);
}

// --------------------------------------------------------------------------- lifecycle

void CGameObject::InitObject(void)							// 0x4703b0
{
	Log("InitObject()");
	while (!m_properties.empty()) m_properties.pop_front();	// (the CProperty blocks are not freed here)
	m_num_properties = 0;
	RegisterProperty("ObjectTag",    m_tag, PROP_ALPHA, 0);
	RegisterProperty("RotateToDest", &f_00a, PROP_BOOL, 0);
	RegisterProperty("ObjectID",     &m_object_id, PROP_LONG, 0);
	RegisterProperty("PositionX",    &f_0ec, PROP_FLOAT, 0);
	RegisterProperty("PositionY",    &f_0f0, PROP_FLOAT, 0);
	RegisterProperty("PositionZ",    &f_0f4, PROP_FLOAT, 0);
	RegisterProperty("RotationX",    &f_0fc, PROP_FLOAT, 0);
	RegisterProperty("RotationY",    &f_100, PROP_FLOAT, 0);
	RegisterProperty("RotationZ",    &f_104, PROP_FLOAT, 0);
}

void CGameObject::ResetObject(void)							// 0x470540: current values = initial values
{
	Log("ResetObject()");
	v124(v25());   v126(v29());   v110(v31());   v120(v33());   v122(v49());
	v128(v35());   v130(v37());   v116(v41());   v118(v43());   v114(v39());
	v132(v45());   v136(v51());   v138(v53());
	v140(v55());   v144(v59());   v146(v61());   v148(v63());   v150(v65());	// (no v142: as shipped)
	v152(bg_v67());   v154(v68());
	float t[4];
	v70(t);  v156(t[0], t[1], t[2], t[3]);
	v72(t);  v161(t[0], t[1], t[2], t[3]);
	v74(t);  v166(t[0], t[1], t[2], t[3]);
	v76(t);  v171(t[0], t[1], t[2], t[3]);
	v78(t);  v175(t[0], t[1], t[2], t[3]);
	v80(t);  v180(t[0], t[1], t[2], t[3]);
	v82(t);  v187(t[0], t[1], t[2], t[3]);
	v84(t);  v190(t[0], t[1], t[2], t[3]);
	v86(t);  v193(t[0], t[1], t[2], t[3]);
	v88(t);  v196(t[0], t[1], t[2], t[3]);
	v91(t);  v202(t[0], t[1], t[2], t[3]);
	v206(v95());  v208(v97());  v210(v99());  v212(v101());  v214(v103());  v216(v105());
	m_touching.clear();
	f_274 = 0; f_278 = 0; f_27c = 0;
	f_298 = 0;
	f_005 = 1;
	f_00a = 0;
	f_006 = 0;
}

// --------------------------------------------------------------------------- collisions

// The touch list holds the other objects (PhysicsCollideObjects passes them, as do C3DConveyor,
// C3DMovingBlocks and C3DRope with the player object 0x5099e4).
void CGameObject::v3(CGameObject *other)					// 0x470270
{
	AddTouching(other);									// vtable +0x40
}

bool CGameObject::IsTouching(CGameObject *other)			// 0x470a60
{
	for (std::list<CGameObject *>::iterator i = m_touching.begin(); i != m_touching.end(); i++)
		if (*i == other) return true;
	return false;
}

void CGameObject::AddTouching(CGameObject *other)			// 0x470a90
{
	if (IsTouching(other) || f_006) return;
	m_touching.push_back(other);
	if (f_408 == -1)
		f_408 = GameTypeTouchStart(this, other);		// g_gametype vtable +0x14c
}

void CGameObject::RemoveTouching(CGameObject *other)		// 0x470b20
{
	if (!v131() && v45()) v132(1);
	m_touching.remove(other);
	if (f_410 == -1)
		f_410 = GameTypeTouchEnd(this, other);			// g_gametype vtable +0x150
}

void CGameObject::v18(CGameObject *other)					// 0x470bc0
{
	if (f_40c == -1)
		f_40c = GameTypeTouch154(this, other);			// g_gametype vtable +0x154
}

void CGameObject::Collide(CGameObject *other, float dist)	// 0x470bf0
{
	if (other == this || other->v119()) return;
	if (v131()) v262(other, dist);						// vtable +0x418
	if (v125()) v24(other);								// vtable +0x60
}

void CGameObject::v21(CGameObject *other)					// 0x470e60
{
	other->v23(this);
}

void CGameObject::AddRotation(float x, float y, float z)	// 0x472480: keep each angle in 0..360
{
	float r[3] = { x + f_224, y + f_228, z + f_22c };
	for (int i = 0; i < 3; i++)
	{
		if (r[i] < 0.0f) r[i] = (float)(360 - abs((int)r[i]));
		else if (r[i] > 360.0f) r[i] -= 360.0f;
	}
	f_224 = r[0]; f_228 = r[1]; f_22c = r[2];
}

void CGameObject::v219(float nx, float ny, float nz, float nw, OMedia3DPolygon *poly)	// 0x472690: hit a ceiling
{
	(void)nx; (void)ny; (void)nz; (void)nw; (void)poly;
	float v[4];
	v174(v);											// velocity: stop going up
	if (v[1] > 0.0f) v175(v[0], 0, v[2], v[3]);			// vtable +0x2bc
	v155(v);											// constant acceleration: the same
	if (v[1] > 0.0f) v156(v[0], 0, v[2], v[3]);			// vtable +0x270
}

void CGameObject::v220(float a, float b, float c, float d, OMedia3DPolygon *poly)	// 0x458890
{
	(void)poly;
	if (v117()) v221(a, b, c, d);
}

// 0x472980: rotate (x, y, z) by the object's angles v201 (degrees; OMT's 14-bit sin/cos tables)
// and add the position. Rotation order as compiled: z, then x, then y.
OMedia3DVector CGameObject::v224(float x, float y, float z)
{
	float rot[4];
	v201(rot);											// vtable +0x324
	const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
	omt_Angle ax = (omt_Angle)((int)(rot[0] * kDegToAngle) & omc_MaxAngleMask);
	omt_Angle ay = (omt_Angle)((int)(rot[1] * kDegToAngle) & omc_MaxAngleMask);
	omt_Angle az = (omt_Angle)((int)(rot[2] * kDegToAngle) & omc_MaxAngleMask);
	OMediaCosSinTable &t = OMediaCosSinTable::global;
	float sx = t.sin(ax), cx = t.cos(ax), sy = t.sin(ay), cy = t.cos(ay), sz = t.sin(az), cz = t.cos(az);
	float a = cz * y - sz * x;							// about z
	float b = sz * y + cz * x;
	float c = cx * z - a * sx;							// about x
	float rx = b * cy - c * sy;							// about y
	float ry = a * cx + sx * z;
	float rz = c * cy + b * sy;
	float p[4];
	v195(p);											// position, vtable +0x30c
	return OMedia3DVector(p[0] + rx, p[1] + ry, p[2] + rz, 1.0f);
}

// 0x472b10: as v224 without the position: (x, y, z) rotated by the object's angles v201, w 0.
OMedia3DVector CGameObject::v226(float x, float y, float z)
{
	float rot[4];
	v201(rot);											// vtable +0x324
	const float kDegToAngle = 45.511112213134766f;		// 0x48e5e8: 16384 / 360
	omt_Angle ax = (omt_Angle)((int)(rot[0] * kDegToAngle) & omc_MaxAngleMask);
	omt_Angle ay = (omt_Angle)((int)(rot[1] * kDegToAngle) & omc_MaxAngleMask);
	omt_Angle az = (omt_Angle)((int)(rot[2] * kDegToAngle) & omc_MaxAngleMask);
	OMediaCosSinTable &t = OMediaCosSinTable::global;
	float sx = t.sin(ax), cx = t.cos(ax), sy = t.sin(ay), cy = t.cos(ay), sz = t.sin(az), cz = t.cos(az);
	float a = cz * y - sz * x;							// about z
	float b = sz * y + cz * x;
	float c = cx * z - a * sx;							// about x
	return OMedia3DVector(b * cy - c * sy, a * cx + sx * z, c * cy + b * sy, 0.0f);	// about y
}

unsigned char CGameObject::v231(CGameObject *other)		// 0x472f80: PhysicsCollideObjects asks both objects
{
	(void)other;
	return 1;
}

// floor polygon and shape (PhysicsSnapToGround): initial values f_10c / f_114, current f_234 / f_23c
OMedia3DPolygon *CGameObject::v95(void)			{ return f_10c; }						// 0x471740
void CGameObject::v96(OMedia3DPolygon *poly)	{ f_10c = poly; v206(poly); }			// 0x471750
OMedia3DShape *CGameObject::v99(void)			{ return f_114; }						// 0x4717a0
void CGameObject::v100(OMedia3DShape *shape)	{ f_114 = shape; v210(shape); }			// 0x4717b0
OMedia3DPolygon *CGameObject::v205(void)		{ return f_234; }						// 0x472570
void CGameObject::v206(OMedia3DPolygon *poly)	{ f_234 = poly; }						// 0x472580
OMedia3DShape *CGameObject::v209(void)			{ return f_23c; }						// 0x4725b0
void CGameObject::v210(OMedia3DShape *shape)	{ f_23c = shape; }						// 0x4725c0

void CGameObject::Link(CGameObject *other)					// 0x472f90
{
	if (!other) return;
	other->m_linked.push_back(this);
	m_linked.push_back(other);
}

void CGameObject::Unlink(CGameObject *other)				// 0x473040
{
	if (!other) return;
	other->m_linked.remove(this);
	m_linked.remove(other);
}

void CGameObject::DisableCollisionID(int id)				// 0x4730f0
{
	Log("DisableCollisionID(%ld)", id);
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
	{
		CGameObject *o = *i;
		if (!o || o->m_object_id != id) continue;
		bool linked = false;
		for (std::list<CGameObject *>::iterator j = m_linked.begin(); j != m_linked.end(); j++)
			if (*j == o) { linked = true; break; }
		if (!linked) Link(o);							// vtable +0x3a4 (slot 233)
	}
}

void CGameObject::EnableCollisionID(int id)					// 0x473170
{
	Log("EnableCollisionID(%ld)", id);
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
		if (*i && (*i)->m_object_id == id) Unlink(*i);	// vtable +0x3ac (slot 235)
}

// --------------------------------------------------------------------------- per frame

void CGameObject::v4(float t)								// 0x470280
{
	f_294 = t;
	if (t != 0.0f)
	{
		if (EditorIsEditing()) v242(t);					// vtable +0x3c8
		else Process(t);								// vtable +0x3c0
	}
}

void CGameObject::Process(float dt)							// 0x473240
{
	CheckSounds(dt);									// vtable +0x3d4
	if (f_298 > 0.0f)
	{
		f_298 -= dt;
		if (f_298 <= 0.0f) ResetObject();				// vtable +0x28
	}
	RotateToDest(dt);									// vtable +0x3d8
}

void CGameObject::CheckSounds(float dt)						// 0x4732b0: forget channels that stopped
{
	(void)dt;
	if (f_408 != -1 && !IsChannelPlaying(f_408)) f_408 = -1;
	if (f_40c != -1 && !IsChannelPlaying(f_40c)) f_40c = -1;
	if (f_410 != -1 && !IsChannelPlaying(f_410)) f_410 = -1;
}

static float wrap180(float d)
{
	if (d > 180.0f) return d - 360.0f;
	if (d < -180.0f) return d + 360.0f;
	return d;
}

void CGameObject::RotateToDest(float dt)					// 0x473320: turn towards f_274.. at rate f_29c
{
	if (!GameIsRunning() || !f_005 || !f_00a) return;	// 0x475ca0
	float dx = f_274, dy = f_278, dz = f_27c;
	float ry = 0, rx = 0, rz = 0;
	f_00a = 0;
	if (dx != f_224) { ry = wrap180(dx - f_224); f_00a = 1; }
	if (dy != f_228) { rx = wrap180(dy - f_228); f_00a = 1; }
	if (dz != f_22c) { rz = wrap180(dz - f_22c); f_00a = 1; }
	float k = dt * f_29c;
	if (k <= fabsf(k * ry)) f_224 = k * ry + f_224; else f_224 = dx;
	if (k <= fabsf(k * rx)) f_228 = k * rx + f_228; else f_228 = dy;
	if (fabsf(rz * k) < k) f_22c = dz; else f_22c = rz * k + f_22c;
}

// 0x473540: the element's angles (OMT units, 0..0x3fff) from the rotation in degrees (slot 249,
// SetElementAngles). Each angle: _CIfmod(deg, 360.0) * (16384 / 360.0), _ftol, & 0x3fff.
void CGameObject::v248(void)
{
	float r[4];
	v201(r);												// vtable +0x328 (once for all three angles)
	const short az = (short)((int)(fmod((double)r[2], 360.0) * 45.51111111111111) & 0x3fff);	// 0x4d6410, 0x4d6408
	const short ay = (short)((int)(fmod((double)r[1], 360.0) * 45.51111111111111) & 0x3fff);
	const short ax = (short)((int)(fmod((double)r[0], 360.0) * 45.51111111111111) & 0x3fff);
	v249(ax, ay, az);										// vtable +0x3e4
}

// --------------------------------------------------------------------------- sphere response

// 0x4740c0: touching `other` (centre distance `dist`): pushed away by it, unless it is a C3DWHEEL.
// (The original pushes `dist` for slot 263 before it calls slot 151, which takes no argument.)
void CGameObject::v262(CGameObject *other, float dist)
{
	if (other->NameContains((char *)"C3DWHEEL")) return;	// vtable +0x18
	float p[4];
	other->v195(p);											// +0x30c
	const float r = other->v151();							// +0x25c radius
	v263(other, OMedia3DVector(p[0], p[1], p[2], p[3]), r, dist);	// +0x41c
}

// 0x474140: the push between two touching spheres. Unless this object is fixed (slot 93 = 2) or
// has no physics, it is pushed out along the line of centres by the overlap (half of it unless
// the other one is fixed; never downwards): the velocity gets overlap * normal * overlap (sic),
// the position is moved against the terrain by overlap * normal. Unless slot 129, a moving other
// also spins it round Y: (its direction turned 90 degrees round Y . normal) * its speed * 0.05.
void CGameObject::v263(CGameObject *other, OMedia3DVector pos, float r_other, float dist)
{
	if (v93() == 2) return;									// +0x174
	const float s = v151() + r_other;						// +0x25c
	if (!(s > dist)) return;
	if (!v131()) return;									// +0x20c
	float me[4];
	v195(me);
	OMedia3DVector n(me[0] - pos.x, me[1] - pos.y, me[2] - pos.z, 0.0f);
	if (!(n.quick_magnitude() > 0.0f)) return;				// 0x48d914 (an unordered result returns too)
	float inv = 1.0f / n.quick_magnitude();					// 0x48d924
	n.x = inv * n.x; n.y = inv * n.y; n.z = inv * n.z;
	const OMedia3DVector normal = n;
	float k = s - dist;
	if (other->v93() != 2) k = k * 0.5f;					// 0x48d948
	n.x = n.x * k;
	if (n.y > 0.0f) n.y = n.y * k;
	else n.y = 0;
	n.z = n.z * k;
	v178(n.x * k, n.y * k, n.z * k);						// +0x2c8 velocity +=
	float p[4];
	v195(p);
	OMedia3DVector at(p[0], p[1], p[2], p[3]);
	PhysicsMoveAgainstTerrain(this, &at, n, true);			// 0x47b4b0
	v196(at.x, at.y, at.z, at.w);							// +0x310
	if (v129()) return;										// +0x204
	float ov[4];
	other->v174(ov);										// +0x2b8 velocity
	OMedia3DVector u(ov[0], ov[1], ov[2], ov[3]);
	const float m = u.quick_magnitude();
	if (!(m > 0.0f)) return;
	inv = 1.0f / u.quick_magnitude();
	u.x = inv * u.x; u.y = inv * u.y; u.z = inv * u.z;
	u.rotate(0, 0x1000, 0);									// (inline, OMediaCosSinTable)
	const float spin = (u.z * normal.z + u.y * normal.y + u.x * normal.x) * m * 0.05f;	// 0x495334
	v173(0, spin, 0);										// +0x2b4 spin +=
}

// 0x472750: bounce off a surface of normal (a, b, c): the horizontal velocity is mirrored about
// the normal (with the length it had, less the slot 145 share), the vertical one kept, and the
// object spins round Y by the turn (degrees, wrapped to -180..180) * speed * 0.001. `d` is unused.
void CGameObject::v221(float a, float b, float c, float d)
{
	(void)d;
	float v[4];
	v174(v);												// +0x2b8 velocity
	OMedia3DVector h(v[0], 0.0f, v[2], v[3]);
	const float speed = h.quick_magnitude();
	float inv = 1.0f / h.quick_magnitude();					// 0x48d924
	h.x = inv * h.x; h.y = inv * h.y; h.z = inv * h.z;
	const float dot = -(h.z * c + a * h.x + b * h.y);
	OMedia3DVector r(a * dot + a * dot + h.x, dot * b + dot * b + h.y, c * dot + c * dot + h.z, 0.0f);
	r.x = r.x * speed;
	float cur[4];
	v174(cur);
	r.y = cur[1];											// the vertical velocity stays
	r.z = r.z * speed;
	const float f = v145();									// +0x244
	v176(r.x - f * r.x, r.y, r.z - f * r.z);				// +0x2c0
	omt_Angle ax, ay;
	h.angles(ax, ay);										// the old heading
	const float before = (float)(short)ay * 0.02197265625f;	// 0x48e5b4: 360 / 16384
	r.y = 0;
	inv = 1.0f / r.quick_magnitude();
	r.x = inv * r.x; r.y = inv * r.y; r.z = inv * r.z;
	r.angles(ax, ay);										// the new one
	float turn = before - (float)(short)ay * 0.02197265625f;
	if (turn != 0.0f)
	{
		if (turn > 180.0f) turn -= 360.0f;					// 0x48e5b0, 0x48e5c4
		else if (turn < -180.0f) turn += 360.0f;			// 0x48e5c0
	}
	v173(0, turn * speed * 0.001f, 0);						// +0x2b4: spin; 0x4d5fd8
}

// Two accessors the decompilation's function list misses (so gen_class.py cannot generate them).
void CGameObject::UnInitObject(void)										// 0x470500
{
	Log("UnInitObject()");
}

unsigned char CGameObject::v133(void)										// 0x471a10
{
	return f_13f;
}

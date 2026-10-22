/*  C3DCube - see C3DCube.h. Reconstructed from Neutron.exe 0x461320..0x461990. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DCube.h"
#include "../Todo.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include "OMedia3DShape.h"
#include "OMedia3DMaterial.h"
#include "OMediaCanvas.h"
#include "OMediaDataBase.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"

using namespace neg;

// 0x477b40 "Load3DMaterial(%d)": material <id> of db ('3DMa'), unlocked.
static OMedia3DMaterial *Load3DMaterial(OMediaDataBase *db, int id)
{
	OMedia3DMaterial *m = NULL;
	if (db)
	{
		m = (OMedia3DMaterial *)db->get_object('3DMa', id);
		if (g_509a12) NEG_TODO(0x477b40, "debug database call (db vtable +0x4c)");
		if (m) m->db_unlock();
	}
	return m;
}

static std::string port_path(const std::string &dos)
{
	std::string s(dos);
	if (s.compare(0, 2, ".\\") == 0) s.erase(0, 2);
	for (size_t i = 0; i < s.size(); i++) if (s[i] == '\\') s[i] = '/';
	return s;
}

// 0x477630: a canvas stored in a file of its own (as C3DSphere.cpp's copy). Errors (no file)
// are not caught: OMT's exception goes to the caller, as in the original.
static OMediaCanvas *LoadCanvasFile(std::string name)
{
	OMediaFilePath path(port_path(name));
	OMediaFileStream fs;
	fs.setpath(&path);
	fs.open(omcfp_Read, false, false);
	OMediaCanvas *canvas = new OMediaCanvas;
	fs >> (OMediaClassStreamer *)canvas;
	return canvas;
}

// 0x4779c0 "Load3DMaterial(\"%s\")": a new material textured with the canvas file <name>.
static OMedia3DMaterial *Load3DMaterial(std::string name)
{
	OMediaCanvas *canvas = LoadCanvasFile(name);
	OMedia3DMaterial *m = new OMedia3DMaterial;
	m->set_texture(canvas);									// material vtable +0x20
	return m;
}

// --------------------------------------------------------------------------- construction

C3DCube::C3DCube()											// 0x461320
{
	SetName((char *)"C3DCUBE");
	Log("C3DCube()");
	C3DCube::InitObject();									// (inlined)
	f_58c = NULL;
	v107(0);
}

C3DCube::~C3DCube() {}										// 0x461440

namespace neg {
CGameObject *New_C3DCube(void) { return new C3DCube; }		// '3CUB' (0x598 bytes)
}

// --------------------------------------------------------------------------- element slots

void C3DCube::ev54(OMediaDataBase *db, int id, float size)	// 0x4618e0
{
	get_shape()->make_cube(size, false);					// shape vtable +0xa8
	get_shape()->set_material(Load3DMaterial(db, id));		// shape vtable +0x70
	FitToShape();											// element +200
}

void C3DCube::InitCube(OMedia3DVector argb, float size)		// 0x4617f0
{
	Log("InitCube(argb,%f)", (double)size);
	get_shape()->make_cube(size, false);					// shape vtable +0xa8
	OMedia3DMaterial *m = new OMedia3DMaterial;
	f_58c = m;
	// The four values are copied as they are to the material's ambient colour (+0x6c, in memory
	// order red, green, blue, alpha), as C3DSphere does.
	m->set_ambient(OMediaFARGBColor(argb.w, argb.x, argb.y, argb.z));
	f_58c->set_light_mode(ommlmc_Light);					// +0x2c = 2
	get_shape()->set_material(f_58c);						// shape vtable +0x70
	FitToShape();											// element +200
}

void C3DCube::InitCube(std::string file, float size)		// 0x461600
{
	Log("InitCube(\"%s\",%f)", file.c_str(), (double)size);
	get_shape()->make_cube(size, false);					// shape vtable +0xa8
	OMedia3DMaterial *m;
	if (file.length() == 0)
	{
		m = new OMedia3DMaterial;
		f_58c = m;
		f_58c->set_shade_mode(omshademc_Gouraud);			// +0x28 = 1
		f_58c->set_light_mode(ommlmc_Light);				// +0x2c = 2
		m = f_58c;
	}
	else m = Load3DMaterial(file);							// 0x4779c0 (not kept in f_58c: never freed)
	get_shape()->set_material(m);							// shape vtable +0x70
	FitToShape();											// element +200
}

// --------------------------------------------------------------------------- CGameObject family

void C3DCube::InitObject(void)								// 0x4614e0
{
	Log("InitObject()");
	C3DObject::InitObject();								// 0x462340
	set_shape(new OMedia3DShape);							// element +0xac
	SetObjectID('3CUB');									// element +0xc4
}

void C3DCube::UnInitObject(void)							// 0x461580
{
	Log("UnInitObject()");
	OMedia3DShape *sh = get_shape();						// element +0xb0
	set_shape(NULL);										// element +0xac
	if (sh) delete sh;
	if (f_58c)
	{
		delete f_58c;
		f_58c = NULL;
	}
	C3DObject::UnInitObject();
}

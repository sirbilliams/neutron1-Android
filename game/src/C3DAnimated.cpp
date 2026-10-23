/*  C3DAnimated - see C3DAnimated.h. */
#include "C3DAnimated.h"
#include "Game.h"
#include "Bridge.h"
#include "Todo.h"
#include "CJimmyGame.h"
#include "CEditor.h"
#include "gen/globals.h"
#include "OMediaWorld.h"
#include "OMediaLayer.h"
#include "OMediaViewPort.h"
#include "OMedia3DShape.h"
#include "OMedia3DPolygon.h"
#include "OMedia3DMaterial.h"
#include "OMedia3DMorphAnim.h"
#include "OMedia3DShapeConverter.h"
#include "OMediaCanvas.h"
#include "OMediaDataBase.h"
#include "OMediaMemStream.h"
#include "OMediaHDPack.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include "OMediaStreamOperators.h"
#include "OMediaStringField.h"
#include <string.h>
#include <strings.h>

using namespace neg;

// --------------------------------------------------------------------------- helpers

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// The original reads C3DPickupItem's member +0x6d8 (game-object offset; object +0x7a0), the
// item's index into g_object_states (pickups/CPickupType.cpp).
namespace neg { int PickupIndexOf(CGameObject *o); }
static int PickupItemIndex(CGameObject *item)	{ return neg::PickupIndexOf(item); }

// --------------------------------------------------------------------------- construction

C3DAnimated::C3DAnimated()									// 0x40d110
{
	SetName((char *)"C3DANIMATED");
	Log("C3DAnimated()");
	f_59c = -1;
	f_5a4 = NULL;
	f_640 = NULL;
	f_5a0 = 0;
	f_5a8 = 0;
	f_594 = -1;
	strcpy(f_5fc, g_4f81a8);
	f_598 = 0;
	for (int i = 0; i < 10; i++) { f_5ac[i] = NULL; f_5d4[i] = NULL; }
	f_63c = 0;
	f_648 = 1;
	f_649 = 1;
	f_64c = -1;												// RequiredLevel
	f_650 = -1;												// ExactLevel
	f_654 = -1;												// RemoveLevel
	f_658 = -1;												// HasCollision
	f_65c = -1;												// InitiallyVisible
	f_660 = 1;												// CanMove
	f_664 = 0;												// SecondPass
	f_668 = 0;
	strcpy(f_669, "none");									// PickupLink
	f_6cd = 0;
	f_58c = NULL;											// port: not set by the original until InitAnim
	f_590 = NULL;											// port: idem
}

C3DAnimated::~C3DAnimated()									// 0x40d300 (only resets the vtables, then ~C3DObject)
{
}

// --------------------------------------------------------------------------- CGameObject family

void C3DAnimated::InitObject(void)							// 0x40d3c0
{
	C3DObject::InitObject();
	RegisterProperty("RequiredLevel", &f_64c, PROP_LONG, 0);
	RegisterProperty("ExactLevel", &f_650, PROP_LONG, 0);
	RegisterProperty("RemoveLevel", &f_654, PROP_LONG, 0);
	RegisterProperty("HasCollision", &f_658, PROP_LONG, 0);
	RegisterProperty("InitiallyVisible", &f_65c, PROP_LONG, 0);
	RegisterProperty("CanMove", &f_660, PROP_LONG, 0);
	RegisterProperty("SecondPass", &f_664, PROP_LONG, 0);
	RegisterProperty("PickupLink", f_669, PROP_ALPHA, 0);
}

void C3DAnimated::UnInitObject(void)						// 0x40e670
{
	if (OMedia3DShape *sh = get_shape())					// element +0xb0
	{
		sh->set_material(NULL);								// shape vtable +0x70
		sh->set_modified(false);							// +8
	}
	C3DObject::UnInitObject();								// 0x462390
	if (f_63c)												// InitAnim done (+0x570)
	{
		set_anim_def(NULL);									// element +0xc0
		// (debug log)
		for (int i = 0; i < 10; i++)
		{
			// port: the original deletes the canvas (f_5ac) before the material (f_5d4) that
			// still points at it; the material's destructor then reads the freed canvas.
			// Windows' heap tolerated it, Android's allocator aborts: material first.
			if (f_5d4[i]) { delete f_5d4[i]; f_5d4[i] = NULL; }
			if (f_5ac[i]) { delete f_5ac[i]; f_5ac[i] = NULL; }
		}
		// Quirk: frees every AnimList but the last one, and f_5a4 keeps pointing at the first.
		AnimList *a = f_5a4;
		if (a && a->next)
		{
			do
			{
				AnimList *dead = a;
				a = a->next;
				delete dead;
			} while (a->next);
		}
		// port: the original leaves f_5a4 dangling and the InitAnim flag set, so a second call
		// (the destructors here call it again after CheckLoadLevel did) frees the list twice.
		f_5a4 = NULL;
		f_640 = NULL;
		f_63c = 0;
	}
}

void C3DAnimated::Process(float dt)							// 0x40e050
{
	float v[4];
	if (!GameIsRunning()) return;							// 0x475ca0
	if (!f_6cd)
	{
		f_6cd = 1;
		CGameObject *item;
		if (strcasecmp(f_669, "none") != 0 && (item = FindObjectByTag(f_669)) != NULL &&
			item->NameContains((char *)"C3DPICKUPITEM"))
		{
			const int idx = PickupItemIndex(item);			// item +0x5f4
			if (idx >= 0 && g_object_states[idx] != 0)		// 0x4f8438; port: idx < 0 = not known yet
			{
				HideAgain();								// element +0x58
				v232();
				v132(0);
			}
		}
	}
	C3DObject::Process(dt);
	if (f_660 == 0)											// CanMove 0: kept at the start position and rotation
	{
		v88(v);	v196(v[0], v[1], v[2], v[3]);
		v91(v);	v202(v[0], v[1], v[2], v[3]);
	}
	if (f_648 && f_649)
	{
		f_598 += dt;
		if (!getplay_loop() && f_640 && getcurrentframe_pos() >= f_640->nframes - 1)	// OMediaAnim +0xad, vtable +0x1c
			ev67();													// element +0x104
	}
}

void C3DAnimated::v242(float t)								// 0x40d3a0: while editing, always shown
{
	(void)t;
	ShowAgain();											// hide count 1, hide(false)
}

void C3DAnimated::v260(void)								// 0x40e7b0
{
	// (debug log)
	if (f_65c == 0) HideAgain();							// InitiallyVisible
	else if (f_65c == 1) ShowAgain();
	if (f_664 == 1) AlphaSecondPass();						// SecondPass: element +0x118
}

void C3DAnimated::v267(int level)							// 0x40e340
{
	CLocalGameObject::v267(level);
	if (f_650 == -1)
	{
		if (f_64c == -1) return;
		if (f_64c <= level)
		{
			if (f_654 == -1) { ev70(1); return; }
			if (level < f_654) { ev70(1); return; }
		}
	}
	else if (level == f_650)
	{
		ev70(1);
		return;
	}
	ev70(0);
}

void C3DAnimated::v268(int state)							// 0x40e1f0
{
	CLocalGameObject::v268(state);
	// (debug log "Calling NextState for %s at %d", empty)
	if (state == 1)
	{
		if (f_658 != 0) v234();
		ShowAgain();
	}
	else if (state == 0)
	{
		v232();
		HideAgain();
	}
}

void C3DAnimated::v277(void) { PauseAnim(1); }					// 0x40e770
void C3DAnimated::v278(void) { PauseAnim(0); }					// 0x40e790

// --------------------------------------------------------------------------- morph animations

void C3DAnimated::CreateAnim(char *name, char *file)		// 0x40d4a0
{
	AnimList *a;
	bool first = (f_5a4 == NULL);
	if (first)
	{
		// (debug log "CreateAnim1 : %s   ", empty)
		a = f_5a4 = new AnimList;
	}
	else
	{
		AnimList *last = f_5a4;
		while (last->next) last = last->next;
		a = last->next = new AnimList;
	}
	if (!a)
	{
		// (debug log "Unable to allocate AnimList in Anim3D_CreateAnim", empty)
		return;
	}
	strcpy(a->name, g_4f81a8);
	a->next = NULL;
	a->id = f_5a0;
	char path[112];
	strcpy(path, file);
	if (CJimmyGame *g = dynamic_cast<CJimmyGame *>(g_gametype))	// port: the original assumes a CJimmyGame
		g->FileIsThere(path, "ase", "ase");					// game element +0x16c: path = ase\<file> (else c:\ase\<file>)
	OMediaFilePath fp(std::string(path), 'null', 'null');
	OMediaFileStream fs;
	fs.setpath(&fp);
	if (!fs.fileexists())
	{
		ErrorLog("ERROR: file %s does not exist for %s", file, m_tag);
		fs.close();
		return;
	}
	fs.open(omcfp_Read, false, false);
	// first animation: a new morph definition, the next ones add a sequence to it
	OMedia3DShapeConverter::set_anim_conv_mode_mode(first ? omc3danconv_AlwaysCreateNewMorphAnimDef : omc3danconv_AddSequence);
	OMedia3DShapeConverter::set_object_convert_mode(omc3doconv_OneShapePerGroup);
	m_db->generate_from_stream(fs, name[0] == 'L' && f_58c ? f_58c : f_590);	// database vtable +0x2c; port: "LOW..." into the full shape while the LOW one (f_58c) is NULL
	if (first) m_db->set_dbsize_dirty();					// +0x4c
	fs.close();
	// The original reads related_morphdefs[0] whether or not this file made a morph definition.
	// A static .ase (no *MESH_ANIMATION, e.g. the intro logo letters) makes none; the converter
	// only erased the vector, and MSVC 6 keeps the old element in place, so the original picks
	// up the id the previous animated file left there (another object's definition).
	// port: keep that id ourselves instead of reading past the end of an empty vector, and do
	// nothing if no animated file was ever converted (the original would crash). Chunk id 0 is
	// a valid id (the first definition in a database), so "none yet" is a flag of its own.
	static omt_FSChunkID last_morph_def = 0;
	static bool have_morph_def = false;
	if (!OMedia3DShapeConverter::related_morphdefs.empty())
	{
		last_morph_def = OMedia3DShapeConverter::related_morphdefs[0].morph_def;
		have_morph_def = true;
	}
	OMedia3DMorphAnimDef *def = !have_morph_def ? NULL :
		(OMedia3DMorphAnimDef *)m_db->get_object(OMedia3DMorphAnimDef::db_type,	// +0x14
		last_morph_def);									// port: 2002 kept the chunk ids only
	a->def = def;
	if (def) def->db_unlock();							// port: NULL check
	f_5a0++;
	g_4f81a4++;
	if (strlen(name) > 0x3f)
	{
		// (debug log "Truncated Animname %s in Anim3D_CreateAnim", empty)
		name[0x3f] = 0;										// (writes into the caller's string)
	}
	strcpy(a->name, name);
}

AnimList *C3DAnimated::GetSequence(char *name)				// 0x40d9e0
{
	if (f_649 && f_648)
		for (AnimList *a = f_5a4; a; a = a->next)
			if (strcasecmp(name, a->name) == 0) return a;
	return NULL;
}

void C3DAnimated::ev58(char *name, unsigned char loop)		// 0x40dd90
{
	if (!f_648 || !f_649) return;
	char buf[80];
	f_598 = 0;
	strcpy(buf, g_4f81a8);
	// The detail level picks the base shape and the name prefix: "HI" + name, or "LOW" + name.
	if (f_594 == 0 || f_594 == 1)
	{
		const float r = v151();								// the shape change below resets the radius
		OMedia3DShapeElement::set_shape(f_594 == 0 || !f_58c ? f_590 : f_58c);	// port: the LOW shape is never loaded (NULL): the full one
		FitToShape();										// element +0xc8
		if (f_124 != 2) v152(r);							// not a box: keep the radius
		strcpy(buf, f_594 == 0 ? "HI" : "LOW");				// 0x4ed3dc / 0x4ed3e0
		strcpy(f_5fc, name);
	}
	strcat(buf, name);
	AnimList *a = GetSequence(buf);							// element +0xdc
	if (!a && buf[0]) a = GetSequence(name);				// port: classes still in Negatron form create unprefixed names
	if (!a) return;
	f_640 = a;
	strcpy(f_5fc, name);
	SetSequence(a->id, loop);								// element +0xe4
	set_anim_def(a->def);									// element +0xc0
}

void C3DAnimated::SetSequence(int seq, unsigned char loop)	// 0x40da30
{
	if (!f_649 || seq < 0) return;
	f_59c = seq;
	setcurrentsequence(seq, loop != 0);						// OMediaAnim +0x90 vtable +0x10
	setplay_loop(loop != 0);								// +0xad
	setplay_timebased(true);								// +0x24
	AnimList *a = f_640;
	OMediaAnimDef *def = a ? a->def : NULL;
	if (a && def && def->getnsequences() != 0)
		a->nframes = (int)(*def->getsequence_list())[current_sequence]->size();	// list +8
}

char *C3DAnimated::GetAnim(void)							// 0x40dab0
{
	if (f_649 && f_648)
	{
		for (AnimList *a = f_5a4; a; a = a->next)
			if (a->id == f_59c) return a->name;
		// (debug log "Animation  not found in Anim3D_GetAnim for %s", empty)
	}
	return g_4f81a8;										// port: the original returns NULL (its callers would _stricmp it)
}

int C3DAnimated::GetAnimCount(void) { return f_5a0; }		// 0x40db10

// 0x40df90: the shared in-memory database (a memory stream 0x4f8410 and its database 0x4f840c),
// made once. Nothing in Boy Genius calls it.
void C3DAnimated::InitMemDB(void)
{
	if (!g_anim_mem_stream) g_anim_mem_stream = new OMediaMemStream;
	if (!g_anim_mem_db) g_anim_mem_db = new OMediaDataBase(g_anim_mem_stream);
}

unsigned char C3DAnimated::IsAnim(char *name)				// 0x40e1d0
{
	return strcasecmp(name, f_5fc) == 0;
}

void C3DAnimated::CreateTexture(char *file, short index)	// 0x40db20
{
	char path[112];
	strcpy(path, "png\\");
	strcat(path, file);
	OMediaFilePath fp(std::string(path), 'null', 'null');
	OMediaFileStream fs;
	fs.setpath(&fp);
	fs.open(omcfp_Read, false, false);						// (no fileexists test: OMT throws)
	OMediaCanvas *c = new OMediaCanvas;
	f_5ac[index] = c;										// (debug log "OMediaCanvas 2 " + tag, empty)
	fs >> (OMediaClassStreamer *)c;
	fs.close();
	c->set_filtering(omtfc_Nearest, omtfc_Linear);			// canvas +0x60 = 0, +0x64 = 1
	OMedia3DMaterial *m = new OMedia3DMaterial;
	f_5d4[index] = m;										// (debug log "OMedia3DMaterial 2 " + tag, empty)
	m->set_blend(omblendfc_One, omblendfc_Zero);			// +0x30 = 1, +0x34 = 0
	m->set_texture(c);										// vtable +0x20
}

void C3DAnimated::ApplyTexture(OMedia3DShape *shape, short index)	// 0x40dd60
{
	if (shape) shape->set_material(f_5d4[index]);			// shape vtable +0x70
}

void C3DAnimated::ev65(short index, int src, int dst)		// 0x40dd40
{
	f_5d4[index]->set_blend((omt_BlendFunc)src, (omt_BlendFunc)dst);	// +0x30 / +0x34
}

char *C3DAnimated::ev66(void) { return f_5fc; }				// 0x40df80

void C3DAnimated::InitAnim(void)							// 0x40e270
{
	if (m_db) return;
	OpenObjectDatabase();									// CGameObject vtable +0x448
	OMediaDataBase::register_object(OMediaCanvas::db_type, OMediaCanvas::db_builder);
	OMediaDataBase::register_object(OMedia3DShape::db_type, OMedia3DShape::db_builder);
	OMediaDataBase::register_object(OMedia3DMaterial::db_type, OMedia3DMaterial::db_builder);
	OMediaDataBase::register_object(OMedia3DMorphAnimDef::db_type, OMedia3DMorphAnimDef::db_builder);
	f_594 = 0;
	OMedia3DShape *sh = (OMedia3DShape *)m_db->get_object(OMedia3DShape::db_type, (omt_FSChunkID)0);
	f_590 = sh;
	f_58c = 0;
	set_morph_flags(ommorf_InterpolateVertices | ommorf_InterpolateVNormals);	// element +0xb8 = 3
	sh->set_modified(false);								// +8
	f_590->db_unlock();										// +0x14
	f_63c = 1;
	ev69();
}

void C3DAnimated::ev70(unsigned char show)					// 0x40e3e0
{
	if (!show)
	{
		HideAgain();
		if (f_658 != -1) { v232(); v132(0); }
		return;
	}
	ev69();
	ShowAgain();
	if (f_658 == -1) return;
	if (f_658 != 0) { v234(); v132(1); }
	else { v232(); v132(0); }
}

void C3DAnimated::ev71(float alpha)							// 0x40e4a0: > 0 additive glow at that alpha (second pass), else opaque
{
	OMedia3DShape *sh = f_590;								// the HI base shape
	if (!sh) return;
	omt_PolygonList *polys = sh->get_polygons();
	for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); ++p)
	{
		OMedia3DMaterial *m = (*p).get_material();
		if (!m) continue;
		if (alpha <= 0.0f)
		{
			m->set_flags(m->get_flags() & ~ommatf_SecondPass);
			m->set_diffuse(OMediaFARGBColor(1.0f, 1.0f, 1.0f, 1.0f));	// +0x4c..+0x58
			m->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
		}
		else
		{
			m->set_diffuse(OMediaFARGBColor(alpha, 1.0f, 1.0f, 1.0f));
			m->set_blend(omblendfc_Src_Alpha, g_windowed ? omblendfc_Inv_Src_Alpha : omblendfc_One);	// 0x509a13
			m->set_flags(m->get_flags() | ommatf_SecondPass);
		}
	}
}

void C3DAnimated::AlphaSecondPass(void)						// 0x40e5e0
{
	OMedia3DShape *sh = f_648 ? f_590 : get_shape();		// animated: the HI base shape
	if (!sh) return;
	omt_PolygonList *polys = sh->get_polygons();
	for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); ++p)
	{
		OMedia3DMaterial *m = (*p).get_material();
		if (!m) continue;
		m->set_flags(m->get_flags() | ommatf_SecondPass);
		m->set_blend(omblendfc_Src_Alpha, omblendfc_Inv_Src_Alpha);
	}
}

void C3DAnimated::PauseAnim(unsigned char pause)					// 0x40d350
{
	if (!pause)
	{
		if (f_668) { OMediaAnim::pause(false); f_668 = 0; }
	}
	else if (!f_668)
	{
		OMediaAnim::pause(true);
		f_668 = 1;
	}
}

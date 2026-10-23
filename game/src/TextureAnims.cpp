/*  Texture animations - see TextureAnims.h. Reconstructed from Neutron.exe 0x468ec0 (scan a
    shape's textures), 0x4690c0 (add one), 0x4693a0 (play), 0x468d50 (free), the list at 0x509810.                         */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "TextureAnims.h"
#include "Todo.h"
#include "gen/globals.h"
#include "OMediaDataBase.h"
#include "OMediaCanvas.h"
#include "OMedia3DMaterial.h"
#include "OMedia3DShape.h"
#include "OMedia3DPolygon.h"
#pragma GCC diagnostic pop
#include <list>
#include <string>

using namespace neg;

namespace {

// 0x2c bytes in the original: the texture's name (+0x00), the material (+0x10), the frames
// (+0x14, the material's own texture first), the time in the frame (+0x20), the frame time
// (+0x24) and the frame shown (+0x28).
struct TextureAnim
{
	std::string							name;
	OMedia3DMaterial					*material;
	std::list<OMediaCanvas *>			frames;
	float								time;
	float								frame_time;
	std::list<OMediaCanvas *>::iterator	current;
};
std::list<TextureAnim *> g_texture_anims;				// 0x509810

// The name of a database object's chunk ("" without one; 0x4f81a8 is an empty string).
std::string ChunkName(OMediaDBObject *o)
{
	std::string n;
	o->get_chunk_name(n);
	return n;
}

// The digit of frame `i` goes in the third character. (The original writes it at ptr + 2 even
// in a two-character name, over the terminator, where the length keeps it out of the copies.)
void SetFrameDigit(std::string &name, int i)
{
	if (name.size() > 2) name[2] = (char)('0' + i);
}

// 0x477780: a canvas chunk of a database, by name (the same code as C3DSprite.cpp's copy).
OMediaCanvas *LoadCanvasName(OMediaDataBase *db, std::string name)
{
	OMediaCanvas *c = NULL;
	if (db && db->object_exists(OMediaCanvas::db_type, name))
	{
		c = (OMediaCanvas *)db->get_object(OMediaCanvas::db_type, name);
		if (g_509a12) NEG_TODO(0x477780, "debug database call (db vtable +0x4c)");
		if (c) c->db_unlock();
	}
	return c;
}

// 0x4690c0: animate `material`'s texture, unless its name already has an animation.
void AddTextureAnim(OMediaDataBase *db, OMedia3DMaterial *material, float frame_time)
{
	std::string name = ChunkName(material->get_texture());
	for (std::list<TextureAnim *>::iterator i = g_texture_anims.begin(); i != g_texture_anims.end(); ++i)
		if ((*i)->name.compare(0, (*i)->name.size(), name) == 0) return;

	TextureAnim *a = new TextureAnim;
	a->name = name;
	a->frames.push_back(material->get_texture());
	a->time = 0;
	a->frame_time = frame_time;
	a->material = material;
	a->current = a->frames.begin();
	for (int i = 1; i < 10; i++)
	{
		SetFrameDigit(name, i);
		if (!db->object_exists(OMediaCanvas::db_type, name)) break;		// db vtable +0x1c
		OMediaCanvas *c = LoadCanvasName(db, name);
		c->set_filtering(omtfc_Linear, omtfc_Linear);	// +0x60 / +0x64 = 1, then delete_imp_slaves
		a->frames.push_back(c);
	}
	g_texture_anims.push_back(a);
}

} // namespace

// 0x468ec0: called by Load3DShape for every shape it loads.
void neg::AddShapeTextureAnims(OMediaDataBase *db, OMedia3DShape *shape)
{
	omt_PolygonList *polys = shape->get_polygons();			// (no NULL test in the original)
	for (omt_PolygonList::iterator p = polys->begin(); p != polys->end(); ++p)
	{
		OMedia3DMaterial *m = (*p).get_material();
		if (!m || !m->get_texture()) continue;
		std::string name = ChunkName(m->get_texture());
		if (name.size() < 2 || name[0] != 'a' || name[1] != '-') continue;
		float t = m->get_ambient_r();						// +0x6c (grey ambient: 0.03..0.23 s in Level3/5)
		if (t == 0.0f) t = 0.5f;							// 0x48d914
		AddTextureAnim(db, m, t);							// 0x4690c0
	}
}

// 0x4693a0: every frame while the game runs (CViewPort::render_viewport).
void neg::UpdateTextureAnims(float dt)
{
	for (std::list<TextureAnim *>::iterator i = g_texture_anims.begin(); i != g_texture_anims.end(); ++i)
	{
		TextureAnim *a = *i;
		a->time += dt;
		if (a->frame_time <= a->time)
		{
			a->time -= a->frame_time;
			if (++a->current == a->frames.end()) a->current = a->frames.begin();
			a->material->set_texture(*a->current);			// material vtable +0x20
		}
	}
}

// 0x468d50: CGameType::InitObject / UnInitGame. The records go, the canvases stay in their
// databases.
void neg::ResetTextureAnims(void)
{
	// (debug log "Reset()")
	while (!g_texture_anims.empty())
	{
		TextureAnim *a = g_texture_anims.front();
		g_texture_anims.pop_front();
		delete a;											// 0x468e20
	}
}

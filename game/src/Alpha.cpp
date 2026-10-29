#include "PortLoadTiming.h" // port: load timing
/*  Database helpers and the "alpha" (bitmap font) sprite pool.  Reconstructed from Neutron.exe.
    0x476170 InitFileStream   0x476000 InitDatabase   0x477890 LoadCanvasID
    0x476080 / 0x476290 UnInitDatabase / UnInitFileStream
    0x4683e0 InitAlpha        0x468510 UnInitAlpha     0x4685f0 ReInitAlpha               */

#include "Game.h"
#include "gen/globals.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include "OMediaDataBase.h"
#include "OMediaCanvas.h"
#include "OMediaCanvasElement.h"
#include "OMediaError.h"
#include <SDL.h>
#include <string>
#include <string.h>

using namespace neg;

namespace neg {
OMediaCanvasElement	 g_alpha[ALPHA_SPRITES];	// 0x4fc6c0  (0xd0 bytes each in the 2002 OMT)
OMediaCanvas		*g_font[FONT_COUNT][FONT_GLYPHS];	// 0x5091e0
unsigned short		 g_alpha_used = 0;			// 0x5097e0
static OMediaFileStream	*s_alpha_stream = 0;	// 0x5097fc
static OMediaDataBase	*s_alpha_db = 0;		// 0x5097f8

// The canvas IDs in alpha.omt of each font's glyphs (character - ' ', -1 = none) are the exe's
// table g_font_glyph_ids (0x4f4ed8, 6 x 64 int16).
}

// --------------------------------------------------------------------------- database helpers

// The game's paths are DOS-style and relative (".\omt\alpha.omt").
static std::string port_path(const char *dos)
{
	std::string s(dos);
	if (s.compare(0, 2, ".\\") == 0) s.erase(0, 2);
	for (size_t i = 0; i < s.size(); i++) if (s[i] == '\\') s[i] = '/';
	return s;
}

OMediaFileStream *neg::InitFileStream(const char *name)		// 0x476170
{
	SDL_Log("InitFileStream(%s)", name ? name : "");
	if (!name) return NULL;
	double load_start = PortLoadTiming::now(); // port: load timing
	OMediaFileStream *fs = new OMediaFileStream;
	try
	{
		OMediaFilePath path(port_path(name));
		fs->setpath(&path);
		fs->open(omcfp_Read);
	}
	catch (OMediaError &e)
	{
		SDL_Log("InitFileStream: cannot open %s (error %d)", name, (int)e.errorcode);
		delete fs;
		return NULL;
	}
	PortLoadTiming::opened(fs, name, PortLoadTiming::now()-load_start); // port: load timing
	return fs;
}

OMediaDataBase *neg::InitDatabase(OMediaFileStream *fs)		// 0x476000
{
	SDL_Log("InitDatabase()");
	if (!fs) return NULL;
	double start = PortLoadTiming::now(); // port: load timing
	OMediaDataBase *db = new OMediaDataBase(fs);
	PortLoadTiming::State &s = PortLoadTiming::state();
	PortLoadTiming::DB d = s.live[fs];
	d.bytes = fs->getsize(); // port: load timing; resolved case-insensitive path size
	s.live[db] = d; s.live.erase(fs);
	if(s.active) { s.totals[d.name].bytes=d.bytes; s.totals[d.name].ms += PortLoadTiming::now()-start; }
	return db;
}

void neg::UnInitDatabase(OMediaDataBase *db)	{ PortLoadTiming::closed(db); delete db; } // port: load timing				// 0x476080
void neg::UnInitFileStream(OMediaFileStream *fs) { delete fs; }			// 0x476290

OMediaCanvas *neg::LoadCanvasID(OMediaDataBase *db, int id)				// 0x477890
{
	if (!db || !db->object_exists(OMediaCanvas::db_type, id)) return NULL;
	OMediaCanvas *c = (OMediaCanvas*)db->get_object(OMediaCanvas::db_type, id);
	// (0x509a12 set: an extra database call - not hit in the shipped config)
	if (c) c->db_unlock();
	return c;
}

// --------------------------------------------------------------------------- 0x4683e0

void CGfx::InitAlpha(void)
{
	SDL_Log("InitAlpha()");
	s_alpha_stream = InitFileStream("alpha.omt");			// (Negatron: ".\\omt\\alpha.omt")
	if (s_alpha_stream)
	{
		s_alpha_db = InitDatabase(s_alpha_stream);
		if (s_alpha_db)
		{
			int loaded = 0;
			for (int f = 0; f < FONT_COUNT; f++)
				for (int g = 0; g < FONT_GLYPHS; g++)
				{
					const short id = g_font_glyph_ids[f * FONT_GLYPHS + g];
					g_font[f][g] = id == -1 ? NULL : LoadCanvasID(s_alpha_db, id);
					if (g_font[f][g]) loaded++;
				}
			SDL_Log("InitAlpha: %d glyphs", loaded);
		}
	}

	for (int i = 0; i < ALPHA_SPRITES; i++)
	{
		OMediaCanvasElement &s = g_alpha[i];
		if (s.islinked()) { s.unlink_layer(); s.unlink(); }
		s.set_auto_align(omaac_Left, omaac_Bottom);		// +0xbc = 1, +0xc0 = 2
		s.set_canvas_flags(omcanef_FreeWorldSize);		// +0xc4 = 1
		// vtable +0xac (2002 OMT; compute_auto_align in 2.5)
		s.set_id('ALPH');								// +0x78
	}
	g_alpha_used = 0;
	SDL_Log("InitAlpha() done");
}

// --------------------------------------------------------------------------- 0x468510

void CGfx::UnInitAlpha(void)
{
	SDL_Log("UnInitAlpha()");
	for (int i = 0; i < ALPHA_SPRITES; i++)
	{
		OMediaCanvasElement &s = g_alpha[i];
		s.set_canvas(NULL);
		if (s.getvisible()) s.hide(true);
		if (s.islinked()) { s.unlink_layer(); s.unlink(); }
	}
	// 0x5097e4: an owned object released here (a caption/text helper; TODO)
	memset(g_font, 0, sizeof(g_font));
	UnInitDatabase(s_alpha_db);   s_alpha_db = NULL;
	UnInitFileStream(s_alpha_stream); s_alpha_stream = NULL;
}

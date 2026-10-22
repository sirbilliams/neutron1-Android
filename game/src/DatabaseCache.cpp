/*  The OMT database caches. Reconstructed from Neutron.exe 0x46a910..0x46b0f0.
    The lists are std::list<DatabaseEntry *> (MSVC6 list at 0x50996c / 0x50995c: head pointer,
    count at +4).                                                                             */

#include "DatabaseCache.h"
#include "Game.h"
#include "OMediaFilePath.h"
#include "OMediaFileStream.h"
#include <SDL.h>
#include <list>
#include <string.h>
#include <strings.h>

namespace {
std::list<neg::DatabaseEntry *> s_level_dbs;			// 0x50996c (count 0x509970)
std::list<neg::DatabaseEntry *> s_permanent_dbs;		// 0x50995c (count 0x509960)

void Trace(const char *fmt, ...) { (void)fmt; }			// CGameObject::v13: the debug log (empty in release)

// Open omt file `path` and add it to `list` under `name`; NULL if it can't be opened.
OMediaDataBase *Open(std::list<neg::DatabaseEntry *> &list, const char *path, const char *name)
{
	OMediaFileStream *fs = neg::InitFileStream(path);		// 0x476170
	if (!fs) return NULL;
	OMediaDataBase *db = neg::InitDatabase(fs);			// 0x476000
	if (!db) return NULL;								// (sic: the stream is left open)
	neg::DatabaseEntry *e = new neg::DatabaseEntry;
	e->open = 1;
	e->db = db;
	e->fs = fs;
	strcpy(e->name, name);
	list.push_back(e);
	return db;
}

OMediaDataBase *Find(std::list<neg::DatabaseEntry *> &list, const char *name)
{
	for (std::list<neg::DatabaseEntry *>::iterator i = list.begin(); i != list.end(); i++)
	{
		neg::DatabaseEntry *e = *i;
		if (e && e->open && strcasecmp(e->name, name) == 0)
			return e->db;								// (an entry without a database ends the search: NULL)
	}
	return NULL;
}

// Close entry e's database and stream and free it (0x46aef0 / 0x46b000 / N2:0x4a4960 inline this).
void Close(std::list<neg::DatabaseEntry *> &list, neg::DatabaseEntry *e)
{
	e->open = 0;
	Trace("Closing DB:%s", e->name);
	e->name[0] = 0;										// = g_empty_string (0x4f81a8)
	if (e->db) { neg::UnInitDatabase(e->db); e->db = NULL; }
	if (e->fs) { neg::UnInitFileStream(e->fs); e->fs = NULL; }
	list.remove(e);
	delete e;
}

void CloseAll(std::list<neg::DatabaseEntry *> &list)
{
	while (!list.empty())
	{
		neg::DatabaseEntry *e = list.front();
		if (!e || !e->open) continue;					// sic: loops forever on a closed first entry, as the original
		Close(list, e);
	}
}
}	// namespace

// 0x46a910: the level database `name` ("level1.omt"), opened from omt\<name> on first use (or
// from c:\omt\<name> when omt\<name> doesn't exist). Icons.omt is a permanent database.
OMediaDataBase *neg::GetDatabase(const char *name)
{
	char path[76], open_path[120];
	strcpy(path, "omt\\");
	strcat(path, name);
	{
		OMediaFilePath fp(path, 'null', 'null');
		OMediaFileStream fs;
		fs.setpath(&fp);
		if (!fs.fileexists())
		{
			fs.close();
			strcpy(open_path, "c:\\");					// the CD (0x4f3ed4)
			strcat(open_path, "omt\\");
			strcat(open_path, name);
			Trace("MEBTEMP %s", open_path);
		}
		else
		{
			fs.close();
			strcpy(open_path, path);
		}
	}
	if (strcasecmp(name, "retainedSprites.omt") == 0) Trace("RS Openend");
	if (strcasecmp(name, "Icons.omt") == 0)
	{
		Trace("icons Openend, opening permanent instead");
		return GetPermanentDatabase(name);
	}
	if (OMediaDataBase *db = Find(s_level_dbs, name)) return db;
	return Open(s_level_dbs, open_path, name);
}

// 0x46acc0: the permanent database `name`, opened from omt\<name> on first use.
OMediaDataBase *neg::GetPermanentDatabase(const char *name)
{
	char path[116];
	strcpy(path, "omt\\");
	strcat(path, name);
	if (OMediaDataBase *db = Find(s_permanent_dbs, name)) return db;
	return Open(s_permanent_dbs, path, name);
}

// 0x46aef0: close the level database `name` if it is open.
void neg::CloseDatabase(const char *name)
{
	for (std::list<DatabaseEntry *>::iterator i = s_level_dbs.begin(); i != s_level_dbs.end(); i++)
	{
		DatabaseEntry *e = *i;
		if (e && e->open && strcasecmp(e->name, name) == 0)
		{
			if (!e->db) return;							// (an entry without a database: nothing closed)
			Close(s_level_dbs, e);
			return;
		}
	}
}

// 0x46b000: close every level database (before a level is loaded).
void neg::CloseAllDatabases(void)
{
	CloseAll(s_level_dbs);
}

// N2:0x4a4960: close every permanent database (at exit).
void neg::CloseAllPermanentDatabases(void)
{
	CloseAll(s_permanent_dbs);
}

// 0x46b0f0: the name of the index-th entry of the level list (the editor lists them).
const char *neg::DatabaseName(short index)
{
	short n = 0;
	for (std::list<DatabaseEntry *>::iterator i = s_level_dbs.begin(); i != s_level_dbs.end(); i++)
	{
		DatabaseEntry *e = *i;
		if (!e) continue;
		if (n == index) return e->open ? e->name : NULL;
		n++;
	}
	return NULL;
}

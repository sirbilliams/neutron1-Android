/*  The game's caches of open OMT databases (omt\*.omt), by file name.
    0x46a910 GetDatabase  0x46acc0 GetPermanentDatabase  0x46aef0 CloseDatabase
    0x46b000 CloseAllDatabases  CloseAllPermanentDatabases (port)  0x46b0f0 DatabaseName

    Two lists: the level's databases (0x50996c, closed by CloseAllDatabases when a level is
    loaded) and the permanent ones (0x50995c, kept for the whole game: Icons.omt and the
    permanent icons).                                                                         */
#pragma once

class OMediaDataBase;
class OMediaFileStream;

namespace neg {

struct DatabaseEntry									// 0x70 bytes, operator new
{
	OMediaDataBase		*db;							// 0x00
	OMediaFileStream	*fs;							// 0x04
	char				 name[100];						// 0x08  as asked for ("level1.omt")
	int					 open;							// 0x6c
};

OMediaDataBase *GetDatabase(const char *name);			// 0x46a910
OMediaDataBase *GetPermanentDatabase(const char *name);	// 0x46acc0
void CloseDatabase(const char *name);					// 0x46aef0
void CloseAllDatabases(void);							// 0x46b000
void CloseAllPermanentDatabases(void);					// port: at exit
const char *DatabaseName(short index);					// 0x46b0f0  name of the index-th level database, NULL if closed

}

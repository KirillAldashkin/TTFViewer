#ifndef FILE_H
#define FILE_H

#include <SDL.h>
#include <stdint.h>
#include "tables/maxp.h"
#include "tables/head.h"
#include "tables/loca.h"
#include "tables/hhea.h"
#include "tables/hmtx.h"
#include "tables/name.h"
#include "tables/glyf.h"
#include "tables/cmap.h"

// TTF file
typedef struct _t_TtfFile {
	OffsetTable offsets;
	TableDirEntry* tables;
	TableHEAD head;
	TableHHEA hhea;
	TableHMTX hmtx;
	TableMAXP maxp;
	TableNAME name;
	TableCMAP cmap;
	uint32_t* loca;
	Glyph* glyf;
} TtfFile;

void free_ttf(TtfFile* file);
int read_ttf(SDL_RWops* io, TtfFile** file);
TtfFile* read_ttf_file(const char* path);

#endif
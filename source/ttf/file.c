#include "ttf/file.h"

#define CHECK_ZERO(a) if((a) != 0) { return -1; }
#define FREE_NOTNULL(ptr) { if(ptr != NULL) { SDL_free(ptr); } }

void read_offsettable(SDL_RWops * io, OffsetTable * table) {
	table->scalerType = SDL_ReadBE32(io);
	table->numTables = SDL_ReadBE16(io);
	table->searchRange = SDL_ReadBE16(io);
	table->entrySelector = SDL_ReadBE16(io);
	table->rangeShift = SDL_ReadBE16(io);
}

void read_tabledirentry(SDL_RWops * io, TableDirEntry * table) {
	for (int i = 0; i < 4; i++) {
		table->tag[i] = SDL_ReadU8(io);
	}
	table->checkSum = SDL_ReadBE32(io);
	table->offset = SDL_ReadBE32(io);
	table->length = SDL_ReadBE32(io);
}

int lookup_table(SDL_RWops * io, TtfFile * ttf, const char* tag, uint32_t * length) {
	for (uint16_t table = 0; table < ttf->offsets.numTables; table++) {
		int same = 1;
		for (int i = 0; i < 4; i++) {
			if (tag[i] != ttf->tables[table].tag[i]) {
				same = 0;
				break;
			}
		}
		if (same) {
			if (length != NULL) {
				*length = ttf->tables[table].length;
			}
			if (SDL_RWseek(io, ttf->tables[table].offset, RW_SEEK_SET) == -1) {
				return SDL_SetError("Couldn't seek to %s table: %s", tag, SDL_GetError());
			}
			return 0;
		}
	}
	return SDL_SetError("TTF file doesn't contain %s table", tag);
}

int read_ttf(SDL_RWops* io, TtfFile** file)
{
	// allocate ttf
	*file = SDL_calloc(1, sizeof(TtfFile));
	if (*file == NULL) {
		return SDL_SetError("Couldn't allocate TtfFile: %s", SDL_GetError());
	}

	// read header
	read_offsettable(io, &(*file)->offsets);
	(*file)->tables = SDL_calloc((*file)->offsets.numTables, sizeof(TableDirEntry));
	if ((*file)->tables == NULL) {
		return SDL_SetError("Couldn't allocate TableDirEntry: %s", SDL_GetError());
	}
	for (uint16_t table = 0; table < (*file)->offsets.numTables; ++table) {
		read_tabledirentry(io, (*file)->tables + table);
	}

	// tables:
	// head
	CHECK_ZERO(lookup_table(io, *file, "head", NULL));
	read_headtable(io, &(*file)->head);
	// hhea
	CHECK_ZERO(lookup_table(io, *file, "hhea", NULL));
	read_hheatable(io, &(*file)->hhea);
	// maxp
	CHECK_ZERO(lookup_table(io, *file, "maxp", NULL));
	read_maxptable(io, &(*file)->maxp);
	// hmtx
	CHECK_ZERO(lookup_table(io, *file, "hmtx", NULL));
	CHECK_ZERO(read_hmtxtable(io, &(*file)->hmtx, (*file)->hhea.numOfLongHorMetrics, (*file)->maxp.numGlyphs));
	// loca
	CHECK_ZERO(lookup_table(io, *file, "loca", NULL));
	CHECK_ZERO(read_locatable(io, &(*file)->loca, (*file)->maxp.numGlyphs, (*file)->head.indexToLocFormat));
	// name
	uint32_t nameLength;
	CHECK_ZERO(lookup_table(io, *file, "name", &nameLength));
	CHECK_ZERO(read_nametable(io, &(*file)->name, nameLength));
	// glyf 
	CHECK_ZERO(lookup_table(io, *file, "glyf", NULL));
	CHECK_ZERO(read_glyftable(io, &(*file)->glyf, (*file)->maxp.numGlyphs, (*file)->loca));
	// cmap
	CHECK_ZERO(lookup_table(io, *file, "cmap", NULL));
	CHECK_ZERO(read_cmaptable(io, &(*file)->cmap));

	return 0;
}

TtfFile* read_ttf_file(const char* path) {
	SDL_RWops* from = SDL_RWFromFile(path, "rb");
	if (from == NULL) {
		SDL_SetError("Couldn't open file: %s", SDL_GetError());
		goto OpenError;
	}
	TtfFile* file = NULL;
	if (read_ttf(from, &file) != 0) {
		SDL_SetError("Couldn't read TTF file: %s", SDL_GetError());
		goto ReadError;
	}
	if (SDL_RWclose(from) != 0) {
		SDL_SetError("Couldn't close file: %s", SDL_GetError());
		goto TtfCloseError;
	}
	return file;
ReadError:
	SDL_RWclose(from);
TtfCloseError:
	free_ttf(file);
OpenError:
	return NULL;
}

void free_ttf(TtfFile* file) {
	if (file != NULL) {
		// header
		FREE_NOTNULL(file->tables);

		// tables
		// HEAD: nothing to free
		// HHEA: nothing to free
		free_hmtxtable(&file->hmtx);
		// MAXP: nothing to free
		free_nametable(&file->name);
		free_cmaptable(&file->cmap);
		FREE_NOTNULL(file->loca);
		free_glyftable(file->glyf, file->maxp.numGlyphs);
		SDL_free(file);
	}
}
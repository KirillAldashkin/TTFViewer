#include "ttf/tables/cmap.h"
#include "ttf/tables/cmap/cmap0.h"
#include "ttf/tables/cmap/cmap2.h"
#include "ttf/tables/cmap/cmap4.h"
#include "ttf/tables/cmap/cmap6.h"
#include "ttf/tables/cmap/cmap8.h"
#include "ttf/tables/cmap/cmap10.h"
#include "ttf/tables/cmap/cmap12.h"
#include "ttf/tables/cmap/cmap13.h"
#include "ttf/tables/cmap/cmap14.h"

int read_cmaptable(SDL_RWops* io, TableCMAP* cmap) {
	int64_t start = SDL_RWtell(io);
	if (start == -1) {
		return SDL_SetError("Couldn't get NAME table start: %s", SDL_GetError());
	}
	cmap->version = SDL_ReadBE16(io);
	cmap->numberSubtables = SDL_ReadBE16(io);
	cmap->entries = SDL_calloc(cmap->numberSubtables, sizeof(cmap->entries[0]));
	if (cmap->entries == NULL) {
		return SDL_SetError("Couldn't allocate entries in CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < cmap->numberSubtables; i++) {
		cmap->entries[i].platformID = SDL_ReadBE16(io);
		cmap->entries[i].platformSpecificID = SDL_ReadBE16(io);
		cmap->entries[i].offset = SDL_ReadBE32(io);
	}
	cmap->tables = SDL_calloc(cmap->numberSubtables, sizeof(void*));
	if (cmap->entries == NULL) {
		return SDL_SetError("Couldn't allocate subtable array in CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < cmap->numberSubtables; i++) {
		if (SDL_RWseek(io, start + cmap->entries[i].offset, RW_SEEK_SET) == -1) {
			return SDL_SetError("Couldn't seek to subtable in CMAP table: %s", SDL_GetError());
		}
		uint16_t format = SDL_ReadBE16(io);
		int result;
		if (format == 0) {
			result = read_cmap0((SubtableCMAP0**)cmap->tables + i, io);
		} else if (format == 2) {
			result = read_cmap2((SubtableCMAP2**)cmap->tables + i, io);
		} else if (format == 4) {
			result = read_cmap4((SubtableCMAP4**)cmap->tables + i, io);
		} else if (format == 6) {
			result = read_cmap6((SubtableCMAP6**)cmap->tables + i, io);
		} else if (format == 8) {
			result = read_cmap8((SubtableCMAP8**)cmap->tables + i, io);
		} else if (format == 10) {
			result = read_cmap10((SubtableCMAP10**)cmap->tables + i, io);
		} else if (format == 12) {
			result = read_cmap12((SubtableCMAP12**)cmap->tables + i, io);
		} else if (format == 13) {
			result = read_cmap13((SubtableCMAP13**)cmap->tables + i, io);
		} else if (format == 14) {
			result = read_cmap14((SubtableCMAP14**)cmap->tables + i, io);
		} else {
			return SDL_SetError("Unknown CMAP subtable format");
		}
		if (result == -1) return -1;
	}
	return 0;
}

void free_cmaptable(TableCMAP* cmap) {
	FREE_NOTNULL(cmap->entries);
	if (cmap->tables != NULL) {
		for (uint16_t i = 0; i < cmap->numberSubtables; i++) {
			if (cmap->tables[i] == NULL) {
				continue;
			}
			uint16_t* ptr = cmap->tables[i];
			if (*ptr == 0) {
				free_cmap0((SubtableCMAP0*)ptr);
			} else if (*ptr == 2) {
				free_cmap2((SubtableCMAP2*)ptr);
			} else if (*ptr == 4) {
				free_cmap4((SubtableCMAP4*)ptr);
			} else if (*ptr == 6) {
				free_cmap6((SubtableCMAP6*)ptr);
			} else if (*ptr == 8) {
				free_cmap8((SubtableCMAP8*)ptr);
			} else if (*ptr == 10) {
				free_cmap10((SubtableCMAP10*)ptr);
			} else if (*ptr == 12) {
				free_cmap12((SubtableCMAP12*)ptr);
			} else if (*ptr == 13) {
				free_cmap13((SubtableCMAP13*)ptr);
			} else if (*ptr == 14) {
				free_cmap14((SubtableCMAP14*)ptr);
			}
		}
		SDL_free(cmap->tables);
	}
}
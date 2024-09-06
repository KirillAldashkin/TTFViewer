#ifndef TABLES_CMAP12_H
#define TABLES_CMAP12_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t reserved;
	uint32_t length;
	uint32_t language;
	uint32_t nGroups;
	struct {
		uint32_t startCharCode;
		uint32_t endCharCode;
		uint32_t startGlyphCode;
	}*groups;
} SubtableCMAP12;

int read_cmap12(SubtableCMAP12** sub12, SDL_RWops* io);
void free_cmap12(SubtableCMAP12* sub12);

#endif
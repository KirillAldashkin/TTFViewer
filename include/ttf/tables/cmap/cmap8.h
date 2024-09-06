#ifndef TABLES_CMAP8_H
#define TABLES_CMAP8_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t reserved;
	uint32_t length;
	uint32_t language;
	uint8_t is32[8192];
	uint32_t nGroups;
	struct {
		uint32_t startCharCode;
		uint32_t endCharCode;
		uint32_t startGlyphCode;
	}* groups;
} SubtableCMAP8;

int read_cmap8(SubtableCMAP8** sub8, SDL_RWops* io);
void free_cmap8(SubtableCMAP8* sub8);

#endif
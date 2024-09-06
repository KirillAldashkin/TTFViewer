#ifndef TABLES_CMAP13_H
#define TABLES_CMAP13_H

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
} SubtableCMAP13;

int read_cmap13(SubtableCMAP13** sub13, SDL_RWops* io);
void free_cmap13(SubtableCMAP13* sub13);

#endif
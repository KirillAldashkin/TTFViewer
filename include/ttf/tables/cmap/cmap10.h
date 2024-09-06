#ifndef TABLES_CMAP10_H
#define TABLES_CMAP10_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t reserved;
	uint32_t length;
	uint32_t language;
	uint32_t startCharCode;
	uint32_t numChars;
	uint16_t* glyphs;
} SubtableCMAP10;

int read_cmap10(SubtableCMAP10** sub10, SDL_RWops* io);
void free_cmap10(SubtableCMAP10* sub10);

#endif
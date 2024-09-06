#ifndef TABLES_CMAP0_H
#define TABLES_CMAP0_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t length;
	uint16_t language;
	uint8_t glyphIndexArray[256];
} SubtableCMAP0;

int read_cmap0(SubtableCMAP0** sub0, SDL_RWops* io);
uint8_t resolve_cmap0(SubtableCMAP0* sub0, uint8_t ch);
void free_cmap0(SubtableCMAP0* sub0);

#endif
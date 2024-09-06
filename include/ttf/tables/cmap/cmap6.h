#ifndef TABLES_CMAP6_H
#define TABLES_CMAP6_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t length;
	uint16_t language;
	uint16_t firstCode;
	uint16_t entryCount;
	uint16_t* glyphIndexArray;
} SubtableCMAP6;

int read_cmap6(SubtableCMAP6** sub6, SDL_RWops* io);
void free_cmap6(SubtableCMAP6* sub6);

#endif
#ifndef TABLES_CMAP4_H
#define TABLES_CMAP4_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t length;
	uint16_t language;
	uint16_t segCountX2;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
	uint16_t* endCode;
	uint16_t reservedPad;
	uint16_t* startCode;
	uint16_t* idDelta;
	uint16_t* idRangeOffset;
	uint16_t* glyphIndexArray;
} SubtableCMAP4;

int read_cmap4(SubtableCMAP4** sub4, SDL_RWops* io);
void free_cmap4(SubtableCMAP4* sub4);

#endif
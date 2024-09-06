#ifndef TABLES_CMAP2_H
#define TABLES_CMAP2_H

#include <SDL.h>

typedef struct {
	uint16_t format;
	uint16_t length;
	uint16_t language;
	uint16_t subHeaderKeys[256];
	struct {
		uint16_t firstCode;
		uint16_t entryCount;
		int16_t idDelta;
		uint16_t idRangeOffset;
	}*subHeaders;
	uint16_t* glyphIndexArray;
} SubtableCMAP2;

int read_cmap2(SubtableCMAP2** sub2, SDL_RWops* io);
uint16_t resolve_cmap2(SubtableCMAP2* sub2, uint8_t* ptr, int* bytesConsumed);
void free_cmap2(SubtableCMAP2* sub2);

#endif
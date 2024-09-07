#ifndef TABLES_HMTX_H
#define TABLES_HMTX_H

#include <SDL.h>
#include "ttf/ttf.h"

typedef struct {
	struct {
		uint16_t advanceWidth;
		int16_t leftSideBearing;
	}* hMetrics;
	FWord* leftSideBearing;
} TableHMTX;

int read_hmtxtable(SDL_RWops* io, TableHMTX* hmtx, int horMetrics, int glyphs);
void free_hmtxtable(TableHMTX* hmtx);

#endif
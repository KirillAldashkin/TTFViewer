#ifndef TABLES_HHEA_H
#define TABLES_HHEA_H

#include <SDL.h>
#include "ttf/ttf.h"

typedef struct {
	Fixed version;
	FWord ascent;
	FWord descent;
	FWord lineGap;
	uFWord advanceWidthMax;
	FWord minLeftSideBearing;
	FWord minRightSideBearing;
	FWord xMaxExtent;
	int16_t caretSlopeRise;
	int16_t caretSlopeRun;
	FWord caretOffset;
	int16_t reserved[4];
	int16_t metricDataFormat;
	uint16_t numOfLongHorMetrics;
} TableHHEA;

void read_hheatable(SDL_RWops* io, TableHHEA* hhea);

#endif
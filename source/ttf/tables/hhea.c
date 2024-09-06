#include "ttf/tables/hhea.h"

void read_hheatable(SDL_RWops* io, TableHHEA* hhea)
{
	read_fixed(io, &hhea->version);
	hhea->ascent = SDL_ReadBE16(io);
	hhea->descent = SDL_ReadBE16(io);
	hhea->lineGap = SDL_ReadBE16(io);
	hhea->advanceWidthMax = SDL_ReadBE16(io);
	hhea->minLeftSideBearing = SDL_ReadBE16(io);
	hhea->minRightSideBearing = SDL_ReadBE16(io);
	hhea->xMaxExtent = SDL_ReadBE16(io);
	hhea->caretSlopeRise = SDL_ReadBE16(io);
	hhea->caretSlopeRun = SDL_ReadBE16(io);
	hhea->caretOffset = SDL_ReadBE16(io);
	for (int i = 0; i < 4; i++) {
		hhea->reserved[i] = SDL_ReadBE16(io);
	}
	hhea->metricDataFormat = SDL_ReadBE16(io);
	hhea->numOfLongHorMetrics = SDL_ReadBE16(io);
}

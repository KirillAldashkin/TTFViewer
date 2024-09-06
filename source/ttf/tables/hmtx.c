#include "ttf/tables/hmtx.h"
#include "util.h"

int read_hmtxtable(SDL_RWops* io, TableHMTX* hmtx, int horMetrics, int glyphs)
{
	hmtx->hMetrics = SDL_calloc(horMetrics, sizeof(hmtx->hMetrics[0]));
	if (hmtx->hMetrics == NULL) {
		return SDL_SetError("Couldn't allocate hMetrics in LOCA table: %s", SDL_GetError());
	}
	hmtx->leftSideBearing = SDL_calloc(glyphs - horMetrics, sizeof(FWord));
	if (hmtx->leftSideBearing == NULL) {
		return SDL_SetError("Couldn't allocate hMetrics in LOCA table: %s", SDL_GetError());
	}
	for (int i = 0; i < horMetrics; i++) {
		hmtx->hMetrics[i].advanceWidth = SDL_ReadBE16(io);
		hmtx->hMetrics[i].leftSideBearing = SDL_ReadBE16(io);
	}
	for (int i = 0; i < glyphs - horMetrics; i++) {
		hmtx->leftSideBearing[i] = SDL_ReadBE16(io);
	}
	return 0;
}

void free_hmtxtable(TableHMTX* hmtx) {
	FREE_NOTNULL(hmtx->hMetrics);
	FREE_NOTNULL(hmtx->leftSideBearing);
}
#include "ttf/tables/cmap/cmap6.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap6(SubtableCMAP6** sub6, SDL_RWops* io) {
	(*sub6) = SDL_calloc(1, sizeof(SubtableCMAP6));
	if ((*sub6) == NULL) {
		return SDL_SetError("Couldn't allocate cmap6 in CMAP table: %s", SDL_GetError());
	}
	(*sub6)->format = 6;
	(*sub6)->length = SDL_ReadBE16(io);
	(*sub6)->language = SDL_ReadBE16(io);
	(*sub6)->firstCode = SDL_ReadBE16(io);
	(*sub6)->entryCount = SDL_ReadBE16(io);

	(*sub6)->glyphIndexArray = SDL_calloc((*sub6)->entryCount, sizeof(uint16_t));
	if ((*sub6)->glyphIndexArray == NULL) {
		return SDL_SetError("Couldn't allocate glyphIndexArray in cmap6 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < (*sub6)->entryCount; i++) {
		(*sub6)->glyphIndexArray[i] = SDL_ReadBE16(io);
	}
	return 0;
}

void free_cmap6(SubtableCMAP6* sub6) {
	FREE_NOTNULL(sub6->glyphIndexArray);
	SDL_free(sub6);
}
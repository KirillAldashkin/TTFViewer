#include "ttf/tables/cmap/cmap0.h"
#include "ttf/tables/cmap.h"

int read_cmap0(SubtableCMAP0** sub0, SDL_RWops* io) {
	(*sub0) = SDL_calloc(1, sizeof(SubtableCMAP0));
	if ((*sub0) == NULL) {
		return SDL_SetError("Couldn't allocate cmap0 in CMAP table: %s", SDL_GetError());
	}
	(*sub0)->format = 0;
	(*sub0)->length = SDL_ReadBE16(io);
	(*sub0)->language = SDL_ReadBE16(io);
	for (int i = 0; i < 256; i++) {
		(*sub0)->glyphIndexArray[i] = SDL_ReadU8(io);
	}
	return 0;
}

uint8_t resolve_cmap0(SubtableCMAP0* sub0, uint8_t ch) {
	return sub0->glyphIndexArray[ch];
}

void free_cmap0(SubtableCMAP0* sub0) {
	SDL_free(sub0);
}
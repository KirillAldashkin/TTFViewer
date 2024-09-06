#include "ttf/tables/cmap/cmap10.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap10(SubtableCMAP10** sub10, SDL_RWops* io) {
	(*sub10) = SDL_calloc(1, sizeof(SubtableCMAP10));
	if ((*sub10) == NULL) {
		return SDL_SetError("Couldn't allocate cmap10 in CMAP table: %s", SDL_GetError());
	}
	(*sub10)->format = 10;
	(*sub10)->reserved = SDL_ReadBE16(io);
	(*sub10)->length = SDL_ReadBE32(io);
	(*sub10)->language = SDL_ReadBE32(io);
	(*sub10)->startCharCode = SDL_ReadBE32(io);
	(*sub10)->numChars = SDL_ReadBE32(io);

	(*sub10)->glyphs = SDL_calloc((*sub10)->numChars, sizeof(uint16_t));
	if ((*sub10)->glyphs == NULL) {
		return SDL_SetError("Couldn't allocate glyphs in cmap10 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub10)->numChars; i++) {
		(*sub10)->glyphs[i] = SDL_ReadBE16(io);
	}
	return 0;
}

void free_cmap10(SubtableCMAP10* sub10) {
	FREE_NOTNULL(sub10->glyphs);
	SDL_free(sub10);
}
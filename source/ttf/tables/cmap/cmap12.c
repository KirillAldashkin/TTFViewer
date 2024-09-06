#include "ttf/tables/cmap/cmap12.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap12(SubtableCMAP12** sub12, SDL_RWops* io) {
	(*sub12) = SDL_calloc(1, sizeof(SubtableCMAP12));
	if ((*sub12) == NULL) {
		return SDL_SetError("Couldn't allocate cmap12 in CMAP table: %s", SDL_GetError());
	}
	(*sub12)->format = 12;
	(*sub12)->reserved = SDL_ReadBE16(io);
	(*sub12)->length = SDL_ReadBE32(io);
	(*sub12)->language = SDL_ReadBE32(io);
	(*sub12)->nGroups = SDL_ReadBE32(io);
	(*sub12)->groups = SDL_calloc((*sub12)->nGroups, sizeof((*sub12)->groups[0]));
	if ((*sub12)->groups == NULL) {
		return SDL_SetError("Couldn't allocate groups in cmap12 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub12)->nGroups; i++) {
		(*sub12)->groups[i].startCharCode = SDL_ReadBE32(io);
		(*sub12)->groups[i].endCharCode = SDL_ReadBE32(io);
		(*sub12)->groups[i].startGlyphCode = SDL_ReadBE32(io);
	}
	return 0;
}

void free_cmap12(SubtableCMAP12* sub12) {
	FREE_NOTNULL(sub12->groups);
	SDL_free(sub12);
}
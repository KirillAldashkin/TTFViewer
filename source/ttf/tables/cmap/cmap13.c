#include "ttf/tables/cmap/cmap13.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap13(SubtableCMAP13** sub13, SDL_RWops* io) {
	(*sub13) = SDL_calloc(1, sizeof(SubtableCMAP13));
	if ((*sub13) == NULL) {
		return SDL_SetError("Couldn't allocate cmap12 in CMAP table: %s", SDL_GetError());
	}
	(*sub13)->format = 13;
	(*sub13)->reserved = SDL_ReadBE16(io);
	(*sub13)->length = SDL_ReadBE32(io);
	(*sub13)->language = SDL_ReadBE32(io);
	(*sub13)->nGroups = SDL_ReadBE32(io);
	(*sub13)->groups = SDL_calloc((*sub13)->nGroups, sizeof((*sub13)->groups[0]));
	if ((*sub13)->groups == NULL) {
		return SDL_SetError("Couldn't allocate groups in cmap13 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub13)->nGroups; i++) {
		(*sub13)->groups[i].startCharCode = SDL_ReadBE32(io);
		(*sub13)->groups[i].endCharCode = SDL_ReadBE32(io);
		(*sub13)->groups[i].startGlyphCode = SDL_ReadBE32(io);
	}
	return 0;
}

void free_cmap13(SubtableCMAP13* sub13) {
	FREE_NOTNULL(sub13->groups);
	SDL_free(sub13);
}
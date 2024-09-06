#include "ttf/tables/cmap/cmap8.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap8(SubtableCMAP8** sub8, SDL_RWops* io) {
	(*sub8) = SDL_calloc(1, sizeof(SubtableCMAP8));
	if ((*sub8) == NULL) {
		return SDL_SetError("Couldn't allocate cmap8 in CMAP table: %s", SDL_GetError());
	}
	(*sub8)->format = 8;
	(*sub8)->reserved = SDL_ReadBE16(io);
	(*sub8)->length = SDL_ReadBE32(io);
	(*sub8)->language = SDL_ReadBE32(io);
	for (int i = 0; i < 8192; i++) {
		(*sub8)->is32[i] = SDL_ReadU8(io);
	}
	(*sub8)->nGroups = SDL_ReadBE32(io);
	(*sub8)->groups = SDL_calloc((*sub8)->nGroups, sizeof((*sub8)->groups[0]));
	if ((*sub8)->groups == NULL) {
		return SDL_SetError("Couldn't allocate groups in cmap8 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub8)->nGroups; i++) {
		(*sub8)->groups[i].startCharCode = SDL_ReadBE32(io);
		(*sub8)->groups[i].endCharCode = SDL_ReadBE32(io);
		(*sub8)->groups[i].startGlyphCode = SDL_ReadBE32(io);
	}
	return 0;
}

void free_cmap8(SubtableCMAP8* sub8) {
	FREE_NOTNULL(sub8->groups);
	SDL_free(sub8);
}
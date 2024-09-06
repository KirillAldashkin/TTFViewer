#include "ttf/tables/loca.h"

int read_locatable(SDL_RWops* io, uint32_t** loca, int glyphs, int is32bit)
{
	*loca = SDL_calloc(glyphs + 1, sizeof(uint32_t));
	if (*loca == NULL) {
		return SDL_SetError("Couldn't allocate LOCA table: %s", SDL_GetError());
	}
	for (int i = 0; i < glyphs + 1; i++) {
		(*loca)[i] = is32bit ? SDL_ReadBE32(io) : SDL_ReadBE16(io);
	}
	return 0;
}

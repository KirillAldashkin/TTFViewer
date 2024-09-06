#include "ttf/tables/name.h"
#include "util.h"

int read_nametable(SDL_RWops* io, TableNAME* name, uint32_t length) {
	int64_t start = SDL_RWtell(io);
	if (start == -1) {
		return SDL_SetError("Couldn't get NAME table start: %s", SDL_GetError());
	}
	name->format = SDL_ReadBE16(io);
	name->count = SDL_ReadBE16(io);
	name->stringOffset = SDL_ReadBE16(io);
	name->nameRecord = SDL_calloc(name->count, sizeof(name->nameRecord[0]));
	if (name->nameRecord == NULL) {
		return SDL_SetError("Couldn't allocate name records in NAME table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < name->count; i++) {
		name->nameRecord[i].platformID = SDL_ReadBE16(io);
		name->nameRecord[i].platformSpecificID = SDL_ReadBE16(io);
		name->nameRecord[i].languageID = SDL_ReadBE16(io);
		name->nameRecord[i].nameID = SDL_ReadBE16(io);
		name->nameRecord[i].length = SDL_ReadBE16(io);
		name->nameRecord[i].offset = SDL_ReadBE16(io);
	}
	if (SDL_RWseek(io, start + name->stringOffset, RW_SEEK_SET) == -1) {
		return SDL_SetError("Couldn't seek to character data in NAME table: %s", SDL_GetError());
	}
	length -= name->stringOffset;
	name->stringHeap = SDL_malloc(length);
	if (name->stringHeap == NULL) {
		return SDL_SetError("Couldn't allocate character data in NAME table: %s", SDL_GetError());
	}
	if (SDL_RWread(io, name->stringHeap, length, 1) == 0) {
		return SDL_SetError("Couldn't read character data in NAME table: %s", SDL_GetError());
	}
	return 0;
}

void free_nametable(TableNAME* name) {
	FREE_NOTNULL(name->nameRecord);
	FREE_NOTNULL(name->stringHeap);
}
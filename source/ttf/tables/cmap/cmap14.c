#include "ttf/tables/cmap/cmap14.h"
#include "ttf/tables/cmap.h"

int read_cmap14(SubtableCMAP14** sub14, SDL_RWops* io) {
	(*sub14) = SDL_calloc(1, sizeof(SubtableCMAP14));
	if ((*sub14) == NULL) {
		return SDL_SetError("Couldn't allocate cmap14 in CMAP table: %s", SDL_GetError());
	}
	(*sub14)->format = 14;
	(*sub14)->length = SDL_ReadBE32(io);

	(*sub14)->numVarSelectorRecords = SDL_ReadBE32(io);
	(*sub14)->varSelectorRecords = SDL_calloc((*sub14)->numVarSelectorRecords, sizeof((*sub14)->varSelectorRecords[0]));
	if ((*sub14)->varSelectorRecords == NULL) {
		return SDL_SetError("Couldn't allocate varSelectorRecords in cmap14 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub14)->numVarSelectorRecords; i++) {
		(*sub14)->varSelectorRecords[i].varSelector = SDL_ReadBE24(io);
		(*sub14)->varSelectorRecords[i].defaultUVSOffset = SDL_ReadBE32(io);
		(*sub14)->varSelectorRecords[i].nonDefaultUVSOffset = SDL_ReadBE32(io);
	}

	(*sub14)->numUnicodeValueRanges = SDL_ReadBE32(io);
	(*sub14)->unicodeValueRanges = SDL_calloc((*sub14)->numUnicodeValueRanges, sizeof((*sub14)->unicodeValueRanges[0]));
	if ((*sub14)->unicodeValueRanges == NULL) {
		return SDL_SetError("Couldn't allocate unicodeValueRanges in cmap14 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub14)->numVarSelectorRecords; i++) {
		(*sub14)->unicodeValueRanges[i].startUnicodeValue = SDL_ReadBE24(io);
		(*sub14)->unicodeValueRanges[i].additionalCount = SDL_ReadU8(io);
	}

	(*sub14)->numUVSMappings = SDL_ReadBE32(io);
	(*sub14)->UVSMappings = SDL_calloc((*sub14)->numUVSMappings, sizeof((*sub14)->UVSMappings[0]));
	if ((*sub14)->UVSMappings == NULL) {
		return SDL_SetError("Couldn't allocate numUVSMappings in cmap14 of CMAP table: %s", SDL_GetError());
	}
	for (uint32_t i = 0; i < (*sub14)->numUVSMappings; i++) {
		(*sub14)->UVSMappings[i].unicodeValue = SDL_ReadBE24(io);
		(*sub14)->UVSMappings[i].glyphId = SDL_ReadBE16(io);
	}
	return 0;
}

void free_cmap14(SubtableCMAP14* sub14) {
	FREE_NOTNULL(sub14->varSelectorRecords);
	FREE_NOTNULL(sub14->unicodeValueRanges);
	FREE_NOTNULL(sub14->UVSMappings);
	SDL_free(sub14);
}
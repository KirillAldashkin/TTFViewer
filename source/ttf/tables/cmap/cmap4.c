#include "ttf/tables/cmap/cmap4.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap4(SubtableCMAP4** sub4, SDL_RWops* io) {
	(*sub4) = SDL_calloc(1, sizeof(SubtableCMAP4));
	if ((*sub4) == NULL) {
		return SDL_SetError("Couldn't allocate cmap4 in CMAP table: %s", SDL_GetError());
	}
	(*sub4)->format = 4;
	(*sub4)->length = SDL_ReadBE16(io);
	(*sub4)->language = SDL_ReadBE16(io);
	(*sub4)->segCountX2 = SDL_ReadBE16(io);
	(*sub4)->searchRange = SDL_ReadBE16(io);
	(*sub4)->entrySelector = SDL_ReadBE16(io);
	(*sub4)->rangeShift = SDL_ReadBE16(io);
	
	(*sub4)->endCode = SDL_calloc((*sub4)->segCountX2 / 2, sizeof(uint16_t));
	if ((*sub4)->endCode == NULL) {
		return SDL_SetError("Couldn't allocate endCode in cmap4 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < (*sub4)->segCountX2 / 2; i++) {
		(*sub4)->endCode[i] = SDL_ReadBE16(io);
	}
	
	(*sub4)->reservedPad = SDL_ReadBE16(io);
	
	(*sub4)->startCode = SDL_calloc((*sub4)->segCountX2 / 2, sizeof(uint16_t));
	if ((*sub4)->startCode == NULL) {
		return SDL_SetError("Couldn't allocate startCode in cmap4 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < (*sub4)->segCountX2 / 2; i++) {
		(*sub4)->startCode[i] = SDL_ReadBE16(io);
	}

	(*sub4)->idDelta = SDL_calloc((*sub4)->segCountX2 / 2, sizeof(uint16_t));
	if ((*sub4)->idDelta == NULL) {
		return SDL_SetError("Couldn't allocate idDelta in cmap4 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < (*sub4)->segCountX2 / 2; i++) {
		(*sub4)->idDelta[i] = SDL_ReadBE16(io);
	}

	(*sub4)->idRangeOffset = SDL_calloc((*sub4)->segCountX2 / 2, sizeof(uint16_t));
	if ((*sub4)->idRangeOffset == NULL) {
		return SDL_SetError("Couldn't allocate idRangeOffset in cmap4 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < (*sub4)->segCountX2 / 2; i++) {
		(*sub4)->idRangeOffset[i] = SDL_ReadBE16(io);
	}
	uint16_t numIndices = 0;
	for (uint16_t i = 0; i < (*sub4)->segCountX2 / 2; i++) {
		uint16_t index;
		if ((*sub4)->idRangeOffset[i] == 0) {
			// to understand three lines of code below:
			// s - startCode, S - startCode + idDelta
			// e - endCode,   E - endCode + idDelta
			// consider this:
			// |     s---------e            |
			// |          S---------E       |
			//                 E>S, ^max is E
			// and this:
			// |                s---------e |
			// |---E                 S------|
			//                        E<S, ^max is 0xFFFF
			uint16_t s = (*sub4)->startCode[i] + (*sub4)->idDelta[i];
			uint16_t e = (*sub4)->endCode[i] + (*sub4)->idDelta[i];
			index = (e >= s) ? e : 0xFFFF;
		} else {
			void* address = (*sub4)->idRangeOffset[i] + 2 * ((*sub4)->endCode[i] - (*sub4)->startCode[i]) + (uint8_t*)((*sub4)->idRangeOffset + i);
			index = (uint16_t)((size_t)repair_pointer((*sub4)->idRangeOffset, (*sub4)->segCountX2, NULL, address) / 2);
		}
		numIndices = max(numIndices, index + 1);
	}

	(*sub4)->glyphIndexArray = SDL_calloc(numIndices, sizeof(uint16_t));
	if ((*sub4)->glyphIndexArray == NULL) {
		return SDL_SetError("Couldn't allocate glyphIndexArray in cmap4 of CMAP table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < numIndices; i++) {
		(*sub4)->glyphIndexArray[i] = SDL_ReadBE16(io);
	}
	return 0;
}

void free_cmap4(SubtableCMAP4* sub4) {
	FREE_NOTNULL(sub4->endCode);
	FREE_NOTNULL(sub4->startCode);
	FREE_NOTNULL(sub4->idDelta);
	FREE_NOTNULL(sub4->idRangeOffset);
	FREE_NOTNULL(sub4->glyphIndexArray);
	SDL_free(sub4);
}
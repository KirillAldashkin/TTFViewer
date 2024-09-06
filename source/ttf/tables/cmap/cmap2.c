#include "ttf/tables/cmap/cmap2.h"
#include "ttf/tables/cmap.h"
#include "util.h"

int read_cmap2(SubtableCMAP2** sub2, SDL_RWops* io) {
	(*sub2) = SDL_calloc(1, sizeof(SubtableCMAP2));
	if ((*sub2) == NULL) {
		return SDL_SetError("Couldn't allocate cmap2 in CMAP table: %s", SDL_GetError());
	}
	(*sub2)->format = 2;
	(*sub2)->length = SDL_ReadBE16(io);
	(*sub2)->language = SDL_ReadBE16(io);
	uint16_t subHeadersLength = 0;
	for (int i = 0; i < 256; i++) {
		(*sub2)->subHeaderKeys[i] = SDL_ReadBE16(io);
		subHeadersLength = max(subHeadersLength, (*sub2)->subHeaderKeys[i] + 1);
	}
	size_t subHeaderSize = sizeof((*sub2)->subHeaders[0]);
	(*sub2)->subHeaders = SDL_calloc(subHeadersLength, subHeaderSize);
	if ((*sub2)->subHeaders == NULL) {
		return SDL_SetError("Couldn't allocate subHeaders in cmap2 of CMAP table: %s", SDL_GetError());
	}
	size_t glyphIndexLength = 0;
	for (uint16_t i = 0; i < subHeadersLength; i++) {
		(*sub2)->subHeaders[i].firstCode = SDL_ReadBE16(io);
		(*sub2)->subHeaders[i].entryCount = SDL_ReadBE16(io);
		(*sub2)->subHeaders[i].idDelta = SDL_ReadBE16(io);
		(*sub2)->subHeaders[i].idRangeOffset = SDL_ReadBE16(io);

		uint16_t* idRangeOffset = &((*sub2)->subHeaders[i].idRangeOffset);
		size_t glyphIndex = (size_t)repair_pointer(
			(*sub2)->subHeaders, subHeadersLength * subHeaderSize, NULL,
			((uint8_t*)idRangeOffset) + *idRangeOffset) / sizeof(uint16_t);
		glyphIndexLength = max(glyphIndexLength, glyphIndex + (*sub2)->subHeaders[i].entryCount);
	}
	(*sub2)->glyphIndexArray = SDL_calloc(glyphIndexLength, sizeof(uint16_t));
	if ((*sub2)->glyphIndexArray == NULL) {
		return SDL_SetError("Couldn't allocate glyphIndexArray in cmap2 of CMAP table: %s", SDL_GetError());
	}
	for (size_t i = 0; i < glyphIndexLength; i++) {
		(*sub2)->glyphIndexArray[i] = SDL_ReadBE16(io);
	}
	return 0;
}

uint16_t resolve_cmap2(SubtableCMAP2* sub2, uint8_t* ptr, int* consumed) {
	uint16_t subIdx = sub2->subHeaderKeys[*ptr] / 8;
	*consumed = (subIdx == 0) ? 1 : 2;
	uint8_t select = *(ptr + (subIdx != 0));

	uint16_t from = sub2->subHeaders[subIdx].firstCode;
	uint16_t to = from + sub2->subHeaders[subIdx].entryCount;
	if ((select >= from) && (select < to)) {
		uint16_t index = select - from;
	} else {
		return 0;
	}
}

void free_cmap2(SubtableCMAP2* sub2) {
	FREE_NOTNULL(sub2->subHeaders);
	FREE_NOTNULL(sub2->glyphIndexArray);
	SDL_free(sub2);
}
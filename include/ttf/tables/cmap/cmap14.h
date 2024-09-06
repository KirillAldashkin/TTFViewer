#ifndef TABLES_CMAP14_H
#define TABLES_CMAP14_H

#include <SDL.h>
#include "util.h"

typedef struct {
	uint16_t format;
	uint32_t length;
	uint32_t numVarSelectorRecords;
	struct {
		uint24_t varSelector;
		uint32_t defaultUVSOffset;
		uint32_t nonDefaultUVSOffset;
	}* varSelectorRecords;
	uint32_t numUnicodeValueRanges;
	struct {
		uint24_t startUnicodeValue;
		uint8_t additionalCount;
	}* unicodeValueRanges;
	uint32_t numUVSMappings;
	struct {
		uint24_t unicodeValue;
		uint16_t glyphId;
	}* UVSMappings;
} SubtableCMAP14;

int read_cmap14(SubtableCMAP14** sub14, SDL_RWops* io);
void free_cmap14(SubtableCMAP14* sub14);

#endif
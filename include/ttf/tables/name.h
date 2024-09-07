#ifndef TABLES_NAME_H
#define TABLES_NAME_H

#include <SDL.h>
#include "ttf/ttf.h"

typedef struct {
	uint16_t format;
	uint16_t count;
	uint16_t stringOffset;
	struct {
		uint16_t platformID;
		uint16_t platformSpecificID;
		uint16_t languageID;
		uint16_t nameID;
		uint16_t length;
		uint16_t offset;
	}* nameRecord;
	uint8_t* stringHeap;
} TableNAME;

int read_nametable(SDL_RWops* io, TableNAME* name, uint32_t length);
void free_nametable(TableNAME* name);

#endif
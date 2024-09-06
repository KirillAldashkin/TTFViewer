#ifndef TABLES_MAXP_H
#define TABLES_MAXP_H

#include <SDL.h>
#include "..\ttf.h"

typedef struct {
	Fixed version;
	uint16_t numGlyphs;
	uint16_t maxPoints;
	uint16_t maxContours;
	uint16_t maxComponentPoints;
	uint16_t maxComponentContours;
	uint16_t maxZones;
	uint16_t maxTwilightPoints;
	uint16_t maxStorage;
	uint16_t maxFunctionDefs;
	uint16_t maxInstructionDefs;
	uint16_t maxStackElements;
	uint16_t maxSizeOfInstructions;
	uint16_t maxComponentElements;
	uint16_t maxComponentDepth;
} TableMAXP;

void read_maxptable(SDL_RWops* io, TableMAXP* maxp);

#endif
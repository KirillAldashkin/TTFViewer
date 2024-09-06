#include "ttf/tables/maxp.h"

void read_maxptable(SDL_RWops* io, TableMAXP* maxp) {
	read_fixed(io, &maxp->version);
	maxp->numGlyphs = SDL_ReadBE16(io);
	maxp->maxPoints = SDL_ReadBE16(io);
	maxp->maxContours = SDL_ReadBE16(io);
	maxp->maxComponentPoints = SDL_ReadBE16(io);
	maxp->maxComponentContours = SDL_ReadBE16(io);
	maxp->maxZones = SDL_ReadBE16(io);
	maxp->maxTwilightPoints = SDL_ReadBE16(io);
	maxp->maxStorage = SDL_ReadBE16(io);
	maxp->maxFunctionDefs = SDL_ReadBE16(io);
	maxp->maxInstructionDefs = SDL_ReadBE16(io);
	maxp->maxStackElements = SDL_ReadBE16(io);
	maxp->maxSizeOfInstructions = SDL_ReadBE16(io);
	maxp->maxComponentElements = SDL_ReadBE16(io);
	maxp->maxComponentDepth = SDL_ReadBE16(io);
}
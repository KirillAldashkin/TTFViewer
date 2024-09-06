#include "ttf/tables/head.h"

void read_headtable(SDL_RWops* io, TableHEAD* head) {
	read_fixed(io, &head->version);
	read_fixed(io, &head->fontRevision);
	head->checkSumAdjustment = SDL_ReadBE32(io);
	head->magicNumber = SDL_ReadBE32(io);
	head->flags = SDL_ReadBE16(io);
	head->unitsPerEm = SDL_ReadBE16(io);
	head->created = SDL_ReadBE64(io);
	head->modified = SDL_ReadBE64(io);
	head->xMin = SDL_ReadBE16(io);
	head->yMin = SDL_ReadBE16(io);
	head->xMax = SDL_ReadBE16(io);
	head->yMax = SDL_ReadBE16(io);
	head->macStyle = SDL_ReadBE16(io);
	head->lowestRecPPEM = SDL_ReadBE16(io);
	head->fontDirectionHint = SDL_ReadBE16(io);
	head->indexToLocFormat = SDL_ReadBE16(io);
	head->glyphDataFormat = SDL_ReadBE16(io);
}
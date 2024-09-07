#ifndef TABLES_HEAD_H
#define TABLES_HEAD_H

#include <SDL.h>
#include "ttf/ttf.h"

typedef struct {
	Fixed version;
	Fixed fontRevision;
	uint32_t checkSumAdjustment;
	uint32_t magicNumber;
	uint16_t flags;
	uint16_t unitsPerEm;
	longDateTime created;
	longDateTime modified;
	FWord xMin;
	FWord yMin;
	FWord xMax;
	FWord yMax;
	uint16_t macStyle;
	uint16_t lowestRecPPEM;
	int16_t fontDirectionHint;
	int16_t indexToLocFormat;
	int16_t glyphDataFormat;
} TableHEAD;

void read_headtable(SDL_RWops* io, TableHEAD* head);

#endif
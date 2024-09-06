#ifndef TABLES_GLYF_H
#define TABLES_GLYF_H

#include <SDL.h>
#include "..\ttf.h"
#include <stdint.h>

// simple glyph point flags
#define PFLAG_ON_CURVE 1
#define PFLAG_X_1BYTE 2
#define PFLAG_Y_1BYTE 4
#define PFLAG_REPEAT 8
#define PFLAG_X_SIGN_OR_SAME 16
#define PFLAG_Y_SIGN_OR_SAME 32

#define CGFLAG_ARG_1_AND_2_ARE_WORDS 1
#define CGFLAG_ARGS_ARE_XY_VALUES 2
#define CGFLAG_ROUND_XY_TO_GRID 4
#define CGFLAG_WE_HAVE_A_SCALE	8
#define CGFLAG_MORE_COMPONENTS	32
#define CGFLAG_WE_HAVE_AN_X_AND_Y_SCALE 64
#define CGFLAG_WE_HAVE_A_TWO_BY_TWO 128
#define CGFLAG_WE_HAVE_INSTRUCTIONS 256
#define CGFLAG_USE_MY_METRICS 512
#define CGFLAG_OVERLAP_COMPOUND 1024

typedef struct {
	uint16_t* endPtsOfContours;
	uint16_t instructionLength;
	uint8_t* instructions;
	uint8_t* flags;
	int16_t* xCoordinates;
	int16_t* yCoordinates;
} SimpleGlyph;

typedef struct _t_comp {
	uint16_t flags;
	uint16_t glyphIndex;
	int16_t argument1;
	int16_t argument2;
	F2Dot14 transform1;
	F2Dot14 transform2;
	F2Dot14 transform3;
	F2Dot14 transform4;
	struct _t_comp* next;
} CompoundGlyphPart;

typedef struct {
	int16_t numberOfContours;
	FWord xMin;
	FWord yMin;
	FWord xMax;
	FWord yMax;
	union {
		SimpleGlyph simple;
		CompoundGlyphPart* compound;
	} data;
} Glyph;

#define IS_SIMPLEGLYPH(glyph) ((glyph).numberOfContours > 0)
#define IS_COMPOUNDGLYPH(glyph) ((glyph).numberOfContours < 0)

int read_glyftable(SDL_RWops* io, Glyph** glyf, uint16_t glyphCount, uint32_t* glyphOffsets);
void free_glyftable(Glyph* glyf, uint16_t glyphCount);

#endif
#include "ttf/tables/glyf.h"
#include "util.h"

int read_simpleglyph(SimpleGlyph* simple, int16_t contours, SDL_RWops* io);
int read_compoundglyph(CompoundGlyphPart** compound, int16_t* count, SDL_RWops* io);

int read_glyftable(SDL_RWops* io, Glyph** glyf, uint16_t glyphCount, uint32_t* glyphOffsets) {
	(*glyf) = SDL_calloc(glyphCount, sizeof(Glyph));
	if ((*glyf) == NULL) {
		return SDL_SetError("Couldn't allocate GLYF table: %s", SDL_GetError());
	}
	int64_t start = SDL_RWtell(io);
	if (start == -1) {
		return SDL_SetError("Couldn't check start of GLYF table: %s", SDL_GetError());
	}
	for (uint16_t i = 0; i < glyphCount; i++) {
		if (SDL_RWseek(io, start + glyphOffsets[i], RW_SEEK_SET) == -1) {
			return SDL_SetError("Couldn't seek to glyph in GLYF table: %s", SDL_GetError());
		}
		Glyph* glyph = (*glyf) + i;
		glyph->numberOfContours = SDL_ReadBE16(io);
		glyph->xMin = SDL_ReadBE16(io);
		glyph->yMin = SDL_ReadBE16(io);
		glyph->xMax = SDL_ReadBE16(io);
		glyph->yMax = SDL_ReadBE16(io);
		if (IS_SIMPLEGLYPH(*glyph)) {
			if (read_simpleglyph(&glyph->data.simple, glyph->numberOfContours, io) != 0) return -1;
		} else if (IS_COMPOUNDGLYPH(*glyph)) {
			// let's store part count in glyph but don't forget about compound glyph having negative number of contours
			if (read_compoundglyph(&glyph->data.compound, &glyph->numberOfContours, io) != 0) return -1;
			glyph->numberOfContours *= -1;
		} else {
			return SDL_SetError("Encountered invalid glyph in GLYF table");
		}
	}
	return 0;
}

void free_glyftable(Glyph* glyf, uint16_t glyphCount) {
	if (glyf != NULL) {
		for (uint16_t glyph = 0; glyph < glyphCount; glyph++) {
			if (IS_SIMPLEGLYPH(glyf[glyph])) {
				FREE_NOTNULL(glyf[glyph].data.simple.endPtsOfContours);
				FREE_NOTNULL(glyf[glyph].data.simple.instructions);
				FREE_NOTNULL(glyf[glyph].data.simple.xCoordinates);
				FREE_NOTNULL(glyf[glyph].data.simple.yCoordinates);
			}
			if (IS_COMPOUNDGLYPH(glyf[glyph])) {
				CompoundGlyphPart* part = glyf[glyph].data.compound;
				while (part != NULL) {
					CompoundGlyphPart* next = part->next;
					SDL_free(part);
					part = next;
				}
			}
		}
		SDL_free(glyf);
	}
}

// Simple glyph reading
void read_simpleglyph_coords(SDL_RWops* io, uint16_t num, uint8_t* flags, uint16_t* coords, uint8_t byteFlag, uint8_t signMask) {
	// 'byteFlag' is 'PFLAG_X_1BYTE' or 'PFLAG_Y_1BYTE'
	// 'signMask' is 'PFLAG_X_SIGN_OR_SAME' or 'PFLAG_Y_SIGN_OR_SAME'
	for (uint16_t i = 0; i < num; i++) {
		if (flags[i] & byteFlag) {
			coords[i] = (int16_t)SDL_ReadU8(io);
			if (!(flags[i] & signMask)) coords[i] *= -1;
		} else {
			coords[i] = (flags[i] & signMask) ? 0 : SDL_ReadBE16(io);
		}
	}
}

int read_simpleglyph(SimpleGlyph* simple, int16_t contours, SDL_RWops* io)
{
	// allocate contours
	simple->endPtsOfContours = SDL_calloc(contours, sizeof(uint16_t));
	if (simple->endPtsOfContours == NULL) {
		return SDL_SetError("Couldn't allocate end points data: %s", SDL_GetError());
	}
	// read contours
	// also get amount of points as a biggest index of contour end point:
	//  Contour 1: 0123
	//  Contour 2:        78
	//  Contour 3:     456
	// LAST POINT:    3  6 8, so amount of points is 8+1 = 9
	uint16_t numPoints = 0;
	for (int16_t contour = 0; contour < contours; contour++) {
		simple->endPtsOfContours[contour] = SDL_ReadBE16(io);
		numPoints = max(numPoints, simple->endPtsOfContours[contour] + 1);
	}

	simple->instructionLength = SDL_ReadBE16(io);
	// allocate instructions
	simple->instructions = SDL_calloc(simple->instructionLength, 1);
	if (simple->endPtsOfContours == NULL) {
		return SDL_SetError("Couldn't allocate instructions data: %s", SDL_GetError());
	}
	// read instructions
	for (uint16_t byte = 0; byte < simple->instructionLength; byte++) {
		simple->instructions[byte] = SDL_ReadU8(io);
	}

	// allocate flags
	simple->flags = SDL_calloc(numPoints, 1);
	if (simple->flags == NULL) {
		return SDL_SetError("Couldn't allocate flags data: %s", SDL_GetError());
	}
	// read flags
	uint16_t pointsRead = 0;
	while (pointsRead < numPoints) {
		uint8_t flag = SDL_ReadU8(io);
		uint8_t count = 1;
		if (flag & PFLAG_REPEAT) {
			count = SDL_ReadU8(io) + 1;
		}
		for (uint8_t i = 0; i < count; i++) {
			simple->flags[pointsRead] = flag;
			++pointsRead;
		}
	}
	// check for flags buffer overrun
	SDL_assert(pointsRead == numPoints);

	// read X coords
	simple->xCoordinates = SDL_calloc(numPoints, sizeof(int16_t));
	if (simple->xCoordinates == NULL) {
		return SDL_SetError("Couldn't allocate X coordinates: %s", SDL_GetError());
	}
	read_simpleglyph_coords(io, numPoints, simple->flags, simple->xCoordinates, PFLAG_X_1BYTE, PFLAG_X_SIGN_OR_SAME);
	
	// read Y coords
	simple->yCoordinates = SDL_calloc(numPoints, sizeof(int16_t));
	if (simple->xCoordinates == NULL) {
		return SDL_SetError("Couldn't allocate Y coordinates: %s", SDL_GetError());
	}
	read_simpleglyph_coords(io, numPoints, simple->flags, simple->yCoordinates, PFLAG_Y_1BYTE, PFLAG_Y_SIGN_OR_SAME);
	return 0;
}

// Compound glyph reading
int read_compoundglyph(CompoundGlyphPart** compound, int16_t* count, SDL_RWops* io) {
	CompoundGlyphPart* cur = ((*compound) = SDL_calloc(1, sizeof(CompoundGlyphPart)));
	*count = 0;
	while (1) {
		if (cur == NULL) {
			return SDL_SetError("Couldn't allocate compound glyph part: %s", SDL_GetError());
		}
		(*count)++;
		// read current part
		cur->flags = SDL_ReadBE16(io);
		cur->glyphIndex = SDL_ReadBE16(io);
		if (cur->flags & CGFLAG_ARG_1_AND_2_ARE_WORDS) {
			cur->argument1 = SDL_ReadBE16(io);
			cur->argument2 = SDL_ReadBE16(io);
		} else {
			cur->argument1 = SDL_ReadU8(io);
			cur->argument2 = SDL_ReadU8(io);
		}

		if (!(cur->flags & CGFLAG_WE_HAVE_A_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_AN_X_AND_Y_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_A_TWO_BY_TWO)) {
			cur->transform1 = F2DOT14_FROM_FLOAT(1);
			cur->transform4 = F2DOT14_FROM_FLOAT(1);
		} else if (
			 (cur->flags & CGFLAG_WE_HAVE_A_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_AN_X_AND_Y_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_A_TWO_BY_TWO)) {
			cur->transform1 = SDL_ReadBE16(io);
			cur->transform4 = cur->transform1;
		} else if (
			!(cur->flags & CGFLAG_WE_HAVE_A_SCALE) &&
			 (cur->flags & CGFLAG_WE_HAVE_AN_X_AND_Y_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_A_TWO_BY_TWO)) {
			cur->transform1 = SDL_ReadBE16(io);
			cur->transform4 = SDL_ReadBE16(io);
		} else if (
			!(cur->flags & CGFLAG_WE_HAVE_A_SCALE) &&
			!(cur->flags & CGFLAG_WE_HAVE_AN_X_AND_Y_SCALE) &&
			 (cur->flags & CGFLAG_WE_HAVE_A_TWO_BY_TWO)) {
			cur->transform1 = SDL_ReadBE16(io);
			cur->transform2 = SDL_ReadBE16(io);
			cur->transform3 = SDL_ReadBE16(io);
			cur->transform4 = SDL_ReadBE16(io);
		} else {
			return SDL_SetError("Invalid transform flags in compound glyph: %s", SDL_GetError());
		}

		// finish or allocate next part
		if (!(cur->flags & CGFLAG_MORE_COMPONENTS)) {
			return 0;
		}
		cur->next = SDL_calloc(1, sizeof(CompoundGlyphPart));
		cur = cur->next;
	}
}
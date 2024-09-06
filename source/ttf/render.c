#include "ttf/render.h"
#include <SDL.h>

#define POINT_OFFSET(axis, index) \
	(float)glyph->data.simple.axis##Coordinates[index] / file->head.unitsPerEm * pxPerEm

vec2 render_contour(TtfFile* file, Renderer renderer, Glyph* glyph, vec2 pos, float pxPerEm, uint16_t start, uint16_t end, RenderData* data) {
	vec2 startPos = pos;
	vec2 finishPos;
	int run = 1;
	for (uint16_t idx = start + 1; run; idx++) {
		vec2 prevPos = pos;
		uint16_t prevIdx = idx - 1;
		if (idx == end + 1) {
			run = 0;
			idx = start;
			pos = startPos;
			finishPos = prevPos;
		} else {
			pos.x += POINT_OFFSET(x, idx);
			pos.y -= POINT_OFFSET(y, idx);
		}
		int prevOnCurve = glyph->data.simple.flags[prevIdx] & PFLAG_ON_CURVE;
		int onCurve = glyph->data.simple.flags[idx] & PFLAG_ON_CURVE;
		if (prevOnCurve && onCurve) {
			data->kind = TTF_RENDER_SEGMENT;
			data->segment.a = prevPos;
			data->segment.b = pos;
			renderer(data);
		} else if(!onCurve) {
			uint16_t nextIdx = (idx == end) ? start : idx + 1;
			vec2 bezA = prevPos;
			if (!prevOnCurve) {
				bezA.x = (bezA.x + pos.x) / 2;
				bezA.y = (bezA.y + pos.y) / 2;
			}
			vec2 nextPos;
			if (nextIdx == start) {
				nextPos = startPos;
			} else {
				nextPos.x = pos.x + POINT_OFFSET(x, nextIdx);
				nextPos.y = pos.y - POINT_OFFSET(y, nextIdx);
			}
			if (!(glyph->data.simple.flags[nextIdx] & PFLAG_ON_CURVE)) {
				nextPos.x = (nextPos.x + pos.x) / 2;
				nextPos.y = (nextPos.y + pos.y) / 2;
			}
			data->kind = TTF_RENDER_BEZIER;
			data->bezier.a = bezA;
			data->bezier.b = pos;
			data->bezier.c = nextPos;
			renderer(data);
		}
	}
	return finishPos;
}

void render_glyph(TtfFile* file, Renderer renderer, Glyph* glyph, vec2 at, float pxPerEm, int depth) {
	RenderData data;
	data.depth = depth;
	data.kind = TTF_RENDER_EMBOX;
	data.emBox.a = at;
	data.emBox.b.x = at.x + pxPerEm;
	data.emBox.b.y = at.y - pxPerEm;
	renderer(&data);
	if (IS_SIMPLEGLYPH(*glyph)) {
		uint16_t start = 0;
		vec2 pos = at;
		for (int16_t contour = 0; contour < glyph->numberOfContours; contour++) {
			uint16_t end = glyph->data.simple.endPtsOfContours[contour];
			pos.x += POINT_OFFSET(x, start);
			pos.y -= POINT_OFFSET(y, start);
			pos = render_contour(file, renderer, glyph, pos, pxPerEm, start, end, &data);
			start = end + 1;
		}
	}
	if (IS_COMPOUNDGLYPH(*glyph)) {
		CompoundGlyphPart* g = glyph->data.compound;
		while(g != NULL) {
			vec2 cat = at;
			cat.x += (float)g->argument1 / file->head.unitsPerEm * pxPerEm;
			cat.y -= (float)g->argument2 / file->head.unitsPerEm * pxPerEm;
			render_glyph(file, renderer, file->glyf + g->glyphIndex, cat, pxPerEm, depth + 1);
			g = g->next;
		}
	}
}

int render_ttf(TtfFile* file, Renderer renderer, uint16_t glyph, vec2* at, float pxPerEm) {
	if (glyph >= file->maxp.numGlyphs) {
		return SDL_SetError("Glyph id is outside of this font's range");
	}

	render_glyph(file, renderer, file->glyf + glyph, *at, pxPerEm, 0);

	// update width
	if (glyph >= file->hhea.numOfLongHorMetrics) {
		glyph = file->hhea.numOfLongHorMetrics - 1;
	}
	at->x += (float)file->hmtx.hMetrics[glyph].advanceWidth / file->head.unitsPerEm * pxPerEm;
	return 0;
}
#ifndef RENDER_H
#define RENDER_H

#include "file.h"
#include "../util.h"

#define TTF_RENDER_EMBOX 1
#define TTF_RENDER_SEGMENT 2
#define TTF_RENDER_BEZIER 3

typedef struct {
	int kind;
	int depth;
	union {
		struct {
			vec2 a;
			vec2 b;
		} segment;
		struct {
			vec2 a;
			vec2 b;
			vec2 c;
		} bezier;
		struct {
			vec2 a;
			vec2 b;
		} emBox;
	};
} RenderData;

typedef void(*Renderer)(RenderData* data);

int render_ttf(TtfFile* file, Renderer renderer, uint16_t glyph, vec2* at, float pxPerEm);
#endif
#ifndef INT24_H
#define INT24_H

#include <stdint.h>
#include <SDL.h>

typedef struct {
	float x;
	float y;
} vec2;

typedef struct { uint8_t a, b, c; } uint24_t;
uint24_t SDL_ReadBE24(SDL_RWops* src);

#define FREE_NOTNULL(ptr) { if (ptr != NULL) SDL_free(ptr); }
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define clamp(minVal, val, maxVal) min(maxVal, max(minVal, val))
#define abs(a) (((a) < 0) ? (-(a)) : (a))

#endif
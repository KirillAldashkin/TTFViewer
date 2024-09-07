#ifndef TABLES_LOCA_H
#define TABLES_LOCA_H

#include <SDL.h>
#include "ttf/ttf.h"

int read_locatable(SDL_RWops* io, uint32_t** loca, int glyphs, int is32bit);

#endif
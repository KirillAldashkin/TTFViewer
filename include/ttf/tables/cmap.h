#ifndef TABLES_CMAP_H
#define TABLES_CMAP_H

#include <SDL.h>
#include "ttf/ttf.h"

typedef struct {
	uint16_t version;
	uint16_t numberSubtables;
	struct {
		uint16_t platformID;
		uint16_t platformSpecificID;
		uint32_t offset;
	}* entries;
	uint16_t** tables;
} TableCMAP;

int read_cmaptable(SDL_RWops* io, TableCMAP* cmap);
void free_cmaptable(TableCMAP* cmap);

#endif
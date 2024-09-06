#include "util.h"

uint24_t SDL_ReadBE24(SDL_RWops* src) {
	uint24_t ret;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	ret.c = SDL_ReadU8(src);
	ret.b = SDL_ReadU8(src);
	ret.a = SDL_ReadU8(src);
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
	ret.a = SDL_ReadU8(src);
	ret.b = SDL_ReadU8(src);
	ret.c = SDL_ReadU8(src);
#else
#error SDL did not define a correct endianness
#endif
	return ret;
}

uint8_t inc_ptr8(uint8_t** ptr) {
	uint8_t old = **ptr;
	++(*ptr);
	return old;
}
uint16_t inc_ptr16(uint16_t** ptr) {
	uint16_t old = **ptr;
	++(*ptr);
	return old;
}

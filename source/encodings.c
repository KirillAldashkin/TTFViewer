#include "encodings.h"
#include "util.h"
#include <SDL.h>

#define MASK_BIT(br) ((br == 2) ? 0 : 1)
#define CHECK_MASK8(b0,b1,b2,b3,b4,b5,b6,b7) (\
	(MASK_BIT(b0) << 7) | (MASK_BIT(b1) << 6) |\
	(MASK_BIT(b2) << 5) | (MASK_BIT(b3) << 4) |\
	(MASK_BIT(b4) << 3) | (MASK_BIT(b5) << 2) |\
	(MASK_BIT(b6) << 1) | (MASK_BIT(b7) << 0))

#define VAL_BIT(br) ((br == 1) ? 1 : 0)
#define CHECK_VAL8(b0,b1,b2,b3,b4,b5,b6,b7) (\
	(VAL_BIT(b0) << 7) | (VAL_BIT(b1) << 6) |\
	(VAL_BIT(b2) << 5) | (VAL_BIT(b3) << 4) |\
	(VAL_BIT(b4) << 3) | (VAL_BIT(b5) << 2) |\
	(VAL_BIT(b6) << 1) | (VAL_BIT(b7) << 0))

#define CHECK_BITS8(num,b0,b1,b2,b3,b4,b5,b6,b7) \
	(((num) & CHECK_MASK8(b0,b1,b2,b3,b4,b5,b6,b7)) == CHECK_VAL8(b0,b1,b2,b3,b4,b5,b6,b7))
	
#define IS_UTF8CONT(num) CHECK_BITS8(num, 1,0,2,2,2,2,2,2)

uint32_t utf16to32(uint16_t** cur) {
	uint16_t* old = *cur;
	uint32_t a = inc_ptr16(cur);
	if ((a < 0xD800) || (a > 0xDFFF)) {
		return a;
	} else if(a >= 0xDC00) {
		*cur = old;
		return SDL_SetError("Invalid first word: from 0xDC00 to 0xDFFF");
	}
	uint32_t b = inc_ptr16(cur);
	if ((b < 0xDC00) || (b < 0xDFFF)) {
		*cur = old;
		return SDL_SetError("Invalid second word: outside of range from 0xDC00 to 0xDFFF");
	}
	return (a | (b & 0x3FF)) + 0x10000;
}

uint32_t utf8to32(uint8_t** cur) {
	uint8_t* old = *cur;
	if (IS_UTF8CONT(**cur)) {
		*cur = old;
		return SDL_SetError("Unexpected continuation byte");
	}
	if (CHECK_BITS8(**cur, 0,2,2,2,2,2,2,2)) {
		return inc_ptr8(cur);
	}
	if (CHECK_BITS8(**cur, 1,1,0,2,2,2,2,2)) {
		uint32_t a = inc_ptr8(cur) & 0b00011111;
		uint32_t b = inc_ptr8(cur);
		if (!IS_UTF8CONT(b)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (2nd in 2-byte sequence)");
		}
		return (a << 6) | (b & 0b00111111);
	}
	if (CHECK_BITS8(**cur, 1,1,1,0,2,2,2,2)) {
		uint32_t a = inc_ptr8(cur) & 0b00001111;
		uint32_t b = inc_ptr8(cur);
		if (!IS_UTF8CONT(b)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (2nd in 3-byte sequence)");
		}
		uint32_t c = inc_ptr8(cur);
		if (!IS_UTF8CONT(c)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (3nd in 3-byte sequence)");
		}
		return (a << 12) | ((b & 0b00111111) << 6) | (c & 0b00111111);
	}
	if (CHECK_BITS8(**cur, 1,1,1,1,0,2,2,2)) {
		uint32_t a = inc_ptr8(cur) & 0b00000111;
		uint32_t b = inc_ptr8(cur);
		if (!IS_UTF8CONT(b)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (2nd in 4-byte sequence)");
		}
		uint32_t c = inc_ptr8(cur);
		if (!IS_UTF8CONT(c)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (3nd in 4-byte sequence)");
		}
		uint32_t d = inc_ptr8(cur);
		if (!IS_UTF8CONT(d)) {
			*cur = old;
			return SDL_SetError("Expected continuation byte (4nd in 4-byte sequence)");
		}
		return (a << 18) | ((b & 0b00111111) << 12) | ((c & 0b00111111) << 6) | (d & 0b00111111);
	}
	*cur = old;
	return SDL_SetError("Invalid UTF8 prefix (neither 0b0_______, 0b110_____, 0b1110____ nor 0b11110___)");
}
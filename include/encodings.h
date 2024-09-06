#ifndef ENCODINGS_H
#define ENCODINGS_H

#include <stdint.h>

uint32_t utf16to32(uint16_t** cur);
uint32_t utf8to32(uint8_t** cur);
uint8_t inc_ptr8(uint8_t** ptr);
uint16_t inc_ptr16(uint16_t** ptr);
#endif
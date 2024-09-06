#ifndef TTF_H
#define TTF_H

#include <SDL.h>
#include <stdint.h>

// Platform constants
#define PLAT_UNICODE 0
#define PLAT_MACINTOSH 1
#define PLAT_RESERVED 2
#define PLAT_WINDOWS 3

#define PLATUNICODE_10 0
#define PLATUNICODE_11 1
#define PLATUNICODE_ISO_10646_1993 2
#define PLATUNICODE_20_BMP 3
#define PLATUNICODE_20_NON_BMP 4
#define PLATUNICODE_VARIATION 5
#define PLATUNICODE_LASTRESORT 6

#define PLATWINDOWS_SYMBOL 0
#define PLATWINDOWS_UNICODE_BMP 1
#define PLATWINDOWS_SHIFT_JIS 2
#define PLATWINDOWS_PRC 3
#define PLATWINDOWS_BIGFIVE 4
#define PLATWINDOWS_JOHAB 5
#define PLATWINDOWS_USC4 5

// TTF defined types
typedef int16_t shortFrac;
typedef struct {
	int16_t a;
	int16_t b;
} Fixed;
typedef int16_t FWord;
typedef uint16_t uFWord;
typedef int16_t F2Dot14;
typedef int64_t longDateTime;

// TTF header
typedef struct {
	uint32_t scalerType;
	uint16_t numTables;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
} OffsetTable;

typedef struct {
	char tag[4];
	uint32_t checkSum;
	uint32_t offset;
	uint32_t length;
} TableDirEntry;

// Functions
#define F2DOT14_TO_FLOAT(x) ((x) / 16384.0f)
#define F2DOT14_FROM_FLOAT(x) ((F2Dot14)((x) * 16384))

void read_fixed(SDL_RWops* io, Fixed* version);

// In some places, TTF defines two buffers and uses some 
// pointer arifmetics on a pointer to the first buffer to 
// obtain a pointer to the second buffer, assuming a fixed
// offset between both buffers (I hate them from the bottom
// of my heart for that). This function fixes that: pass 
// information about both buffers and naively calcutated 
// pointer to get a correct one.
// You can pass NULL to firstBuf to pass a byte-index to naivePtr instead
// You can pass NULL to secondBuf to get byte-index instead
void* repair_pointer(void* firstBuf, size_t firstToSecondBytes, void* secondBuf, void* naivePtr);

#endif
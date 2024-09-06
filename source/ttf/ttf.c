#include "ttf/ttf.h"
#include "util.h"

void read_fixed(SDL_RWops* io, Fixed* version) {
	version->a = SDL_ReadBE16(io);
	version->b = SDL_ReadBE16(io);
}

void* repair_pointer(void* firstBuf, size_t assumedOffsetfirstToSecond, void* secondBuf, void* naivePtr)
{
	// naive pointer is still in the first buffer and therefore is correct
	if ((size_t)naivePtr < (size_t)firstBuf + assumedOffsetfirstToSecond) {
		return naivePtr;
	}
	// naive pointer should point to the second buffer
	size_t actualOffset = (size_t)secondBuf - (size_t)firstBuf;
	return (void*)((size_t)naivePtr + actualOffset - assumedOffsetfirstToSecond);
}
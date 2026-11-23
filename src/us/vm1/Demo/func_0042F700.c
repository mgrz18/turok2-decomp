#include "common.h"
#include "structs/demo.h"

/* A timer whose durations are read from a bit stream (see Demo_ReadRunLength). */

/* Reads count bits, most significant first. */
s32 DemoStream_ReadBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

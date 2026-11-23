#include "common.h"
#include "structs/demo.h"

void Demo_ReadRunLength(DemoStream *);

/* Reads count bits, most significant first. */
static __inline__ s32 readBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

void Demo_DecodeStick(DemoStream *t) {
    t->value = readBits(t, 8);
    Demo_ReadRunLength(t);
}

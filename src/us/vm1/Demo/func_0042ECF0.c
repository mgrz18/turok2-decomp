/* WRONG CUT: REAL Demo_DecodeButtons size 0x23C (splat cut it into Demo_DecodeButtons + func_0042EE4C). */
#include "common.h"
#include "structs/demo.h"

extern void Demo_ReadRunLength(DemoStream *r);

static __inline__ s32 readBit(DemoStream *r) {
    u32 pos = r->pos++;

    return (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
}

/* Reads a nine-bit code, its bits stored in a scrambled order, then decodes it. */
void Demo_DecodeButtons(DemoStream *r) {
    r->value = readBit(r) << 15;
    r->value |= readBit(r) << 14;
    r->value |= readBit(r) << 4;
    r->value |= readBit(r) << 13;
    r->value |= readBit(r) << 3;
    r->value |= readBit(r) << 1;
    r->value |= readBit(r);
    r->value |= readBit(r) << 2;
    r->value |= readBit(r) << 8;
    Demo_ReadRunLength(r);
}

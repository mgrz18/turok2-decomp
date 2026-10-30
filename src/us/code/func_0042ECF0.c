/* WRONG CUT: REAL func_0042ECF0 size 0x23C (splat cut it into func_0042ECF0 + func_0042EE4C). */
#include "common.h"

typedef struct BitReader {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 *bits;
    /* 0x08 */ u32 pos;
} BitReader;

extern void func_0042EC30(BitReader *r);

static __inline__ s32 readBit(BitReader *r) {
    u32 pos = r->pos++;

    return (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
}

/* Reads a nine-bit code, its bits stored in a scrambled order, then decodes it. */
void func_0042ECF0(BitReader *r) {
    r->value = readBit(r) << 15;
    r->value |= readBit(r) << 14;
    r->value |= readBit(r) << 4;
    r->value |= readBit(r) << 13;
    r->value |= readBit(r) << 3;
    r->value |= readBit(r) << 1;
    r->value |= readBit(r);
    r->value |= readBit(r) << 2;
    r->value |= readBit(r) << 8;
    func_0042EC30(r);
}

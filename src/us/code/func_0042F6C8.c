#include "common.h"

typedef struct BitReader {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ u8 *data;
    /* 0x8 */ u32 pos;
} BitReader;

/* Reads the next bit. */
s32 func_0042F6C8(BitReader *r) {
    u32 bit = r->pos++;

    return (r->data[bit >> 3] & (0x80 >> (bit & 7))) != 0;
}

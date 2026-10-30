#include "common.h"

/* Bit `bit` of a big-endian bit array. */
s32 func_0042F6A4(u8 *bits, u32 bit) {
    u8 *p = bits + (bit >> 3);
    s32 mask = 0x80 >> (bit & 7);

    return (*p & mask) != 0;
}

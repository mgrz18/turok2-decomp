/* The second VM module's copy of src/us/vm1/Demo/func_0042F6A4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"

/* Bit `bit` of a big-endian bit array. */
s32 func_004685C4(u8 *bits, u32 bit) {
    u8 *p = bits + (bit >> 3);
    s32 mask = 0x80 >> (bit & 7);

    return (*p & mask) != 0;
}

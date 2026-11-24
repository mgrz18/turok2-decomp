/* The second VM module's copy of src/us/vm1/Demo/func_0042F700.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"

/* A timer whose durations are read from a bit stream (see Demo_ReadRunLength_2). */

/* Reads count bits, most significant first. */
s32 func_00468620(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

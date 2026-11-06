/* WRONG CUT: real function is 0x266B90 size 0xCC (all 51 words byte-identical to the ROM). splat cut it at 0x266C00, so its word-copy tail became func_00266C00 (now separately in src/us/code/func_00266C00.c, a false function: the beqz at +0x10 is this function branching to its own else-block). */
#include "common.h"

/* memmove: bytes unless everything is 4-aligned, then words. */
void memmove(u8 *dst, u8 *src, s32 len) {
    u8 *d;
    u8 *s;
    s32 *dw;
    s32 *sw;

    if (len == 0) {
        return;
    }
    if (((u32)src | (u32)dst | len) & 3) {
        d = dst;
        s = src;
        if (dst < src) {
            do {
                *d++ = *s++;
            } while (--len != 0);
        } else {
            d += len - 1;
            s += len - 1;
            do {
                *d-- = *s--;
            } while (--len != 0);
        }
    } else {
        dw = (s32 *)dst;
        sw = (s32 *)src;
        len >>= 2;
        if (dw < sw) {
            do {
                *dw++ = *sw++;
            } while (--len != 0);
        } else {
            dw += len - 1;
            sw += len - 1;
            do {
                *dw-- = *sw--;
            } while (--len != 0);
        }
    }
}

/* WRONG CUT: REAL Str_CatN size 0x58 (splat cut it into Str_CatN + func_00426F7C + func_00426F9C). */
#include "common.h"

/* strncat. */
u8 *Str_CatN(u8 *dst, u8 *src, s32 n) {
    u8 *p;

    p = dst;
    while (*p++) {
    }
    p--;
    while (n--) {
        if ((*p++ = *src++) == 0) {
            return dst;
        }
    }
    *p = 0;
    return dst;
}

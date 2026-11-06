/* WRONG CUT: REAL Str_CompareN_2 size 0x48 (splat cut it into Str_CompareN_2 + func_0045FEE4). Second-module copy of Str_CompareN. */
#include "common.h"

/* strncmp. */
s32 Str_CompareN_2(u8 *a, u8 *b, s32 n) {
    if (n == 0) {
        return 0;
    }
    n--;
    while (n != 0 && *a != 0 && *a == *b) {
        n--;
        a++;
        b++;
    }
    return *a - *b;
}

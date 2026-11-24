/* WRONG CUT: REAL Str_CompareN size 0x48 (splat cut it into Str_CompareN + func_00426FC4). */
#include "common.h"

/* strncmp. */
s32 Str_CompareN(u8 *a, u8 *b, s32 n) {
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

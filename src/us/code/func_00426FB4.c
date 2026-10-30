/* WRONG CUT: REAL func_00426FB4 size 0x48 (splat cut it into func_00426FB4 + func_00426FC4). */
#include "common.h"

/* strncmp. */
s32 func_00426FB4(u8 *a, u8 *b, s32 n) {
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

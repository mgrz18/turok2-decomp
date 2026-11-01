/* WRONG CUT: REAL func_0045FED4 size 0x48 (splat cut it into func_0045FED4 + func_0045FEE4). Second-module copy of func_00426FB4. */
#include "common.h"

/* strncmp. */
s32 func_0045FED4(u8 *a, u8 *b, s32 n) {
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

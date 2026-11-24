/* WRONG CUT: REAL Str_Compare size 0x38 (splat cut it into Str_Compare + func_00426D5C). */
#include "common.h"

/* strcmp. */
s32 Str_Compare(u8 *a, u8 *b) {
    while (*a == *b) {
        if (*a++ == 0) {
            return 0;
        }
        b++;
    }
    return *a - *b;
}

/* WRONG CUT: REAL func_00426D54 size 0x38 (splat cut it into func_00426D54 + func_00426D5C). */
#include "common.h"

/* strcmp. */
s32 func_00426D54(u8 *a, u8 *b) {
    while (*a == *b) {
        if (*a++ == 0) {
            return 0;
        }
        b++;
    }
    return *a - *b;
}

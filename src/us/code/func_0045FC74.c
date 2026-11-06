#include "common.h"

/* strcmp. */
s32 Str_Compare_2(u8 *a, u8 *b) {
    while (*a == *b) {
        if (*a == '\0') {
            return 0;
        }
        a++;
        b++;
    }
    return *a - *b;
}

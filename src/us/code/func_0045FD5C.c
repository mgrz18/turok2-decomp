#include "common.h"

/* Returns 1 if a "~" occurs in the first len + 1 bytes of str. */
s32 func_0045FD5C(u8 *str, s32 len) {
    do {
        if (*str++ == 0x7E) {
            return 1;
        }
    } while (--len != -1);
    return 0;
}

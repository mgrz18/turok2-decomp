#include "common.h"

/* Returns 1 if a "~" occurs in the first len + 1 bytes of str. */
s32 Pak_TextHasInvalidChar_2(u8 *str, s32 len) {
    do {
        if (*str++ == 0x7E) {
            return 1;
        }
    } while (--len != -1);
    return 0;
}

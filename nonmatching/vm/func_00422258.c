/* NON-MATCHING: 3 of 47 words differ (W batch). */
/* NON-MATCHING: 3 of 47 words differ: the space-fill loop compares the old count (i-- > 0) instead of the
 * ROM's decremented count with bgez and a pointer giv; do/while(--i >= 0) gets the test but GCC then does not
 * strength-reduce field[i]. */
#include "common.h"

extern char D_800B4B04[];
extern char D_800B4B34[];
extern u8 D_80130954[8]; /* name, padded with spaces */
extern s32 D_801309A8;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00422258(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *dst;
    s32 i;
    char c;

    if (D_801309A8 != 0) {
        item->text = D_800B4B04;
    } else {
        item->text = D_800B4B34;
        src = &D_80130954[7];
        while (*src-- == ' ' && len != 0) {
            len--;
        }
        field = item->text;
        field += 11;
        c = ' ';
        i = 8;
        while (i-- > 0) {
            field[i] = c;
        }
        dst = &field[8 - len];
        for (i = 0; i < len; i++) {
            *dst++ = D_80130954[i];
        }
    }
    return 0;
}

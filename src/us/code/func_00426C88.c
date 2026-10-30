/* WRONG CUT: REAL func_00426C88 size 0x4C (splat cut it into func_00426C88 + func_00426CAC). */
#include "common.h"

/* Pak font code -> character. */
extern u8 D_800C222C[];

/* Decodes n pak font codes from src into characters at dst; codes past the table become '~'. */
void func_00426C88(u8 *src, u8 *dst, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            *dst = D_800C222C[src[i]];
        } else {
            *dst = '~';
        }
        dst++;
    }
}

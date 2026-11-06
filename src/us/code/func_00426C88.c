/* WRONG CUT: REAL Pak_DecodeText size 0x4C (splat cut it into Pak_DecodeText + func_00426CAC). */
#include "common.h"

/* Pak font code -> character. */
extern u8 D_800C222C[];

/* Decodes n pak font codes from src into characters at dst; codes past the table become '~'. */
void Pak_DecodeText(u8 *src, u8 *dst, s32 n) {
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

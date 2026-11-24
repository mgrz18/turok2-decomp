/* The second VM module's copy of src/us/vm1/Pak/func_00426C88.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* WRONG CUT: REAL Pak_DecodeText size 0x4C (splat cut it into Pak_DecodeText + func_00426CAC). */
#include "common.h"
#include "globals/pak.h"

/* Pak font code -> character. */

/* Decodes n pak font codes from src into characters at dst; codes past the table become '~'. */
void Pak_DecodeText(u8 *src, u8 *dst, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            *dst = gPakCharTable[src[i]];
        } else {
            *dst = '~';
        }
        dst++;
    }
}

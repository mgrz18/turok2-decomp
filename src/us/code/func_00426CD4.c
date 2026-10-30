/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_encode, with Turok 2's layouts. */
#include "common.h"

/* Pak font code -> character. */
extern u8 D_800C222C[];

static __inline__ s32 toUpper(s32 c) {
    if ((u32)(c - 'a') < 26) {
        c -= 0x20;
    }
    return c;
}

/* Upper-cases n characters of src in place and encodes them as pak font codes at dst (0 when none fits). */
void func_00426CD4(u8 *src, u8 *dst, s32 n) {
    s32 i;
    s32 j;

    for (i = 0; i < n; i++) {
        src[i] = toUpper(src[i]);
        for (j = 0; j < 0x42; j++) {
            if (src[i] == D_800C222C[j]) {
                dst[i] = j;
                break;
            }
        }
        if (j == 0x42) {
            dst[i] = 0;
        }
    }
}

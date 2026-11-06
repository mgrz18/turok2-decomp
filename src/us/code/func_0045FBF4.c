/* Second-module copy of Pak_EncodeText (first module; this one at link address 0x00426BF4), calling this module's own functions at their link addresses. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_encode, with Turok 2's layouts. */
#include "common.h"

/* Pak font code -> character. */
extern u8 gPakCharTable[];

static __inline__ s32 toUpper(s32 c) {
    if ((u32)(c - 'a') < 26) {
        c -= 0x20;
    }
    return c;
}

/* Upper-cases n characters of src in place and encodes them as pak font codes at dst (0 when none fits). */
void Pak_EncodeText_2(u8 *src, u8 *dst, s32 n) {
    s32 i;
    s32 j;

    for (i = 0; i < n; i++) {
        src[i] = toUpper(src[i]);
        for (j = 0; j < 0x42; j++) {
            if (src[i] == gPakCharTable[j]) {
                dst[i] = j;
                break;
            }
        }
        if (j == 0x42) {
            dst[i] = 0;
        }
    }
}

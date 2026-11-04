/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_8040458C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"

extern u8 D_800C222C[];       /* pak font code -> character */

extern void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */

static __inline__ void DecodePakText(u8 *dst, u8 *src, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            dst[i] = D_800C222C[src[i]];
        } else {
            dst[i] = '~';
        }
    }
}

/* Reports note index of the pak on channel ch from the cached directory. */
s32 func_0045ED94(s32 ch, s32 index, s32 *exists, u8 *name, u8 *ext, s32 *size, u8 *company, u8 *code) {
    if (D_80130C60[ch] != 3) {
        return -2;
    }
    if (D_800C2224[ch].notes[index].file_size == 0) {
        *exists = 0;
    } else {
        *exists = 1;
        *size = D_800C2224[ch].notes[index].file_size >> 8;
        func_0029E460(company, &D_800C2224[ch].notes[index].company_code, 2);
        func_0029E460(code, &D_800C2224[ch].notes[index].game_code, 4);
        DecodePakText(name, D_800C2224[ch].notes[index].game_name, 16);
        DecodePakText(ext, D_800C2224[ch].notes[index].ext_name, 4);
        code[4] = 0;
        company[2] = 0;
    }
    return 0;
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_8040458C_de, with Turok 2's layouts. */
/* WRONG CUT: the real function is 0x00425E74-0x00426064 (size 0x1F0, 124 words); splat cut its tail (the two text-decoding loops and the
 * epilogue) off as func_00425F84 (0x00425F84, 0xE0, listed in part x4). All 124 words match (checked over the real extent).
 * First-module copy of src/us/code/Pak_FindNote_2.c. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern u8 gPakCharTable[];       /* pak font code -> character */

extern void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */

static __inline__ void DecodePakText(u8 *dst, u8 *src, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            dst[i] = gPakCharTable[src[i]];
        } else {
            dst[i] = '~';
        }
    }
}

/* Reports note index of the pak on channel ch from the cached directory. */
s32 Pak_FindNote(s32 ch, s32 index, s32 *exists, u8 *name, u8 *ext, s32 *size, u8 *company, u8 *code) {
    if (gPakState[ch] != 3) {
        return -2;
    }
    if (gPakDirectories[ch].notes[index].file_size == 0) {
        *exists = 0;
    } else {
        *exists = 1;
        *size = gPakDirectories[ch].notes[index].file_size >> 8;
        func_0029E460(company, &gPakDirectories[ch].notes[index].company_code, 2);
        func_0029E460(code, &gPakDirectories[ch].notes[index].game_code, 4);
        DecodePakText(name, gPakDirectories[ch].notes[index].game_name, 16);
        DecodePakText(ext, gPakDirectories[ch].notes[index].ext_name, 4);
        code[4] = 0;
        company[2] = 0;
    }
    return 0;
}

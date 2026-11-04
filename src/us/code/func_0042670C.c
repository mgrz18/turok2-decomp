/* WRONG CUT: REAL func_0042670C size 0x94 (splat cut it into func_0042670C + func_00426734). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_8.CPP, func_804050CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
extern void **D_800C2228;
extern s32 D_800C2220;
extern u8 D_800F6750;

extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_002053A8(s32);
extern void func_002052D8(s32, void **);
extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 func_004258E8(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

/* Counts the empty notes of the pak on channel ch. */
s32 func_0042670C(s32 ch, s32 *count) {
    s32 i;

    if (D_80130C60[ch] != 3) {
        return -2;
    }
    *count = 0;
    if (D_80130C70[ch] == 0) {
        for (i = 0; i < 16; i++) {
            if (D_800C2224[ch].notes[i].file_size == 0) {
                (*count)++;
            }
        }
    }
    return D_80130C70[ch];
}

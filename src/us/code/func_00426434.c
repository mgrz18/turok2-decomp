/* WRONG CUT: REAL func_00426434 size 0x4C (splat cut its last word into func_0042647C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404DDC_de, with Turok 2's layouts. */
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

/* Releases the pak directory buffer and clears its global state. */
void func_00426434(void) {
    func_002053A8(0);
    if (D_800C2228) {
        func_002052D8(0, D_800C2228);
        D_800C2228 = 0;
        D_800C2224 = 0;
    }
    D_800C2220 = 0;
}

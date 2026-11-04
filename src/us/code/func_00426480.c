/* WRONG CUT: REAL func_00426480 size 0xDC (splat cut it into func_00426480 + func_004264B4 + func_004264D0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de, with Turok 2's layouts. */
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
extern u8 D_800F6778[][4];
extern u8 D_800F6798[];
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);

/* Probes the pak in slot ch and records its state: 2 for a Rumble Pak, otherwise 3 or 1 depending on func_004258E8. */
void func_00426480(s32 ch) {
    RumblePak *cur;
    s32 ready;

    func_00285878(1);
    func_002847E0();
    D_800F6750 = 2;

    cur = &D_80130AA0[ch];
    if (D_800F6778[ch][3] != 0) {
        cur->present = 0;
        ready = 0;
    } else {
        cur->present = osMotorInit(D_800F6798, &cur->pfs, ch) == 0;
        ready = cur->present;
    }

    if (ready != 0) {
        D_80130C60[ch] = 2;
    } else if (func_004258E8(ch) != 0) {
        D_80130C60[ch] = 3;
    } else {
        D_80130C60[ch] = 1;
    }
    func_002858A4();
}

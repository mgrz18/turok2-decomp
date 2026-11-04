/* WRONG CUT: REAL func_00426C14 size 0x74 (splat cut it into func_00426C14 + func_00426C74). */
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

/* Probes slot ch for a Rumble Pak; returns whether one answered. */
s32 func_00426C14(s32 ch) {
    RumblePak *cur;
    s32 ready;

    cur = &D_80130AA0[ch];
    if (D_800F6778[ch][3] == 0) {
        cur->present = osMotorInit(D_800F6798, &cur->pfs, ch) == 0;
        ready = cur->present;
    } else {
        cur->present = 0;
        ready = 0;
    }
    return ready;
}

#include "common.h"

/* Controller pak / rumble service, run once per frame while enabled. */
typedef struct PadState {
    u8 data[0x224];
} PadState;

extern u8 D_800B6CF0;           /* service enabled */
extern s32 D_800B6CF4;          /* rescan countdown */
extern u8 D_800F5A50[];
extern PadState D_800F5EC0[4];
extern u8 D_800F6750;           /* rescan request */
extern u8 D_800F6758[];        /* OSMesgQueue: pak access lock */
extern u8 D_800F6798[];
extern s32 D_8011AC88;

extern s32 osRecvMesg(void *queue, void *msg, s32 flags);
extern s32 osSendMesg(void *queue, void *msg, s32 flags);
extern void func_0026ECF0(void *);
extern void func_002847E0(void);
extern void func_002851C0(PadState *);
extern s32 func_00285A20(void);
extern void func_00298670(void *);
extern void func_00298720(void *);

void func_00284B94(void) {
    PadState *pad;
    s32 i;

    if (D_800B6CF0 == 0) {
        return;
    }
    func_0026ECF0(D_800F5A50);
    if (osRecvMesg(D_800F6758, NULL, 0) == 0) {
        func_002847E0();
        for (i = 0, pad = D_800F5EC0; i < 4; i++, pad++) {
            func_002851C0(pad);
        }
        switch (D_800F6750) {
        case 0:
            if (func_00285A20() != 0) {
                D_800F6750 = 1;
                func_00298670(D_800F6798);
                break;
            }
            func_00298720(D_800F6798);
            break;
        case 1:
        case 2:
        case 3:
            D_800F6750 = 0;
            func_00298720(D_800F6798);
            break;
        }
        osSendMesg(D_800F6758, NULL, 0);
    }
    if (D_8011AC88 == 8) {
        if (--D_800B6CF4 <= 0) {
            D_800B6CF4 = 60;
            D_800F6750 = 1;
        }
    }
}

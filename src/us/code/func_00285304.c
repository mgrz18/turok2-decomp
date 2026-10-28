#include "common.h"

/* Per-controller rumble pak state. */
typedef struct PadState {
    s32 active;         /* 0x000 */
    s8 channel;         /* 0x004 */
    u8 pad5[0xC8 - 0x5];
    s32 motorFailed;    /* 0x0C8 */
    s32 fieldCC;        /* 0x0CC */
    s32 fieldD0;        /* 0x0D0 */
    s32 fieldD4;        /* 0x0D4 */
    u8 motor[0x140 - 0xD8];   /* 0x0D8: OSPfs */
    u8 field140[0x16C - 0x140]; /* 0x140 */
    u8 field16C[0x224 - 0x16C]; /* 0x16C */
} PadState;

extern u8 D_800B6CF0;           /* service enabled */
extern PadState D_800F5EC0[4];
extern u8 D_800F6750;           /* rescan request */
extern u8 D_800F6758[];         /* OSMesgQueue: pak access lock */
extern u8 D_800F6798[];         /* OSMesgQueue for the SI */

extern s32 osRecvMesg(void *queue, void *msg, s32 flags);
extern s32 osSendMesg(void *queue, void *msg, s32 flags);
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);
extern void func_002847E0(void);
extern void func_0026EB3C(void *, void *, s32);
extern void func_00298E10(void *);

static __inline__ void ResetPadMotor(PadState *pad) {
    osRecvMesg(D_800F6758, NULL, 1);
    func_002847E0();
    D_800F6750 = 2;
    pad->fieldCC = 0;
    pad->fieldD0 = 0;
    pad->fieldD4 = 0;
    func_0026EB3C(pad->field140, pad->field16C, 3);
    osMotorInit(D_800F6798, pad->motor, pad->channel);
    func_00298E10(pad->motor);
    func_00298E10(pad->motor);
    if (osMotorInit(D_800F6798, pad->motor, pad->channel) == 0) {
        pad->motorFailed = 1;
    }
    D_800F6750 = 2;
    osSendMesg(D_800F6758, NULL, 0);
}

void func_00285304(void) {
    PadState *pad;
    s32 i;

    for (i = 0; i < 4; i++) {
        pad = &D_800F5EC0[i];
        if (D_800B6CF0 != 0 && pad->active != 0) {
            ResetPadMotor(pad);
        }
    }
}

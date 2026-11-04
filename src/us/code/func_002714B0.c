#include "common.h"

/* A message queue used as a recursive lock: count of holders after it. */
typedef struct {
    /* 0x00 */ u8 queue[0x1C];
    /* 0x1C */ s32 count;
} MesgLock;

s32 osRecvMesg(void *, s32, s32);    /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void func_002714B0(s32 arg0) {
    s32 temp_a0;
    s32 temp_v1;
    MesgLock *temp_s0;

    temp_s0 = (MesgLock *) (arg0 + 0x110);
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        return;
    }
    __osRestoreInt(temp_a0);
}

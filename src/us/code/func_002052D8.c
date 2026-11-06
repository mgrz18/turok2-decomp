#include "common.h"

s32 CCache__DeallocateInternal(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 gCache_mutex;
extern s32 gCache_mutex_count;

void CCache__Deallocate(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg((s32) &gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    CCache__DeallocateInternal(0, arg1);
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg((s32) &gCache_mutex, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

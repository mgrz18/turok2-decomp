#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"

s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 gCache_mutex_count;

/* Releases one use of a resource; the last one clears its 0x100 flag. */
void CCache__Unlock(s32 arg0, CMemEntry *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
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
    temp_v0 = arg1->lockCount - 1;
    arg1->lockCount = temp_v0;
    if (temp_v0 == 0) {
        arg1->flags = (s32) (arg1->flags & ~0x100);
    }
    temp_v0_2 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_2);
        osSendMesg((s32) &gCache_mutex, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_2);
}

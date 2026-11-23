#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/cache.h"
#include "protos/ultra.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);


void CCache__TouchAndLock(s32 arg0, CMemEntry *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    CCache__TouchMemEntry(0, arg1);
    arg1->lockCount = (s32) (arg1->lockCount + 1);
    arg1->flags = (s32) (arg1->flags | 0x100);
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

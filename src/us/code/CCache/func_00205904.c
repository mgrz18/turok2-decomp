#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/ultra.h"
#include "protos/osint.h"
#include "protos/cache.h"

/* Two cache references. */
typedef struct {
    /* 0x00 */ CMemEntry *first;
    /* 0x04 */ CMemEntry *second;
} CacheRefs;



void CCache__ReleaseAndReplaceData(s32 arg0, void *refs, CMemEntry *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    CMemEntry *temp_a0_2;
    CMemEntry *temp_a0_3;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_a0_2 = ((CacheRefs *)refs)->first;
    if (temp_a0_2 != NULL) {
        temp_v0 = temp_a0_2->lockCount - 1;
        temp_a0_2->lockCount = temp_v0;
        if (temp_v0 == 0) {
            temp_a0_2->flags &= ~0x100;
        }
    }
    temp_a0_3 = ((CacheRefs *)refs)->second;
    if (temp_a0_3 != NULL) {
        temp_v0_2 = temp_a0_3->lockCount - 1;
        temp_a0_3->lockCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            temp_a0_3->flags &= ~0x100;
        }
    }
    ((CacheRefs *)refs)->first = arg2;
    temp_v0_3 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_3);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_3);
}

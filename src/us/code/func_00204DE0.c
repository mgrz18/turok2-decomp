#include "common.h"
#include "globals/cache.h"
#include "protos/cache.h"

s32 osRecvMesg(OSMesgQueue *queue, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* Turok 3's CCache__Advance, once a frame: counts the frame delay down,
 * ages the cache and compacts it 10 steps under the cache lock. */
void CCache__Advance(void) {
    u32 mask;
    u32 mask2;
    s32 i;

    if (gCache.frameDelay != 0) {
        gCache.frameDelay--;
    }
    gCache.ageCount++;

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, NULL, 1);
        i = 0;
    } else {
        __osRestoreInt(mask);
        i = 0;
    }
    do {
        CCache__Compact(0, 0);
        i++;
    } while (i < 10);
    mask2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask2);
        osSendMesg(&gCache.mutex.queue, NULL, 1);
        return;
    }
    __osRestoreInt(mask2);
}

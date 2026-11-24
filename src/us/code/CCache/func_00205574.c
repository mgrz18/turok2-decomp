/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00253A74.c, func_00253A74.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/cache.h"
#include "protos/cache.h"
#include "protos/ultra.h"
#include "protos/osint.h"

#include "structs/cache.h"
#include "globals/cache.h"
/* The port: typed on gCache (structs/cache.h) and CMemEntry/CCacheEntry. The ROM reaches the
 * cache through a B12Mgr view of gCache (Rage Wars offsets) and B12Node/B12Msg views at N64 offsets: on the host
 * they missed the cache's own fields (the display-list locks landed in CMemEntry.lockCount, so
 * a model still in the frame's display list could be freed; the cache thread waited on a queue
 * that was never created, so async loads and display-list releases never completed). */
s32 CCache__EndDisplayList(void) {
    u32 mask;
    u32 m2;
    s32 r;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    r = gCache.dlistValid[gCache.currentDlist];
    gCache.currentDlist = -1;
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    return r;
}

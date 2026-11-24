/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002533DC.c, func_002533DC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/cache.h"
#include "protos/ultra.h"
#include "protos/osint.h"

#include "protos/cache.h"
B12Node *CCache__Allocate(s32 unused, s32 a1, u32 a2, sptr a3) {
    u32 mask;
    u32 m2;
    B12Node *r;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    r = ((B12Node *(*)(s32, s32, u32, s32))CCache__AllocateInternal)(0, a1, a2, a3);   /* the ROM passes a fourth argument */
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    return r;
}

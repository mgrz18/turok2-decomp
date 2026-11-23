/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802511AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"
#include "protos/cache.h"

extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void CCache__WaitForDisplayLists(void) {
    extern s32 osSendMesg(void *, sptr, s32);
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    u32 temp_a0;
    u32 temp_v0;
    var_s1 = 0;
    var_s0 = 0;
    do {
        if (gCache_dlistCount[var_s0] != 0) {
            if (gCache_currentDlist != var_s0) {
                var_s1 += 1;
                gCache_dlistBlocking[var_s0] = 1;
                goto block_7;
            }
            gCache_dlistValid[var_s0] = 0;
            CCache__ClearDisplayListLocks(0, var_s0);
            var_s0 += 1;
        } else {
block_7:
            var_s0 += 1;
        }
    } while (var_s0 < 2);
    if (var_s1 != 0) {
        temp_v0 = __osDisableInt();
        temp_v1 = gCache_mutex_count - 1;
        gCache_mutex_count = temp_v1;
        if (temp_v1 != 0) {
            __osRestoreInt(temp_v0);
            osSendMesg(&gCache_mutex, 0, 1);
            var_s0_2 = 0;
        } else {
            __osRestoreInt(temp_v0);
            var_s0_2 = 0;
        }
        if (var_s1 > 0) {
            do {
                osRecvMesg(&gCache.blocker.queue, 0, 1);
                var_s0_2 += 1;
            } while (var_s0_2 < var_s1);
        }
        temp_a0 = __osDisableInt();
        temp_v1_2 = gCache_mutex_count + 1;
        gCache_mutex_count = temp_v1_2;
        if (temp_v1_2 != 1) {
            __osRestoreInt(temp_a0);
            osRecvMesg(&gCache_mutex, 0, 1);
            return;
        }
        __osRestoreInt(temp_a0);
    }
}

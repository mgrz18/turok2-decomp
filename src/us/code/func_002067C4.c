/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254F9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"

extern s32 gCache;
extern int gCache_dlistBlocking[];
extern s32 gCache_dlistCount[];
extern s32 gCache_blocker;
extern void osSendMesg(void *arg0, s32 arg1, s32 arg2);
void CCache__ReleaseDisplayList(s32 unused, s32 arg1) {
    s32 mask;
    u32 count;
    s32 new_var;
    u32 new_var2;
    s32 offset;
    u32 limit;
    if (gCache_dlistBlocking[arg1] != 0) {
        gCache_dlistBlocking[arg1] = 0;
        osSendMesg(&gCache_blocker, 0, 1);
    }
    mask = -0x201;
    if (arg1 != 0) {
        mask = -0x401;
    }
    do { count = 0; limit = gCacheEntryCount; } while (0);
    new_var2 = limit;
    if (limit != 0) {
        offset = count;
        do {
            new_var = offset + gCache;
            count += 1;
            *(s32 *)&((Node80254C10 *)new_var)->flags &= mask;
            offset += 0x28;
        } while (count < new_var2);
    }
    do { gCache_dlistCount[arg1] = 0; } while (0);
}

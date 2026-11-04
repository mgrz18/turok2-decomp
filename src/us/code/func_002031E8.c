/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00251448.c, func_802514A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/cache.h"

extern s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent);
struct Pool;
struct Pool {
    s32 count;
    char pad[0x40];
    s32 field44;
};
struct Pool;
typedef struct Pool Pool;
extern Pool D_800D8DAC;
extern void **D_800D81D4;
extern s32 CList__AddHead(void *arg0, s32 arg1);
static __inline__ void *pool_alloc(s32 arg1) {
    void *temp_s0;
    if (D_800D8DAC.count == 0) {
        return 0;
    }
    D_800D8DAC.count -= 1;
    temp_s0 = D_800D81D4[D_800D8DAC.count];
    ((CMemEntry *)(temp_s0))->flags = MEMENTRY_FLAG_ALLOCATED;
    ((CMemEntry *)(temp_s0))->lockCount = 0;
    ((CMemEntry *)(temp_s0))->data = 0;
    ((CMemEntry *)(temp_s0))->lockedNext = 0;
    ((CMemEntry *)(temp_s0))->lockedLast = 0;
    ((CMemEntry *)(temp_s0))->cacheEntry = 0;
    ((CMemEntry *)(temp_s0))->age = D_800D8DAC.field44;
    CList__AddHead((char *)&D_800D8DAC - 0xBCC, (s32)temp_s0);
    ((CMemEntry *)(temp_s0))->flags |= (arg1 & 0xC);
    return temp_s0;
}
void *func_002031E8(s32 unused0, s32 flags) {
    void *node;
    node = pool_alloc(flags);
    if (node == 0) {
        s32 keep = flags & 0x10;
        if (func_00203068(0, 2, keep == 0) != 0) {
            node = pool_alloc(flags);
        }
    }
    return node;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_9.c, func_8025558C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"

extern s32 D_800AB0B8;
void CHashTable__GetLongestChain(void) {
    s32 var_a1;
    s32 var_v1;
    u32 var_a0;
    u32 var_a0_2;
    u32 limit;
    HashEntry *var_v0;
    HashEntry *buckets;
    var_a1 = 0;
    var_a0 = var_a1;
    limit = gHashSize;
    if (limit != 0) {
        var_v1 = limit;
        do {
            var_a0 += 1;
        } while (var_a0 < (u32)var_v1);
    }
    var_a0_2 = 0;
    if (gHashSize != 0) {
        buckets = gHashTable;
        do {
            var_v0 = &buckets[var_a0_2];
            var_v1 = 0;
            if (var_v0 != 0) {
                do {
                    var_v0 = var_v0->next;
                    var_v1 += 1;
                } while (var_v0 != 0);
            }
            if (var_v1 < var_a1) {
                var_v1 = var_a1;
            }
            var_a0_2 += 1;
            var_a1 = var_v1;
        } while (var_a0_2 < gHashSize);
    }
    if (D_800AB0B8 != 0) {
        D_800AB0B8 = 0;
    }
}

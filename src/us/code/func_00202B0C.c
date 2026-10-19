/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_9.c, func_8025558C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 D_800AB0B8;
extern s32 D_800AB0B0;
void func_00202B0C(void) {
    extern s32 D_800D81C4;
    s32 var_a1;
    s32 var_v1;
    u32 var_a0;
    u32 var_a0_2;
    u32 limit;
    Shared_HashLink *var_v0;
    Shared_HashLink *buckets;
    var_a1 = 0;
    var_a0 = var_a1;
    limit = D_800AB0B0;
    if (limit != 0) {
        var_v1 = limit;
        do {
            var_a0 += 1;
        } while (var_a0 < (u32)var_v1);
    }
    var_a0_2 = 0;
    if (D_800AB0B0 != 0) {
        buckets = (Shared_HashLink *)D_800D81C4;
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
        } while (var_a0_2 < D_800AB0B0);
    }
    if (D_800AB0B8 != 0) {
        D_800AB0B8 = 0;
    }
}

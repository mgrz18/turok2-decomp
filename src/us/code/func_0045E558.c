/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80405454_de_2.c, func_80405CDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/cache.h"

extern void GfxPool_Free_2(struct RecordRecAt1C *arg0);
void GfxPool_Free_2(struct RecordRecAt1C *arg0) {
    sptr temp_a1;
    sptr temp_a1_2;
    sptr temp_a1_3;
    sptr temp_a1_4;
    sptr temp_a1_5;
    s32 var_s1;
    s32 var_v1;
    temp_a1 = arg0->b;
    if (temp_a1 != 0) {
        CCache__Deallocate(0, temp_a1);
    }
    temp_a1_2 = arg0->c;
    if (temp_a1_2 != 0) {
        CCache__Deallocate(0, temp_a1_2);
    }
    temp_a1_3 = arg0->d;
    if (temp_a1_3 != 0) {
        CCache__Deallocate(0, temp_a1_3);
    }
    temp_a1_4 = arg0->e;
    if (temp_a1_4 != 0) {
        CCache__Deallocate(0, temp_a1_4);
    }
    var_s1 = 0;
    do {
        temp_a1_5 = arg0->values[var_s1];
        if (temp_a1_5 != 0) {
            CCache__Deallocate(0, temp_a1_5);
        }
        var_s1 += 1;
    } while (var_s1 < 3);
    var_v1 = 2;
    arg0->a = 0;
    arg0->b = 0;
    arg0->c = 0;
    arg0->d = 0;
    arg0->e = 0;
    do {
        arg0->values[var_v1] = 0;
        var_v1 -= 1;
    } while (var_v1 >= 0);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8200.c, func_802B3130_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002933B0(void *arg0);
extern void func_002933E0(void *arg0, void **arg1);
s32 func_00294090(void *arg0, void **arg1, s16 arg2) {
    s16 var_a2;
    s32 var_s2;
    s32 var_v0;
    void *var_s0;
    var_a2 = arg2;
    var_s0 = ((ALSynth_func_802B3000_de *)arg0)->pLameList.next;
    var_s2 = 0;
    if ((var_s0 != 0) || (var_s0 = ((ALSynth_func_802B3000_de *)arg0)->pFreeList.next, (var_s0 != 0))) {
        *arg1 = var_s0;
        func_002933B0(var_s0);
        func_002933E0(var_s0, &((ALSynth_func_802B3000_de *)arg0)->pAllocList.next);
    } else {
        var_s0 = ((ALSynth_func_802B3000_de *)arg0)->pAllocList.next;
        var_v0 = var_s2;
        if (var_s0 != 0) {
            do {
                if ((var_a2 >= ((Node_func_802B3130_de *)var_s0)->limit->limit) &&
                    (((Node_func_802B3130_de *)var_s0)->flags == 0)) {
                    *arg1 = var_s0;
                    var_s2 = 1;
                    var_a2 = (s16)(u16)((Node_func_802B3130_de *)var_s0)->limit->limit;
                }
                var_s0 = ((Node_func_802B3130_de *)var_s0)->next;
                var_v0 = var_s2;
            } while (var_s0 != 0);
        }
    }
    return var_s2;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B5540_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 func_00294E3C(Obj_func_802B3EDC_de *arg0, s32 arg1, s32 arg2) {
    int new_var;
    s32 *base;
    s32 count;
    base = arg0->base;
    if (arg1 == 2) {
        new_var = arg0->count;
        count = new_var;
        base[count] = arg2;
        new_var = count + 1;
        arg0->count = new_var;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8370.c, func_802B3350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *func_00294B58(void);
extern void *func_00294B58(void);
void func_002942B0(void *arg0, void *arg1, u8 arg2) {
    s32 new_var;
    void *Shared_func_802B3350_de_v0;
    void *temp_a0;
    SharedRecV5 fn;
    if ((((ObjectStateC_2 *)(arg1))->unk_8.v0) != 0) {
        Shared_func_802B3350_de_v0 = func_00294B58();
        if (Shared_func_802B3350_de_v0 != 0) {
            new_var = (((struct Shape_typemap_71 *)(arg0))->field_1C) + (((struct IntegerStateDC_2 *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->state);
            ((ObjectState10_3 *)(Shared_func_802B3350_de_v0))->unk_8 = 0xC;
            ((ObjectState10_3 *)(Shared_func_802B3350_de_v0))->unk_C = arg2;
            ((ObjectState10_3 *)(Shared_func_802B3350_de_v0))->unk_0 = 0;
            ((ObjectState10_3 *)(Shared_func_802B3350_de_v0))->unk_4 = new_var;
            temp_a0 = ((struct Draw *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->model;
            fn = ((CallbackStateC_4 *)(temp_a0))->callback;
            fn(temp_a0, 3, Shared_func_802B3350_de_v0);
        }
    }
}
extern void *func_00294B58(void);

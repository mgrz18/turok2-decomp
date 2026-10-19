/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/synfreevoice.c, alSynFreeVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *func_00294B58(void);
extern void func_00294B9C(s32 arg0, void *arg1);
void func_00294160(void *arg0, void *arg1) {
    void *Shared_func_802B3200_de_v0;
    void *temp_a0;
    SharedRecV8 fn;
    s32 new_var;
    if ((((ObjectStateC_2 *)(arg1))->unk_8.v0) != 0) {
        if ((((struct IntegerStateDC_2 *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->state) != 0) {
            Shared_func_802B3200_de_v0 = func_00294B58();
            if (Shared_func_802B3200_de_v0 != 0) {
                new_var = (((struct Shape_typemap_71 *)(arg0))->field_1C) + (((struct IntegerStateDC_2 *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->state);
                ((ObjectLinks10_2 *)(Shared_func_802B3200_de_v0))->unk_8 = 0;
                ((ObjectLinks10_2 *)(Shared_func_802B3200_de_v0))->unk_4 = new_var;
                ((ObjectLinks10_2 *)(Shared_func_802B3200_de_v0))->unk_C = ((ObjectStateC_2 *)(arg1))->unk_8.v0;
                temp_a0 = ((struct Draw *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->model;
                fn = ((CallbackStateC_4 *)(temp_a0))->callback;
                fn(temp_a0, 3, Shared_func_802B3200_de_v0);
                ((ObjectStateC_2 *)(arg1))->unk_8.v0 = 0;
            }
        } else {
            func_00294B9C((s32) arg0, ((ObjectStateC_2 *)(arg1))->unk_8.v0);
            ((ObjectStateC_2 *)(arg1))->unk_8.v0 = 0;
        }
    }
}

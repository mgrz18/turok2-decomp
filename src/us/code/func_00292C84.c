/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/seqplayer_802B6BE4.c, func_802B1D24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00292C84(void *arg0);
extern s32 func_00291034(void *, s16 *, s32);
extern s32 func_00293254(void *arg0, s32 *arg1);
extern s32 func_00293028(void *arg0);
extern void func_0029321C(void *arg0, void *arg1);
void func_00292C84(void *arg0) {
    ALEvent sp10;
    s32 sp20;
    s32 temp_v1;
    void *temp_s0;
    temp_s0 = ((ObjectLinks88 *)(arg0))->unk_18;
    if ((((ObjectLinks88 *)(arg0))->unk_2C == 1) && (temp_s0 != 0) &&
        (func_00293254(temp_s0, &sp20) & 0xFF)) {
        if ((((ObjectLinks88 *)(arg0))->unk_84 != 0) &&
            ((func_00293028(temp_s0) + sp20) >=
             ((struct func_80254D70_S2 *) ((ObjectLinks88 *) arg0)->unk_80)->unk8)) {
            func_0029321C(temp_s0, ((ObjectLinks88 *)(arg0))->unk_7C);
            temp_v1 = ((ObjectLinks88 *)(arg0))->unk_84;
            if (temp_v1 != -1) {
                ((ObjectLinks88 *)(arg0))->unk_84 = temp_v1 - 1;
            }
        }
        sp10.type = 0;
        func_00291034(&((ObjectLinks88 *)(arg0))->unk_48, &sp10,
                      sp20 * ((ObjectLinks88 *)(arg0))->unk_24);
    }
}

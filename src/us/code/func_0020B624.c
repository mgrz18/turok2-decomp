/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026AC38_de_7.c, func_8026D9D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Render_DrawBatches();
extern void Render_FlushModelBatch();
extern void Render_FlushBatches();
extern s32 D_800DD170;
extern void CCache__Unlock(s32 arg0, s32 arg1);
extern s32 D_800DD178[];
void Render_FlushBatches(void) {
    extern void CCache__LockForDisplayList(s32 arg0, s32 arg1);
    s32 temp_s0;
    s32 var_s1;
    Render_FlushModelBatch();
    var_s1 = 0;
    Render_DrawBatches();
    if (D_800DD170 > 0) {
        do {
            temp_s0 = D_800DD178[var_s1];
            CCache__LockForDisplayList(0, temp_s0);
            CCache__Unlock(0, temp_s0);
            var_s1 += 1;
        } while (var_s1 < D_800DD170);
    }
}

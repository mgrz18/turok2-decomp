/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026AC38_de_7.c, func_8026D9D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/batch.h"

extern void Render_DrawBatches();
extern void Render_FlushModelBatch();
extern void Render_FlushBatches();
extern void CCache__Unlock(s32 arg0, sptr arg1);
void Render_FlushBatches(void) {
    extern void CCache__LockForDisplayList(s32 arg0, s32 arg1);
    sptr temp_s0;
    s32 var_s1;
    Render_FlushModelBatch();
    var_s1 = 0;
    Render_DrawBatches();
    if (sBatchResourceCount > 0) {
        do {
            temp_s0 = (sptr) sBatchResources[var_s1];
            CCache__LockForDisplayList(0, temp_s0);
            CCache__Unlock(0, temp_s0);
            var_s1 += 1;
        } while (var_s1 < sBatchResourceCount);
    }
}

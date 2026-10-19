/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044524C_us_rev1_14.c, func_804453F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002759C4(s32 a);
extern s32 D_800C1F78;
s32 func_0041A028(s32 arg0, struct RecordRecAt20V1 *arg1) {
    s32 var_a0;
    var_a0 = D_800C1F78;
    if (arg1->inner->unkB0 & 0x20202) {
        var_a0 -= 1;
        if (var_a0 < 0) {
            var_a0 = 0x16;
        } else if (var_a0 == 0xE) {
            var_a0 = 0xB;
        }
    }
    if (arg1->inner->unkB0 & 0x4D101) {
        var_a0 += 1;
        if (var_a0 >= 0x17) {
            var_a0 = 0;
        } else if (var_a0 == 0xC) {
            var_a0 = 0xF;
        }
    }
    D_800C1F78 = var_a0;
    func_002759C4(var_a0);
    return 0;
}

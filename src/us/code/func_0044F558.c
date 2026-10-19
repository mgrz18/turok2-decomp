/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_80442488_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
s32 func_0044F558(func_8022A404_S1 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 temp_s0;
    s32 var_s0;
    s32 var_v0;
    var_s0 = arg1;
    if (((s32 (*)(s32))func_00285628)(arg0->unk20) != 0) {
        var_s0 -= arg2;
        if (var_s0 < arg3) {
            var_s0 = arg3;
            if (arg5 != 0) {
                var_s0 = arg4;
            }
        }
    }
    var_v0 = var_s0;
    if (((s32 (*)(s32))func_00285670)(arg0->unk20) != 0) {
        temp_s0 = var_s0 + arg2;
        var_v0 = temp_s0;
        if (arg4 < temp_s0) {
            var_s0 = arg4;
            if (arg5 != 0) {
                var_s0 = arg3;
            }
            var_v0 = var_s0;
        }
    }
    return var_v0;
}

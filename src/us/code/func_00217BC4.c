/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_80260724_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00217BC4(u32 arg0, u32 arg1, u32 arg2);
void func_00217BC4(u32 arg0, u32 arg1, u32 arg2) {
    u32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    u32 temp_a1;
    u32 temp_a2;
    u32 temp_shift;
    u32 var_a2;
    u32 var_a3;
    u32 var_v1;
    temp_a1 = (1 << arg1) - 1;
    var_v1 = temp_a1;
    temp_v0 = arg0 & 0x1F;
    temp_shift = temp_v0;
    temp_a2 = arg2 & temp_a1;
    if (temp_v0 == 0) {
        var_a3 = temp_a2;
        temp_a1 = 0;
        var_a2 = 0;
    } else {
        var_v1 = temp_a1 << temp_shift;
        temp_v0_2 = 0x20 - temp_shift;
        temp_a1 >>= temp_v0_2;
        var_a3 = temp_a2 << temp_shift;
        var_a2 = temp_a2 >> temp_v0_2;
    }
    temp_a0 = (arg0 & 0xF0000000) | ((u32)(arg0 & 0x0FFFFFE0) >> 3);
    *(s32 *)temp_a0 = (*(s32 *)temp_a0 & ~var_v1) | var_a3;
    ((struct func_80203E78_S1 *) temp_a0)->unk4 = (((struct func_80203E78_S1 *) temp_a0)->unk4 & ~temp_a1) | var_a2;
}

/* WRONG CUT: real func_0044F2FC is 0x44F2FC-0x44F3C0 (size 0xC4); splat split it at func_0044F33C (its case bodies). MATCHES whole. Second-module copy of func_004162FC. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_8044222C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_0044F2FC(int c, unsigned char next, float sx, float sy);
f32 func_0044F2FC(s32 arg0, u8 arg1, f32 arg2, f32 arg3) {
    f32 var_f0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a1_2;
    temp_a0 = arg0 & 0xFF;
    var_f0 = 1.0f;
    switch (temp_a0) {
    case 0x24:
    case 0x26:
    case 0x7E:
        return arg3;
    case 0x49:
    case 0x69:
        var_f0 = 0.5f;
    default:
        break;
    case 0x4C:
    case 0x6C:
        temp_a1 = arg1 & 0xFF;
        if ((temp_a1 == 0x74) || (temp_a1 == 0x54)) {
            var_f0 = 0.7f;
        }
        break;
    case 0x41:
    case 0x61:
        temp_a1_2 = arg1 & 0xFF;
        if ((temp_a1_2 == 0x74) || (temp_a1_2 == 0x54)) {
            var_f0 = 0.8f;
        }
        break;
    case 0x4D:
    case 0x6D:
        var_f0 = 1.2f;
        break;
    case 0x21:
    case 0x2E:
        var_f0 = 0.4f;
        break;
    }
    return var_f0 * arg2;
}

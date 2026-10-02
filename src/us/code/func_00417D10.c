#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800F5EC0;
extern M2C_UNK D_8011AAD8;

s32 func_00417D10(void) {
    M2C_UNK *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = &D_8011AAD8;
    var_v1 = 0;
loop_1:
    if (*(((__typeof__(&D_800F5EC0))((s8 *)&D_800F5EC0 + var_v1))) != 0) {
        var_a2 += 1;
        if (M2C_FIELD(var_a0, u8 *, 0xA8) == 0) {
            return 0;
        }
    }
    var_a0 = (__typeof__(var_a0))((s8 *)((__typeof__(var_a0))((s8 *)var_a0 + 0x40)));
    var_a1 += 1;
    var_v1 += 0x224;
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}

#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0028B430(s32);                         /* extern */

void func_0028CD68(s32 arg0) {
    s32 temp_a0;
    s32 temp_s0;
    s32 var_a0;
    s32 var_a0_2;

    temp_s0 = arg0 & 0xF;
    temp_a0 = (arg0 >> 4) & 0xF;
    if (temp_a0 >= 0xA) {
        var_a0 = temp_a0 + 0x57;
    } else {
        var_a0 = temp_a0 | 0x30;
    }
    func_0028B430(var_a0);
    var_a0_2 = temp_s0 | 0x30;
    if (temp_s0 >= 0xA) {
        var_a0_2 = temp_s0 + 0x57;
    }
    func_0028B430(var_a0_2);
}

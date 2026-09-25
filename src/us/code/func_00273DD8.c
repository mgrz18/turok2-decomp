#include "common.h"
#include "m2c_macros.h"

s32 func_00293EC0(s32);

s32 func_00293580(void *);                          /* extern */
M2C_UNK func_00293E60(void *, s16);                 /* extern */

void func_00273DD8(s32 arg0, s16 arg1) {
    void *temp_a0;
    void *var_s0;
    void *var_s1;

    var_s0 = (arg1 * 0xCC) + arg0;
    var_s0 += 4;
    temp_a0 = M2C_FIELD(var_s0, void **, 0xB0);
    M2C_FIELD(var_s0, s32 *, 0xAC) = 1;
    M2C_FIELD(var_s0, s32 *, 0x50) = 0;
    var_s1 = temp_a0 + 0x84;
    if (M2C_FIELD(var_s0, s32 *, 0x10) != M2C_FIELD(temp_a0, s32 *, 0x104)) {
        func_00293E60(var_s1, M2C_FIELD((temp_a0 + (M2C_FIELD(var_s0, s32 *, 0) << 1)), s16 *, 0xDC));
        if (func_00293580(var_s1) != 0) {
            func_00293EC0((s32) var_s1);
        }
        M2C_FIELD(var_s0, s32 *, 4) = -1;
    }
}

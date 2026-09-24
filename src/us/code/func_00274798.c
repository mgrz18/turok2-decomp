#include "common.h"
#include "m2c_macros.h"

s32 func_00293D90(s32, s32);

M2C_UNK func_00293E60(s32, s16);                    /* extern */

void func_00274798(void *arg0, s32 arg1) {
    s32 var_s0;
    s32 var_s1;
    void *temp_v0;

    var_s1 = arg1;
    var_s0 = M2C_FIELD(arg0, s32 *, 0xB0);
    temp_v0 = var_s0 + (M2C_FIELD(arg0, s32 *, 0) * 2);
    var_s0 += 0x84;
    func_00293E60(var_s0, M2C_FIELD(temp_v0, s16 *, 0xDC));
    func_00293D90(var_s0, var_s1 & 0xFF);
}

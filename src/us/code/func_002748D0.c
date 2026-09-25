#include "common.h"
#include "m2c_macros.h"

s32 func_00293530(s32, s32);

void func_002748D0(void *arg0) {
    s32 temp_a0;
    void *var_s0;
    void *var_s1;

    var_s0 = arg0;
    temp_a0 = M2C_FIELD(var_s0, s32 *, 0xB0);
    var_s1 = temp_a0 + 0x7C;
    func_00293530(temp_a0 + 0x84, (s32) M2C_FIELD((var_s1 + (M2C_FIELD(var_s0, s32 *, 0) << 1)), s16 *, 0x60));
    var_s1 += M2C_FIELD(var_s0, s32 *, 0) * 2;
    M2C_FIELD(var_s1, s16 *, 0x60) = -1;
    M2C_FIELD(var_s0, s16 *, 0x38) = 0;
    M2C_FIELD(var_s0, s32 *, 0xC) = -1;
    M2C_FIELD(var_s0, s32 *, 8) = -1;
    M2C_FIELD(var_s0, s32 *, 0xA8) = -1;
    M2C_FIELD(var_s0, s16 *, 0x3A) = -1;
    M2C_FIELD(var_s0, s32 *, 0xB4) = -1;
}

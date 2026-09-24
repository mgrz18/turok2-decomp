#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0027392C(s32, M2C_UNK);                /* extern */
M2C_UNK func_00276658(s32, M2C_UNK);                /* extern */
M2C_UNK func_00276DFC(void *);                      /* extern */
M2C_UNK func_0029B6F0(void *, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK func_0029B820(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 func_0029DFF0();                                /* extern */
M2C_UNK func_0029E010(s32);                         /* extern */

void func_002717C0(void *arg0, M2C_UNK arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_a0;
    void *temp_s0;
    void *temp_s0_2;

    temp_s0 = arg0 + 0x110;
    temp_a0 = func_0029DFF0();
    temp_v1 = M2C_FIELD(temp_s0, s32 *, 0x1C) + 1;
    M2C_FIELD(temp_s0, s32 *, 0x1C) = temp_v1;
    if (temp_v1 != 1) {
        func_0029E010(temp_a0);
        func_0029B6F0(temp_s0, 0, 1);
        var_a0 = arg0 + 0x1DB8;
    } else {
        func_0029E010(temp_a0);
        var_a0 = arg0 + 0x1DB8;
    }
    func_0027392C(var_a0, arg1);
    func_00276658(arg0 + 0x138, arg1);
    temp_s0_2 = arg0 + 0x110;
    M2C_FIELD(arg0, s32 *, 0x2B9C) = 0;
    temp_v0 = func_0029DFF0();
    temp_v1_2 = M2C_FIELD(temp_s0_2, s32 *, 0x1C) - 1;
    M2C_FIELD(temp_s0_2, s32 *, 0x1C) = temp_v1_2;
    if (temp_v1_2 != 0) {
        func_0029E010(temp_v0);
        func_0029B820(temp_s0_2, 0, 1);
    } else {
        func_0029E010(temp_v0);
    }
    func_00276DFC(arg0 + 0x2BC0);
}

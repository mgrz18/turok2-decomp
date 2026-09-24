#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B820(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 func_0029DFF0();                                /* extern */
M2C_UNK func_0029E010(s32);                         /* extern */

void func_00271514(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    void *temp_s0;

    temp_s0 = arg0 + 0x110;
    temp_v0 = func_0029DFF0();
    temp_v1 = M2C_FIELD(temp_s0, s32 *, 0x1C) - 1;
    M2C_FIELD(temp_s0, s32 *, 0x1C) = temp_v1;
    if (temp_v1 != 0) {
        func_0029E010(temp_v0);
        func_0029B820(temp_s0, 0, 1);
        return;
    }
    func_0029E010(temp_v0);
}

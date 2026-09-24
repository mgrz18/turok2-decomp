#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00210790(s32, s32);                    /* extern */
M2C_UNK func_00210B38(s32, f32);                    /* extern */

void func_00262B58(s32 arg0, void *arg1) {
    void *temp_a0;
    void *temp_s1;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg1, void **, 0);
    temp_s1 = M2C_FIELD(temp_a0, s32 *, 0x14) + 0x14;
    if (M2C_FIELD(arg1, s32 *, 0x90) == M2C_FIELD(temp_s1, s32 *, 0x38)) {
        func_00210790(arg0, M2C_FIELD(temp_a0, s32 *, 0x24C));
    }
    if (M2C_FIELD(arg1, s32 *, 0x90) == M2C_FIELD(temp_s1, s32 *, 0x3C)) {
        temp_v0 = M2C_FIELD(arg1, void **, 0);
        func_00210B38(arg0, M2C_FIELD(temp_v0, f32 *, 0x1D8) - M2C_FIELD(temp_v0, f32 *, 0x50));
    }
}

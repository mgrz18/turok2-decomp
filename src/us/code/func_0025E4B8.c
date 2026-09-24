#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00210790(s32, s32);                    /* extern */
M2C_UNK func_00210B38(s32, s32);                    /* extern */

void func_0025E4B8(s32 arg0, void *arg1) {
    s32 temp_v1;
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg1, void **, 0x14);
    temp_v1 = M2C_FIELD(arg1, s32 *, 0x90);
    if ((temp_v1 >= M2C_FIELD(temp_a0, s8 *, 0x18)) && (M2C_FIELD(temp_a0, s8 *, 0x19) >= temp_v1)) {
        func_00210B38(arg0, M2C_FIELD(M2C_FIELD(arg1, void **, 0), s32 *, 0x164));
        func_00210790(arg0, M2C_FIELD(M2C_FIELD(arg1, void **, 0), s32 *, 0x168));
    }
}

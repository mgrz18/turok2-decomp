#include "common.h"
#include "m2c_macros.h"

s32 func_002247E4(s32, s32, s32);
s32 func_00227780(s32);
s32 func_0023E820(s32, s32, s32);
s32 func_00268C04(s32);

M2C_UNK func_0026E19C(s32, M2C_UNK, s32);           /* extern */
extern M2C_UNK D_800F7078;

void func_0023F05C(s32 arg0, void *arg1) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v1 = M2C_FIELD(arg1, u16 *, 0x16C);
    if (temp_v1 & 8) {
        if (M2C_FIELD(arg1, s32 *, 0x1A0) & 1) {
            goto block_4;
        }
    } else if (!(temp_v1 & 1)) {
block_4:
        if (func_0023E820(arg0, (s32) arg1, func_00268C04((s32) arg1)) != 0) {
            temp_v0 = M2C_FIELD(arg1, u16 *, 0x16C) | 1;
            M2C_FIELD(arg1, u16 *, 0x16C) = temp_v0;
            if (temp_v0 & 8) {
                func_00227780((s32) arg1);
                return;
            }
            func_002247E4((s32) &D_800F7078, (s32) arg1, 1);
            func_0026E19C(M2C_FIELD(arg1, s32 *, 0x10), 0x400, arg0);
        }
    }
}

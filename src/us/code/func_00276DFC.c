#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002714B0(s32);                         /* extern */
M2C_UNK func_00271514(s32);                         /* extern */
s32 func_002718F4(s32);                             /* extern */
M2C_UNK func_00293E60(s32, s16);                    /* extern */
M2C_UNK func_00293EC0(s32);                         /* extern */

void func_00276DFC(void *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    func_002714B0(M2C_FIELD(arg0, s32 *, 0));
    temp_v0 = M2C_FIELD(arg0, s32 *, 8);
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        M2C_FIELD(arg0, s32 *, 8) = 2;
        temp_v0_2 = func_002718F4(M2C_FIELD(arg0, s32 *, 0));
        func_00293E60(temp_v0_2, M2C_FIELD(arg0, s16 *, 0x1E));
        func_00293EC0(temp_v0_2);
    }
    func_00271514(M2C_FIELD(arg0, s32 *, 0));
}

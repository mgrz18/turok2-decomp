#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
M2C_UNK func_00275AE0(s32);                         /* extern */

void func_0028205C(void *arg0) {
    s32 temp_a1;

    M2C_FIELD(arg0, s32 *, 0x11E8) = 1;
    func_00275AE0(arg0 + 0x40);
    func_002053A8(0);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0);
    if (temp_a1 != 0) {
        func_002052D8(0, temp_a1);
        M2C_FIELD(arg0, s32 *, 0) = 0;
        M2C_FIELD(arg0, s32 *, 4) = 0;
        M2C_FIELD(arg0, s32 *, 8) = 0;
    }
}

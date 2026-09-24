#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00228B4C(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_8010FF34;

void func_00256FD4(s32 arg0, s32 *arg1) {
    s32 temp_a1;

    temp_a1 = *arg1;
    if (temp_a1 != 0) {
        func_00228B4C(&D_8010FF34, temp_a1);
        *arg1 = 0;
    }
}

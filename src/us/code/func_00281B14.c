#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200518(s32, M2C_UNK);                /* extern */
M2C_UNK func_00200738(s32, M2C_UNK);                /* extern */

void func_00281B14(s32 arg0, s32 arg1, M2C_UNK arg2) {
    func_00200738(arg1 + 0xE28, arg2);
    func_00200518(arg0 + 0xF0C, arg2);
}

#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200518(s32, M2C_UNK);                /* extern */
M2C_UNK func_00200738(s32);                         /* extern */

void func_00283730(s32 arg0, M2C_UNK arg1) {
    func_00200738(arg0 + 0x8B4);
    func_00200518(arg0 + 0x8A0, arg1);
}

#include "common.h"
#include "m2c_macros.h"

M2C_UNK CList__AddHead(s32, M2C_UNK);                /* extern */
M2C_UNK CList__Remove(s32, M2C_UNK);                /* extern */

void func_00281B14(s32 arg0, s32 arg1, M2C_UNK arg2) {
    CList__Remove(arg1 + 0xE28, arg2);
    CList__AddHead(arg0 + 0xF0C, arg2);
}

#include "common.h"
#include "m2c_macros.h"

M2C_UNK CList__AddHead(s32, M2C_UNK);                /* extern */
M2C_UNK CList__Remove(s32);                         /* extern */

void func_00283730(s32 arg0, M2C_UNK arg1) {
    CList__Remove(arg0 + 0x8B4);
    CList__AddHead(arg0 + 0x8A0, arg1);
}

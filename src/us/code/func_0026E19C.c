#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00224FC0(M2C_UNK *, s32, s32 *, s32 *); /* extern */
M2C_UNK func_0026E4B8(s32, s32, M2C_UNK, s32, s32, s32, s32); /* extern */
extern M2C_UNK D_800F7078;

void func_0026E19C(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp20;
    s32 sp24;

    func_00224FC0(&D_800F7078, arg0, &sp20, &sp24);
    if (sp24 != 0) {
        func_0026E4B8(sp20, sp24, 0, arg1 & 0x7FF, arg1 & 0x3800, 0x3800, arg2);
    }
}

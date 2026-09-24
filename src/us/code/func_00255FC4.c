#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00283FF8(M2C_UNK, M2C_UNK, M2C_UNK *); /* extern */
extern s32 D_800F1FD8;
extern M2C_UNK func_002577DC;

void func_00255FC4(s32 arg0) {
    D_800F1FD8 = arg0;
    func_00283FF8(0x50, 0, &func_002577DC);
}

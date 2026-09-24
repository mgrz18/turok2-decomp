#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00266C5C(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_0029B030(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_0029B060(s32, s32, M2C_UNK *, s32, s32, s32); /* extern */
M2C_UNK func_0029BB10(s32);                         /* extern */
extern s32 D_800B6E7C;
extern M2C_UNK func_00201C60;

void func_00201AD0(s32 arg0, s32 arg1) {
    func_0029B030(arg0 + 0x230, arg0 + 0x248, 0x80);
    func_00266C5C(arg0 + 0x448, arg1 & 0xFF, 0x1000);
    func_0029B060(arg0, arg1, &func_00201C60, arg0, arg0 + 0x1448, D_800B6E7C);
    func_0029BB10(arg0);
}

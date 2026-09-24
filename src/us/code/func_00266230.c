#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00220260(M2C_UNK *, s32, M2C_UNK);     /* extern */
M2C_UNK func_0026E110(s32, M2C_UNK, s32);           /* extern */
extern M2C_UNK D_800F7078;

void func_00266230(s32 arg0, void *arg1) {
    func_00220260(&D_800F7078, arg0, 0);
    func_0026E110(arg0, 0x200000, arg0);
    M2C_FIELD(arg1, s32 *, 0x114) = 0;
}

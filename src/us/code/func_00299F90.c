#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B030(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
M2C_UNK func_0029B820(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_800B7A50;
extern M2C_UNK D_80121670;
extern M2C_UNK D_80121678;

void func_00299F90(void) {
    D_800B7A50 = 1;
    func_0029B030(&D_80121678, &D_80121670, 1);
    func_0029B820(&D_80121678, 0, 0);
}

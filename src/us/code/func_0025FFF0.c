#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern M2C_UNK D_800F5C66;

void func_0025FFF0(s32 arg0, s32 arg1) {
    if ((M2C_FIELD(&D_800F5C66, u16 *, 0) != 0) || (M2C_FIELD(&D_800F5C66, u16 *, 6) != 0) || (M2C_FIELD(&D_800F5C66, u16 *, 0xC) != 0)) {
        func_00243414(arg0, arg1, 0x32);
    }
}

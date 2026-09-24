#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800F22C8;
extern M2C_UNK D_800F2790;
extern M2C_UNK func_002700D0;

M2C_UNK *func_0026FD4C(M2C_UNK **arg0) {
    if (M2C_FIELD(&D_800F2790, u8 *, 0) == 0) {
        M2C_FIELD(&D_800F2790, M2C_UNK **, 8) = &D_800F22C8;
        M2C_FIELD(&D_800F2790, s32 *, 4) = 0;
        M2C_FIELD(&D_800F2790, u8 *, 0) = 1U;
    }
    *arg0 = &D_800F2790;
    return &func_002700D0;
}

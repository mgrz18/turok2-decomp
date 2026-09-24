#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_800F7078;

s32 func_00263714(s32 arg0) {
    return func_00220408(&D_800F7078, arg0) == 1;
}

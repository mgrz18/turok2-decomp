#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_801101A8;

void func_0027B0C0(f32 arg0, f32 arg1) {
    M2C_FIELD(&D_801101A8, f32 *, 0) = arg0;
    M2C_FIELD(&D_801101A8, f32 *, 4) = arg1;
}

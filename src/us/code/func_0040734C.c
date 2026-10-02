#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8012F9E0;

void func_0040734C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
    M2C_FIELD(&D_8012F9E0, s8 *, 0) = 0;
    M2C_FIELD(&D_8012F9E0, s8 *, 1) = 0;
    M2C_FIELD(&D_8012F9E0, s32 *, -8) = 1;
}

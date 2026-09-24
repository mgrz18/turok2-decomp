#include "common.h"
#include "m2c_macros.h"

void func_002565C0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xAB8) = 0;
    M2C_FIELD(arg0, s32 *, 0xABC) = 0;
    M2C_FIELD(arg0, f32 *, 0xAB4) = (f32) M2C_FIELD(arg0, f32 *, 0xAB0);
}

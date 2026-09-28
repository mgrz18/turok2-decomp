#include "common.h"
#include "m2c_macros.h"

extern f32 D_800AA4D4;

s32 func_00294D18(void *arg0, s32 arg1) {
    return (s32) (((f32) arg1 * (f32) M2C_FIELD(arg0, s32 *, 0x44) * 0.000001f) + D_800AA4D4);
}
